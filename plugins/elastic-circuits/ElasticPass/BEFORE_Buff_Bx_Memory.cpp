#include "ElasticPass/CircuitGenerator.h"
#include "ElasticPass/Utils.h"
#include "ElasticPass/Memory.h"
#include "MemElemInfo/MemUtils.h"
#include "OptimizeBitwidth/OptimizeBitwidth.h"


/**
 * @brief connectEnodeWithMCEnode
 * Performs the necessary connections between a memory instruction
 * enode and a memory controller/LSQ enode once it has been determined
 * that the two should connect.
 */
void connectEnodeWithMCEnode(ENode* enode, ENode* mcEnode, BBNode* bb) {
    enode->Mem = mcEnode;
    enode->CntrlSuccs->push_back(mcEnode);
    mcEnode->CntrlPreds->push_back(enode);
    enode->memPort = true;
    setBBIndex(enode, bb);
    setMemPortIndex(enode, mcEnode);
    setBBOffset(enode, mcEnode);
}

/**
 * @brief getOrCreateMCEnode
 * @param base: base address of the array associated with the MC
 * @returns pointer to MC Enode associate with the @p base
 */
ENode* CircuitGenerator::getOrCreateMCEnode(const Value* base,
                                            std::map<const Value*, ENode*>& baseToMCEnode) {
    auto iter     = baseToMCEnode.find(base);
    auto* mcEnode = iter->second;
    if (iter == baseToMCEnode.end()) {
        mcEnode             = new ENode(MC_, base->getName().str().c_str());
        baseToMCEnode[base] = mcEnode;
        // Add new MC to enode_dag
        enode_dag->push_back(mcEnode);
    }
    return mcEnode;
}

/**
 * @brief CircuitGenerator::addMCForEnode
 * For a given load or store instruction (which has not previously
 * been connected to an LSQ), associate it with an MC.
 * @param enode
 * @param baseToMCEnode
 */
void CircuitGenerator::addMCForEnode(ENode* enode, std::map<const Value*, ENode*>& baseToMCEnode) {
    Instruction* I = enode->Instr;
    auto* mcEnode  = getOrCreateMCEnode(findBase(I), baseToMCEnode);
    connectEnodeWithMCEnode(enode, mcEnode, findBB(enode));
    updateMCConstant(enode, mcEnode);
}

void CircuitGenerator::constructLSQNodes(std::map<const Value*, ENode*>& LSQnodes) {
    /* Create an LSQ enode for each of the LSQs reported
     * by MemElemInfo, and create a mapping between the unique
     * bases of the LSQs and the corresponding LSQ enodes */
    auto LSQlist = MEI.getLSQList();
    for (auto lsq : LSQlist) {
        auto* base     = lsq->base;
        auto* e        = new ENode(LSQ_, base->getName().str().c_str());
        LSQnodes[base] = e;
    }

    /* Append all the LSQ ENodes to the enode_dag */
    for (const auto& it : LSQnodes)
        enode_dag->push_back(it.second);
}

void CircuitGenerator::updateMCConstant(ENode* enode, ENode* mcEnode) {
    /* Each memory controller contains a notion of how many
     * store operations will be executed for a given BB, when
     * entering said BB. This constant is used to ensure that
     * all pending stores are completed, before the program is
     * allowed to terminate.
     * If present, locate this constant node and increment the
     * count, else create a new constant node.
     */

    if (enode->Instr->getOpcode() == Instruction::Store) {
        bool found = false;
        //for (auto& pred : *mcEnode->JustCntrlPreds) // Aya
		// Aya
		for (auto& pred : *mcEnode->CntrlOrderPreds)
            if (pred->BB == enode->BB && pred->type == Cst_) {
                pred->cstValue++;
                found = true;
             }
        if (!found) {
            ENode* cstNode    = new ENode(Cst_, enode->BB);
            cstNode->cstValue = 1;
            //cstNode->JustCntrlSuccs->push_back(mcEnode); // Aya
			// Aya
			cstNode->CntrlOrderSuccs->push_back(mcEnode);

            cstNode->id = cst_id++;
            //mcEnode->JustCntrlPreds->push_back(cstNode);  // Aya
			// Aya
			mcEnode->CntrlOrderPreds->push_back(cstNode);

            // Add new constant to enode_dag
            enode_dag->push_back(cstNode);

			// AYA: 16/11/2021:
			cstNode->is_const_feed_memory = true;


			// AYA: 15/11/2021: These constants need to be triggered when the flow enters the BB!
			
			// this constant needs to be triggered by start_ putting any bridges/ phis necessary, so we put it in our nice JustCntrl network 
			// search for the start_ node
			for(auto& cnt_node : *bbnode_dag->front()->Cntrl_nodes) {
				if(cnt_node->type == Start_) {
					cstNode->JustCntrlPreds->push_back(cnt_node);
					cnt_node->JustCntrlSuccs->push_back(cstNode);
					break; // found the start node, no need to continue looping
				}
			} 

        }
    }
}

/**
 * @brief CircuitGenerator::addMemoryInterfaces
 * This pass is responsible for emitting all connections between
 * memory accessing instructions and associated memory controllers.
 * If @p useLSQ, LSQs will be emitted for all instructions determined
 * to have possible data dependencies by the MemElemInfo pass.
 */
void CircuitGenerator::addMemoryInterfaces(const bool useLSQ, bool is_smart_cntrlOrder_flag) {
    std::map<const Value*, ENode*> LSQnodes;

    if (useLSQ) {
        constructLSQNodes(LSQnodes);
    }

    // for convenience, we store a mapping between the base arrays in a program,
    // and their associated memory interface ENode (MC or @todo LSQ)
    std::map<const Value*, ENode*> baseToMCEnode;

    /* For each memory instruction present in the enode_dag, connect it
     * to a previously created LSQ if MEI has determined so, else, connect
     * it to a regular MC.
     * Iterate using index, given that iterators are invalidated once a
     * container is modified (which happens during addMCForEnode).
     */
    for (unsigned int i = 0; i < enode_dag->size(); i++) {
        auto* enode = enode_dag->at(i);
        if (!enode->isLoadOrStore())
            continue;

        if (useLSQ && MEI.needsLSQ(enode->Instr)) {
            const LSQset& lsq = MEI.getInstLSQ(enode->Instr);
            connectEnodeWithMCEnode(enode, LSQnodes[lsq.base], findBB(enode));
            continue;
        }

        addMCForEnode(enode, baseToMCEnode);
    }

    // Check if LSQ and MC are targetting the same memory
    for (auto lsq : LSQnodes) {
        auto* base     = lsq.first;
        if (baseToMCEnode[base] != NULL) {
            auto *lsqNode = lsq.second;
            lsqNode->lsqToMC = true;
            auto *mcEnode = baseToMCEnode[base];
            mcEnode->lsqToMC = true;

            // LSQ will connect as a predecessor of MC
            // MC should keep track of stores arriving from LSQ
            for (auto& enode : *lsqNode ->CntrlPreds) 
               updateMCConstant(enode, mcEnode);
            
            lsqNode->CntrlSuccs->push_back(mcEnode);
            mcEnode->CntrlPreds->push_back(lsqNode);
            //setBBIndex(lsqNode, mcEnode->bbNode);
            setLSQMemPortIndex(lsqNode, mcEnode);
            setBBOffset(lsqNode, mcEnode);
        }
    }	

	// Aya: added this function to connect the LSQ to the CntrlOrder network
    // Aya: 12/09/2022: broke it down into two functions and depending on the flag we either do the naive conservative CntrlOrder network
        // or the smart FPGA'23 algorithm on the CntrlOrder network
    if(is_smart_cntrlOrder_flag) {
        add_smart_ControlForLSQ();
    } else {
        add_naive_ControlForLSQ();
    } 
 
}

// AYA: 13/09/2022: added the following function that returns a vector of all of the loop header BBs for any loop enclosing the 2 passed BBNodes!
void CircuitGenerator::returnCommonLoopHeaders(BBNode* bb_1, BBNode* bb_2, std::vector<BBNode*>& common_loop_headers) {
    // check for the naive case that they both happen to have the same innermost loop!
    if(bb_1->loop == bb_2->loop) {
        BasicBlock *loop_header = bb_2->loop->getHeader();
        common_loop_headers.push_back(BBMap->at(loop_header));
    } else {
        ///////////////////////////////////////////////////////////////
        // gather and store all outer loops in the hierarchy of the loops enclosing bb_1 and bb_2 and simply compare the 2 structures
        Loop* tmp_loop = bb_1->loop;
        std::vector<Loop*> bb_1_loop_hierarchy;
        while(tmp_loop != nullptr) {
            bb_1_loop_hierarchy.push_back(tmp_loop);
            tmp_loop = tmp_loop->getParentLoop();
        }

        // repeat the same for bb_2
        tmp_loop = bb_2->loop;
        std::vector<Loop*> bb_2_loop_hierarchy;
        while(tmp_loop != nullptr) {
            bb_2_loop_hierarchy.push_back(tmp_loop);
            tmp_loop = tmp_loop->getParentLoop();
        }

        // loop over the vector with the smaller size and search for any of its elements in the other vector
        std::vector<Loop*> small_bb_loop_hierarchy;
        std::vector<Loop*> big_bb_loop_hierarchy;
        if(bb_1_loop_hierarchy.size() > bb_2_loop_hierarchy.size()) {
            big_bb_loop_hierarchy = bb_1_loop_hierarchy;
            small_bb_loop_hierarchy = bb_2_loop_hierarchy;
        } else {
            big_bb_loop_hierarchy = bb_2_loop_hierarchy;
            small_bb_loop_hierarchy = bb_1_loop_hierarchy;
        }
        
        // My goal is to have loops in the common_loop_headers starting from the outermost common loop (i.e., .at(0) is the outermost) all the way to the inner
            // yet the 2 vectors of loop hierarchy I have is arranged starting from inner loop (i.e., .at(0) is the innermost) all the way to the outer
        for(int i = small_bb_loop_hierarchy.size() - 1; i >= 0; i--) {
            for(int j = big_bb_loop_hierarchy.size() - 1; j >= 0 ; j--) {
                if(small_bb_loop_hierarchy.at(i) == big_bb_loop_hierarchy.at(j)) {
                    BasicBlock *loop_header = small_bb_loop_hierarchy.at(i)->getHeader();
                    common_loop_headers.push_back(BBMap->at(loop_header));
                }
            }
        }
        ///////////////////////////////////////////////////////////////
    }
}

