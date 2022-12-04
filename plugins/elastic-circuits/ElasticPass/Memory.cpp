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
void CircuitGenerator::addMemoryInterfaces(const bool useLSQ, bool is_smart_cntrlOrder_flag, bool lazy_fork_flag) {
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
        add_smart_ControlForLSQ(lazy_fork_flag);
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

/**
 * @brief identifies all of the dependent memory operations for a specific LSQ enode
 * @param lsq_enode The LSQ object currently under study
 * @param all_mem_deps A vector that is filled by the function such that each element in the vector represents a pair of dependent memory operations
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


/**
 * @brief fills the group_idx field of memory operation enodes which is the BB idx of the BB that is the head of a Group
 * @param lsq_enode The lSQ object currently under study 
 * @note The function now simply assumes that a group is at most 1 BB (i.e., the BB that contains the memory operation). In the future,
 *      if we are to define groups differently such that a group can contain multiple BBs, then the only thing we need to change is how we calculate the group_idx field
 */
void CircuitGenerator::identifyGroupIdxs(ENode* lsq_enode) {
    // loop over the predecessors of the LSQ node
    for(int i = 0; i < lsq_enode->CntrlPreds->size(); i++) {
        ENode* mem_operation = lsq_enode->CntrlPreds->at(i);
        // 1st) every store operation will surely have its BB idx as its group since we can not have ST earlier than its BB!!
        if(isa<StoreInst>(mem_operation->Instr)) {
            mem_operation->group_idx = BBMap->at(mem_operation->BB)->Idx;

            // raise the flag of that BBnode!
            BBMap->at(mem_operation->BB)->is_group_head = true;

        } else {

           if(isa<LoadInst>(mem_operation->Instr)) {
                mem_operation->group_idx = BBMap->at(mem_operation->BB)->Idx;

                // raise the flag of that BBnode!
                BBMap->at(mem_operation->BB)->is_group_head = true;

                // a load can have a group_idx = BB Idx of the BB containing the latest ST operation that should happen before it if they have the
                        // same conditions of execution. And if they have different conditions of execution (i.e., different dependencies), 
                            // then put its group idx as the earliest BB idx that has the same depdendencies as the LD and comes right after the BB of the ST
                                // which by the way might end up being its BB!! :)

             /*   std::vector<int> store_bb_idxs;
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
                */
            } 
        }
    }

}


/**
 * @brief constructs a graph of groups
 * @param lsq_enode The LSQ object currently under study
 * @param all_mem_deps is a vector where a single element represents a single memory dependency that should be honored
 * @param groups the graph of groups that gets constructed inside the function 
 */
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
            if(pos == groups.end()) {  // if the group is not already found, create a new entry for it!!
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


void CircuitGenerator::OLDapplyGSA_Non_Loops_simplified(std::ofstream& dbg_file, const std::vector<Group>& groups) {
    // loop over the groups
    for(int i = 0; i < groups.size(); i++) {
          // search for the Fork instance of the Bx in this group since the predecessor should feed the Fork (i.e., if we are to insert a Phi it should the fork!!)
        ENode* consumer_Bx_fork = nullptr;
        ENode* consumer_Bx_buff = nullptr;
        for(int f = 0; f < enode_dag->size(); f++) {
            if(enode_dag->at(f)->type == LazyFork_ && enode_dag->at(f)->is_mem_dep_component && enode_dag->at(f)->BB == bbnode_dag->at(groups.at(i).first_bb_idx)->BB) {
                consumer_Bx_fork = enode_dag->at(f);

                // 18/09/2022
                // AYa: extract also the buffer_bx of this Bx component to be used in checking if GSA was previously applied between the pair of prod, cons or not (in case the Bx component is reused by multiple LSQs)
                    // search for the Buff in the successors of the fork!!
                for(int bb = 0; bb < enode_dag->at(f)->CntrlOrderSuccs->size(); bb++) {
                    if(enode_dag->at(f)->CntrlOrderSuccs->at(bb)->type == Bx_Buffer_wrapper_ && enode_dag->at(f)->CntrlOrderSuccs->at(bb)->is_mem_dep_component) {
                        consumer_Bx_buff = enode_dag->at(f)->CntrlOrderSuccs->at(bb);
                        break;
                    }
                }
                assert(consumer_Bx_buff != nullptr);

                break;
            }
        }
        assert(consumer_Bx_fork != nullptr);

        // loop over the predecessors of each group
        for(int j = 0; j < groups.at(i).pred_groups_first_bb_idx.size(); j++) {
            /*// AYA: 17/09/2022: Added the following condition to avoid applying GSA multiple times if the producer and consumer are reused as is by the same LSQ
            assert(j < consumer_Bx_buff->handled_loop_gsa_with_producer_group.size());
            if(consumer_Bx_buff->handled_loop_gsa_with_producer_group.at(j)) 
                continue;   // skip this iteration which means skipping the handling of GSA between this producer and this consumer because it was previously handled
            */

            // search for the Buff instance of the Bx in this group since if we are to insert a Phi, it should be fed by the Buff od this Bx!
            ENode* producer_Bx_buffer = nullptr;
            for(int f = 0; f < enode_dag->size(); f++) {
                if(enode_dag->at(f)->type == Bx_Buffer_wrapper_ && enode_dag->at(f)->is_mem_dep_component && enode_dag->at(f)->CntrlOrderPreds->at(0)->BB == bbnode_dag->at(groups.at(i).pred_groups_first_bb_idx.at(j))->BB) {
                    // sanity check, the successor of this buffer must be a Fork!!!
                    assert(enode_dag->at(f)->CntrlOrderPreds->at(0)->type == LazyFork_ && enode_dag->at(f)->CntrlOrderPreds->at(0)->is_mem_dep_component);
                    producer_Bx_buffer = enode_dag->at(f);
                    break;
                }
            }
            assert(producer_Bx_buffer != nullptr);


            // NOW: get the conditions of production and consumption!!!
            std::vector<int> modif_consBB_deps;
            std::vector<int> modif_prodBB_deps;
            eliminateCommonControlAncestors(dbg_file, BBMap->at(consumer_Bx_fork->BB), BBMap->at(producer_Bx_buffer->BB), modif_consBB_deps, modif_prodBB_deps); 
            std::vector<std::vector<pierCondition>> f_cons;
            constructSOP(dbg_file, bbnode_dag->at(0), BBMap->at(consumer_Bx_fork->BB), modif_consBB_deps, f_cons); 
            std::vector<std::vector<pierCondition>> f_prod;
            constructSOP(dbg_file, bbnode_dag->at(0), BBMap->at(producer_Bx_buffer->BB), modif_prodBB_deps, f_prod);


            std::vector<std::vector<pierCondition>> f_gen;
            bool generate_flag;
            generate_flag = calculate_F_Gen(dbg_file, f_prod, f_cons, f_gen);
            dbg_file << "\n\n###########################################################\n\n";
            dbg_file << "\n\n\tLet's check what came out!!\n\n";
            if(generate_flag) {// add a new MUX with the condition f_gen!
                dbg_file << "\nNeed to insert a MUX with: ";
                if(f_gen.size()==1) {
                    if(f_gen.at(0).size() == 1) {
                        dbg_file << " condition = " << f_gen.at(0).at(0).pierBB_index + 1 << "\n";
                    } else {
                        dbg_file << " a complicated condition!!\n";
                    }
                } else {
                    dbg_file << " a complicated condition!!\n";
                }
            } else {
                dbg_file << "\nNo need to insert a GENERATE!!\n";
            }


            if(generate_flag) {  // if the generate condition is not false!
                bool is_negated_cond;
                dbg_file << "\n\tRight before calling applyShannon, the size of f_gen is: " << f_gen.size() << "\n";
                ENode* gen_condition_enode = applyShannon(dbg_file, dbg_file, producer_Bx_buffer->BB, f_gen, is_negated_cond);

                // create a new phiC MUX and insert it between the producer and consumer, and connect its second input a Branch fed with a START 
                ENode* gsa_mux = new ENode(Phi_c, "phiC", producer_Bx_buffer->BB);  // we insert the GENERATE in the consumer BB 
                gsa_mux->id = phi_id++;

                // I commented this because I don't want it to be skipped in the AddSuppress!!
                // gsa_mux->is_mem_dep_component = true;  
                gsa_mux->isMux = true;
                gsa_mux->is_non_loop_gsa_mux = true;

                gsa_mux->is_advanced_component = true;
                // condition should be negated or not depending on is_negated_cond
                gsa_mux->is_negated_input->push_back(is_negated_cond); 
                // the other 2 inputs of the enode MUX shouldn't be negated!!
                gsa_mux->is_negated_input->push_back(false);
                gsa_mux->is_negated_input->push_back(false);

                enode_dag->push_back(gsa_mux);

                // convert it to MUX and manage its inputs!
                // the .at(0) of the CntrlPreds(data network!!) the MUX is the condition and it should be the gen_condition_enode
                gsa_mux->CntrlPreds->push_back(gen_condition_enode);
                gen_condition_enode->CntrlSuccs->push_back(gsa_mux);

                // the .at(0) (i.e., when the condition is false) of the CntrlOrderPreds should be the original producer
                gsa_mux->CntrlOrderPreds->push_back(producer_Bx_buffer);
                gsa_mux->CntrlOrderSuccs->push_back(consumer_Bx_fork);
                // identify the index of the original producer in the CntrlOrderPreds of the original consumer and the original consumer in the CntrlOrderSuccs of the original producer
                int idx_prod_in_cons_pred = returnPosRemoveNode(consumer_Bx_fork->CntrlOrderPreds, producer_Bx_buffer); // this function only finds the index of the passed element in the passed vector!
                int idx_cons_in_prod_succ = returnPosRemoveNode(producer_Bx_buffer->CntrlOrderSuccs, consumer_Bx_fork);
                consumer_Bx_fork->CntrlOrderPreds->at(idx_prod_in_cons_pred) = gsa_mux;
                producer_Bx_buffer->CntrlOrderSuccs->at(idx_cons_in_prod_succ) = gsa_mux;

                // connect the 2nd input of the gsa_mux directly to START_!
                for(auto& cnt_node : *bbnode_dag->front()->Cntrl_nodes) {
                    if(cnt_node->type == Start_) {
                        gsa_mux->CntrlOrderPreds->push_back(cnt_node);
                        cnt_node->CntrlOrderSuccs->push_back(gsa_mux);
                        break; // found the start node, no need to continue looping
                    }
                } 

                // the .at(1) (i.e., when the condition is true) of the CntrlOrderSuccs should be a Branch fed with START!
                /*ENode* gsa_branch = new ENode(Branch_c, "branchC", consumer_Bx_fork->BB);  // we insert the BRANCH also in the consumer BB 
                gsa_branch->id = branch_id++;

                gsa_branch->is_mem_dep_component = true;

                ///////////////////////////////////
                gsa_branch->is_advanced_component = true;
                // the data input of the branch is not negated
                gsa_branch->is_negated_input->push_back(false); 
                // the passed is_negated_cond suggests if the condition of the Branch should be negated or not
                gsa_branch->is_negated_input->push_back(is_negated_cond);
                /////////////////////////////////////////

                enode_dag->push_back(gsa_branch);

                // the .at(0) of the CntrlPreds is the condition of the branch
                gsa_branch->CntrlPreds->push_back(gen_condition_enode);
                gen_condition_enode->CntrlSuccs->push_back(gsa_branch);

                // the data input of the branch is START!
                for(auto& cnt_node : *bbnode_dag->front()->Cntrl_nodes) {
                    if(cnt_node->type == Start_) {
                        gsa_branch->CntrlOrderPreds->push_back(cnt_node);
                        cnt_node->CntrlOrderSuccs->push_back(gsa_branch);
                        break; // found the start node, no need to continue looping
                    }
                } 

                // the SUCCS of the branch is the gsa_mux, the second SUCC should be a sink that will be inserted automatically later in addFork!
                gsa_mux->CntrlOrderPreds->push_back(gsa_branch);
                gsa_branch->true_branch_succs_Ctrl_lsq->push_back(gsa_mux); */ 
                
            }
          
        }


    }
} 


void CircuitGenerator::applyGSA_Loops_simplified(std::ofstream& dbg_file, ENode* lsq_enode, std::vector<Group>& groups, bool lazy_fork_flag) {
    node_type fork_type; // shall we create a Fork_c or LazyFork_
    if(lazy_fork_flag) {
        fork_type = LazyFork_;
    } else {
        fork_type = Fork_c;
    }
    // loop over the groups
    for(int i = 0; i < groups.size(); i++) {
         // search for the Fork instance of the Bx in this group since phis will feed the fork of this Bx
        ENode* consumer_Bx_fork = nullptr;
        //ENode* consumer_Bx_buff = nullptr;
        for(int f = 0; f < enode_dag->size(); f++) {
            if(enode_dag->at(f)->type == fork_type && enode_dag->at(f)->is_mem_dep_component && enode_dag->at(f)->my_lsq_enode == lsq_enode && enode_dag->at(f)->BB == bbnode_dag->at(groups.at(i).first_bb_idx)->BB) {
                consumer_Bx_fork = enode_dag->at(f);

                /*// 18/09/2022
                // AYa: extract also the buffer_bx of this Bx component to be used in checking if GSA was previously applied between the pair of prod, cons or not (in case the Bx component is reused by multiple LSQs)
                    // search for the Buff in the successors of the fork!!
                for(int bb = 0; bb < enode_dag->at(f)->CntrlOrderSuccs->size(); bb++) {
                    if(enode_dag->at(f)->CntrlOrderSuccs->at(bb)->type == Bx_Buffer_wrapper_ && enode_dag->at(f)->CntrlOrderSuccs->at(bb)->is_mem_dep_component && enode_dag->at(f)->CntrlOrderSuccs->at(bb)->my_lsq_enode == lsq_enode) {
                        consumer_Bx_buff = enode_dag->at(f)->CntrlOrderSuccs->at(bb);
                        break;
                    }
                }
                assert(consumer_Bx_buff != nullptr);*/

                break;
            }
        }
        assert(consumer_Bx_fork != nullptr);

        // loop over the predecessors of each group
        for(int j = 0; j < groups.at(i).pred_groups_first_bb_idx.size(); j++) {

            // search for the Buff instance of the Bx in this group since if we are to insert a Phi, it should be fed by the Buff of this Bx!
            ENode* producer_Bx_buffer = nullptr;
            for(int f = 0; f < enode_dag->size(); f++) {
                if(enode_dag->at(f)->type == Bx_Buffer_wrapper_ && enode_dag->at(f)->is_mem_dep_component && enode_dag->at(f)->my_lsq_enode == lsq_enode && enode_dag->at(f)->CntrlOrderPreds->at(0)->BB == bbnode_dag->at(groups.at(i).pred_groups_first_bb_idx.at(j))->BB) {
                    // sanity check, the successor of this buffer must be a Fork!!!
                    assert(enode_dag->at(f)->CntrlOrderPreds->at(0)->type == fork_type && enode_dag->at(f)->CntrlOrderPreds->at(0)->is_mem_dep_component);
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

                    phi->orig_bx_buff_producer = producer_Bx_buffer;
                    phi->orig_bx_fork_consumer = consumer_Bx_fork;

                    dbg_file << "\n\n\tFrom inside applyGSA_Loops_simplified, just added a PHi_c in BB" << BBMap->at(common_loop_headers.at(k)->BB)->Idx + 1 << "\n";

                    // converting them to MUXes will be managed by FPL's inside the addSuppress stuff!
                }

                // 2nd) Cut the connection between the original producer and consumer Bx!
                // check because the non-loop GSA might have inserted a MUX between the producer and the consumer, in which case, I should do all my analysis as before but when it comes to inserting new components, make sure to be fed from the MUX not from the original producer!
                // search for a MUX in the succs of the producer

                // 31/10/2022: added extra logic to check if there has been a phi_C inserted in the non-loop GSA, in which case, we should cut the connection between this phi_C and the consumer Bx (not between the original producer Bx and the consumer Bx!!)
                ENode* actual_prod = nullptr; // could be either the original producer_Bx_buffer or a phi_c inserted by the non-loop GSA!!
                int idx_cons_in_prod_succ = -1;
                int idx_prod_in_cons_pred = -1;
                auto pos_cons_in_prod_succ = std::find(producer_Bx_buffer->CntrlOrderSuccs->begin(), producer_Bx_buffer->CntrlOrderSuccs->end(), consumer_Bx_fork);
                if(pos_cons_in_prod_succ == producer_Bx_buffer->CntrlOrderSuccs->end()) {
                    bool found_gsa_mux = false;
                    // sanity checks: make sure then that there must be a Phi_c inserted by the non loop GSA between the original producer and consumer!!
                    assert(producer_Bx_buffer->CntrlOrderSuccs->size() > 0);
                    // one of the successors of the producer_Bx_buffer should be of type Phi_c and should have the original consuemr as its successor!!
                    for(int r = 0; r < producer_Bx_buffer->CntrlOrderSuccs->size(); r++) {
                        assert(producer_Bx_buffer->CntrlOrderSuccs->at(r)->type == Phi_c || producer_Bx_buffer->CntrlOrderSuccs->at(r)->type == fork_type);
                        if(producer_Bx_buffer->CntrlOrderSuccs->at(r)->type == Phi_c && producer_Bx_buffer->CntrlOrderSuccs->at(r)->is_non_loop_gsa_mux) {
                            ENode* gsa_mux = producer_Bx_buffer->CntrlOrderSuccs->at(r);
                            // search for the original consumer in its succs!
                            auto pos_cons_in_phi = std::find(gsa_mux->CntrlOrderSuccs->begin(), gsa_mux->CntrlOrderSuccs->end(), consumer_Bx_fork);
                            if(pos_cons_in_phi != gsa_mux->CntrlOrderSuccs->end()) {
                               found_gsa_mux = true;
                               actual_prod = gsa_mux;
                               idx_cons_in_prod_succ = pos_cons_in_phi - gsa_mux->CntrlOrderSuccs->begin();
                            }
                        }
                    }
                    assert(found_gsa_mux == true);
                } else {
                    // the consumer is directly connected to the producer (i.e., the non-loop GSA didn't insert any MUX)
                    actual_prod = producer_Bx_buffer;
                    idx_cons_in_prod_succ = pos_cons_in_prod_succ - producer_Bx_buffer->CntrlOrderSuccs->begin();
                }
                assert(actual_prod!=nullptr);
                assert(idx_cons_in_prod_succ != -1);
                groups.at(i).actual_producers.push_back(actual_prod);


                // identify the index of the actual_prod in the preds of the consumer!
                auto pos_prod_in_cons_pred = std::find(consumer_Bx_fork->CntrlOrderPreds->begin(), consumer_Bx_fork->CntrlOrderPreds->end(), actual_prod);
                assert(pos_prod_in_cons_pred != consumer_Bx_fork->CntrlOrderPreds->end());
                idx_prod_in_cons_pred = pos_prod_in_cons_pred - consumer_Bx_fork->CntrlOrderPreds->begin();

                /*bool found_gsa_mux = false;
                int idx_prod_in_cons_pred = -1;
                int idx_cons_in_prod_succ = -1;
                ENode* gsa_mux =  nullptr;
                for(int rr = 0; rr < producer_Bx_buffer->CntrlOrderSuccs->size(); rr++) {
                    // check that it is a MUX and that it is in the same BB as that of the consumer I currently have!
                    if(producer_Bx_buffer->CntrlOrderSuccs->at(rr)->type == Phi_c && producer_Bx_buffer->CntrlOrderSuccs->at(rr)->isMux && producer_Bx_buffer->CntrlOrderSuccs->at(rr)->is_non_loop_gsa_mux && producer_Bx_buffer->CntrlOrderSuccs->at(rr)->BB == producer_Bx_buffer->BB) {
                        // need to make sure that this potential_gsa_mux has the original consumer as its successor!!
                        ENode* potential_gsa_mux = producer_Bx_buffer->CntrlOrderSuccs->at(rr);
                        auto pos____ = std::find(potential_gsa_mux->CntrlOrderSuccs->begin(), potential_gsa_mux->CntrlOrderSuccs->end(), consumer_Bx_fork);
                        if(pos____ != potential_gsa_mux->CntrlOrderSuccs->end()) {
                            gsa_mux = producer_Bx_buffer->CntrlOrderSuccs->at(rr);
                            // sanity checks: the consumer must have this mux in its predecessor!!!
                            auto pos_ = std::find(consumer_Bx_fork->CntrlOrderPreds->begin(), consumer_Bx_fork->CntrlOrderPreds->end(), gsa_mux);
                            assert(pos_ != consumer_Bx_fork->CntrlOrderPreds->end());
                            idx_prod_in_cons_pred = pos_ - consumer_Bx_fork->CntrlOrderPreds->begin();

                            auto pos__ = std::find(gsa_mux->CntrlOrderSuccs->begin(), gsa_mux->CntrlOrderSuccs->end(), consumer_Bx_fork);
                            assert(pos__!= gsa_mux->CntrlOrderSuccs->end());
                            idx_cons_in_prod_succ = pos__ - gsa_mux->CntrlOrderSuccs->begin();

                            found_gsa_mux = true;
                            break;
                        }
                    }
                }
                
                if(!found_gsa_mux) {
                    idx_prod_in_cons_pred = returnPosRemoveNode(consumer_Bx_fork->CntrlOrderPreds, producer_Bx_buffer); // this function only finds the index of the passed element in the passed vector!
                    idx_cons_in_prod_succ = returnPosRemoveNode(producer_Bx_buffer->CntrlOrderSuccs, consumer_Bx_fork);
                }
                assert(idx_prod_in_cons_pred != -1 && idx_cons_in_prod_succ != -1);

                // sanity check
                if(found_gsa_mux) {
                    assert(gsa_mux!=nullptr);
                }
                */

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

                actual_prod->CntrlOrderSuccs->at(idx_cons_in_prod_succ) = phis.at(0);
                phis.at(0)->CntrlOrderPreds->push_back(actual_prod);
                // the 2nd input of this phi should be the original producer!
                /*if(found_gsa_mux) {  // in this case the producer is the gsa_mux not the original producer!!!!
                    gsa_mux->CntrlOrderSuccs->at(idx_cons_in_prod_succ) = phis.at(0);
                    phis.at(0)->CntrlOrderPreds->push_back(gsa_mux);
                } else {
                    producer_Bx_buffer->CntrlOrderSuccs->at(idx_cons_in_prod_succ) = phis.at(0);
                    phis.at(0)->CntrlOrderPreds->push_back(producer_Bx_buffer);
                }*/
              
                // do the chain of connections between the different Phis that may exist!
                for(int k = 1; k < phis.size(); k++) {
                    // the first input of any phi (apart from the 0th which we handled above) should be from the previous phi corresponding to a loop that is one level outer to me
                    phis.at(k)->CntrlOrderPreds->push_back(phis.at(k-1));
                    phis.at(k-1)->CntrlOrderSuccs->push_back(phis.at(k));

                    // the second input of ALL phis should be from the producer!!
                    /*if(found_gsa_mux) {
                        phis.at(k)->CntrlOrderPreds->push_back(gsa_mux);
                        gsa_mux->CntrlOrderSuccs->push_back(phis.at(k)); 
                    } else {
                        phis.at(k)->CntrlOrderPreds->push_back(producer_Bx_buffer);
                        producer_Bx_buffer->CntrlOrderSuccs->push_back(phis.at(k)); 
                    }*/
                    phis.at(k)->CntrlOrderPreds->push_back(actual_prod);
                    actual_prod->CntrlOrderSuccs->push_back(phis.at(k)); 
                    
                }
                // the phis.at(phis.size()-1) should feed the original consumer and be fed normally from the previous PHI as before inside the loop
                phis.at(phis.size()-1)->CntrlOrderSuccs->push_back(consumer_Bx_fork);
                consumer_Bx_fork->CntrlOrderPreds->at(idx_prod_in_cons_pred) = phis.at(phis.size()-1);
                ///////////////////////

                // 4th) Push all of the newly created phis objects to the enode_dag
                for(int k = 0; k < phis.size(); k++) {
                    enode_dag->push_back(phis.at(k));
                }

            } 

            // 18/09/2022: mark it as handled!
                // I removed this to happen inside addLoopBranches_postGSA instead!!!
            // consumer_Bx_buff->handled_loop_gsa_with_producer_group.at(j) = true;  
        }
    }

}


ENode* CircuitGenerator::addLoopBranches_postGSA_singleLoop(std::ofstream& dbg_file, ENode* phi_node, ENode* producer) {
    // The goal is to 1) Insert a Branch between the Phi_node and the producer, 2) Convert the Phi_node to a MUX and add an INIT component for it!!
    ENode* consumer = phi_node;
    // sanity check: phi_node must be in a loop header!!
    assert(BBMap->at(phi_node->BB)->is_loop_header);

    // I will do it the same way we reason in FPL and will reuse some of the functions that are defined and/or used inside addSuppress.cpp!!
    // 1st) get the conditions of the loop exits (may be just one or more if the loop has multiple exits!)
    std::vector<int> loop_exits;//modif_consBB_deps;
    //addLoopsExitstoDeps_Phi_regeneration(consumer, producer, modif_consBB_deps);  // this function returns the loops exits of all loops enclosing its first parameter!! It does not use its second parameter in anything!
    std::vector<BBNode*> one_loop_exits;
    FindLoopExits(BBMap->at(phi_node->BB)->loop, one_loop_exits);

    // push back all of the loop exit BBs of the one_loop_exits
    for(int k = 0; k < one_loop_exits.size(); k++) {
        loop_exits.push_back(one_loop_exits.at(k)->Idx);
    }


    dbg_file << "\n\tPrinting the indices of the loop's condition!\n\t";
    if(loop_exits.size() == 0) {
        dbg_file << "NO loop conditions were found!!!\n";
    }
    for(int i = 0; i < loop_exits.size(); i++) {
        dbg_file << "BB" << loop_exits.at(i) + 1 << ", ";
    }
    dbg_file << "\n";

    // 2nd) generate a boolean expression out of the loop exit conditions
    std::vector<std::vector<pierCondition>> br_cond;
    if(loop_exits.size() > 0)  // since the producer might be in a loop tail (not a loop exit) and its path to the consumer might not be including any loop exit!!!
        constructSOP(dbg_file, BBMap->at(producer->BB), BBMap->at(consumer->BB), loop_exits, br_cond);

    // 3rd) do some checks/tweaks and then insert the branch and convert to MUX!
    ////////////////////////////////////////////////////////////////////////////////////////////
    // 26/06/2022: do a check on br_cond to make sure that it includes the loop exit condition only once if the loop has a single loop exit!!
    if(loop_exits.size() == 1) {
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

    // 15/09/2022: added logic to make sure to insert the Branch and any MUXes associated with its condition (if any) in the loop tail of the loop!!
        // I chose loop tail to not restrict myself to any loop exit which will fail if the loop has multiple loop exits!
    // Search for the loop tail BB of the loop having its loop exits is modif_consBB_deps
    Loop* our_loop = bbnode_dag->at(loop_exits.at(0))->loop;
    // sanity check: we are tackling here a single outer loop so even if it has multiple exits, they must all be pointing to this loop!!
    for(int k = 0; k < loop_exits.size(); k++) {
        assert(bbnode_dag->at(loop_exits.at(k))->loop == our_loop);
    }
    // NOw let's search for the loop tail of our_loop
    BBNode* our_target_bb = nullptr;
    for(int k = 0; k < bbnode_dag->size(); k++) {
        if(bbnode_dag->at(k)->loop == our_loop && bbnode_dag->at(k)->is_loop_latch) {
            our_target_bb = bbnode_dag->at(k);
            break;
        }
    }
    assert(our_target_bb != nullptr);


    bool is_negated_cond;
    ENode* added_branch = nullptr;
    // AYA: 23/08/2022: enclosed the following statements inside this condition because we should not do anything if there are no dependencies!!
    if(loop_exits.size() > 0) {
        ENode* supp_condition_enode = applyShannon(dbg_file, dbg_file, our_target_bb->BB, br_cond, is_negated_cond); // takes the our_target_bb->BB because this will be the targetBB (i.e., the BB that we will insert the Branch in!!!!) 
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
        added_branch = consumer->CntrlOrderPreds->at(1);
        added_branch->is_mem_dep_component = true;

        // 15/09/2022: the above function (insertBranch) will by default insert the branch in the BB of the producer, but here the producer is a Bx that could be inside an inner loop and I want my branch to be inserted in the outer loop!! 
            // So, I should overwrite the BB field of the inserted branch to be in our_target_bb
        added_branch->BB = our_target_bb->BB;
        added_branch->orig_bx_buff_producer = phi_node->orig_bx_buff_producer;
        added_branch->orig_bx_fork_consumer = phi_node->orig_bx_fork_consumer;

        // we should convert it to MUX and "supp_condition_enode" should serve as the SEL of the MUX (either as is or negated!)
        assert(!consumer->isMux);
        // the following function converts the Phi enode to a MUX 
        convertToMux(CircuitGenerator::memDeps, consumer, supp_condition_enode, is_negated_cond);
        
    }                           
    ////////////////////////////////////////////////////////////////////////////////////////////
    // sanity check
    assert(added_branch != nullptr);
    return added_branch;

}

void CircuitGenerator::addLoopBranches_postGSA(std::ofstream& dbg_file, ENode* lsq_enode, const std::vector<Group>& groups, bool lazy_fork_flag) {
    node_type fork_type; // shall we create a Fork_c or LazyFork_
    if(lazy_fork_flag) {
        fork_type = LazyFork_;
    } else {
        fork_type = Fork_c;
    }
    // loop over the first BB of the different Bxs and identify the Bx components that have one or more Phi_c components inserted between them!
        // Specficially, in case of a common loop-nest, we will have 1 Phi_c component inserted in the middle between the 2 Bx and this Phi_c will be fed with a chain of other Phi_c!!

    for(int i = 0; i < groups.size(); i++) {
        // search for the Fork instance of the Bx in this group since the predecessor should feed the Fork (i.e., if we are to insert a Phi it should the fork!!)
        ENode* consumer_Bx_fork = nullptr;
       // ENode* consumer_Bx_buff = nullptr;
        for(int f = 0; f < enode_dag->size(); f++) {
            if(enode_dag->at(f)->type == fork_type && enode_dag->at(f)->is_mem_dep_component && enode_dag->at(f)->my_lsq_enode == lsq_enode && enode_dag->at(f)->BB == bbnode_dag->at(groups.at(i).first_bb_idx)->BB) {
                consumer_Bx_fork = enode_dag->at(f);

                /*// 18/09/2022
                // AYa: extract also the buffer_bx of this Bx component to be used in checking if GSA was previously applied between the pair of prod, cons or not (in case the Bx component is reused by multiple LSQs)
                    // search for the Buff in the successors of the fork!!
                for(int bb = 0; bb < enode_dag->at(f)->CntrlOrderSuccs->size(); bb++) {
                    if(enode_dag->at(f)->CntrlOrderSuccs->at(bb)->type == Bx_Buffer_wrapper_ && enode_dag->at(f)->CntrlOrderSuccs->at(bb)->is_mem_dep_component && enode_dag->at(f)->CntrlOrderSuccs->at(bb)->my_lsq_enode == lsq_enode) {
                        consumer_Bx_buff = enode_dag->at(f)->CntrlOrderSuccs->at(bb);
                        break;
                    }
                }
                assert(consumer_Bx_buff != nullptr);*/

                break;
            }
        }
        assert(consumer_Bx_fork != nullptr);

        // loop over the predecessors of each group
        for(int j = 0; j < groups.at(i).pred_groups_first_bb_idx.size(); j++) {

            // 31/10/2022: I no longer need to search for the producer_Bx_buffer because I rely on the actual_producers field of groups that  is filled by the applyGSA_Loops_simplified function 
            /*// search for the Buff instance of the Bx in this group since if we are to insert a Phi, it should be fed by the Buff od this Bx!
            ENode* producer_Bx_buffer = nullptr;
            for(int f = 0; f < enode_dag->size(); f++) {
                if(enode_dag->at(f)->type == Bx_Buffer_wrapper_ && enode_dag->at(f)->is_mem_dep_component && enode_dag->at(f)->my_lsq_enode == lsq_enode && enode_dag->at(f)->CntrlOrderPreds->at(0)->BB == bbnode_dag->at(groups.at(i).pred_groups_first_bb_idx.at(j))->BB) {
                    // sanity check, the successor of this buffer must be a Fork!!!
                    assert(enode_dag->at(f)->CntrlOrderPreds->at(0)->type == Fork_c && enode_dag->at(f)->CntrlOrderPreds->at(0)->is_mem_dep_component);
                    producer_Bx_buffer = enode_dag->at(f);
                    break;
                }
            }*/

            // now I have the producer component being producer_Bx_buffer and the consumer component being consumer_Bx_fork, check if a PHi_c has been inserted between them to account for common loops!
                // I will declare an enode to hold the producer which is either the original producer_Bx_buffer or a gsa_mux inserted by the non loop gsa function
            /*ENode* my_producer = nullptr;
            bool found_gsa_mux = false;
            ENode* gsa_mux =  nullptr;
            for(int rr = 0; rr < producer_Bx_buffer->CntrlOrderSuccs->size(); rr++) {
                // check that it is a MUX and that it is in the same BB as that of the consumer I currently have!
                if(producer_Bx_buffer->CntrlOrderSuccs->at(rr)->type == Phi_c && producer_Bx_buffer->CntrlOrderSuccs->at(rr)->isMux && producer_Bx_buffer->CntrlOrderSuccs->at(rr)->is_non_loop_gsa_mux && producer_Bx_buffer->CntrlOrderSuccs->at(rr)->BB == producer_Bx_buffer->BB) {
                    // check that this gsa_mux is feeding my consumer!!
                    ENode* potential_gsa_mux = producer_Bx_buffer->CntrlOrderSuccs->at(rr);
                    // search for the consumer in the direct successors of the potential_gsa_mux or in the successors of its phi_c successors!!
                    auto pos__ = std::find(potential_gsa_mux->CntrlOrderSuccs->begin(), potential_gsa_mux->CntrlOrderSuccs->end(), consumer_Bx_fork);  
                    if(pos__ != potential_gsa_mux->CntrlOrderSuccs->end()) {
                        gsa_mux = producer_Bx_buffer->CntrlOrderSuccs->at(rr);
                        found_gsa_mux = true;
                        break;
                    } else {
                        // search for the consumer in the succs of the phi_c succs of my potential_gsa_mux!
                        for(int gg = 0; gg < potential_gsa_mux->CntrlOrderSuccs->size(); gg++) {
                            if(potential_gsa_mux->CntrlOrderSuccs->at(gg)->type == Phi_c && potential_gsa_mux->CntrlOrderSuccs->at(gg)->is_mem_dep_component) {
                                auto pos__ = std::find(potential_gsa_mux->CntrlOrderSuccs->at(gg)->CntrlOrderSuccs->begin(), potential_gsa_mux->CntrlOrderSuccs->at(gg)->CntrlOrderSuccs->end(), consumer_Bx_fork);  
                                if(pos__ != potential_gsa_mux->CntrlOrderSuccs->at(gg)->CntrlOrderSuccs->end()) {
                                    gsa_mux = producer_Bx_buffer->CntrlOrderSuccs->at(rr);
                                    found_gsa_mux = true;
                                    break;
                                }
                            }
                        }
                    } 
                }
            }

            if(found_gsa_mux) {
                assert(gsa_mux!=nullptr);
                my_producer = gsa_mux;
            } else {
                my_producer = producer_Bx_buffer;
            }

            assert(my_producer!=nullptr); */
            ///////////////////////////////////////////////////////
            // I no longer need the above commented logic because I rely on the new actual_producers field of groups that is filled by the applyGSA_Loops_simplified function holding the producer (whether it is the original bx_buffer or a non-loop-gsa MUX!) 


            if(groups.at(i).first_bb_idx < groups.at(i).pred_groups_first_bb_idx.at(j)) {
                assert(groups.at(i).actual_producers.size() > j);
                ENode* actual_producer = groups.at(i).actual_producers.at(j);
                // 1) search for a Phi_c in the preds of the producer 
                for(int u = 0; u < actual_producer->CntrlOrderSuccs->size(); u++) {
                    if(actual_producer->CntrlOrderSuccs->at(u)->type == Phi_c) {
                        assert(actual_producer->CntrlOrderSuccs->at(u)->is_mem_dep_component && !actual_producer->CntrlOrderSuccs->at(u)->is_non_loop_gsa_mux);

                        // 2) check if this Phi_c is the one intermediate between the producer and consumer I currently have
                            // This check is IMP because in a general case, the producer could be having multiple preds where multiple of them could be a Phi_c but not outgoing to the specific consumer I'm currently working on
                                // search for this Phi_c in the predecessors of the consumer I currently have!
                        auto pos = std::find(consumer_Bx_fork->CntrlOrderPreds->begin(), consumer_Bx_fork->CntrlOrderPreds->end(), actual_producer->CntrlOrderSuccs->at(u));
                        if(pos != consumer_Bx_fork->CntrlOrderPreds->end()) {
                            // SO, this Phi_c is for sure inserted between the producer and consumer I currently have!
                            ENode* innermost_phi = actual_producer->CntrlOrderSuccs->at(u);
                             // sanity check: I always insert the producer Bx as the second (.at(1)) predecessor of a Phi_c
                            assert(innermost_phi->CntrlOrderPreds->size() == 2);
                            assert(innermost_phi->CntrlOrderPreds->at(1) == actual_producer);

                            // 3) check if this PHi_c is being fed by a chain of other Phi_c corresponding to a loop-nest or if it is the only one and is thus fed from START
                           //if(innermost_phi->CntrlOrderPreds->at(0)->type == Start_) {
                           //     addLoopBranches_postGSA_singleLoop(dbg_file, innermost_phi, producer_Bx_buffer);
                           //} else {
                                // this means that there is a chain of Phis between the producer and the consumer. The chain could be made of just 2 phis (i.e., the innermost_phi and its predecessor) or could be made of more
                                    // loop over the chain!
                                ENode* current_phi = innermost_phi;
                                ENode* last_branch = actual_producer; // later this will be overwritten with branches in the chain!
                                
                                while(current_phi->type != Start_) {
                                    // check that the predecessor of the innermost phi must be of type Phi_c and the other predecessor is the original producer!
                                    assert(current_phi->CntrlOrderPreds->at(0)->type == Phi_c || current_phi->CntrlOrderPreds->at(0)->type == Start_);
                                    assert(current_phi->CntrlOrderPreds->at(1) == actual_producer);
                                    // Also, check that this phi_c must be another successor of the original producer!
                                    auto pos_ = std::find(actual_producer->CntrlOrderSuccs->begin(), actual_producer->CntrlOrderSuccs->end(), current_phi);
                                    assert(pos_ != actual_producer->CntrlOrderSuccs->end());

                                    if(last_branch != actual_producer) {  // exception for the innermost loop where the current_branch is actually the original producer!!
                                        actual_producer->CntrlOrderSuccs->erase(pos_);
                                        current_phi->CntrlOrderPreds->erase(1 + current_phi->CntrlOrderPreds->begin());

                                        // add a connection between the last_branch and the current_phi!
                                        assert(current_phi->CntrlOrderPreds->size() == 1);
                                        current_phi->CntrlOrderPreds->push_back(last_branch);

                                        assert(last_branch->type == Branch_c && (last_branch->true_branch_succs_Ctrl_lsq->size() > 0 ||  last_branch->false_branch_succs_Ctrl_lsq->size() > 0));
                                        if(last_branch->true_branch_succs_Ctrl_lsq->size() > 0) {
                                            assert(last_branch->false_branch_succs_Ctrl_lsq->size() == 0);
                                            last_branch->false_branch_succs_Ctrl_lsq->push_back(current_phi);
                                        } else {
                                            assert(last_branch->false_branch_succs_Ctrl_lsq->size() > 0);
                                            last_branch->true_branch_succs_Ctrl_lsq->push_back(current_phi);
                                        }
                                    }
                                  
                                    // the following function inserts a Branch between the passed phi and the passed producer in the loop tail of the passed phi and converts the phi to a MUX and adds its necessary INIT component
                                        // and it returns this Branch object 
                                    last_branch = addLoopBranches_postGSA_singleLoop(dbg_file, current_phi, last_branch);

                                    // because we converted the phi to a MUX inside the previous function, it might be the case that the order of its inputs was adjusted depending on the sign of the condition, so we need to check which of CntrlOrderPreds->at(0) and CntrlOrderPreds->at(1) to pass for the next iteration!
                                    if(current_phi->CntrlOrderPreds->at(0)->type == Branch_c) {
                                        assert(current_phi->CntrlOrderPreds->at(0)->is_mem_dep_component);
                                        assert(current_phi->CntrlOrderPreds->at(1)->type != Branch_c);
                                        current_phi = current_phi->CntrlOrderPreds->at(1);
                                    } else {
                                        assert(current_phi->CntrlOrderPreds->at(1)->type == Branch_c);
                                        current_phi = current_phi->CntrlOrderPreds->at(0);
                                    }
                                   
                                }

                            //}                     
                        }
                    }
                }
                
            }

            // I no longer need this because I decided to not care for area-optimizing by checking if a Bx is already present in a specific BB by a previous LSQ object analysis
            // 18/09/2022: mark it as handled!
            //consumer_Bx_buff->handled_loop_gsa_with_producer_group.at(j) = true;  

        }
        // assertion: sanity check: if the input to this intermediate MUX is another MUX (i.e., the case of common nested loops), the consumer must be feeding the input of this second MUX!!!

    }
}

/**
 * @brief create new Bx instances, 1 for each group and connect different groups the same way they are connected in the graph of groups
 * @note This function translates each group into a new Bx component without checking if there is an already present Bx in the same BB that could have been
 *      inserted by the analysis of another LSQ object.. IN other words, the analysis of each LSQ is separate
 */
void CircuitGenerator::translateGroups_toCircuit(ENode* lsq_enode, const std::vector<Group>& groups, bool lazy_fork_flag) {
    node_type fork_type; // shall we create a Fork_c or LazyFork_
    std::string fork_name; 

    if(lazy_fork_flag) {
        fork_type = LazyFork_;
        fork_name = "LazyFork";
    } else {
        fork_type = Fork_c;
        fork_name = "forkC";
    }

    // loop over the groups
    for(int i = 0; i < groups.size(); i++) {
        // add 1 Bx component instance in each group and connect the first output of the fork to the buffer and the second input to the LSQ!!
      
        // 1st) add a fork 
        ENode* fork = new ENode(fork_type, fork_name.c_str(),  bbnode_dag->at(groups.at(i).first_bb_idx)->BB); 
        fork->id = lazy_fork_id++;

        fork->is_mem_dep_component = true;

        fork->my_lsq_enode = lsq_enode;

        enode_dag->push_back(fork);

        // 2nd) add a Buffer and connect its output to the Bx of the consumer
        ENode* buff_node = new ENode(Bx_Buffer_wrapper_, "buffer_bx_op", bbnode_dag->at(groups.at(i).first_bb_idx)->BB);
        buff_node->id = bx_buffer_wrapper_id++;//buff_id++;

        buff_node->is_mem_dep_component = true;

        buff_node->my_lsq_enode = lsq_enode;

        enode_dag->push_back(buff_node);

        // 3rd) connect the two components
        fork->CntrlOrderSuccs->push_back(buff_node);
        buff_node->CntrlOrderPreds->push_back(fork);

        // 4th) connect the second output of the Fork to the LSQ!!!
        fork->CntrlOrderSuccs->push_back(lsq_enode);
        lsq_enode->CntrlOrderPreds->push_back(fork);

        // 18/09/2022: copy the predecessors of this Bx to the producer_group_indices
        buff_node->producer_group_indices = groups.at(i).pred_groups_first_bb_idx;

        // 17/10/2022
        // I no longer need this field because I decided to not care for area-optimizing by checking if a Bx is already present in a specific BB by a previous LSQ object analysis
        // create entries for each predecessor (producer) in handled_loop_gsa_with_producer_group and set it to false
        /*for(int b = 0; b < buff_node->producer_group_indices.size(); b++) {
            buff_node->handled_loop_gsa_with_producer_group.push_back(false); 
        } */       

    }

    // Next let's do the connections between different Bx components
    for(int i = 0; i < groups.size(); i++) {
        // search for the Fork instance of the Bx in this group since the predecessor should feed the Fork
        ENode* consumer_Bx_fork = nullptr;
        for(int f = 0; f < enode_dag->size(); f++) {
            if(enode_dag->at(f)->type == fork_type && enode_dag->at(f)->is_mem_dep_component && enode_dag->at(f)->my_lsq_enode == lsq_enode && enode_dag->at(f)->BB == bbnode_dag->at(groups.at(i).first_bb_idx)->BB) {
                consumer_Bx_fork = enode_dag->at(f);
                break;
            }
        }
        assert(consumer_Bx_fork != nullptr);

        // loop over the predecessors of each group
        for(int j = 0; j < groups.at(i).pred_groups_first_bb_idx.size(); j++) {
            // search for the Buff instance of the Bx in this group since this should feed the fork of the other group!
            ENode* producer_Bx_buffer = nullptr;
            for(int f = 0; f < enode_dag->size(); f++) {
                if(enode_dag->at(f)->type == Bx_Buffer_wrapper_ && enode_dag->at(f)->is_mem_dep_component && enode_dag->at(f)->my_lsq_enode == lsq_enode && enode_dag->at(f)->CntrlOrderPreds->at(0)->BB == bbnode_dag->at(groups.at(i).pred_groups_first_bb_idx.at(j))->BB) {
                    // sanity check, the successor of this buffer must be a Fork!!!
                    assert(enode_dag->at(f)->CntrlOrderPreds->at(0)->type == fork_type && enode_dag->at(f)->CntrlOrderPreds->at(0)->is_mem_dep_component);
                    producer_Bx_buffer = enode_dag->at(f);
                    break;
                }
            }
            assert(producer_Bx_buffer != nullptr); 

            // do the necessary connections between the producer_Bx_buffer and the consumer_Bx_fork!!
            producer_Bx_buffer->CntrlOrderSuccs->push_back(consumer_Bx_fork);
            consumer_Bx_fork->CntrlOrderPreds->push_back(producer_Bx_buffer);

            // Old messy code that attempted to not add a new Bx if one already existed in the same BB by a previous LSQ object.. I replaced it with the above 2 statements
            // 18/09/2022
            // 17/09/2022:
            // HERE ALL I NEED TO DO IS TO THAT BEFORE I DO THE CONNECTION BETWEEN THE PRODUCER AND CONSUMER, ONLY MAKE SURE THAT THERE IS NO ALREADY EXISTING CONNECTION!!
            // THIS IS IMP IN CASE WE ARE REUSING 1 BX COMPONENT FOR MULTIPLE LSQs!
            /*auto pos = std::find(producer_Bx_buffer->CntrlOrderSuccs->begin(), producer_Bx_buffer->CntrlOrderSuccs->end(), consumer_Bx_fork);
            if(pos == producer_Bx_buffer->CntrlOrderSuccs->end()) {
                // first, do a second check to see if there is a Branch_c successor (added as a result of previously applying GSA on this Bx in a prior LSQ) that is having our consumer_Bx_fork in its successors!
                bool found_phic_to_consumer = false;
                for(int pp = 0; pp < producer_Bx_buffer->CntrlOrderSuccs->size(); pp++) {
                    // IMP TODO!!! THE FOLLOWING CHECK ACCOUNTS ONLY FOR ONE COMMON OUTER LOOP (I.E., IF THERE ARE MULTIPLE COMMON LOOPS, IT DOES NOT SEARCH FOR THE CONSUMER IN THE CHAIN OF BRANCHES!!)
                    if(producer_Bx_buffer->CntrlOrderSuccs->at(pp)->type == Branch_c) {
                        ENode* branchc_node = producer_Bx_buffer->CntrlOrderSuccs->at(pp);
                        // this branch should be feeding a PhiC in either its true or false succs, and we want to check if this PhiC is feeding our consumer or not!
                        for(int dd = 0; dd < branchc_node->true_branch_succs_Ctrl_lsq->size(); dd++) {
                            if(branchc_node->true_branch_succs_Ctrl_lsq->at(dd)->type == Phi_c) {
                                ENode* phic_node = branchc_node->true_branch_succs_Ctrl_lsq->at(dd);
                                auto pos__ = std::find(phic_node->CntrlOrderSuccs->begin(), phic_node->CntrlOrderSuccs->end(), consumer_Bx_fork);
                                if(pos__ != phic_node->CntrlOrderSuccs->end())
                                    found_phic_to_consumer = true;
                            }
                        }

                        if(!found_phic_to_consumer) {  // check the false if not found in the true
                            for(int dd = 0; dd < branchc_node->false_branch_succs_Ctrl_lsq->size(); dd++) {
                                if(branchc_node->false_branch_succs_Ctrl_lsq->at(dd)->type == Phi_c) {
                                    ENode* phic_node = branchc_node->false_branch_succs_Ctrl_lsq->at(dd);
                                    auto pos__ = std::find(phic_node->CntrlOrderSuccs->begin(), phic_node->CntrlOrderSuccs->end(), consumer_Bx_fork);
                                    if(pos__ != phic_node->CntrlOrderSuccs->end())
                                        found_phic_to_consumer = true;
                                }
                            }
                        }

                    } 
                }

                if(!found_phic_to_consumer) {
                    // do the necessary connections between the producer_Bx_buffer and the consumer_Bx_fork!!
                    producer_Bx_buffer->CntrlOrderSuccs->push_back(consumer_Bx_fork);
                    consumer_Bx_fork->CntrlOrderPreds->push_back(producer_Bx_buffer); 
                }
                    

            } */
            /*auto pos_ = std::find(consumer_Bx_fork->CntrlOrderPreds->begin(), consumer_Bx_fork->CntrlOrderPreds->end(), producer_Bx_buffer);
            if(pos_ == consumer_Bx_fork->CntrlOrderPreds->end()) {
                // do the necessary connections between the producer_Bx_buffer and the consumer_Bx_fork!!
                
            }*/

        }

    }

    // connect START and SINK to Bxs of groups having no predecessors or successors respectively
    for(int i = 0; i < groups.size(); i++) {
        if(groups.at(i).pred_groups_first_bb_idx.size() == 0) {
            // search for the Fork instance of the Bx in this group since the predecessor should feed the Fork
            ENode* consumer_Bx_fork = nullptr;
            for(int f = 0; f < enode_dag->size(); f++) {
                if(enode_dag->at(f)->type == fork_type && enode_dag->at(f)->is_mem_dep_component && enode_dag->at(f)->my_lsq_enode == lsq_enode && enode_dag->at(f)->BB == bbnode_dag->at(groups.at(i).first_bb_idx)->BB) {
                    consumer_Bx_fork = enode_dag->at(f);
                    break;
                }
            }
            assert(consumer_Bx_fork != nullptr); 

            if(consumer_Bx_fork->CntrlOrderPreds->size() == 0) {
                // search for the Start_ node
                for(auto& cnt_node : *bbnode_dag->front()->Cntrl_nodes) {
                    if(cnt_node->type == Start_) {
                        consumer_Bx_fork->CntrlOrderPreds->push_back(cnt_node);
                        cnt_node->CntrlOrderSuccs->push_back(consumer_Bx_fork);
                        break; // found the start node, no need to continue looping
                    }
                } 
            } 

        }

        if(groups.at(i).succ_groups_first_bb_idx.size() == 0) {
             // search for the Buff instance of the Bx in this group since this should feed the sink
            ENode* producer_Bx_buffer = nullptr;
            for(int f = 0; f < enode_dag->size(); f++) {
                if(enode_dag->at(f)->type == Bx_Buffer_wrapper_&& enode_dag->at(f)->is_mem_dep_component && enode_dag->at(f)->my_lsq_enode == lsq_enode && enode_dag->at(f)->CntrlOrderPreds->at(0)->BB == bbnode_dag->at(groups.at(i).first_bb_idx)->BB) {
                    // sanity check, the successor of this buffer must be a Fork!!!
                    assert(enode_dag->at(f)->CntrlOrderPreds->at(0)->type == fork_type && enode_dag->at(f)->CntrlOrderPreds->at(0)->is_mem_dep_component);
                    producer_Bx_buffer = enode_dag->at(f);
                    break;
                }
            }
            assert(producer_Bx_buffer != nullptr);

            if(producer_Bx_buffer->CntrlOrderSuccs->size() == 0) {
                  // create a new SINK node
                ENode* sink_node = new ENode(Sink_, "sink"); 
                sink_node->id    = sink_id++;
                enode_dag->push_back(sink_node);
                producer_Bx_buffer->CntrlOrderSuccs->push_back(sink_node);
                sink_node->CntrlOrderPreds->push_back(producer_Bx_buffer);
            }

        }
    }
}

// This function assumes that the predecessors of the LSQ are always in the 
void CircuitGenerator::convert_LSQ_fork_to_lazy() {
    for(auto& enode :*enode_dag) {
        if(enode->type == LSQ_) {
            for(auto& lsq_pred :*enode->CntrlOrderPreds) {
                if(lsq_pred->type == Fork_c) {
                    lsq_pred->type = LazyFork_;
                }
            }
        }
    }
}

void CircuitGenerator::removeUselessBxs(bool lazy_fork_flag) {
    node_type fork_type; // shall we create a Fork_c or LazyFork_
    if(lazy_fork_flag) {
        fork_type = LazyFork_;
    } else {
        fork_type = Fork_c;
    }

    std::vector<ENode*> bx_buff_to_delete;
    // loop on the enode dage searching for a Buffer that has is_mem_dep_component true and has a sucessor sink!
    for(auto& enode : *enode_dag) {
        if(enode->type == Bx_Buffer_wrapper_ && enode->is_mem_dep_component && enode->CntrlOrderSuccs->size() == 1) {
            if(enode->CntrlOrderSuccs->at(0)->type == Sink_) {

                
                assert(enode->CntrlOrderPreds->size() == 1 && enode->CntrlOrderPreds->at(0)->type == fork_type); // should be having only a Fork as its predecessor!!
               /* // AYA: 30/10/2022: added an extra condition to skip if the predecessor of the fork of the Bx is a Branch_c because for some unknown reason
                    // connecting the Branch_c directly to the LSQ does not work!!
                if(enode->CntrlOrderPreds->at(0)->CntrlOrderPreds->at(0)->type == Branch_c) {
                    return;
                }*/


                bx_buff_to_delete.push_back(enode);

                 // erase the buffer from the predecessors of the sink
                auto pos_buff_in_sink = std::find(enode->CntrlOrderSuccs->at(0)->CntrlOrderPreds->begin(), enode->CntrlOrderSuccs->at(0)->CntrlOrderPreds->end(), enode);
                assert(pos_buff_in_sink != enode->CntrlOrderSuccs->at(0)->CntrlOrderPreds->end());
                enode->CntrlOrderSuccs->at(0)->CntrlOrderPreds->erase(pos_buff_in_sink);

                 // erase the sink from the enode_dag
                auto pos_sink_ = std::find(enode_dag->begin(), enode_dag->end(), enode->CntrlOrderSuccs->at(0));
                assert(pos_sink_ != enode_dag->end());
                enode_dag->erase(pos_sink_);

                 // erase the sink from the successors of the Buffer!!
                auto pos_sink = std::find(enode->CntrlOrderSuccs->begin(), enode->CntrlOrderSuccs->end(), enode->CntrlOrderSuccs->at(0));
                assert(pos_sink != enode->CntrlOrderSuccs->end());
                enode->CntrlOrderSuccs->erase(pos_sink);
                assert(enode->CntrlOrderSuccs->size() == 0);

                assert(enode->CntrlOrderPreds->size() == 1 && enode->CntrlOrderPreds->at(0)->type == fork_type); // buffer should be having only a Fork as its predecessor!!
                ENode* fork_of_bx = enode->CntrlOrderPreds->at(0);
                // extra check on the fork
                assert(fork_of_bx->CntrlOrderSuccs->size() == 2);
                assert(fork_of_bx->CntrlOrderSuccs->at(0)->type == Bx_Buffer_wrapper_ && fork_of_bx->CntrlOrderSuccs->at(1)->type == LSQ_); // the fork should be having only two successors: the LSQ and the Buffer!
                ENode* lsq_enode = fork_of_bx->CntrlOrderSuccs->at(1);

                // erase the buffer from the successors of the fork!!
                auto pos_buffer = std::find(fork_of_bx->CntrlOrderSuccs->begin(), fork_of_bx->CntrlOrderSuccs->end(), enode);
                assert(pos_buffer != fork_of_bx->CntrlOrderSuccs->end());
                fork_of_bx->CntrlOrderSuccs->erase(pos_buffer);

                /*// should completely erase the LazyFork_ and the Buffer and connect the LSQ directly to the predecessor of the Fork!!
                // identify the index of the Fork of this Bx in the CntrlOrderPreds of the LSQ!!
                int idx_fork_in_lsq = returnPosRemoveNode(lsq_enode->CntrlOrderPreds,  fork_of_bx); // this function only finds the index of the passed element in the passed vector!
                lsq_enode->CntrlOrderPreds->at(idx_fork_in_lsq) = fork_of_bx->CntrlOrderPreds->at(0);

                // Aya: 29/10/2022: in case the predecessor of the fork is of type branch, then the fork should be either in its truePreds or falsePreds, so added an extra condition to check
                // identify the index of the Fork of this Bx in the CntrlOrderSuccs of the fork's predecessor!!
                int idx_fork_in_its_pred_succs = -1;
                if(fork_of_bx->CntrlOrderPreds->at(0)->type == Branch_c) {
                    // first identify if the fork is in its true or false succs!
                    auto pos_true = std::find(fork_of_bx->CntrlOrderPreds->at(0)->true_branch_succs_Ctrl_lsq->begin(), fork_of_bx->CntrlOrderPreds->at(0)->true_branch_succs_Ctrl_lsq->end(), fork_of_bx);
                    if(pos_true != fork_of_bx->CntrlOrderPreds->at(0)->true_branch_succs_Ctrl_lsq->end()) {
                        idx_fork_in_its_pred_succs = pos_true - fork_of_bx->CntrlOrderPreds->at(0)->true_branch_succs_Ctrl_lsq->begin();


                        fork_of_bx->CntrlOrderPreds->at(0)->true_branch_succs_Ctrl_lsq->at(idx_fork_in_its_pred_succs) = lsq_enode;
                    } else {
                        // it must be in the false succs then!
                        auto pos_false = std::find(fork_of_bx->CntrlOrderPreds->at(0)->false_branch_succs_Ctrl_lsq->begin(), fork_of_bx->CntrlOrderPreds->at(0)->false_branch_succs_Ctrl_lsq->end(), fork_of_bx);
                        assert(pos_false != fork_of_bx->CntrlOrderPreds->at(0)->false_branch_succs_Ctrl_lsq->end());
                        idx_fork_in_its_pred_succs = pos_false - fork_of_bx->CntrlOrderPreds->at(0)->false_branch_succs_Ctrl_lsq->begin();

                        fork_of_bx->CntrlOrderPreds->at(0)->false_branch_succs_Ctrl_lsq->at(idx_fork_in_its_pred_succs) = lsq_enode;
                    }

                } else {
                    // extra check: it can't be of type Branch_n
                    assert(fork_of_bx->CntrlOrderPreds->at(0)->type != Branch_n);
                    idx_fork_in_its_pred_succs = returnPosRemoveNode(fork_of_bx->CntrlOrderPreds->at(0)->CntrlOrderSuccs, fork_of_bx);
                    fork_of_bx->CntrlOrderPreds->at(0)->CntrlOrderSuccs->at(idx_fork_in_its_pred_succs) = lsq_enode;
                }


                fork_of_bx->CntrlOrderPreds->clear();
                fork_of_bx->CntrlOrderSuccs->clear();
                // erase the Fork and the Buffer and the sink that is fed by the Buffer!!
                // find the position of the Fork in the enode_dag
                auto pos_fork = std::find(enode_dag->begin(), enode_dag->end(), fork_of_bx);
                assert(pos_fork != enode_dag->end());
                enode_dag->erase(pos_fork); */

                // find the position of the Buffer in the enode_dag
                /*auto pos_buff = std::find(enode_dag->begin(), enode_dag->end(), enode);
                assert(pos_buff != enode_dag->end());
                enode_dag->erase(pos_buff);*/

            } 
        }
    }

    for(int k = 0; k < bx_buff_to_delete.size(); k++) {
        auto pos_buff = std::find(enode_dag->begin(), enode_dag->end(), bx_buff_to_delete.at(k));
        assert(pos_buff != enode_dag->end());
        enode_dag->erase(pos_buff);
    }
    // do another loop to erase all of the Bx components that should now be having no successors!!!
   /* for(auto& enode : *enode_dag) {
        if(enode->type == Bx_Buffer_wrapper_ && enode->is_mem_dep_component && enode->CntrlOrderSuccs->size() == 0) {
            auto pos_buff = std::find(enode_dag->begin(), enode_dag->end(), enode);
            assert(pos_buff != enode_dag->end());
            enode_dag->erase(pos_buff);
        }
    }*/

}

void CircuitGenerator::applyGSA_Non_Loops_simplified(std::ofstream& dbg_file, ENode* lsq_enode, const std::vector<Group>& groups, bool lazy_fork_flag) {
    node_type fork_type; // shall we create a Fork_c or LazyFork_
    if(lazy_fork_flag) {
        fork_type = LazyFork_;
    } else {
        fork_type = Fork_c;
    }
    for(int i = 0; i < groups.size(); i++) {
         // search for the Fork instance of the Bx in this group since phis will feed the fork of this Bx
        ENode* consumer_Bx_fork = nullptr;
        //ENode* consumer_Bx_buff = nullptr;
        for(int f = 0; f < enode_dag->size(); f++) {
            if(enode_dag->at(f)->type == fork_type && enode_dag->at(f)->is_mem_dep_component && enode_dag->at(f)->my_lsq_enode == lsq_enode && enode_dag->at(f)->BB == bbnode_dag->at(groups.at(i).first_bb_idx)->BB) {
                consumer_Bx_fork = enode_dag->at(f);
                break;
            }
        }
        assert(consumer_Bx_fork != nullptr);

        // loop over the predecessors of each group
        for(int j = 0; j < groups.at(i).pred_groups_first_bb_idx.size(); j++) {
            // search for the Buff instance of the Bx in this group since if we are to insert a Phi, it should be fed by the Buff of this Bx!
            ENode* producer_Bx_buffer = nullptr;
            for(int f = 0; f < enode_dag->size(); f++) {
                if(enode_dag->at(f)->type == Bx_Buffer_wrapper_ && enode_dag->at(f)->is_mem_dep_component && enode_dag->at(f)->my_lsq_enode == lsq_enode && enode_dag->at(f)->CntrlOrderPreds->at(0)->BB == bbnode_dag->at(groups.at(i).pred_groups_first_bb_idx.at(j))->BB) {
                    // sanity check, the successor of this buffer must be a Fork!!!
                    assert(enode_dag->at(f)->CntrlOrderPreds->at(0)->type == fork_type && enode_dag->at(f)->CntrlOrderPreds->at(0)->is_mem_dep_component);
                    producer_Bx_buffer = enode_dag->at(f);
                    break;
                }
            }
            assert(producer_Bx_buffer != nullptr);

            ////////////////////////////////////////////////////////////////////////////////
            // NOw I have a producer and consumer, check their conditions of production and consumption!
            std::vector<int> modif_consBB_deps;
            std::vector<int> modif_prodBB_deps;
            eliminateCommonControlAncestors(dbg_file, BBMap->at(consumer_Bx_fork->BB), BBMap->at(producer_Bx_buffer->BB), modif_consBB_deps, modif_prodBB_deps); 
            std::vector<std::vector<pierCondition>> f_cons;
            constructSOP(dbg_file, bbnode_dag->at(0), BBMap->at(consumer_Bx_fork->BB), modif_consBB_deps, f_cons); 
            std::vector<std::vector<pierCondition>> f_prod;
            constructSOP(dbg_file, bbnode_dag->at(0), BBMap->at(producer_Bx_buffer->BB), modif_prodBB_deps, f_prod);

            std::vector<std::vector<pierCondition>> f_gen;
            bool generate_flag;
            generate_flag = calculate_F_Gen(dbg_file, f_prod, f_cons, f_gen);

            // print for debugging
            dbg_file << "\n\n###########################################################\n\n";
            dbg_file << "\n\n\tLet's check what came out!!\n\n";
            if(generate_flag) {// add a new MUX with the condition f_gen!
                dbg_file << "\nNeed to insert a MUX with: ";
                if(f_gen.size()==1) {
                    if(f_gen.at(0).size() == 1) {
                        dbg_file << " condition = " << f_gen.at(0).at(0).pierBB_index + 1 << "\n";
                    } else {
                        dbg_file << " a complicated condition!!\n";
                    }
                } else {
                    dbg_file << " a complicated condition!!\n";
                }
            } else {
                dbg_file << "\nNo need to insert a GENERATE!!\n";
            }

            if(generate_flag) {
                // this means I need to insert a GENERATE component but in the producer's BB!! It should be inserted between the producer and the consumer or between the producer and any intermediate component that stores the consumer in its field!!
                bool is_negated_cond;
                dbg_file << "\n\tRight before calling applyShannon, the size of f_gen is: " << f_gen.size() << "\n";
                ENode* gen_condition_enode = applyShannon(dbg_file, dbg_file, producer_Bx_buffer->BB, f_gen, is_negated_cond);

                // create a new phiC MUX and insert it between the producer and consumer, and connect its second input a Branch fed with a START 
                dbg_file << "\nAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n";
                dbg_file << "\tWill insert a MUX in BB" << BBMap->at(producer_Bx_buffer->BB)->Idx + 1 << " and the consumer Bx is " << BBMap->at(consumer_Bx_fork->BB)->Idx + 1 << "\n\n";
                ENode* gsa_mux = new ENode(Phi_c, "phiC", producer_Bx_buffer->BB);  // we insert the GENERATE in the producer BB 
                gsa_mux->id = phi_id++;

                // I commented this because I don't want it to be skipped in the AddSuppress!!
                // gsa_mux->is_mem_dep_component = true;

                gsa_mux->isMux = true;
                gsa_mux->is_non_loop_gsa_mux = true;

                // the naming of this field is confusing, but it's meant to hold the producer enode that feeds a Phi of our creation
                    // for all of the phis I'm adding, I store in them the original producer's Bx's Buffer!!
                gsa_mux->producer_to_branch = producer_Bx_buffer;

                gsa_mux->is_advanced_component = true;
                // condition should be negated or not depending on is_negated_cond
                gsa_mux->is_negated_input->push_back(is_negated_cond); 
                // the other 2 inputs of the enode MUX shouldn't be negated!!
                gsa_mux->is_negated_input->push_back(false);
                gsa_mux->is_negated_input->push_back(false);

                enode_dag->push_back(gsa_mux);

                // convert it to MUX and manage its inputs!
                // the .at(0) of the CntrlPreds(data network!!) the MUX is the condition and it should be the gen_condition_enode
                gsa_mux->CntrlPreds->push_back(gen_condition_enode);
                gen_condition_enode->CntrlSuccs->push_back(gsa_mux);

                // gsa_mux should be fed directly from the original producer!! So, search for the original consumer in the successors of the original producer!!
                ENode* correct_producer_succ = nullptr;
                for(int uu = 0; uu < producer_Bx_buffer->CntrlOrderSuccs->size(); uu++) {
                    if(producer_Bx_buffer->CntrlOrderSuccs->at(uu) == consumer_Bx_fork) {
                        correct_producer_succ = consumer_Bx_fork;
                        break;
                    }
                }
                assert(correct_producer_succ != nullptr);
                assert(correct_producer_succ->type == fork_type);

                // the .at(0) (i.e., when the condition is false) of the CntrlOrderPreds should be the original producer
                gsa_mux->CntrlOrderPreds->push_back(producer_Bx_buffer);
                gsa_mux->CntrlOrderSuccs->push_back(correct_producer_succ);
                // identify the index of the original producer in the CntrlOrderPreds of the original consumer and the original consumer in the CntrlOrderSuccs of the original producer
                int idx_prod_in_cons_pred = returnPosRemoveNode(correct_producer_succ->CntrlOrderPreds, producer_Bx_buffer); // this function only finds the index of the passed element in the passed vector!
                int idx_cons_in_prod_succ = returnPosRemoveNode(producer_Bx_buffer->CntrlOrderSuccs, correct_producer_succ);
                correct_producer_succ->CntrlOrderPreds->at(idx_prod_in_cons_pred) = gsa_mux;
                producer_Bx_buffer->CntrlOrderSuccs->at(idx_cons_in_prod_succ) = gsa_mux;

                // the second input of the gsa_mux should be START!!
                for(auto& cnt_node : *bbnode_dag->front()->Cntrl_nodes) {
                    if(cnt_node->type == Start_) {
                        gsa_mux->CntrlOrderPreds->push_back(cnt_node);
                        cnt_node->CntrlOrderSuccs->push_back(gsa_mux);
                        break; // found the start node, no need to continue looping
                    }
                } // FPL'22 will take care of adding any necessary suppress or stuff for token count mismatch in case the phi_c is inside a loop!

            }
            ////////////////////////////////////////////////////////////////////////////////

        }
    }

}

void CircuitGenerator::old_applyGSA_Non_Loops_simplified(std::ofstream& dbg_file, const std::vector<Group>& groups) {
    // loop over the enodes in search for Bx components and loop over their succs
    ENode* consumer_Bx_fork;
    ENode* producer_Bx_buffer;
    for(int i = 0; i < enode_dag->size(); i++) {
        if(enode_dag->at(i)->type == Bx_Buffer_wrapper_) {
            assert(enode_dag->at(i)->CntrlOrderPreds->size() == 1 && enode_dag->at(i)->CntrlOrderPreds->at(0)->type == LazyFork_);
            consumer_Bx_fork = enode_dag->at(i)->CntrlOrderPreds->at(0);

            // loop over the predecessors (producers of this Bx component!)
            for(int j = 0; j < consumer_Bx_fork->CntrlOrderPreds->size(); j++) {
                if(consumer_Bx_fork->CntrlOrderPreds->at(j)->type != Bx_Buffer_wrapper_) {
                    assert(consumer_Bx_fork->CntrlOrderPreds->at(j)->type == Phi_c && consumer_Bx_fork->CntrlOrderPreds->at(j)->is_mem_dep_component);
                    assert(consumer_Bx_fork->CntrlOrderPreds->at(j)->orig_bx_buff_producer != nullptr);
                    producer_Bx_buffer = consumer_Bx_fork->CntrlOrderPreds->at(j)->orig_bx_buff_producer;
                } else {
                    producer_Bx_buffer = consumer_Bx_fork->CntrlOrderPreds->at(j);
                }

                // NOw I have a producer and consumer, check their conditions of production and consumption!
                // NOW: get the conditions of production and consumption!!!
                std::vector<int> modif_consBB_deps;
                std::vector<int> modif_prodBB_deps;
                eliminateCommonControlAncestors(dbg_file, BBMap->at(consumer_Bx_fork->BB), BBMap->at(producer_Bx_buffer->BB), modif_consBB_deps, modif_prodBB_deps); 
                std::vector<std::vector<pierCondition>> f_cons;
                constructSOP(dbg_file, bbnode_dag->at(0), BBMap->at(consumer_Bx_fork->BB), modif_consBB_deps, f_cons); 
                std::vector<std::vector<pierCondition>> f_prod;
                constructSOP(dbg_file, bbnode_dag->at(0), BBMap->at(producer_Bx_buffer->BB), modif_prodBB_deps, f_prod);


                std::vector<std::vector<pierCondition>> f_gen;
                bool generate_flag;
                generate_flag = calculate_F_Gen(dbg_file, f_prod, f_cons, f_gen);
                dbg_file << "\n\n###########################################################\n\n";
                dbg_file << "\n\n\tLet's check what came out!!\n\n";
                if(generate_flag) {// add a new MUX with the condition f_gen!
                    dbg_file << "\nNeed to insert a MUX with: ";
                    if(f_gen.size()==1) {
                        if(f_gen.at(0).size() == 1) {
                            dbg_file << " condition = " << f_gen.at(0).at(0).pierBB_index + 1 << "\n";
                        } else {
                            dbg_file << " a complicated condition!!\n";
                        }
                    } else {
                        dbg_file << " a complicated condition!!\n";
                    }
                } else {
                    dbg_file << "\nNo need to insert a GENERATE!!\n";
                }

                if(generate_flag) {
                    // this means I need to insert a GENERATE component but in the producer's BB!! It should be inserted between the producer and the consumer or between the producer and any intermediate component that stores the consumer in its field!!
                    bool is_negated_cond;
                    dbg_file << "\n\tRight before calling applyShannon, the size of f_gen is: " << f_gen.size() << "\n";
                    ENode* gen_condition_enode = applyShannon(dbg_file, dbg_file, producer_Bx_buffer->BB, f_gen, is_negated_cond);

                    // create a new phiC MUX and insert it between the producer and consumer, and connect its second input a Branch fed with a START 
                    ENode* gsa_mux = new ENode(Phi_c, "phiC", producer_Bx_buffer->BB);  // we insert the GENERATE in the producer BB 
                    gsa_mux->id = phi_id++;

                    // I commented this because I don't want it to be skipped in the AddSuppress!!
                    // gsa_mux->is_mem_dep_component = true;

                    gsa_mux->isMux = true;
                    gsa_mux->is_non_loop_gsa_mux = true;

                    gsa_mux->is_advanced_component = true;
                    // condition should be negated or not depending on is_negated_cond
                    gsa_mux->is_negated_input->push_back(is_negated_cond); 
                    // the other 2 inputs of the enode MUX shouldn't be negated!!
                    gsa_mux->is_negated_input->push_back(false);
                    gsa_mux->is_negated_input->push_back(false);

                    enode_dag->push_back(gsa_mux);

                    // convert it to MUX and manage its inputs!
                    // the .at(0) of the CntrlPreds(data network!!) the MUX is the condition and it should be the gen_condition_enode
                    gsa_mux->CntrlPreds->push_back(gen_condition_enode);
                    gen_condition_enode->CntrlSuccs->push_back(gsa_mux);

                    // gsa_mux should be fed directly from the original producer!! So, search for the original consumer in the successors of the oriignal producer!!
                    ENode* correct_producer_succ = nullptr;
                    for(int uu = 0; uu < producer_Bx_buffer->CntrlOrderSuccs->size(); uu++) {
                        if(producer_Bx_buffer->CntrlOrderSuccs->at(uu) == consumer_Bx_fork) {
                            correct_producer_succ = consumer_Bx_fork;
                            break;
                        } else {
                            if(producer_Bx_buffer->CntrlOrderSuccs->at(uu)->is_mem_dep_component) {
                                assert(producer_Bx_buffer->CntrlOrderSuccs->at(uu)->orig_bx_fork_consumer != nullptr);
                                if(producer_Bx_buffer->CntrlOrderSuccs->at(uu)->orig_bx_fork_consumer == consumer_Bx_fork)
                                    correct_producer_succ = producer_Bx_buffer->CntrlOrderSuccs->at(uu);
                                break;
                            }
                        }
                    }
                    assert(correct_producer_succ != nullptr);

                    // the .at(0) (i.e., when the condition is false) of the CntrlOrderPreds should be the original producer
                    gsa_mux->CntrlOrderPreds->push_back(producer_Bx_buffer);
                    gsa_mux->CntrlOrderSuccs->push_back(correct_producer_succ);
                    // identify the index of the original producer in the CntrlOrderPreds of the original consumer and the original consumer in the CntrlOrderSuccs of the original producer
                    int idx_prod_in_cons_pred = returnPosRemoveNode(correct_producer_succ->CntrlOrderPreds, producer_Bx_buffer); // this function only finds the index of the passed element in the passed vector!
                    int idx_cons_in_prod_succ = returnPosRemoveNode(producer_Bx_buffer->CntrlOrderSuccs, correct_producer_succ);
                    correct_producer_succ->CntrlOrderPreds->at(idx_prod_in_cons_pred) = gsa_mux;
                    producer_Bx_buffer->CntrlOrderSuccs->at(idx_cons_in_prod_succ) = gsa_mux;
                    
                    //////////////////////////////////////////////////////////
                    // the second input of the gsa_mux should be START!!
                     for(auto& cnt_node : *bbnode_dag->front()->Cntrl_nodes) {
                        if(cnt_node->type == Start_) {
                            gsa_mux->CntrlOrderPreds->push_back(cnt_node);
                            cnt_node->CntrlOrderSuccs->push_back(gsa_mux);
                            break; // found the start node, no need to continue looping
                        }
                    } 

                }
                ////////////////////////////////////////////////////////////////////////////////////////////
                
            }

        }
    }

}


void CircuitGenerator::addJoin(bool lazy_fork_flag) {
    node_type fork_type; // shall we create a Fork_c or LazyFork_
    if(lazy_fork_flag) {
        fork_type = LazyFork_;
    } else {
        fork_type = Fork_c;
    }
    for(auto& enode : *enode_dag) {
        if(enode->type == fork_type && enode->is_mem_dep_component && enode->CntrlOrderPreds->size() > 1) {
            ENode* join = new ENode(Bx_Join_wrapper_, "join_bx_op", enode->BB); 
            join->id = bx_join_wrapper_id++;
            enode_dag->push_back(join);

            // loop over the preds of the fork of the bx component
            for(int i = 0; i < enode->CntrlOrderPreds->size(); i++) {
                int idx_ = returnPosRemoveNode(enode->CntrlOrderPreds->at(i)->CntrlOrderSuccs, enode); // this function only finds the index of the passed element in the passed vector!
                enode->CntrlOrderPreds->at(i)->CntrlOrderSuccs->at(idx_) = join;
                join->CntrlOrderPreds->push_back(enode->CntrlOrderPreds->at(i));
            }

            enode->CntrlOrderPreds->clear();
            assert(enode->CntrlOrderPreds->size() == 0);

            enode->CntrlOrderPreds->push_back(join);
            join->CntrlOrderSuccs->push_back(enode);

            // the following code was working but restricts on Bx to be fed with a maximum of only 2 Bxs!!
            /*assert(enode->CntrlOrderPreds->size() == 2);
            ENode* join = new ENode(Bx_Join_wrapper_, "join_bx_op", enode->BB); 
            join->id = bx_join_wrapper_id++;
            enode_dag->push_back(join);

            // the 2 preds of the Merge should be the 2 preds of the enode!
            int idx_1 = returnPosRemoveNode(enode->CntrlOrderPreds->at(0)->CntrlOrderSuccs, enode); // this function only finds the index of the passed element in the passed vector!
            int idx_2 = returnPosRemoveNode(enode->CntrlOrderPreds->at(1)->CntrlOrderSuccs, enode);

            enode->CntrlOrderPreds->at(0)->CntrlOrderSuccs->at(idx_1) = join;
            join->CntrlOrderPreds->push_back(enode->CntrlOrderPreds->at(0));
            enode->CntrlOrderPreds->at(1)->CntrlOrderSuccs->at(idx_2) = join;
            join->CntrlOrderPreds->push_back(enode->CntrlOrderPreds->at(1));

            enode->CntrlOrderPreds->clear();
            assert(enode->CntrlOrderPreds->size() == 0);

            enode->CntrlOrderPreds->push_back(join);
            join->CntrlOrderSuccs->push_back(enode); */
        }
    }
}

bool compareByFirstBBIndex(const CircuitGenerator::Group& group_1, const CircuitGenerator::Group& group_2) {
    return group_1.first_bb_idx < group_2.first_bb_idx;
}

void CircuitGenerator::printDebugDominatorTree() {
    std::ofstream dbg_file;
    dbg_file.open("dominatorTree.txt");

    for(int b = 0; b < bbnode_dag->size(); b++) {
        dbg_file << "\n\n*********************************************************************\n";
        dbg_file << "BB" << bbnode_dag->at(b)->Idx + 1 << " dominates the following BBs\n";
        for(int i = 0; i < bbnode_dag->size(); i++) {
            if(my_dom_tree->properlyDominates(bbnode_dag->at(b)->BB, bbnode_dag->at(i)->BB)) {
                 dbg_file << "BB" << bbnode_dag->at(i)->Idx + 1 << ", ";
            }
        }
    }

}

// TODO: this function needs to be properly tested.. 
void CircuitGenerator::applyTransitiveReduction(std::vector<Group>& groups) {
    // loop over the groups, if one group has more than 1 pred and the 2 preds have and edge between them, cut the edge of one of the preds corresponding to the smaller group idx
    int smaller_group_idx;
    int bigger_group_idx;

    std::vector<int> group_preds_to_delete;

    // Making sure tht groups are sorted ascendingly in terms of the first_bb_idx
    std::sort(groups.begin(), groups.end(), compareByFirstBBIndex);

    for(int i = groups.size()-1; i >= 0; i--) {
        group_preds_to_delete.clear();
        // loop over the predecessors of this group comparing pair by pair
        for(int j = 0; j < groups.at(i).pred_groups_first_bb_idx.size()-1; j++) {
            for(int k = j+1; k < groups.at(i).pred_groups_first_bb_idx.size(); k++) {
                if(groups.at(i).pred_groups_first_bb_idx.at(j) > groups.at(i).pred_groups_first_bb_idx.at(k)) {
                    smaller_group_idx = groups.at(i).pred_groups_first_bb_idx.at(k);
                    bigger_group_idx = groups.at(i).pred_groups_first_bb_idx.at(j);
                } else {
                    assert(groups.at(i).pred_groups_first_bb_idx.at(j) < groups.at(i).pred_groups_first_bb_idx.at(k));
                    smaller_group_idx = groups.at(i).pred_groups_first_bb_idx.at(j);
                    bigger_group_idx = groups.at(i).pred_groups_first_bb_idx.at(k);
                }

                // Now check if the bigger_group_idx contains the smaller_group_idx in its preds!

                // 1st identify the index of the 2 group nodes in the original groups graph
                auto pos_big = std::find(groups.begin(), groups.end(), bigger_group_idx);
                assert(pos_big != groups.end());
                int idx_big = pos_big - groups.begin();
                auto pos_small = std::find(groups.begin(), groups.end(), smaller_group_idx);
                assert(pos_small != groups.end());
                int idx_small = pos_small - groups.begin();

                // search for the smaller_group_idx in the predecessors of the bigger_group_idx
                auto pos_ = std::find(groups.at(idx_big).pred_groups_first_bb_idx.begin(), groups.at(idx_big).pred_groups_first_bb_idx.end(), smaller_group_idx);

                // Aya: 20/10/2022: added an extra condition to apply the transitivity property ONLY if the bigger_idx group DOMINATES groups.at(i)
                if(pos_ != groups.at(idx_big).pred_groups_first_bb_idx.end() && my_dom_tree->properlyDominates(bbnode_dag->at(groups.at(idx_big).first_bb_idx)->BB, bbnode_dag->at(groups.at(i).first_bb_idx)->BB)) {
                    // this means we should delete the smaller_group_idx from the predecessors of the group we are currently studying (groups.at(i))
                        // I will do this delete once in the end before starting a new iteration with a new group
                            // push to the vector only if it is not already there!
                    auto pos_check = std::find(group_preds_to_delete.begin(), group_preds_to_delete.end(), smaller_group_idx);
                    if(pos_check == group_preds_to_delete.end())
                        group_preds_to_delete.push_back(smaller_group_idx);

                    // and similarly delete groups.at(i) from the successors of the smaller_group_idx group 
                    auto pos_2 = std::find(groups.at(idx_small).succ_groups_first_bb_idx.begin(), groups.at(idx_small).succ_groups_first_bb_idx.end(), groups.at(i).first_bb_idx);
                    assert(pos_2 != groups.at(idx_small).succ_groups_first_bb_idx.end());
                    groups.at(idx_small).succ_groups_first_bb_idx.erase(pos_2);

                }


            }


        }

        // loop over the group_preds_to_delete of this group to delete preds that should be deleted, if any
        for(int z = 0; z < group_preds_to_delete.size(); z++) {
            // delete the connection between groups.at(i) and the smaller_idx!!
            auto pos_1 = std::find(groups.at(i).pred_groups_first_bb_idx.begin(), groups.at(i).pred_groups_first_bb_idx.end(), group_preds_to_delete.at(z));
            assert(pos_1 != groups.at(i).pred_groups_first_bb_idx.end());
            groups.at(i).pred_groups_first_bb_idx.erase(pos_1);

        }

    }

    //  the following code was working but assuming that any group has a maximum of 2 predecessors only!!
    /* for(int i = groups.size()-1; i >= 0; i--) {
        if(groups.at(i).pred_groups_first_bb_idx.size() > 1) {
            assert(groups.at(i).pred_groups_first_bb_idx.size() == 2);
            if(groups.at(i).pred_groups_first_bb_idx.at(0) > groups.at(i).pred_groups_first_bb_idx.at(1)) {
                smaller_group_idx = groups.at(i).pred_groups_first_bb_idx.at(1);
                bigger_group_idx = groups.at(i).pred_groups_first_bb_idx.at(0);
            } else {
                assert(groups.at(i).pred_groups_first_bb_idx.at(0) < groups.at(i).pred_groups_first_bb_idx.at(1));
                smaller_group_idx = groups.at(i).pred_groups_first_bb_idx.at(0);
                bigger_group_idx = groups.at(i).pred_groups_first_bb_idx.at(1);
            }

            // nOw check if the bigger_group_idx contains the smaller_group_idx in its preds!
            auto pos_big = std::find(groups.begin(), groups.end(), bigger_group_idx);
            assert(pos_big != groups.end());
            int idx_big = pos_big - groups.begin();

            // needed so that I can access the group corresponding to this smaller_idx to delete the groups.at(i) from its successors!!
            auto pos_small = std::find(groups.begin(), groups.end(), smaller_group_idx);
            assert(pos_small != groups.end());
            int idx_small = pos_small - groups.begin();

            auto pos_ = std::find(groups.at(idx_big).pred_groups_first_bb_idx.begin(), groups.at(idx_big).pred_groups_first_bb_idx.end(), smaller_group_idx);
            if(pos_ != groups.at(idx_big).pred_groups_first_bb_idx.end()) {
                // delete the connection between groups.at(i) and the smaller_idx!!
                auto pos_1 = std::find(groups.at(i).pred_groups_first_bb_idx.begin(), groups.at(i).pred_groups_first_bb_idx.end(), smaller_group_idx);
                assert(pos_1 != groups.at(i).pred_groups_first_bb_idx.end());
                groups.at(i).pred_groups_first_bb_idx.erase(pos_1);

                auto pos_2 = std::find(groups.at(idx_small).succ_groups_first_bb_idx.begin(), groups.at(idx_small).succ_groups_first_bb_idx.end(), groups.at(i).first_bb_idx);
                assert(pos_2 != groups.at(idx_small).succ_groups_first_bb_idx.end());
                groups.at(idx_small).succ_groups_first_bb_idx.erase(pos_2);

            }

        }
    } */
}


/**
 * @brief applies the algorithm of FPGA'23 for interfacing our FPL'22 circuits with the LSQ. It is an alternative to add_naive_ControlForLSQ
 */
void CircuitGenerator::add_smart_ControlForLSQ(bool lazy_fork_flag) {
    // 12/09/2022
    std::vector<prod_cons_mem_dep> all_mem_deps; // one element of this should hold the index of the producer and consumer operations
            // (based on the type of memory dependency) in the CntrlPreds of the LSQ
    std::ofstream dbg_file;
    dbg_file.open("G_Graph.txt");

    std::ofstream general_dbg_file;
    general_dbg_file.open("general_MEMORY_dbg.txt");

    std::ofstream dbg_file_3;
    dbg_file_3.open("Managing_GSA_GENERATE.txt");
    // for each LSQ object, run the algorithm
    for(auto & enode : *enode_dag) {
        all_mem_deps.clear();
        if(enode->type == LSQ_) {
            dbg_file << "\n\n*******************************************************\n";
            dbg_file << "\tStarting a new LSQ!!!!\n\n";

            dbg_file_3 << "\n\n*******************************************************\n";
            dbg_file_3 << "\tStarting a new LSQ!!!!\n\n";
            
            // 1st) loop over the memory operations that are preds of the LSQ and fill the group_idx field for each of them
                // THis group_idx field is simply the original BB idx of the enode or another earlier BB idx!
            identifyGroupIdxs(enode);  // internally modifies the group_idx field of the enodes that are the memory operations preds of the LSQ
                                    // and also modify a new field of the bbnode class which is a flag denoting that it is a group either because the group is made up only of a single BB (itself) or because it is the BB with the smallest index of the group

            // 2nd) Construct a graph with group_idx nodes
            // each node is a BB Idx and the producer and consumer relations are maintained in a predecessor and successor vector for each element!
                // this is done by looping over the enode memory operations and if they do not have the same group_idx and are either LD,ST or ST,ST, make an edge!!
            identifyMemDeps(dbg_file, enode, all_mem_deps); // fills all_mem_deps where each element of it is 1 edge from the M graph (and it labels if edges are backward)
            std::vector<Group> groups;
            constructGroupsGraph(enode, groups, all_mem_deps);

            // 3rd) remove redundant connections from the groups graph but make sure to maintain some necessary control flow edges!!!
            applyTransitiveReduction(groups); 
           
            printDebugGroupsGraph(dbg_file, groups, enode, all_mem_deps);
            // The following is extra reassuring information about the case of having a single node in the Gropus graph (ofc with no incoming edges)
            // At this point if all my memory operations happen to be in a single group, the graph of Groups will have a single node with no incoming or outgoing edges
                // In this case, a single Bx will be inserted and START will be incoming to it and a SINK will be outgoing from it (will be optimzied in the end)!!!

            // 4th) translate each group into Bx components with connections corresponding to the edges of the G graph!
            translateGroups_toCircuit(enode, groups, lazy_fork_flag);
            general_dbg_file << "\n*********************************** New LSQ ****************************************\n";

            // 5th) now I know the minimum set of producers and consumers, run GSA to see if any MUXes should be added!
            general_dbg_file << "\nBefore applyGSA_Non_Loops_simplified!\n";
            applyGSA_Non_Loops_simplified(dbg_file_3, enode, groups, lazy_fork_flag);
            general_dbg_file << "\nAfter applyGSA_Non_Loops_simplified!\n";

            // 6th) now I know the minimum set of producers and consumers, run GSA to see if any MUXes should be added for loops!
            general_dbg_file << "\nBefore applyGSA_Loops_simplified!\n";
            applyGSA_Loops_simplified(dbg_file, enode, groups, lazy_fork_flag); 
            general_dbg_file << "\nAfter applyGSA_Loops_simplified!\n";

            // 6th) 14/09/2022: added the following function to manage the insertion of Branches for the common loops (chain of Branches in case of common loop-nests)
            std::ofstream dbg_file_2;
            dbg_file_2.open("Managing_Loops_muxes_branches.txt");
            general_dbg_file << "\nBefore addLoopBranches_postGSA!\n";
            addLoopBranches_postGSA(dbg_file_2, enode, groups, lazy_fork_flag);
            general_dbg_file << "\nAfter addLoopBranches_postGSA!\n";

            general_dbg_file << "\nBefore addJoin!\n";
            // insert a Join for any component having more than one input
            addJoin(lazy_fork_flag);  // simply, if any group has more than one input, insert a Join in the same BB as that of the Bx that will represent the group!
            general_dbg_file << "\nAfter addJoin!\n";
        }

    }
         
}


void CircuitGenerator::convert_Merges_to_CMerges() {
    for(auto& enode: *enode_dag) {
        if(enode->type == Phi_c && !enode->isMux) {
            assert(enode->is_redunCntrlNet); // Any phi_c in my circuit that is not a MUX (i.e., a Merge) must be part of the redundant ordered network
            assert(enode->CntrlOrderSuccs->size() == 1);
            // convert it to a CMerge and connect the second output to a sink!
            enode->isCntrlMg = true;

            ENode* sink_node = new ENode(Sink_, "sink"); 
            sink_node->id    = sink_id++;
            enode_dag->push_back(sink_node);
            enode->CntrlOrderSuccs->push_back(sink_node);
            sink_node->CntrlOrderPreds->push_back(enode);
        }
    }
}

void CircuitGenerator::add_naive_ControlForLSQ() {
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