// Aya: 12/09/2022: added the following function to check if the 2 basicblocks passed in the parameters are enclosed inside any common loop(s)
bool CircuitGenerator::is_common_loop(std::ofstream& dbg_file, BBNode* bb_1, BBNode* bb_2) {
    dbg_file << "\n\n\tInside is_common_loop with BB" << bb_1->Idx + 1<< " and BB" << bb_2->Idx + 1 << "!\n\n";
    // check the loop field of each of the 2 BBNodes which represents the innermost loop that a BBNode is inside. It is a nullptr if the BBNode is not inside any loop
    if(bb_1->loop == nullptr || bb_2->loop == nullptr) {
        dbg_file << "One of the two groups (or both) is not in any loop!\n";
        return false; // because at least one of the 2 bbs is not inside any loop!
    }

    // At this point I'm sure that both bbs are enclosed inside at least 1 loop. Let's see if they have any common loops!

    // check for the naive case that they both happen to have the same innermost loop!
    if(bb_1->loop == bb_2->loop) {
        dbg_file << "Both BBs are in the same loop!!\n";
        return true;
    }

    // At this point, they are both inside some loop but we are not sure if they are enclosed inside any common loop or not

    // gather and store all outer loops in the hierarchy of the loops enclosing bb_1 and bb_2 and simply compare the 2 structures
    Loop* tmp_loop = bb_1->loop;
    std::vector<Loop*> bb_1_loop_hierarchy;
    while(tmp_loop != nullptr) {
        bb_1_loop_hierarchy.push_back(tmp_loop);
        tmp_loop = tmp_loop->getParentLoop();
    }

    // repeat the same for bb_2
    tmp_loop = bb_2->loop;
    std::vector<Loop*> bb_2_loop_hierarchy;
    while(tmp_loop != nullptr) {
        bb_2_loop_hierarchy.push_back(tmp_loop);
        tmp_loop = tmp_loop->getParentLoop();
    }

    // loop over the vector with the smaller size and search for any of its elements in the other vector
    std::vector<Loop*> small_bb_loop_hierarchy;
    std::vector<Loop*> big_bb_loop_hierarchy;
    if(bb_1_loop_hierarchy.size() > bb_2_loop_hierarchy.size()) {
        big_bb_loop_hierarchy = bb_1_loop_hierarchy;
        small_bb_loop_hierarchy = bb_2_loop_hierarchy;
    } else {
        big_bb_loop_hierarchy = bb_2_loop_hierarchy;
        small_bb_loop_hierarchy = bb_1_loop_hierarchy;
    }
    bool is_common_loop_flag = false;
    for(int i = 0; i < small_bb_loop_hierarchy.size(); i++) {
        for(int j = 0; j < big_bb_loop_hierarchy.size(); j++) {
            if(small_bb_loop_hierarchy.at(i) == big_bb_loop_hierarchy.at(j)) {
                is_common_loop_flag = true;
                break; // found a common loop, no need to continue looping!!
            }
        }
    }

    return is_common_loop_flag;
}

/*
Plan for the flow of the paper: 
1) I had an initial idea that each BB should produce its own control signal denoting when it will execute and that this happens normally
    irrespective of any type of memory dependency and it happens by running FPL between the BB and START
2) Then, we have the problem of synchronizing between the dependent memory operations to know when to send the control tokens to the LSQ. This should 
    be done by synchronizing the 2 control signals of the 2 BBs involved in the memory dependency. For this, we need the Bx component because its
    specifications are important!  BUT, the issue is how to connect the Bx instances provided that the input of each Bx is already connected to START!
    Please note that in the paper I should only explain the specifications of the Bx component not its implementation using Fork and Buffer!!!

3) For our presentation in the paper, does it make sense to get rid of the Bx completely and say we have just a single source for the control token
    but it is not enough to run FPL on START and the BB, we also need to get a signal from the dependent BB that is has completed execution!!
         (i.e., the idea of HOLD that I had in the beginning!!)

*/


void CircuitGenerator::identifyMemDeps(std::ofstream& dbg_file, ENode* lsq_enode, std::vector<prod_cons_mem_dep>& all_mem_deps) {

    // 1st) loop over all of the predecessors of the LSQ enode and push the ones of them that are memory operations to all_mem_deps

    // 2nd) the following logic manages how to connect the enodes of all_mem_deps!

    // think of the all_mem_deps as a vector of the edges of graph M (each element of this vector is a directed edge of M)
    for(int i = 0; i < lsq_enode->CntrlPreds->size()-1; i++) {
        // The hazards are: 1) WAW so 2 ST in different BBs, 2) RAW and WAR are between loads and stores
            // I will identify the forward dependencies using the BB Ids (i.e., the smaller BB Id should be the producer)
            // I will identify the backward dependencies by checking if they are enclosed in a loop (i.e., the larger BB Id should be the producer)
        lsq_pred_type pred_type; 
        if(isa<LoadInst>(lsq_enode->CntrlPreds->at(i)->Instr))
            pred_type = load;
        else {
            if(isa<StoreInst>(lsq_enode->CntrlPreds->at(i)->Instr))
                pred_type = store;
            else
                pred_type = none;
        }

        if(pred_type == none)
            continue; // it is not a memory operation so no need to check for any dependencies

        // compare the lsq_enode->CntrlPreds->at(i) (which is a memory operation) against all other predecessors of the LSQ!
        for(int j = i+1; j < lsq_enode->CntrlPreds->size(); j++) {
            // AYA: 13/09/2022: changed the comparison to be with group_idx not bbs!!!
            // if(lsq_enode->CntrlPreds->at(i)->BB == lsq_enode->CntrlPreds->at(j)->BB)
            if(lsq_enode->CntrlPreds->at(i)->group_idx == lsq_enode->CntrlPreds->at(j)->group_idx)
                continue; // both are in the same BB, so do not worry about managing them as a prod, cons
            if(pred_type == load && isa<LoadInst>(lsq_enode->CntrlPreds->at(j)->Instr))
                continue;  // both are load instructions so no memory dependency
            if(!( isa<LoadInst>(lsq_enode->CntrlPreds->at(j)->Instr) || isa<StoreInst>(lsq_enode->CntrlPreds->at(j)->Instr) ))
                continue; // this pred is not a memory operation so no memory dependency

            // if we reach here, then the two preds are either both store or one is store and one is load

            prod_cons_mem_dep one_dep;
            bool is_i_prod_flag = true;
            // AYA: 13/09/2022: changed the comparison and storage in all_mem_deps to be with group_idx not bbs!!!
            // if(BBMap->at(lsq_enode->CntrlPreds->at(j)->BB)->Idx > BBMap->at(lsq_enode->CntrlPreds->at(i)->BB)->Idx) {
            if(lsq_enode->CntrlPreds->at(j)->group_idx > lsq_enode->CntrlPreds->at(i)->group_idx) {
                one_dep.prod_group_idx = lsq_enode->CntrlPreds->at(i)->group_idx;
                one_dep.cons_group_idx = lsq_enode->CntrlPreds->at(j)->group_idx;
                one_dep.is_backward = false;

                is_i_prod_flag = true;

            } else {
                assert(lsq_enode->CntrlPreds->at(j)->group_idx < lsq_enode->CntrlPreds->at(i)->group_idx);
                one_dep.prod_group_idx = lsq_enode->CntrlPreds->at(j)->group_idx;
                one_dep.cons_group_idx = lsq_enode->CntrlPreds->at(i)->group_idx;
                one_dep.is_backward = false;

                is_i_prod_flag = false;
            }
            all_mem_deps.push_back(one_dep);


            // might need to add another entry in all_mem_deps (i.e., another edge between those two memory operations!) 
                    // if they are both enclosed in 1 or more loops
            // 12/09/2022: added an extra check to see if the 2 preds are both enclosed inside one or more loops; in which case, we need to add another 
            // entry for them in all_mem_deps and label it as "is_backward"!!
            if(is_common_loop(dbg_file, bbnode_dag->at(lsq_enode->CntrlPreds->at(i)->group_idx), bbnode_dag->at(lsq_enode->CntrlPreds->at(j)->group_idx) )) {
                if(is_i_prod_flag) {
                    // j should be the producer this time! (for the backward)
                    one_dep.prod_group_idx = lsq_enode->CntrlPreds->at(j)->group_idx;
                    one_dep.cons_group_idx = lsq_enode->CntrlPreds->at(i)->group_idx;
                    one_dep.is_backward = true;
                } else {
                    // i should be the producer this time! (for the backward)
                    one_dep.prod_group_idx = lsq_enode->CntrlPreds->at(i)->group_idx;
                    one_dep.cons_group_idx = lsq_enode->CntrlPreds->at(j)->group_idx;
                    one_dep.is_backward = true;
                }
                all_mem_deps.push_back(one_dep);
            }

        }
    }
}


// added on 12/09/2022
void CircuitGenerator::identifyGroupIdxs(ENode* lsq_enode) {
    // loop over the predecessors of the LSQ node
    for(int i = 0; i < lsq_enode->CntrlPreds->size(); i++) {
        ENode* mem_operation = lsq_enode->CntrlPreds->at(i);
        // 1st) every store operation will surely have its BB idx as its group since we can not ST earlier than their BB!!
        if(isa<StoreInst>(mem_operation->Instr)) {
            mem_operation->group_idx = BBMap->at(mem_operation->BB)->Idx;

            // raise the flag of that BBnode!
            BBMap->at(mem_operation->BB)->is_group_head = true;

        } else {
            if(isa<LoadInst>(mem_operation->Instr)) {
                // a load can have a group_idx = BB Idx of the BB containing the latest ST operation that should happen before it if they have the
                        // same conditions of execution. And if they have different conditions of execution (i.e., different dependencies), 
                            // then put its group idx as the earliest BB idx that has the same depdendencies as the LD and comes right after the BB of the ST
                                // which by the way might end up being its BB!! :)

                std::vector<int> store_bb_idxs;
                // loop over all of the preds of the LSQ in search for the latest ST that my LD is dependent on to check the conditions of execution of its BB
                for(int j = 0; j < lsq_enode->CntrlPreds->size(); j++) {
                    // check only if the lsq_enode->CntrlPreds->at(j) is  a ST!
                    if(isa<StoreInst>(lsq_enode->CntrlPreds->at(j)->Instr) && BBMap->at(lsq_enode->CntrlPreds->at(j)->BB)->Idx <= BBMap->at(mem_operation->BB)->Idx) {
                        // save the BB Idx of all of the ST operations that either come before the LD or are in the same BB as the LD!
                        store_bb_idxs.push_back(BBMap->at(lsq_enode->CntrlPreds->at(j)->BB)->Idx);
                    }
                }
                // descendingly sort the store_bb_idxs to have the largest idx in the 0th element!
                sort(store_bb_idxs.begin(), store_bb_idxs.end(), greater <int>());

                // check if the BB of the latest ST has the same conditions of execution as the LD? 
                if(bbnode_dag->at(store_bb_idxs.at(0))->BB_deps == BBMap->at(mem_operation->BB)->BB_deps) {
                    // place the LD in the same group as the ST!!
                    mem_operation->group_idx = store_bb_idxs.at(0);
                    // raise the flag of that BBnode!
                    bbnode_dag->at(store_bb_idxs.at(0))->is_group_head = true;
                } else {
                   // IF no, check the earliest BB that has the same conditions of execution as the LD and comes right after the 0th element!!
                   // search for the earliest BB between the BB of the LD and the latest ST preceding it with the same conditions of execution as the LD!!
                    int idx = store_bb_idxs.at(0);
                    while(idx <= BBMap->at(mem_operation->BB)->Idx) {
                        if(bbnode_dag->at(idx)->BB_deps == BBMap->at(mem_operation->BB)->BB_deps) {
                            mem_operation->group_idx = idx;
                            // raise the flag of that BBnode!
                            bbnode_dag->at(idx)->is_group_head = true;
                            break;  // found a BB Idx for my group, stop looping!!
                        }
                        idx++;
                    }

                }

                // extra sanity check: the group_idx of the memory_operation should not be -1!!
                assert(mem_operation->group_idx != -1);
            }
        }
    }

}

/*
    For reference, these are the fields for a single Group object

    int first_bb_idx;  // for any group, I insert the Bx component simply in the BB with the smallest index belonging to the group!
    std::vector<int> pred_groups_first_bb_idx;
    std::vector<int> succ_groups_first_bb_idx;
*/
void CircuitGenerator::constructGroupsGraph(ENode* lsq_enode, std::vector<Group>& groups, const std::vector<prod_cons_mem_dep>& all_mem_deps) {
    // loop over the memory operations preds of the lsq and create a new Group object for every new Group idx of each operation
    for(int i = 0; i < lsq_enode->CntrlPreds->size(); i++) {
        ENode* mem_operation = lsq_enode->CntrlPreds->at(i);
        if(isa<StoreInst>(mem_operation->Instr) || isa<LoadInst>(mem_operation->Instr)) {
            // check if we still didn't add a Group for the group_idx field of each of these operations and add a Group object if not previously added
            auto pos = std::find(groups.begin(), groups.end(), mem_operation->group_idx);
            if(pos == groups.end()) {  // if not found, create a new entry for it!!
                Group new_group;
                new_group.first_bb_idx = mem_operation->group_idx;
                groups.push_back(new_group);
            }
            
        }
    }

    // After creating all of the Group objects, it is the time to connet their preds and succs
    // each entry in all_mem_deps should represent an edge in the graph of groups 
    for(int k = 0; k < all_mem_deps.size(); k++) {
        // find the Group object that has first_bb_idx the same as prod_group_idx
        auto pos = std::find(groups.begin(), groups.end(), all_mem_deps.at(k).prod_group_idx);
        assert(pos != groups.end());
        int prod_idx = pos - groups.begin(); 
        // find the Group object that has first_bb_idx the same as cons_group_idx
        pos = std::find(groups.begin(), groups.end(), all_mem_deps.at(k).cons_group_idx);
        assert(pos != groups.end());
        int cons_idx = pos - groups.begin();

        // Add the consumer in the producer's succ_groups_first_bb_idx only if it didn't exist before
        auto pos_ = std::find(groups.at(prod_idx).succ_groups_first_bb_idx.begin(), groups.at(prod_idx).succ_groups_first_bb_idx.end(), groups.at(cons_idx).first_bb_idx);
        if(pos_ == groups.at(prod_idx).succ_groups_first_bb_idx.end()) {
            groups.at(prod_idx).succ_groups_first_bb_idx.push_back(groups.at(cons_idx).first_bb_idx);
        }

        // Add the producer in the consumer's pred_groups_first_bb_idx only if it didn't exist before
        pos_ = std::find(groups.at(cons_idx).pred_groups_first_bb_idx.begin(), groups.at(cons_idx).pred_groups_first_bb_idx.end(), groups.at(prod_idx).first_bb_idx);
        if(pos_ == groups.at(cons_idx).pred_groups_first_bb_idx.end()) {
            groups.at(cons_idx).pred_groups_first_bb_idx.push_back(groups.at(prod_idx).first_bb_idx);
        }
    }
}

void CircuitGenerator::printDebugGroupsGraph(std::ofstream& dbg_file, const std::vector<Group>& groups, ENode* lsq_enode, const std::vector<prod_cons_mem_dep>& all_mem_deps) {
    dbg_file << "\nPrinting the predecessors of a LSQ enode:\t";
    for(int y= 0; y < lsq_enode->CntrlPreds->size(); y++) {
        dbg_file << lsq_enode->CntrlPreds->at(y)->type << " in BB" << BBMap->at(lsq_enode->CntrlPreds->at(y)->BB)->Idx + 1 << ", ";
    }


    dbg_file << "\n\n***********************************************************\n";
    dbg_file << "\nPrinting the details of all Groups in the G graph\n";

    for(int i = 0; i < groups.size(); i++) {
        dbg_file << "\n\n***********************************************************\n";
        dbg_file << "\n\t For Group with first_bb_idx = " << groups.at(i).first_bb_idx + 1 << "\n\n";
        // printing predecessors 
        dbg_file << "The first_bb_idx of Predecessor Groups are:  ";
        if(groups.at(i).pred_groups_first_bb_idx.size() == 0) {
            dbg_file << " NOTHING!\n";
        } else {
            for(int j = 0; j < groups.at(i).pred_groups_first_bb_idx.size(); j++) {
                dbg_file << groups.at(i).pred_groups_first_bb_idx.at(j) + 1 << ", ";
            }
            dbg_file <<"\n\n";
            
        }

        // printing successors 
        dbg_file << "The first_bb_idx of Successor Groups are:  ";
        if(groups.at(i).succ_groups_first_bb_idx.size() == 0) {
            dbg_file << " NOTHING!\n";
        } else {
            for(int j = 0; j < groups.at(i).succ_groups_first_bb_idx.size(); j++) {
                dbg_file << groups.at(i).succ_groups_first_bb_idx.at(j) + 1 << ", ";
            }
            dbg_file <<"\n\n";
            
        }

    }
}

// I rely on the addSuppress logic of FPL'22 to convert any PHIs added here to MUXes!!
void CircuitGenerator::applyGSA_simplified(std::ofstream& dbg_file, const std::vector<Group>& groups) {
    // loop over the groups
    for(int i = 0; i < groups.size(); i++) {
         // search for the Fork instance of the Bx in this group since the predecessor should feed the Fork (i.e., if we are to insert a Phi it should the fork!!)
        ENode* consumer_Bx_fork = nullptr;
        for(int f = 0; f < enode_dag->size(); f++) {
            if(enode_dag->at(f)->type == Fork_c && enode_dag->at(f)->is_mem_dep_component && enode_dag->at(f)->BB == bbnode_dag->at(groups.at(i).first_bb_idx)->BB) {
                consumer_Bx_fork = enode_dag->at(f);
                break;
            }
        }
        assert(consumer_Bx_fork != nullptr);

        // loop over the predecessors of each group
        for(int j = 0; j < groups.at(i).pred_groups_first_bb_idx.size(); j++) {
            // search for the Buff instance of the Bx in this group since if we are to insert a Phi, it should be fed by the Buff od this Bx!
            ENode* producer_Bx_buffer = nullptr;
            for(int f = 0; f < enode_dag->size(); f++) {
                if(enode_dag->at(f)->type == Bx_Buffer_wrapper_ && enode_dag->at(f)->is_mem_dep_component && enode_dag->at(f)->CntrlOrderPreds->at(0)->BB == bbnode_dag->at(groups.at(i).pred_groups_first_bb_idx.at(j))->BB) {
                    // sanity check, the successor of this buffer must be a Fork!!!
                    assert(enode_dag->at(f)->CntrlOrderPreds->at(0)->type == Fork_c && enode_dag->at(f)->CntrlOrderPreds->at(0)->is_mem_dep_component);
                    producer_Bx_buffer = enode_dag->at(f);
                    break;
                }
            }
            assert(producer_Bx_buffer != nullptr);

            // 1st check the predecessor j is connected to my group i through a backward edge
            // MY check is based only on comparing the indices of the first_bb of each group 
            // If the index of the predecessor j is greater than the index of the i group, this is a backward edge and we should insert a MUX (or more dependening on how many loops)!!
            if(groups.at(i).first_bb_idx < groups.at(i).pred_groups_first_bb_idx.at(j)) {
                // for every loop enclosing both groups, add a Phi_c in the loop header and raise the flag is_mem_dep_MUX to true!
                // fill a vector of the loop header BBs of all loops enclosing the 2 groups starting from the outermost loop all the way to the inner..
                std::vector<BBNode*> common_loop_headers;
                returnCommonLoopHeaders(bbnode_dag->at(groups.at(i).first_bb_idx), bbnode_dag->at(groups.at(i).pred_groups_first_bb_idx.at(j)), common_loop_headers);

                // sanity check: they must be enclosed inside at least 1 common loop!!
                assert(common_loop_headers.size() > 0);

                std::vector<ENode*>phis;
                // 1st) create a new phi at the loop header of each of the loops enclosing the 2 Bxs
                for(int k = 0; k < common_loop_headers.size(); k++) {
                    // the assumption is that the above function returns common_loop_headers such that the .at(0) entry corresponds to the loop header of the outermost loop enclosing both groups
                    ENode* phi = new ENode(Phi_c, "phiC", common_loop_headers.at(k)->BB);  
                    phi->id = phi_id++;
                    // the naming of this field is confusing, but it's meant to hold the producer enode that feeds a Phi of our creation
                    // for all of the phis I'm adding, I store in them the original producer's Bx's Buffer!!
                    phi->producer_to_branch = producer_Bx_buffer;

                    phi->is_mem_dep_component = true;

                    phis.push_back(phi);

                    dbg_file << "\n\n\tFrom inside applyGSA_simplified, just added a PHi_c in BB" << BBMap->at(common_loop_headers.at(k)->BB)->Idx + 1 << "\n";

                    // converting them to MUXes will be managed by FPL's inside the addSuppress stuff!
                }

                // 2nd) Cut the connection between the original producer and consumer Bxs!
                int idx_prod_in_cons_pred = returnPosRemoveNode(consumer_Bx_fork->CntrlOrderPreds, producer_Bx_buffer); // this function only finds the index of the passed element in the passed vector!
                int idx_cons_in_prod_succ = returnPosRemoveNode(producer_Bx_buffer->CntrlOrderSuccs, consumer_Bx_fork);

                // 3rd) Connect the preds and succs of the chain of phis between the original producer and consumer Bxs
                // the 0th phi should be fed from START and the producer
                    // search for the Start_ node
                for(auto& cnt_node : *bbnode_dag->front()->Cntrl_nodes) {
                    if(cnt_node->type == Start_) {
                        phis.at(0)->CntrlOrderPreds->push_back(cnt_node);
                        cnt_node->CntrlOrderSuccs->push_back(phis.at(0));
                        break; // found the start node, no need to continue looping
                    }
                } 
                // the 2nd input of this phi should be the original producer!
                producer_Bx_buffer->CntrlOrderSuccs->at(idx_cons_in_prod_succ) = phis.at(0);
                phis.at(0)->CntrlOrderPreds->push_back(producer_Bx_buffer);

                // do the chain of connections between the different Phis that may exist!
                for(int k = 1; k < phis.size(); k++) {
                    // the first input of any phi (apart from the 0th which we handled above) should be from the previous phi corresponding to a loop that is one level outer to me
                    phis.at(k)->CntrlOrderPreds->push_back(phis.at(k-1));
                    phis.at(k-1)->CntrlOrderSuccs->push_back(phis.at(k));

                    // the second input of ALL phis should be from the original producer!!
                    phis.at(k)->CntrlOrderPreds->push_back(producer_Bx_buffer);
                    producer_Bx_buffer->CntrlOrderSuccs->push_back(phis.at(k));
                }
                // the phis.at(phis.size()-1) should feed the original consumer and be fed normally from the previous PHI as before inside the loop
                phis.at(phis.size()-1)->CntrlOrderSuccs->push_back(consumer_Bx_fork);
                consumer_Bx_fork->CntrlOrderPreds->at(idx_prod_in_cons_pred) = phis.at(phis.size()-1);
                ///////////////////////

                // 4th) Push all of the newly created phis objects to the enode_dag
                for(int k = 0; k < phis.size(); k++) {
                    enode_dag->push_back(phis.at(k));
                }

            } /*else {
                // check for the cases of having a confluence point!!!

            }*/
        }
    }

    // TEST THE MUX INSERTION ON ST ST EXAMPLE WITH A DEEP LOOP!!!! THEN, AND ONLY THEN, PROCEED IN THE OTHER MUX CASE!!! THEN DO A MIX OF BOTH!!

}

void CircuitGenerator::addLoopBranches_postGSA_singleLoop(std::ofstream& dbg_file, ENode* phi_node, ENode* producer) {
    // The goal is to 1) Insert a Branch between the Phi_node and the producer, 2) Convert the Phi_node to a MUX and add an INIT component for it!!
    ENode* consumer = phi_node;
    // sanity check: phi_node must be in a loop header!!
    assert(BBMap->at(phi_node->BB)->is_loop_header);

    // I will do it the same way we reason in FPL and will reuse some of the functions that are defined and/or used inside addSuppress.cpp!!
    // 1st) get the conditions of the loop exits (may be just one or more if the loop has multiple exits!)
    std::vector<int> modif_consBB_deps;
    addLoopsExitstoDeps_Phi_regeneration(producer, consumer, modif_consBB_deps);  // this function takes the producer but doesn't use it in any analysis!!

    // 2nd) generate a boolean expression out of the loop exit conditions
    std::vector<std::vector<pierCondition>> br_cond;
    if(modif_consBB_deps.size() > 0)  // since the producer might be in a loop tail (not a loop exit) and its path to the consumer might not be including any loop exit!!!
        constructSOP(dbg_file, BBMap->at(producer->BB), BBMap->at(consumer->BB), modif_consBB_deps, br_cond);

    // 3rd) do some checks/tweaks and then insert the branch and convert to MUX!
    ////////////////////////////////////////////////////////////////////////////////////////////
    // 26/06/2022: do a check on br_cond to make sure that it includes the loop exit condition only once if the loop has a single loop exit!!
    if(modif_consBB_deps.size() == 1) {
        if(br_cond.size() > 1) {
            dbg_file << "\n\tISSUE: need to fix the calculated loop exit condition!!\n";
            // some sanity checks
            int check_cond_idx = br_cond.at(0).at(0).pierBB_index;
            for(int ii = 0; ii < br_cond.size(); ii++) {
                assert(br_cond.at(ii).size() == 1); // each product should be made of only a single condition
                assert(br_cond.at(ii).at(0).pierBB_index == check_cond_idx);
            }

            // keep just one entry in br_cond of this condition with the correct negation
            br_cond.clear();
            pierCondition corrected_cond;
            corrected_cond.pierBB_index = check_cond_idx;

            // this is a loop containing both the producer and the consumer, we should suppress
                // if the loop is done iterating, so the negation should be depending on the loop exit's successor that is outside the loop!!
            assert(BBMap->at(consumer->BB)->loop == bbnode_dag->at(check_cond_idx)->loop);
            if(!bbnode_dag->at(check_cond_idx)->loop->contains(bbnode_dag->at(check_cond_idx)->CntrlSuccs->at(0)->BB)) {
                // sanity check, the loop must contain the false successor!!
                assert(bbnode_dag->at(check_cond_idx)->loop->contains(bbnode_dag->at(check_cond_idx)->CntrlSuccs->at(1)->BB));

                // this means the true successor is outside the loop, so I should suppress when the condition is true!!
                corrected_cond.is_negated = true; // this condition is opposite to the SUCCESSOR!!
            } else {
                // sanity check, the loop must NOT contain the false successor!!
                assert(!bbnode_dag->at(check_cond_idx)->loop->contains(bbnode_dag->at(check_cond_idx)->CntrlSuccs->at(1)->BB));
                // this means the false successor is outside the loop, so I should suppress when the condition is false!!
                corrected_cond.is_negated = false; // this condition is opposite to the SUCCESSOR!!
            }

            std::vector<pierCondition> corrected_cond_product;
            corrected_cond_product.push_back(corrected_cond);
            br_cond.push_back(corrected_cond_product);

        }
    }
    //////////////////////////////////////////////////////////////////////////////////////////

    /////////////////// Printing for debugging!! 
    dbg_file << "\n\t\t Printing the SOP of the SUPP in this case! The condition is opposite to SUPPRESS!!:\n";
    if(br_cond.size() == 0) {
        dbg_file << " There is no SUPPRESS!!\n ";
    } 
    for(int u = 0; u < br_cond.size(); u++) {
        for(int v = 0; v < br_cond.at(u).size(); v++) {
            if(br_cond.at(u).at(v).is_negated)
                dbg_file << " not C";
            else
                dbg_file << " C";

            dbg_file << br_cond.at(u).at(v).pierBB_index + 1 << " & ";
        }
        dbg_file << " + ";
    }
    dbg_file << "\n\n";
    ///////////////////////////////////// end of print for debugging
    bool is_negated_cond;
    // AYA: 23/08/2022: enclosed the following statements inside this condition because we should not do anything if there are no dependencies!!
    if(modif_consBB_deps.size() > 0) {
        ENode* supp_condition_enode = applyShannon(dbg_file, dbg_file, producer->BB, br_cond, is_negated_cond); // takes the producer->BB because this will be the targetBB (i.e., the BB that we will insert the Branch in!!!!) 
        ///////////// 
        if(is_negated_cond) {
            is_negated_cond = false; // this condition is opposite to the SUCCESSOR!!
        } else {
            is_negated_cond = true; // this condition is opposite to the SUCCESSOR!!
        }
        //////////////////////////////////////////////////////
        insertBranch(CircuitGenerator::memDeps, supp_condition_enode, producer, consumer, is_negated_cond);

        // I need to raise the is_mem_dep_component flag of the Branch_c enode that we have just inserted!!
         // To do so, retreive the Branch object.. THe easiest thing is to retrieve it from the consumer which is Phi_c and the Branch is .at(1)!!
        assert(consumer->CntrlOrderPreds->at(1)->type == Branch_c);
        ENode* added_branch = consumer->CntrlOrderPreds->at(1);
        added_branch->is_mem_dep_component = true;
        

        // TEMPORARILY COMMENTING THIS!!!!

        // we should convert it to MUX and "supp_condition_enode" should serve as the SEL of the MUX (either as is or negated!)
        //assert(!consumer->isMux);
        // the following function converts the Phi enode to a MUX 
        if(!consumer->isMux)
            convertToMux(CircuitGenerator::memDeps, consumer, supp_condition_enode, is_negated_cond);
        
    }                           
    ////////////////////////////////////////////////////////////////////////////////////////////

}

/*void CircuitGenerator::addLoopBranches_postGSA_nestedLoop(std::ofstream& dbg_file) {
    
}*/

void CircuitGenerator::addLoopBranches_postGSA(std::ofstream& dbg_file, const std::vector<Group>& groups) {
    // loop over the first BB of the different Bxs and identify the Bx components that have one or more Phi_c components inserted between them!
        // Specficially, in case of a common loop-nest, we will have 1 Phi_c component inserted in the middle between the 2 Bx and this Phi_c will be fed with a chain of other Phi_c!!

    for(int i = 0; i < groups.size(); i++) {
        // search for the Fork instance of the Bx in this group since the predecessor should feed the Fork (i.e., if we are to insert a Phi it should the fork!!)
        ENode* consumer_Bx_fork = nullptr;
        for(int f = 0; f < enode_dag->size(); f++) {
            if(enode_dag->at(f)->type == Fork_c && enode_dag->at(f)->is_mem_dep_component && enode_dag->at(f)->BB == bbnode_dag->at(groups.at(i).first_bb_idx)->BB) {
                consumer_Bx_fork = enode_dag->at(f);
                break;
            }
        }
        assert(consumer_Bx_fork != nullptr);

        // loop over the predecessors of each group
        for(int j = 0; j < groups.at(i).pred_groups_first_bb_idx.size(); j++) {
            // search for the Buff instance of the Bx in this group since if we are to insert a Phi, it should be fed by the Buff od this Bx!
            ENode* producer_Bx_buffer = nullptr;
            for(int f = 0; f < enode_dag->size(); f++) {
                if(enode_dag->at(f)->type == Bx_Buffer_wrapper_ && enode_dag->at(f)->is_mem_dep_component && enode_dag->at(f)->CntrlOrderPreds->at(0)->BB == bbnode_dag->at(groups.at(i).pred_groups_first_bb_idx.at(j))->BB) {
                    // sanity check, the successor of this buffer must be a Fork!!!
                    assert(enode_dag->at(f)->CntrlOrderPreds->at(0)->type == Fork_c && enode_dag->at(f)->CntrlOrderPreds->at(0)->is_mem_dep_component);
                    producer_Bx_buffer = enode_dag->at(f);
                    break;
                }
            }

            // now I have the producer component being producer_Bx_buffer and the consumer component being consumer_Bx_fork, check if a MUX in inserted between them!

            // We care here ONLY for nodes connected through a backward edge!!!!
            if(groups.at(i).first_bb_idx < groups.at(i).pred_groups_first_bb_idx.at(j)) {
                // 1) search for a Phi_c in the preds of the producer 
                for(int u = 0; u < producer_Bx_buffer->CntrlOrderSuccs->size(); u++) {
                    if(producer_Bx_buffer->CntrlOrderSuccs->at(u)->type == Phi_c) {
                        assert(producer_Bx_buffer->CntrlOrderSuccs->at(u)->is_mem_dep_component);

                        // 2) check if this Phi_c is the one intermediate between the producer and consumer I currently have
                            // This check is IMP because in a general case, the producer could be having multiple preds where multiple of them could be a Phi_c but not outgoing to the specific consumer I'm currently working on
                                // search for this Phi_c in the predecessors of the consumer I currently have!
                        auto pos = std::find(consumer_Bx_fork->CntrlOrderPreds->begin(), consumer_Bx_fork->CntrlOrderPreds->end(), producer_Bx_buffer->CntrlOrderSuccs->at(u));
                        if(pos != consumer_Bx_fork->CntrlOrderPreds->end()) {
                            // SO, this Phi_c is for sure inserted between the producer and consumer I currently have!
                            ENode* innermost_phi = producer_Bx_buffer->CntrlOrderSuccs->at(u);
                             // sanity check: I always insert the producer Bx as the second (.at(1)) predecessor of a Phi_c
                            assert(innermost_phi->CntrlOrderPreds->size() == 2);
                            assert(innermost_phi->CntrlOrderPreds->at(1) == producer_Bx_buffer);

                            // 3) check if this PHi_c is being fed by a chain of other Phi_c corresponding to a loop-nest or if it is the only one and is thus fed from START
                            if(innermost_phi->CntrlOrderPreds->at(0)->type == Start_) {
                                addLoopBranches_postGSA_singleLoop(dbg_file, innermost_phi, producer_Bx_buffer);
                            } else {
                                // sanity checks: 
                                // the predecessor must be of type Phi_c!!
                                assert(innermost_phi->CntrlOrderPreds->at(0)->type == Phi_c && innermost_phi->CntrlOrderPreds->at(0)->is_mem_dep_component);
                                // that predecessor must be another successor of the original producer!!!
                                auto pos_ = std::find(producer_Bx_buffer->CntrlOrderSuccs->begin(), producer_Bx_buffer->CntrlOrderSuccs->end(), innermost_phi->CntrlOrderPreds->at(0));
                                assert(pos_ != producer_Bx_buffer->CntrlOrderSuccs->end());
                                // TODOOOO: DO NOT FORGET!! CURRENTLY I'M COMMENTING IT OUT!!!!
                                // addLoopBranches_postGSA_nestedLoop(dbg_file);
                            }                     
                        }
                    }
                }

            }

        }
        // assertion: sanity check: if the input to this intermediate MUX is another MUX (i.e., the case of common nested loops), the consumer must be feeding the input of this second MUX!!!

    }
}

void CircuitGenerator::translateGroups_toCircuit(ENode* lsq_enode, const std::vector<Group>& groups) {
    for(int i = 0; i < groups.size(); i++) {
        // add 1 Bx component instance in each group and connect the first output of the fork to the buffer and the second input to the LSQ!!

        // 1st) add a fork 
        ENode* fork = new ENode(Fork_c, "forkC",  bbnode_dag->at(groups.at(i).first_bb_idx)->BB); 
        fork->id = fork_id++;

        fork->is_mem_dep_component = true;

        enode_dag->push_back(fork);

        // 2nd) add a Buffer and connect its output to the Bx of the consumer
        ENode* buff_node = new ENode(Bx_Buffer_wrapper_, "buffer_bx_op", bbnode_dag->at(groups.at(i).first_bb_idx)->BB);
        buff_node->id = CircuitGenerator::bx_buffer_wrapper_id++;//buff_id++;

        buff_node->is_mem_dep_component = true;

        enode_dag->push_back(buff_node);

        // 3rd) connect the two components
        fork->CntrlOrderSuccs->push_back(buff_node);
        buff_node->CntrlOrderPreds->push_back(fork);

        // 4th) connect the second output of the Fork to the LSQ!!!
        fork->CntrlOrderSuccs->push_back(lsq_enode);
        lsq_enode->CntrlOrderPreds->push_back(fork);
    }

    // Next let's do the connections between different Bx components
    for(int i = 0; i < groups.size(); i++) {
        // search for the Fork instance of the Bx in this group since the predecessor should feed the Fork
        ENode* consumer_Bx_fork = nullptr;
        for(int f = 0; f < enode_dag->size(); f++) {
            if(enode_dag->at(f)->type == Fork_c && enode_dag->at(f)->is_mem_dep_component && enode_dag->at(f)->BB == bbnode_dag->at(groups.at(i).first_bb_idx)->BB) {
                consumer_Bx_fork = enode_dag->at(f);
                break;
            }
        }
        assert(consumer_Bx_fork != nullptr);

        // loop over the predecessors of each group!
        for(int j = 0; j < groups.at(i).pred_groups_first_bb_idx.size(); j++) {
            // search for the Buff instance of the Bx in this group since this should feed the fork of the other group!
            ENode* producer_Bx_buffer = nullptr;
            for(int f = 0; f < enode_dag->size(); f++) {
                if(enode_dag->at(f)->type == Bx_Buffer_wrapper_ && enode_dag->at(f)->is_mem_dep_component && enode_dag->at(f)->CntrlOrderPreds->at(0)->BB == bbnode_dag->at(groups.at(i).pred_groups_first_bb_idx.at(j))->BB) {
                    // sanity check, the successor of this buffer must be a Fork!!!
                    assert(enode_dag->at(f)->CntrlOrderPreds->at(0)->type == Fork_c && enode_dag->at(f)->CntrlOrderPreds->at(0)->is_mem_dep_component);
                    producer_Bx_buffer = enode_dag->at(f);
                    break;
                }
            }
            assert(producer_Bx_buffer != nullptr);

            // do the necessary connections between the producer_Bx_buffer and the consumer_Bx_fork!!
            producer_Bx_buffer->CntrlOrderSuccs->push_back(consumer_Bx_fork);
            consumer_Bx_fork->CntrlOrderPreds->push_back(producer_Bx_buffer);  
        }

    }

    // connect START and SINK to Bxs of groups having no predecessors or successors respectively
    for(int i = 0; i < groups.size(); i++) {
        if(groups.at(i).pred_groups_first_bb_idx.size() == 0) {
            // search for the Fork instance of the Bx in this group since the predecessor should feed the Fork
            ENode* consumer_Bx_fork = nullptr;
            for(int f = 0; f < enode_dag->size(); f++) {
                if(enode_dag->at(f)->type == Fork_c && enode_dag->at(f)->is_mem_dep_component && enode_dag->at(f)->BB == bbnode_dag->at(groups.at(i).first_bb_idx)->BB) {
                    consumer_Bx_fork = enode_dag->at(f);
                    break;
                }
            }
            assert(consumer_Bx_fork != nullptr); 

            // search for the Start_ node
            for(auto& cnt_node : *bbnode_dag->front()->Cntrl_nodes) {
                if(cnt_node->type == Start_) {
                    consumer_Bx_fork->CntrlOrderPreds->push_back(cnt_node);
                    cnt_node->CntrlOrderSuccs->push_back(consumer_Bx_fork);
                    break; // found the start node, no need to continue looping
                }
            } 

        }

        if(groups.at(i).succ_groups_first_bb_idx.size() == 0) {
             // search for the Buff instance of the Bx in this group since this should feed the sink
            ENode* producer_Bx_buffer = nullptr;
            for(int f = 0; f < enode_dag->size(); f++) {
                if(enode_dag->at(f)->type == Bx_Buffer_wrapper_ && enode_dag->at(f)->is_mem_dep_component && enode_dag->at(f)->CntrlOrderPreds->at(0)->BB == bbnode_dag->at(groups.at(i).first_bb_idx)->BB) {
                    // sanity check, the successor of this buffer must be a Fork!!!
                    assert(enode_dag->at(f)->CntrlOrderPreds->at(0)->type == Fork_c && enode_dag->at(f)->CntrlOrderPreds->at(0)->is_mem_dep_component);
                    producer_Bx_buffer = enode_dag->at(f);
                    break;
                }
            }
            assert(producer_Bx_buffer != nullptr);

            // create a new SINK node
            ENode* sink_node = new ENode(Sink_, "sink"); 
            sink_node->id    = sink_id++;
            enode_dag->push_back(sink_node);
            producer_Bx_buffer->CntrlOrderSuccs->push_back(sink_node);
            sink_node->CntrlOrderPreds->push_back(producer_Bx_buffer);

        }
    }
}

void CircuitGenerator::removeUselessBxs() {
    // loop on the enode dage searching for a Buffer that has is_mem_dep_component true and has a sucessor sink!
    for(auto& enode : *enode_dag) {
        if(enode->type == Bx_Buffer_wrapper_ && enode->is_mem_dep_component && enode->CntrlOrderSuccs->at(0)->type == Sink_) {
            // assertions for sanity checks
            assert(enode->CntrlOrderSuccs->size() == 1);  // should be having only the sink as its successor!!

            // erase the sink from the enode_dag
            auto pos_sink_ = std::find(enode_dag->begin(), enode_dag->end(), enode->CntrlOrderSuccs->at(0));
            assert(pos_sink_ != enode_dag->end());
            enode_dag->erase(pos_sink_);

            // erase the sink from the successors of the Buffer!!
            auto pos_sink = std::find(enode->CntrlOrderSuccs->begin(), enode->CntrlOrderSuccs->end(), enode->CntrlOrderSuccs->at(0));
            assert(pos_sink != enode->CntrlOrderSuccs->end());
            enode->CntrlOrderSuccs->erase(pos_sink);

            assert(enode->CntrlOrderPreds->size() == 1 && enode->CntrlOrderPreds->at(0)->type == Fork_c); // should be having only a Fork as its predecessor!!

            ENode* fork_of_bx = enode->CntrlOrderPreds->at(0);
            // extra check on the fork
            assert(fork_of_bx->CntrlOrderSuccs->size() == 2);
            assert(fork_of_bx->CntrlOrderSuccs->at(0)->type == Bx_Buffer_wrapper_ && fork_of_bx->CntrlOrderSuccs->at(1)->type == LSQ_); // the fork should be having only two successors: the LSQ and the Buffer!
            ENode* lsq_enode = fork_of_bx->CntrlOrderSuccs->at(1);

            // should completely erase the Fork_c and the Buffer and connect the LSQ directly to the predecessor of the Fork!!
            // identify the index of the Fork of this Bx in the CntrlOrderPreds of the LSQ!!
            int idx_fork_in_lsq = returnPosRemoveNode(lsq_enode->CntrlOrderPreds,  fork_of_bx); // this function only finds the index of the passed element in the passed vector!
            
            // identify the index of the Fork of this Bx in the CntrlOrderSuccs of the fork's predecessor!!
            int idx_fork_in_its_pred = returnPosRemoveNode(fork_of_bx->CntrlOrderPreds->at(0)->CntrlOrderSuccs, fork_of_bx);

            lsq_enode->CntrlOrderPreds->at(idx_fork_in_lsq) = fork_of_bx->CntrlOrderPreds->at(0);
            fork_of_bx->CntrlOrderPreds->at(0)->CntrlOrderSuccs->at(idx_fork_in_its_pred) = lsq_enode;

            // erase the Fork and the Buffer and the sink that is fed by the Buffer!!
            // find the position of the Fork in the enode_dag
            auto pos_fork = std::find(enode_dag->begin(), enode_dag->end(), fork_of_bx);
            assert(pos_fork != enode_dag->end());
            enode_dag->erase(pos_fork);
    
            // find the position of the Buffer in the enode_dag
            /*auto pos_buff = std::find(enode_dag->begin(), enode_dag->end(), enode);
            assert(pos_buff != enode_dag->end());
            enode_dag->erase(pos_buff);*/
            
        }
    }

    // do another loop to erase all of the Bx components that should now be having no successors!!!
    for(auto& enode : *enode_dag) {
        if(enode->type == Bx_Buffer_wrapper_ && enode->is_mem_dep_component && enode->CntrlOrderSuccs->size() == 0) {
            auto pos_buff = std::find(enode_dag->begin(), enode_dag->end(), enode);
            assert(pos_buff != enode_dag->end());
            enode_dag->erase(pos_buff);
        }
    }

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CircuitGenerator::add_smart_ControlForLSQ() {
    // 12/09/2022
    std::vector<prod_cons_mem_dep> all_mem_deps; // one element of this should hold the index of the producer and consumer operations
            // (based on the type of memory dependency) in the CntrlPreds of the LSQ

    // for each LSQ object, run the algorithm
    for(auto & enode : *enode_dag) {
        all_mem_deps.clear();
        if(enode->type == LSQ_) {
            // TODO: test each of these functions alone!!!
            // 1st) loop over the memory operations that are preds of the LSQ and fill the group_idx field for each of them
                // THis group_idx field is either simply the original BB idx of the enode or another earlier BB idx!
            identifyGroupIdxs(enode);  // internally modifies the group_idx field of the enodes that are the memory operations preds of the LSQ
                                    // and also modify a new field of the bbnode class which is a flag denoting that it is a group either because the group is made up only of a single BB (itself) or because it is the BB with the smallest index of the group

            // 2nd) Construct a graph with group_idx nodes
            // each node is a BB Idx and the producer and consumer relations are maintained in a predecessor and successor vector for each element!
                // this is done by looping over the enode memory operations and if they do not have the same group_idx and are either LD,ST or ST,ST, make an edge!!
            
            // 13/09/2022: I decided to call this function right before the function that constructs the graph of groups to make use
                    // of the info it provides about the producer, consumer relationships!
            std::ofstream dbg_file;
            dbg_file.open("G_Graph.txt");
            identifyMemDeps(dbg_file, enode, all_mem_deps); // fills all_mem_deps where each element of it is 1 edge from the M graph (and it labels if edges are backward)
            std::vector<Group> groups;
            constructGroupsGraph(enode, groups, all_mem_deps);

       
            printDebugGroupsGraph(dbg_file, groups, enode, all_mem_deps); 

            // TEMPORARILY COMMENTING THIS ONE!!!!
            // 3rd) remove redundant connections from the groups graph but make sure to maintain some necessary control flow edges!!!
            //applyTransitiveReduction(); 
           // fixTransitiveReduction();  // to add back any necessary (control-wise) edges that were removed by the transitive reduction 

            // The following is extra reassuring information about the case of having a single node in the Gropus graph (ofc with no incoming edges)
            // At this point if all my memory operations happen to be in a single group, the graph of Groups will have a single node with no incoming or outgoing edges
                // In this case, a single Bx will be inserted and START will be incoming to it and a SINK will be outgoing from it (will be optimzied in the end)!!!

            // 4th) translate each group into Bx components with connections corresponding to the edges of the G graph!
            translateGroups_toCircuit(enode, groups);

            // 5th) now I know the minimum set of producers and consumers, run GSA to see if any MUXes should be added!
            applyGSA_simplified(dbg_file, groups);  // STILL TODO: THE 2ND CONDITION OF MERGE INSERTION!!!! AND SEE ITS EFFECT ON ADD_SUPPRESS!!!!!

            // 6th) 14/09/2022: added the following function to manage the insertion of Branches for the common loops (chain of Branches in case of common loop-nests)
            std::ofstream dbg_file_2;
            dbg_file_2.open("Managing_Loops_muxes_branches.txt");
            addLoopBranches_postGSA(dbg_file_2, groups);

            // DO A SANITY CHECK ON THE INPUTS AND OUTPUTS OF THE BX COMPONENTS???????

            // insert a Join for any component having more than one input!!!
                // temporarily put an assert here to make sure that for now, we do not trigger this case simply because we are not supporting it!!
           // addJoin();  // simply, if any group has more than one input, insert a Join in the same BB as that of the Bx that will represent the group!

            // TODO: at this point we will be done here and should run FPL and that is, BUT, make sure in FPL
                // that it does not do token count mismatch in any wrong place! Maybe it works naturally but just check!             
        }

    }


    /*
    // Aya added this function to provide the control tokens to the LSQ
    // Everything about the control interfaces of the LSQ or the MC should be put in the CntrlOrder graph to be compatible with the printDot!

    std::vector<prod_cons_mem_dep> all_mem_deps; // one element of this should hold the index of the producer and consumer operations
            // (based on the type of memory dependency) in the CntrlPreds of the LSQ
    for(auto & enode : *enode_dag) {
        all_mem_deps.clear();
        if(enode->type == LSQ_) {
            identifyMemDeps(enode, all_mem_deps);  // STILL TODO: counting for the backward dependencies!!!

             // if the size of all_mem_deps is 0, all memory operations are in a single BB, so insert a single Bx and trigger it from START
            if(all_mem_deps.size() == 0){
                // sanity check: make sure that all preds of type memory operation are in the same BB!!
                int old_bb_idx = -1;
                int bb_idx = -1;

                bool sanity_check_flag = false;
                
                // the following loop does a sanity check to assure that all the LSQ's pred memory operations are in the same BB and extracts the ID of this BB
                for(int k = 0; k < enode->CntrlPreds->size(); k++) {
                    if(isa<LoadInst>(enode->CntrlPreds->at(k)->Instr) || isa<StoreInst>(enode->CntrlPreds->at(k)->Instr)) {
                        bb_idx = BBMap->at(enode->CntrlPreds->at(k)->BB)->Idx;
                        // compare the bb_idx to the one of the previous memory operation
                        if(old_bb_idx != -1) {
                            if(bb_idx != old_bb_idx) {
                                sanity_check_flag = true;
                                break;
                            }
                        }
                        old_bb_idx = bb_idx;
                    }
                }
                assert(sanity_check_flag == false);  // this flag is just for sanity check.. there is something wrong if the flag is true!!
                assert(bb_idx != -1);
                ///////////////////////////////// End of sanity check 

                // Inserting the Bx component (composed of Fork and Buffer) in this case doesn't work (deadlocks), so I will just connect START directly
                    // to the LSQ in this particular case
                
                // add a new Bx instance, then connect it to START_ and the next function calls from the main should apply FPL's algorithm!
                // 1st) add a fork 
                ENode* fork = new ENode(Fork_c, "forkC", bbnode_dag->at(bb_idx)->BB); 
                fork->id = fork_id++;
                enode_dag->push_back(fork);
                // search for the Start_ node
                for(auto& cnt_node : *bbnode_dag->front()->Cntrl_nodes) {
                    if(cnt_node->type == Start_) {
                        fork->CntrlOrderPreds->push_back(cnt_node);
                        cnt_node->CntrlOrderSuccs->push_back(fork);
                        break; // found the start node, no need to continue looping
                    }
                } 
                // the first output of the fork should be the LSQ
                fork->CntrlOrderSuccs->push_back(enode);
                enode->CntrlOrderPreds->push_back(fork);

                // IN that very particular case, I do not need the buffer node
                // 2nd) add a Buffer and connect its output to sink..
                ENode* buff_node = new ENode(Buffera_, "buffA");
                buff_node->id = CircuitGenerator::buff_id++;
                enode_dag->push_back(buff_node);

                fork->CntrlOrderSuccs->push_back(buff_node);
                buff_node->CntrlOrderPreds->push_back(fork);

                ENode* sink_node = new ENode(Sink_, "sink"); 
                sink_node->id    = sink_id++;
                enode_dag->push_back(sink_node);
                buff_node->CntrlOrderSuccs->push_back(sink_node);
                sink_node->CntrlOrderPreds->push_back(buff_node);
                
                //sink_node->CntrlOrderPreds->push_back(fork);
                //fork->CntrlOrderSuccs->push_back(sink_node);
            
               

            } else {
                // loop over the memory dependencies and connect the different Bx instance 
                for(int i = 0; i < all_mem_deps.size(); i++) {
                    // add new Bx instances in the BB of the prodcuer and the consumer and connect them to eachother, then connect START to the Bx of the producer
                    // 1st consider the producer BB
                    // add a new Bx instance, then connect it to START_ and the next function calls from the main should apply FPL's algorithm!
                        // 1st) add a fork 
                    ENode* fork_prod = new ENode(Fork_c, "forkC",  enode->CntrlPreds->at(all_mem_deps.at(i).prod_idx)->BB); 
                    fork_prod->id = fork_id++;
                    enode_dag->push_back(fork_prod);
                    // search for the Start_ node
                    for(auto& cnt_node : *bbnode_dag->front()->Cntrl_nodes) {
                        if(cnt_node->type == Start_) {
                            fork_prod->CntrlOrderPreds->push_back(cnt_node);
                            cnt_node->CntrlOrderSuccs->push_back(fork_prod);
                            break; // found the start node, no need to continue looping
                        }
                    } 
                        // the first output of the fork should be the LSQ
        // TODO: URGENT! I THINK THE ORDER OF THE CONTROL TOKENS FROM DIFFERENT BBS SHOULD CORRESPOND TO THE ORDER OF THE MEMORY OPERATIONS?????
                    // CHECK AND ENSURE THIS IF NEEDED!!!!!
                    fork_prod->CntrlOrderSuccs->push_back(enode);
                    enode->CntrlOrderPreds->push_back(fork_prod);

                        // 2nd) add a Buffer and connect its output to the Bx of the consumer
                    ENode* buff_node_prod = new ENode(Buffera_, "buffA");
                    buff_node_prod->id = CircuitGenerator::buff_id++;
                    enode_dag->push_back(buff_node_prod);

                    fork_prod->CntrlOrderSuccs->push_back(buff_node_prod);
                    buff_node_prod->CntrlOrderPreds->push_back(fork_prod);

            
                    // 2nd consider the consumer BB
                     // 1st) add a fork 
                    ENode* fork_cons = new ENode(Fork_c, "forkC",  enode->CntrlPreds->at(all_mem_deps.at(i).cons_idx)->BB); 
                    fork_cons->id = fork_id++;
                    enode_dag->push_back(fork_cons);
                   
                        // the first output of the fork should be the LSQ
        // TODO: URGENT! I THINK THE ORDER OF THE CONTROL TOKENS FROM DIFFERENT BBS SHOULD CORRESPOND TO THE ORDER OF THE MEMORY OPERATIONS?????
                    // CHECK AND ENSURE THIS IF NEEDED!!!!!
                    fork_cons->CntrlOrderSuccs->push_back(enode);
                    enode->CntrlOrderPreds->push_back(fork_cons);

                        // 2nd) add a Buffer and connect its output to a sink
                    ENode* buff_node_cons = new ENode(Buffera_, "buffA");
                    buff_node_cons->id = CircuitGenerator::buff_id++;
                    enode_dag->push_back(buff_node_cons);

                    fork_cons->CntrlOrderSuccs->push_back(buff_node_cons);
                    buff_node_cons->CntrlOrderPreds->push_back(fork_cons);

                    ENode* sink_node = new ENode(Sink_, "sink"); 
                    sink_node->id    = sink_id++;
                    enode_dag->push_back(sink_node);
                    buff_node_cons->CntrlOrderSuccs->push_back(sink_node);
                    sink_node->CntrlOrderPreds->push_back(buff_node_cons);


                    // 3rd connect the producer to the consumer 
                    buff_node_prod->CntrlOrderSuccs->push_back(fork_cons);
                    fork_cons->CntrlOrderPreds->push_back(buff_node_prod);
                }

                // TODO: for now I assume that a memory operation is involved in only a single type of dependency so I blindly add new Bx instances
                    // and I don't consider the need for an Elastic Join!!

            }
        }
    } */

    /////////////////////////////////// Look at the following TODOs once done with everything above!!
    // TODO: check the rest of the functions here, do they rely on the old ordered control network or will they work fine?????
        
    // TODO: My Bx is a Fork with a Buffer on one output: Make sure that:
    // 1) it doesn't affect the Forks algorithm, 2) Do not add an extra Buffer in the smart buffers algorithm!
         
}


void CircuitGenerator::add_naive_ControlForLSQ() {
	
    ////////////////////////////////////////// Old code was used to interface the LSQ with the strictly ordered control network
    // The following code connected the LSQ to the strictly ordered control network (CntrlOrder) network
	// loop over all the enodes searching for a node of type LSQ
	for(auto & enode : *enode_dag) {
		if(enode->type == LSQ_) {
			// loop over its CntrlPreds
			for(auto & pred : *enode->CntrlPreds) {
				// search for the redun Phi_c in the BB of each preds
				for(auto & enode_2 : *enode_dag) {
					if(enode_2->BB == pred->BB && ((enode_2->type == Phi_c && enode_2->is_redunCntrlNet) || enode_2->type == Start_)) {
						// this enode_2 should be in the CntrlOrderPreds of the LSQ enode
						// first make sure that the connection is not there from before
						if(!contains(enode->CntrlOrderPreds, enode_2)) {	
							enode->CntrlOrderPreds->push_back(enode_2);
							enode_2->CntrlOrderSuccs->push_back(enode);
						} 
					}
				} 
			} 

		}
	}

}

// As of 13/09/2022: I know longer use this function!!!!!
// Aya: 21/08/2022: temporary for debugging!!
void CircuitGenerator::FixSingleBBLSQ_special() {
    // In the very end (after completing the FPL algorithm) I call this function to remove the fork (of the Bx) feeding the LSQ
        // in the special case of having all the memory deps inside a single BB!
    // To identify the special case, I call the identifyMemDeps function again

      std::vector<prod_cons_mem_dep> all_mem_deps; // one element of this should hold the index of the producer and consumer operations
            // (based on the type of memory dependency) in the CntrlPreds of the LSQ
    for(auto & enode : *enode_dag) {
        all_mem_deps.clear();
        if(enode->type == LSQ_) {
            //identifyMemDeps(enode, all_mem_deps);  

             // if the size of all_mem_deps is 0, all memory operations are in a single BB, so insert a single Bx and trigger it from START
            if(all_mem_deps.size() == 0){
                // sanity check: assert if there is no predecessor to the LSQ in the CntrlOrderPreds
                assert(enode->CntrlOrderPreds->size() == 1);  
                assert(enode->CntrlOrderPreds->at(0)->type == Fork_c);
                assert(enode->CntrlOrderPreds->at(0)->CntrlOrderSuccs->at(1)->type == Sink_);

                // delete both the Fork and the sink it feeds
                auto pos_fork = std::find(enode_dag->begin(), enode_dag->end(), enode->CntrlOrderPreds->at(0));
                assert(pos_fork != enode_dag->end());
                enode_dag->erase(pos_fork);

                auto pos_sink = std::find(enode_dag->begin(), enode_dag->end(), enode->CntrlOrderPreds->at(0)->CntrlOrderSuccs->at(1));
                assert(pos_sink != enode_dag->end());
                enode_dag->erase(pos_sink);

                 // search for the Fork in the successors of enode->CntrlOrderPreds->at(0)->CntrlOrderPreds->at(0)
                auto pos = std::find(enode->CntrlOrderPreds->at(0)->CntrlOrderPreds->at(0)->CntrlOrderSuccs->begin(), enode->CntrlOrderPreds->at(0)->CntrlOrderPreds->at(0)->CntrlOrderSuccs->end(), enode->CntrlOrderPreds->at(0));
                assert(pos != enode->CntrlOrderPreds->at(0)->CntrlOrderPreds->at(0)->CntrlOrderSuccs->end());
                int pos_idx = pos - enode->CntrlOrderPreds->at(0)->CntrlOrderPreds->at(0)->CntrlOrderSuccs->begin();

                // put the LSQ in the successors of the predecessor of the Fork in place of the Fork
                enode->CntrlOrderPreds->at(0)->CntrlOrderPreds->at(0)->CntrlOrderSuccs->at(pos_idx) = enode;
                // put the predecessor of the Fork in the predecessor of the LSQ in place of the Fork
                enode->CntrlOrderPreds->at(0) = enode->CntrlOrderPreds->at(0)->CntrlOrderPreds->at(0);

            }
        }
    }



}

// set index of BasicBlock in load/store connected to MC or LSQ
void setBBIndex(ENode* enode, BBNode* bbnode) { enode->bbId = bbnode->Idx + 1; }

// load/store port index for LSQ-MC connection
void setLSQMemPortIndex(ENode* enode, ENode* memnode) {

    assert (enode->type == LSQ_);
    int loads = 0;
    int stores = 0;
    for (auto& node : *memnode->CntrlPreds) {
	    if (enode == node)
	        break;
	    if (node->type == LSQ_){
	        loads++;
	        stores++;
	    }
	    else {
	        auto* I = node->Instr;
	        if (isa<LoadInst>(I))
	        	loads++;
	        else {
	        	assert (isa<StoreInst>(I));
	        	stores++;
	        }
	    }
	}
	enode->lsqMCLoadId = loads; 
    enode->lsqMCStoreId = stores;
}

// load/store port index for LSQ or MC
void setMemPortIndex(ENode* enode, ENode* memnode) {
    int offset = 0;
    for (auto& node : *memnode->CntrlPreds) {
        if (enode == node)
            break;
        if (!compareMemInst(enode, node))
            offset++;
    }
    enode->memPortId = offset;

    if (enode->type == LSQ_){
    	int loads = 0;
    	int stores = 0;
    	for (auto& node : *memnode->CntrlPreds) {
	        if (enode == node)
	            break;
	        if (node->type == LSQ_){
	            loads++;
	            stores++;
	        }
	        else {
	        	auto* I = node->Instr;
	        	if (isa<LoadInst>(I))
	        		loads++;
	        	else {
	        		assert (isa<StoreInst>(I));
	        		stores++;
	        	}
	        }
	    }
	    enode->lsqMCLoadId = loads; 
        enode->lsqMCStoreId = stores;
	}
}

// ld/st offset within bb (for LSQ or MC grop allocator)
void setBBOffset(ENode* enode, ENode* memnode) {
    int offset = 0;
    for (auto& node : *memnode->CntrlPreds) {
        if (enode == node)
            break;
        if (compareMemInst(enode, node) && enode->BB == node->BB)
            offset++;
    }
    enode->bbOffset = offset;
}

int getBBIndex(ENode* enode) { return enode->bbId; }

int getMemPortIndex(ENode* enode) { return enode->memPortId; }

int getBBOffset(ENode* enode) { return enode->bbOffset; }

// check if two mem instructions are different type (for offset calculation)
bool compareMemInst(ENode* enode1, ENode* enode2) {
	// When MC connected to LSQ
	// LSQ has both load and store port connected to MC 
	// Need to increase offset of both load and store
    if (enode1->type == LSQ_ || enode2->type == LSQ_)
        return false;

    auto* I1 = enode1->Instr;
    auto* I2 = enode2->Instr;

    return ((isa<LoadInst>(I1) && isa<StoreInst>(I2)) || (isa<LoadInst>(I2) && isa<StoreInst>(I1)));
}

// total count of loads connected to lsq
int getMemLoadCount(ENode* memnode) {
    int ld_count = 0;
    for (auto& enode : *memnode->CntrlPreds) {
        if (enode->type != Cst_ && enode->type != LSQ_) {
            auto* I = enode->Instr;
            if (isa<LoadInst>(I))
                ld_count++;
        }
    }
    return ld_count;
}

// total count of stores connected to lsd
int getMemStoreCount(ENode* memnode) {
    int st_count = 0;
    for (auto& enode : *memnode->CntrlPreds) {
        if (enode->type != Cst_ && enode->type != LSQ_) {
            auto* I = enode->Instr;
            if (isa<StoreInst>(I))
                st_count++;
        }
    }
    return st_count;
}

// total count of bbs connected to lsq (corresponds to control fork connection
// count)
int getMemBBCount(ENode* memnode) { 
	// return memnode->JustCntrlPreds->size();  /// Aya
	return memnode->CntrlOrderPreds->size();
}

// total input count to MC/LSQ
int getMemInputCount(ENode* memnode) {
    return getMemBBCount(memnode) + 2 * getMemStoreCount(memnode) + getMemLoadCount(memnode);
}

// total output count to MC/LSQ
int getMemOutputCount(ENode* memnode) { return getMemLoadCount(memnode); }

// adress index for dot (fork connections go first)
int getDotAdrIndex(ENode* enode, ENode* memnode) {
    int count = 0;
    for (auto& node : *memnode->CntrlPreds) {
        if (node == enode)
            break;
        auto* I = node->Instr;
        if (isa<StoreInst>(I))
            count += 2;
        else
            count++;
    }

    return getMemBBCount(memnode) + count + 1;
}

// data index for dot
int getDotDataIndex(ENode* enode, ENode* memnode) {
    auto* I = enode->Instr;
    if (isa<StoreInst>(I))
        return getDotAdrIndex(enode, memnode) + 1;
    else {
        int count = 0;

        for (auto& node : *memnode->CntrlPreds) {
            if (node == enode)
                break;
            auto* I = node->Instr;
            if (isa<LoadInst>(I))
                count++;
        }

        return count + 1;
    }
}

int getBBLdCount(ENode* enode, ENode* memnode) {
    int ld_count = 0;

    for (auto& node : *memnode->CntrlPreds) {

        if (node->type != Cst_) {
            auto* I = node->Instr;
            if (isa<LoadInst>(I) && node->BB == enode->BB)
                ld_count++;
        }
    }

    return ld_count;
}

int getBBStCount(ENode* enode, ENode* memnode) {
    int st_count = 0;

    for (auto& node : *memnode->CntrlPreds) {

        if (node->type != Cst_) {
            auto* I = node->Instr;
            if (isa<StoreInst>(I) && node->BB == enode->BB)
                st_count++;
        }
    }

    return st_count;
}

int getLSQDepth(ENode* memnode) {
    // Temporary: determine LSQ depth based on the BB with largest ld/st count
    // Does not guarantee maximal throughput!

    int d = 0;

    // for (auto& enode : *memnode->JustCntrlPreds) {  // Aya
	// Aya
	for (auto& enode : *memnode->CntrlOrderPreds) {
        BBNode* bb = enode->bbNode;

        int ld_count = getBBLdCount(enode, memnode);
        int st_count = getBBStCount(enode, memnode);

        d = (d < ld_count) ? ld_count : d;
        d = (d < st_count) ? st_count : d;
    }

    int power = 16;

    // round up to power of two
    while (power < d)
        power *= 2;

    // increase size for performance (not optimal!)
    // power *= 2;

    return power;
}
