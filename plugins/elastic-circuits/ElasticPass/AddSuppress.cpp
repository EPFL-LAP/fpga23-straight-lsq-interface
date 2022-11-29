/*
 * Authors: Ayatallah Elakhras
 * 			
 */
#include "ElasticPass/CircuitGenerator.h"
#include<iterator>
#include<algorithm>

/**
 * @brief 
 * @param 
 */
void CircuitGenerator::remove_SUPP_COND_Negation(){
	for(auto& enode: *enode_dag) {
		if(enode->type == Branch_n) {
			// check if the condition input (.at(1)) is negated!
			if(enode->is_negated_input->at(1)) {
				// switch the first output with the second output and set the is_negated_input to false!
				ENode* temp = enode->CntrlSuccs->at(1);
				enode->CntrlSuccs->at(1) = enode->CntrlSuccs->at(0);
				enode->CntrlSuccs->at(0) = temp;

				enode->is_negated_input->at(1) = false;
			}
		// repeating the same for control branches
		} else if(enode->type == Branch_c) {
			// check if the condition input (.at(1)) is negated!
			if(enode->is_negated_input->at(1)) {
				// switch the first output with the second output and set the is_negated_input to false!
				ENode* temp = enode->JustCntrlSuccs->at(1);
				enode->JustCntrlSuccs->at(1) = enode->JustCntrlSuccs->at(0);
				enode->JustCntrlSuccs->at(0) = temp;

				enode->is_negated_input->at(1) = false;
			}

		}
	}

}


/**
 * @brief fills the indices of BBs that the passed BB is control dependent on.
 * @param BB The basic block to identify control dependencies for.
 * @param BB_deps The function should fill it. The function assumes the ptr already points to an allocated memory
 * @return True if the passed BB is control dependent on at least 1 BB.
 * @note The properlyDominates function of the post-dominator tree counts a BB as not post-dominating itself
 */
void CircuitGenerator::identifyBBsForwardControlDependency() {
	std::ofstream dbg_file;
    dbg_file.open("bbs_control_deps.txt");

    std::ofstream dbg_file_verbose;
    dbg_file_verbose.open("bbs_control_deps_verbose.txt");

    /**
	 * By definition, bbnode_dag->at(i) is control dependent on bbnode_dag->at(j) iff 
	 * 1) there exists a directed path from bbnode_dag->at(j) to bbnode_dag->at(i) with all nodes in the path (excluding i and j) post-dominated by bbnode_dag->at(i)
	 * AND
	 * 2) bbnode_dag->at(j) is not post-dominated by bbnode_dag->at(i)
	 */
	for(int i = bbnode_dag->size()-1; i > 0; i--) {
		dbg_file_verbose << "\n****************************************\n";
		dbg_file_verbose << "\nCurrently studying the control dependencies of BB" << bbnode_dag->at(i)->Idx + 1 << "\n";
		// for every bbnode, check for forward control dependencies in all BBs before it in the CFG
		for(int j = 0; j < i; j++) {
			assert(bbnode_dag->at(i)->BB != bbnode_dag->at(j)->BB);
			// if bbnode is post-dominated by BB, then for sure BB is not control dependent on bbnode
			if(my_post_dom_tree->properlyDominates(bbnode_dag->at(i)->BB, bbnode_dag->at(j)->BB)) {
				dbg_file_verbose << "BB" << bbnode_dag->at(i)->Idx + 1 << " is not control dependent on BB" << bbnode_dag->at(j)->Idx + 1 << "!!\n";
				continue;  // no need to do anything!
			} else {
				// BB doesn't post-dominate bbnode, so check condition 1) for control dependence by enumerating paths
				assert(!my_post_dom_tree->properlyDominates(bbnode_dag->at(i)->BB, bbnode_dag->at(j)->BB));
				int cfg_size = bbnode_dag->size();
				bool *visited = new bool[cfg_size];
				int *path = new int[cfg_size];
				int path_index = 0; 
				std::vector<std::vector<int>>* final_paths = new std::vector<std::vector<int>>;
				enumerateBBpaths_dfs(bbnode_dag->at(i), bbnode_dag->at(j), visited, path, 0, final_paths, false);
				if(final_paths->size() == 0) {
					dbg_file_verbose << "BB" << bbnode_dag->at(i)->Idx + 1 << " is not reachable from BB" << bbnode_dag->at(j)->Idx + 1 << "!!\n";
					continue;
				} else {
					bool is_dep = true;

					///////////// Debugging ///////////////
					dbg_file_verbose << "\nThere are " << final_paths->size() << " paths between BB" << bbnode_dag->at(i)->Idx + 1 << " and BB" << bbnode_dag->at(j)->Idx + 1 << "\n";
					for(int kk = 0; kk < final_paths->size(); kk++) {
						dbg_file_verbose << "\nThe size of path " << kk << " is " << final_paths->at(kk).size() << "\n";
						dbg_file_verbose << "\tOne path is composed of: ";
						for(int hh = 0; hh < final_paths->at(kk).size(); hh++) {
							dbg_file_verbose << "BB" << bbnode_dag->at(final_paths->at(kk).at(hh))->Idx + 1 << ", ";
						} 
						dbg_file_verbose << "\n";
					} 
					///////////////////////////////////////

					// loop over all BBs in all of the enumerated paths
					for(int kk = 0; kk < final_paths->size(); kk++) {
						for(int hh = 0; hh < final_paths->at(kk).size(); hh++) {
							if(bbnode_dag->at(final_paths->at(kk).at(hh)) != bbnode_dag->at(i) && bbnode_dag->at(final_paths->at(kk).at(hh)) != bbnode_dag->at(j) && !my_post_dom_tree->properlyDominates(bbnode_dag->at(i)->BB, bbnode_dag->at(final_paths->at(kk).at(hh))->BB )) {
								is_dep = false;
								break;
							}
						} 
					}

					if(is_dep) { // BB is control dependent on bbnode
						dbg_file_verbose << "\n\tBB" << bbnode_dag->at(i)->Idx + 1 << " is control dependent on BB" << bbnode_dag->at(j)->Idx + 1 << "!!\n";
						bbnode_dag->at(i)->BB_deps->push_back(bbnode_dag->at(j)->Idx);
					} 
				}
				delete final_paths;
			}

		}
	}


	// For each BB in the dependency list, append also the dependencies of this BB
	for(auto& bbnode: *bbnode_dag) {
		// loop over the BBs in BB_deps of the bbnode
		for(int i = 0; i < bbnode->BB_deps->size(); i++) {
			int dep_bb_idx = bbnode->BB_deps->at(i);
			for(int j = 0; j < bbnode_dag->at(dep_bb_idx)->BB_deps->size(); j++) {
				// An extra check, append only if it was not already there
				if(std::find(bbnode->BB_deps->begin(), bbnode->BB_deps->end(), bbnode_dag->at(dep_bb_idx)->BB_deps->at(j)) == bbnode->BB_deps->end()) {
					bbnode->BB_deps->push_back(bbnode_dag->at(dep_bb_idx)->BB_deps->at(j));
				}
			}
		}
	}

	// Printing the control dependencies of each BB
	dbg_file << "\nThis file lists the control dependencies of each BB\n\n";
	dbg_file_verbose << "\n\n===================================================\n\n";
	dbg_file_verbose << "\nThe entire recurisve control dependencies is:\n";
	for(auto& bbnode : *bbnode_dag) {
		dbg_file << "\n****************************************\n";
		dbg_file << "\tBB" << bbnode->Idx + 1 << ":\n";

		dbg_file_verbose << "\n****************************************\n";
		dbg_file_verbose << "\tBB" << bbnode->Idx + 1 << ":\n";

		if(bbnode->BB_deps->size() == 0) {
			dbg_file << "Has no control dependencies!!\n\n";
			dbg_file_verbose << "Has no control dependencies!!\n\n";
		} else {
			for(int k = 0; k < bbnode->BB_deps->size(); k++) {
				dbg_file << bbnode->BB_deps->at(k) + 1<< ",";
				dbg_file_verbose << bbnode->BB_deps->at(k) + 1<< ",";
			}
			dbg_file << "\n\n";
			dbg_file_verbose << "\n\n";

		}
	}
	
}

/**
 * @brief backward enumerates all paths starting at start_BB till end_BB and saves indices of each path in final_paths.
 * @param start_BB Start enumerating from it by going through its predecessors.
 * @param end_BB End enumerating when you arrive at it.
 * @param visited True if the BB at that index is already visited. Should be initialized to False.
 * @param path Records the indices of a single path being enumerated at one point in time.
 * @param path_index Keeps track of the size of path[]. Initialized to 0.
 * @return False if there is no backward path between starting at start_BB and ending at end_BB.
 * @note The function assumes that final_paths already points to allocated memory. 
 * @note The function includes the start_BB and end_BB in any enumerated path.
 */
void CircuitGenerator::enumerateBBpaths_dfs(BBNode* start_BB, BBNode* end_BB, bool visited[], int path[], int path_index, std::vector<std::vector<int>>* final_paths, bool self_loop_flag) { 
	// visit your start_BB
	visited[start_BB->Idx] = true;
	path[path_index] = start_BB->Idx;
	path_index++;

	// AYA: 09/11/2022: commented the following code and replaced it with the below code!
	if(start_BB == end_BB) {  // 1 path is completed!
	    // insert this path in the structure final_paths
	    std::vector<int> temp(path, path + path_index);
	    final_paths->push_back(temp);
	} else {  // start node is still not the destination
		for(auto& bb_pred : *start_BB->CntrlPreds){
			if(!visited[bb_pred->Idx])
		    	enumerateBBpaths_dfs(bb_pred, end_BB, visited, path, path_index, final_paths, false);
	    }
	}

	path_index--;
	visited[start_BB->Idx] = false;
}

void CircuitGenerator::printCDFG() {
	std::ofstream dbg_file;
    dbg_file.open("cdfg.txt");

    for(auto& enode : *enode_dag) {
    	for(int i = 0; i < enode->CntrlPreds->size(); i++) {
    		dbg_file << "\n\n*****************************************\n\n";
    		dbg_file << "\n\tNew (Producer,Consumer) pair:\n";
    		if(enode->CntrlPreds->at(i)->BB != nullptr)
    			dbg_file << "ProducerBB: BB" << (BBMap->at(enode->CntrlPreds->at(i)->BB))->Idx + 1 << "  and producer type is: " << getNodeDotTypeNew(enode->CntrlPreds->at(i));
    		else
    			dbg_file << "ProducerBB: BB_null" << "  ";

    		if(enode->BB != nullptr)
    			dbg_file << "   ConsumerBB: BB" << (BBMap->at(enode->BB))->Idx + 1 << "  and producer type is: " << getNodeDotTypeNew(enode) << "\n";
    		else
    			dbg_file << "   ConsumerBB: BB_null" << "\n";
    	}
    	
    }
}

void CircuitGenerator::printControlDeps(std::ofstream& dbg_file, BBNode* virtBB) {
	dbg_file << "\tBB" << virtBB->Idx + 1 << ":\n";

	if(virtBB->BB_deps->size() == 0) {
		dbg_file << "Has no control dependencies!!\n\n";
	} else {
		for(int k = 0; k < virtBB->BB_deps->size(); k++) {
			dbg_file << virtBB->BB_deps->at(k) + 1<< ",";
		}
		dbg_file << "\n\n";

	}
}

/**
 * @brief 
 * @param 
 */
void CircuitGenerator::findVirtBBDeps(std::ofstream& dbg_file, BBNode* virtualBB) {
	BBNode* startBB = bbnode_dag->at(0);
	BBNode* endBB = bbnode_dag->at(bbnode_dag->size()-2);
	bool *visited = new bool[bbnode_dag->size()];
	int *path = new int[bbnode_dag->size()];
	int path_index = 0; 
	// needed to hold the enumerated paths 
	std::vector<std::vector<int>>* final_paths = new std::vector<std::vector<int>>;
	// 2nd enumerate all the paths between the current producer and this
	// reseting the data structures for a new prod_con pair
	for(int i = 0; i < bbnode_dag->size(); i++){
		visited[i] = false;
	}
	final_paths->clear();

	enumerate_paths_dfs_start_to_end(startBB, virtualBB, visited, path, path_index, final_paths);  // implemented in AddBridges.cpp
	for(int kk = 0; kk < final_paths->size(); kk++) {
		std::vector<int>* pierBBs = new std::vector<int>;
		find_path_piers(final_paths->at(kk), pierBBs, virtualBB, endBB);

		///// Debugging to check the pierBBs 
		dbg_file << "\n Printing all the piers of that path!\n";
		for(int u = 0; u < pierBBs->size(); u++) {
			dbg_file << pierBBs->at(u) + 1 << ", ";
		}	
		dbg_file << "\n\n";

		// loop over the pierBBs identified in each path and push them to BB_deps if they weren't already there!
		for(int h = 0; h < pierBBs->size(); h++) {
			if(std::find(virtualBB->BB_deps->begin(), virtualBB->BB_deps->end(), pierBBs->at(h)) == virtualBB->BB_deps->end())
				virtualBB->BB_deps->push_back(pierBBs->at(h));
		}
		delete pierBBs;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CircuitGenerator::addSuppress_with_loops(networkType network_flag) {
	std::ofstream dbg_file;
	if(network_flag == data)
    	dbg_file.open("check_eliminate_common_ancestors.txt");

    std::ofstream dbg_file_2;
    std::string file_name_3;
     switch(network_flag) {
    	case data:
    		file_name_3 = "check_boolean_f_prod_f_cons_data.txt";
    	break;
    	case constCntrl:
    		file_name_3 = "check_boolean_f_prod_f_cons_constants.txt";
    	break;
    	case memDeps:
    		file_name_3 = "check_boolean_f_prod_f_cons_LSQ_mem.txt";
    }
    dbg_file_2.open(file_name_3.c_str());

    std::ofstream dbg_file_3;
    dbg_file_3.open("check_MUX_SEL_predicate.txt");

    std::ofstream dbg_file_4;
    std::string file_name_2;
    switch(network_flag) {
    	case data:
    		file_name_2 = "check_virtualBBs_deps_data.txt";
    	break;
    	case constCntrl:
    		file_name_2 = "check_virtualBBs_deps._constants.txt";
    	break;
    	case memDeps:
    		file_name_2 = "check_virtualBBs_deps_LSQ_mem.txt";
    }
    dbg_file_4.open(file_name_2.c_str());

    std::ofstream dbg_file_5;
    std::string file_name;

    switch(network_flag) {
    	case data:
    		file_name = "check_final_f_prod_f_cons_data.txt";
    	break;
    	case constCntrl:
    		file_name = "check_final_f_prod_f_cons_constants.txt";
    	break;
    	case memDeps:
    		file_name = "check_final_f_prod_f_cons_LSQ_mem.txt";
    }

    dbg_file_5.open(file_name.c_str());

    std::ofstream dbg_file_6;
    dbg_file_6.open("check_Shannon's_on_Suppress_conds.txt");

	std::ofstream dbg_file_7;
    dbg_file_7.open("check_calculate_f_supp.txt");

	ENode* producer;
	ENode* consumer;

	for(auto& enode : *enode_dag) {  
		// AYa: 13/09/2022: added the following condition to BLINDLY skip if the consumer is a Phi_c at the loop header added as a part of
				// the GSA analysis of smart LSQs
		if(enode->type == Phi_c && enode->is_mem_dep_component && BBMap->at(enode->BB)->is_loop_header)
			continue;

		if(enode->BB == nullptr)
			continue;

		if(enode->CntrlPreds->size() > 0) {
			dbg_file << "\n$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n";
			dbg_file << "\nStarting a new consumer of type " << getNodeDotTypeNew(enode) << "!!\n\n";
		}

		std::vector<ENode*>* cons_preds;
		switch(network_flag) {
			case data:
				cons_preds = enode->CntrlPreds;
			break;
			case constCntrl:
				cons_preds = enode->JustCntrlPreds;
			break;
			case memDeps:
				cons_preds = enode->CntrlOrderPreds;
		}

		consumer = enode;

		for (int h = 0; h < cons_preds->size(); h++) {  // loop over its producers
			// AYA: 30/10/2022: added a condition to skip the SEL of the gsa_non_loop_mux from the analysis (smart LSQ related stuff)
			if(consumer->is_non_loop_gsa_mux && cons_preds == consumer->CntrlPreds && h == 0) {  // i.e., SEL of a mux is always the 0th predecessors in the CntrlPreds!
				assert(consumer->isMux && consumer->type == Phi_c && consumer->CntrlOrderPreds->size() == 2);
				// extra sanity check: THE FOLLOWING CHECK IS NO LONGER VALID because the non_loop_gsa_mux might be inside a loop so the addPhi pass would add Phis for regeneration!
				// assert(consumer->CntrlOrderPreds->at(1)->type == Start_);
				continue;
			} 

			// AYa: 13/09/2022: added the following condition to BLINDLY skip if the producer is a Branch_c inserted at the loop exit added as a part of
				// the GSA analysis of smart LSQs (because whether it feeds another branch in the context of nested loops or if it feeds a MUX, in both cases we do not want to run FPL!!)
			if(cons_preds->at(h)->type == Branch_c && cons_preds->at(h)->is_mem_dep_component && BBMap->at(cons_preds->at(h)->BB)->is_loop_exiting)
				continue;

			if(cons_preds->at(h)->BB == nullptr)
				continue;

			// Aya: 30/10/2022: extended the condition to also skip if the consumer is a non_loop_gsa_mux that could be inserted in the smart LSQ interfacing!
			if(cons_preds->at(h)->BB == consumer->BB && ( !(consumer->type == Phi_ || consumer->type == Phi_n || consumer->type == Phi_c) || (consumer->type == Phi_c  && consumer->is_non_loop_gsa_mux) ) ){
				// sanity checks: it can't be the case that the BB is both loop header and loop exit!!
				//assert(!(BBMap->at(enode->BB)->is_loop_header && (BBMap->at(enode->BB)->is_loop_exiting)));
				continue; // skip if the producerBB == consumerBB and the consumer is not of any Phi type (for regeneration or LLVM's loop-carried dependency)
			}

			producer = cons_preds->at(h);

			if(producer->type == Branch_n || producer->type == Branch_c || consumer->type == Branch_n || consumer->type == Branch_c) {
				// AYA: 15/09/2022: Added an extra check here to not skip if the consumer is a Branch_c that has is_mem_dep_component true and the producer is a BUffer_Bx!!
				if(!(consumer->type == Branch_c && consumer->is_mem_dep_component && producer->type == Bx_Buffer_wrapper_))
					continue;  // skip if you come across a SUPPRESS that is a producer or a consumer
			}


			if(producer->is_mux_for_cond || consumer->is_mux_for_cond) {  // is_mux_for_cond is a flag denoting MUXes that were added by Shannon's expansion in calculating the condition of any SUPPRESS or the SEL of any other MUX
				// extra sanity check, it should be a Phi_n of type MUX because those MUXes should output at least 1 bit of the condition!
				assert(producer->type == Phi_n /*&& producer->isMux*/ || consumer->type == Phi_n /*&& consumer->isMux*/);
				continue; // skip if you come across a MUX inserted for the COND of a SUPPRESS!! 
							// I added a flag in the ENode class to be able to easily detect those
			}

			// 16/06/2022: added this to skip if the producer or the consumer are the constant that is added as the input of a MUX feeding the cond of SUPPRESS or as the input of the Merge that serves as INIT
			if(producer->is_const_for_cond || consumer->is_const_for_cond || 
					producer->is_merge_init || consumer->is_merge_init)
				continue;

			std::vector<int> modif_consBB_deps;
			std::vector<int> modif_prodBB_deps;

			BBNode* consumerBB;
			BBNode* virtualBB = nullptr;

			dbg_file_2 << "\n\n======================================================\n\n";
			if(consumer->type == Phi_)
				dbg_file_2 << "\nConsumer of type LLVM Phi!!\n\n";
			else
				dbg_file_2 << "\nConsumer of type " << getNodeDotTypeNew(consumer) << "!!\n\n";
			dbg_file_2 << "\n\tNew (Producer,Consumer) pair: (BB" << BBMap->at(producer->BB)->Idx + 1 << ", BB" << BBMap->at(consumer->BB)->Idx  + 1<< ")\n";

			dbg_file_5 << "\n\n======================================================\n\n";
			dbg_file_5 << "\nNew (Producer,Consumer) pair: (BB" << BBMap->at(producer->BB)->Idx + 1 << ", BB" << BBMap->at(consumer->BB)->Idx  + 1<< ")\n";
			if(consumer->type == Phi_)
				dbg_file_5 << "\nConsumer of type LLVM Phi!!\n\n";
			else
				dbg_file_5 << "\nConsumer of type " << getNodeDotTypeNew(consumer) << "!!\n\n";


			// Note that I do this hack of inserting a virtualBB instead of having to later adjust f_cons when the cconsumer is a MUX .. It is an easier way of reaching for what we desire!
			// identify the consumer BB, it will be a virtualBB representing an edge if the consumer is of type PHI_
						// in the forward path not the loop header!!

			if((consumer->type == Phi_) && !consumer->isMux && !(BBMap->at(consumer->BB)->is_loop_header && 
				(BBMap->at(consumer->BB)->Idx <= BBMap->at(cons_preds->at(0)->BB)->Idx 
						|| BBMap->at(consumer->BB)->Idx <= BBMap->at(cons_preds->at(1)->BB)->Idx)					
			) ) {	
				dbg_file_5 << "\n\t Inserted a virtualBB between BB" << BBMap->at(producer->BB)->Idx + 1<< " and BB" <<  BBMap->at(consumer->BB)->Idx + 1<<"\n\n";
				virtualBB = new BBNode(nullptr , bbnode_dag->size());
				bbnode_dag->push_back(virtualBB);
				BasicBlock *llvm_predBB = (dyn_cast<PHINode>(consumer->Instr))->getIncomingBlock(h);
				dbg_file_4<< "\n\t The index of the llvm_predBB is " << BBMap->at(llvm_predBB)->Idx + 1 << "\n";
				// insert a virtualBB at the edge corresponding to this producer
				insertVirtualBB(consumer, virtualBB, llvm_predBB);  // implemented inside AddBridges.cpp
				consumerBB = virtualBB;
				// fill the BB_deps of the new virtualBB
				dbg_file_4 << "\n\n===================================================\n\n";
				dbg_file_4 << "\n\t VirtualBB between BB" << BBMap->at(producer->BB)->Idx + 1<< " and BB" <<  BBMap->at(consumer->BB)->Idx + 1<<"\n\n";
				findVirtBBDeps(dbg_file_4,virtualBB);
				printControlDeps(dbg_file_4, virtualBB);

			} else {
				consumerBB = BBMap->at(consumer->BB);
			}

			// Additions made to handle the case of the producer in a loop that the consumer is not inside (i.e., MORE PROD THAN CONS)
			bool more_prod_than_cons = false;
			if((BBMap->at(producer->BB))->loop != nullptr) {  // check if the producer is inside any loop
				// check if the consumer is not inside the loop that the producer is inside
				if(!(BBMap->at(producer->BB))->loop->contains(consumer->BB)) {
					// exclude cases of LLVM Phis inserted at loop headers!
					if(!(consumer->type == Phi_ && BBMap->at(producer->BB)->Idx >= BBMap->at(consumer->BB)->Idx))
						more_prod_than_cons = true;
				}
			}

			std::vector<std::vector<pierCondition>> f_cons; // sum of products representing the conditions of consuming the token
			std::vector<std::vector<pierCondition>> f_prod; // sum of products representing the conditions of producing the token

			// 19/06/2022: added the following flag that should be true only if the consumer is a PHi_ and the producer is arriving to it through a backward edge
				// the goal is to exclude the branches insertion of this part from the rest of the flow!!
			bool llvm_phi_case = false;

			if(more_prod_than_cons) {
				dbg_file_5 << "\n\t\t Triggering the more_prod_than_cons case!!!!\n";
				// need to force f_prod to be just 1 and before calculating f_cons, 
					// need to push the loop exit BBs of the loops that the producer is inside 
						// and the consumer is not inside to the modif_consBB_deps 
				// temp debugging printing the dependencies of modif_consBB_deps
				dbg_file_5 << "\n\tThe consumer deps before calling addLoopsExitstoDeps_ProdInConsOut are: ";
				for(int p = 0; p < modif_consBB_deps.size(); p++) {
					dbg_file_5 << " BB" << modif_consBB_deps.at(p) + 1 << ",  ";
				}	
				dbg_file << "\n\n";
				addLoopsExitstoDeps_ProdInConsOut(dbg_file_5, producer, consumer, modif_consBB_deps);
				if(consumer->type == Phi_ /*&& (BBMap->at(consumer->BB)->is_loop_header)*/) {  // the case of a Phi_ at a confluence point!
					// We should pass the original consumer's BB not consumerBB since it's a virtualBB!!
					constructSOP(dbg_file_2, bbnode_dag->at(0), BBMap->at(consumer->BB), modif_consBB_deps, f_cons);
				} else {
					constructSOP(dbg_file_2, bbnode_dag->at(0), consumerBB, modif_consBB_deps, f_cons);
				}
				
				///////////////////////// Trying the naive way to insert a Brnach every loop exit!!
				// The goal is to insert a Branch in every loop exit in the nest
				// 1st: identify the indices of the loop exits (ASSUMING THAT EACH LOOP HAS A SINGLE LOOP EXIT!!)
				/*std::vector<Loop*> loops_in_nest;
				std::vector<BBNode*> loop_exits;
				ENode* new_branch;
				FindLoopsInNest((BBMap->at(producer->BB))->loop, loops_in_nest);

				ENode* my_producer = producer;

				for(int i = 0; i < loops_in_nest.size(); i++) {
					loop_exits.clear();
					FindLoopExits(loops_in_nest.at(i), loop_exits);
					assert(loop_exits.size() == 1);

					// identify the condition in this loop exit
					std::vector<std::vector<pierCondition>> f;
					pierCondition loop_exit_condition;
					loop_exit_condition.pierBB_index = loop_exits.at(0)->Idx;
					//////////////// To identify if the condition should be negated or not 
					if((BBMap->at(producer->BB))->loop->contains((BBMap->at(producer->BB))->CntrlSuccs->at(0)->BB)) {
						// sanity check: the other successor must be outside the loop!
						assert(!(BBMap->at(producer->BB))->loop->contains((BBMap->at(producer->BB))->CntrlSuccs->at(1)->BB));
						loop_exit_condition.is_negated = false;
					} else {
						// sanity check: the other successor must be inside the loop!
						assert((BBMap->at(producer->BB))->loop->contains((BBMap->at(producer->BB))->CntrlSuccs->at(1)->BB));
						loop_exit_condition.is_negated = true;
					}
					//////////////////////////////////////////////////////////////
					std::vector<pierCondition> one_cond;
					one_cond.push_back(loop_exit_condition);
					f.push_back(one_cond);

					bool is_negated_flag;
					ENode* loop_exit_condition_enode;
					assert(f.size() == 1);
					loop_exit_condition_enode = returnSingleCondENode(f, is_negated_flag);
					// sanity checks
					assert(loop_exit_condition_enode != nullptr);
					assert(is_negated_flag == true || is_negated_flag == false);

					ENode* new_branch = insertBranch(const_trigger_network_flag, loop_exit_condition_enode, my_producer, consumer, is_negated_flag);
					my_producer = new_branch;
				}
				llvm_phi_case = true;*/
				///////////////////////////////////////////////////////////////////////////////////

				// TODO: in this previous if condition, I don't consider any control dependencies of the producer.. Check that this is correct!! 
			} else { 
				// the following is a very specific case of the phis of my insertion (phi_n or phi_c) for regeneration!
				if(producer == consumer) {

					// AYAAAA: TODO: 09/11/2022: CHANGE HERE TO HAVE THE THING IN THE INNERMOST LOOP ONLY IN EXPERIMENTING FOR Regenerate!!!! 

					dbg_file_5 << "\n\t\t Triggering the producer == consumer case!!!!\n";
					dbg_file_5 << "\n\t NEW DEUBUG INFO: The original producer is in BB" << BBMap->at((consumer->producer_to_branch)->BB)->Idx + 1 << "\n";

					// sanity check, they must be in the same loop
					assert((BBMap->at(producer->BB))->loop != nullptr && (BBMap->at(producer->BB))->loop == (BBMap->at(consumer->BB))->loop);
					
					// sanity check, the consumer BB is the loop header and the consumer is a PHI_n or PHI_c and we didn't convert it to a MUX yet
					assert( (consumer->type == Phi_n || consumer->type == Phi_c) && !consumer->isMux && (BBMap->at(consumer->BB)->is_loop_header) ); 

					// Although the the producer is a loop-header, the entire loop might be conditional, so this assertion is not accurate!!!!!!!
					// The producer is a Phi_n (or Phi_c) in the loop header so it shouldn't have any dependencies!
						// sanity check
					//assert((BBMap->at(producer->BB))->BB_deps->size() == 0 && modif_prodBB_deps.size() == 0);
					////////////////////////////////////////////////////////////////////////////////////////////

					// AYAA 09/11/2022: TEMPORARILY DOING THE OLD APPROACH ONLY FOR THE TWO CONTROL NETWORKS!
					//if(network_flag == constCntrl || network_flag == memDeps) {
						// add to the consumer dependency list all loop exits of all the loops in the loop-nest
						addLoopsExitstoDeps_Phi_regeneration(producer, consumer, modif_consBB_deps);
						// IMP to note that internally this function calls enumeratePaths between the startBB and the consumerBB. Here,
							// I'm passing the producerBB as the startBB to be able to count any loop exits (between the producer and the consumer through the backward edge) in the path 
						constructSOP(dbg_file_2, BBMap->at(producer->BB), consumerBB, modif_consBB_deps, f_cons); 
					 /*} else {
						// AYAA 09/11/2022: NEW CODE REPLACING THE ABOVE!
						// identify the loop exits of loops that the Phi_n is inside but the ENode stored in the producer_to_branch field is not inside!
						addLoopsExits_ConsInProdOut(consumer->producer_to_branch, consumer, modif_consBB_deps);

						dbg_file_5 << "\n\t NEW DEUBUG INFO: The side of modif_BB_deps is " << modif_consBB_deps.size() << " and the indices of BBs is ";

						// TEMPORARILY FOR DEBUGGING:
						for(int i = 0; i < modif_consBB_deps.size(); i++) {
							dbg_file_5 << modif_consBB_deps.at(i) + 1 << ", ";
						}
						//////////////////////////////////////////

						// EXPERIEMENT: 09/11/2022: I will cheat and assume that the consumer BB is the BB with the largest index in modif_consBB_deps
						//int largest_deps_bb_idx = -1;
						//for(int kk = 0; kk < modif_consBB_deps.size(); kk++) {
						//	if(modif_consBB_deps.at(kk) > largest_deps_bb_idx) {
						//		largest_deps_bb_idx = modif_consBB_deps.at(kk);
						//	}
						//}
						//assert(largest_deps_bb_idx != -1);

						constructSOP(dbg_file_2, BBMap->at(producer->BB), consumerBB, modif_consBB_deps, f_cons, true); 
					} */

				} else {
					// check if it is a case of LLVM's handling of a loop-carried dependency
					// with the consumer being PHI_ at the loop header and the producer coming late, connected through a backward edge
					if(BBMap->at(producer->BB)->Idx >= BBMap->at(consumer->BB)->Idx) {
						dbg_file_5 << "\n\t\t Triggering the backward edge case when the consumer is LLVM PHI_ !!!!\n";
						// NO, they don't have to be in the same loop!!
						// sanity check, they must be in the same loop
						//assert((BBMap->at(producer->BB))->loop != nullptr && (BBMap->at(producer->BB))->loop == (BBMap->at(consumer->BB))->loop);
						
						// sanity check, the consumer BB is the loop header and the consumer is LLVM PHI_ and we didn't convert it to a MUX yet

						// 31/10/2022
						// TEMPORARY CONDITION TO IGNORE FOR NOW THE FACT THAT the a Phi_c inserted in non_loop_gsa_mux will be a producer that is connected to the consumer through a backward edge!!
						if(producer->is_non_loop_gsa_mux) {
							assert(producer->type == Phi_c);
							continue;
						}

						assert( (consumer->type == Phi_) /*&& !consumer->isMux*/ && (BBMap->at(consumer->BB)->is_loop_header) ); 
						assert(BBMap->at(consumer->BB)->loop != nullptr && BBMap->at(producer->BB)->loop != nullptr);
						//////////////////////////////////// the goal now is to insert a single Branch for every loop!!
						// 1st check if both the producer and consumer are in the same loop since this will imply adding a single Branch
						if(BBMap->at(consumer->BB)->loop == BBMap->at(producer->BB)->loop) {
							// insert a single SUPPRESS with a condition = to all the loop exits of this loop
							bool is_negated_cond;
							std::vector<std::vector<pierCondition>> br_cond;
							// need to correctly fill modif_consBB_deps simply with the Ids of the loop exits of this loop!
							// identify all the BBs that serve as loop exits of this loop

							// Aya: 23/08/2022: did a fix to make sure that the loop exits has to be in the path between the producer and the consumer
									// (i.e., not before the producer for instance!)
							for(auto& bbnode: *bbnode_dag) {
								// if(BBMap->at(producer->BB)->loop->isLoopExiting(bbnode->BB) && BBMap->at(producer->BB)->loop->contains(bbnode->BB)) {
								if(BBMap->at(producer->BB)->loop->isLoopExiting(bbnode->BB) && BBMap->at(producer->BB)->loop->contains(bbnode->BB) && BBMap->at(bbnode->BB)->Idx >=  BBMap->at(producer->BB)->Idx) {
									modif_consBB_deps.push_back(bbnode->Idx);
								}
							}
							if(modif_consBB_deps.size() > 0)
								constructSOP(dbg_file_2, BBMap->at(producer->BB), consumerBB, modif_consBB_deps, br_cond);

							// 26/06/2022: do a check on br_cond to make sure that it includes the loop exit condition only once if the loop has a single loop exit!!
							if(modif_consBB_deps.size() == 1) {
								if(br_cond.size() > 1) {
									dbg_file_5 << "\n\tISSUE: need to fix the calculated loop exit condition!!\n";
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
							dbg_file_5 << "\n\t\t Printing the SOP of the SUPP in this case! The condition is opposite to SUPPRESS!!:\n";
							if(br_cond.size() == 0) {
								dbg_file_5 << " There is no SUPPRESS!!\n ";
							} 
							for(int u = 0; u < br_cond.size(); u++) {
								for(int v = 0; v < br_cond.at(u).size(); v++) {
									if(br_cond.at(u).at(v).is_negated)
										dbg_file_5 << " not C";
									else
										dbg_file_5 << " C";

									dbg_file_5 << br_cond.at(u).at(v).pierBB_index + 1 << " & ";
								}
								dbg_file_5 << " + ";
							}
							dbg_file_5 << "\n\n";
							///////////////////////////////////// end of print for debugging

							// AYA: 23/08/2022: enclosed the following statements inside this condition because we should not do anything if there are no dependencies!!
							if(modif_consBB_deps.size() > 0) {
								ENode* supp_condition_enode = applyShannon(dbg_file_6, dbg_file_5, producer->BB, br_cond, is_negated_cond);
								///////////// 
								if(is_negated_cond) {
									is_negated_cond = false; // this condition is opposite to the SUCCESSOR!!
								} else {
									is_negated_cond = true; // this condition is opposite to the SUCCESSOR!!
								}
								//////////////////////////////////////////////////////
								insertBranch(network_flag, supp_condition_enode, producer, consumer, is_negated_cond);
				

								// AYAA: 03/10/2022!!!
								// TEMPORARILY COMMENT THIS IF YOU WANT TO LEAVE MERGES AT LOOP HEADERS INSTEAD OF MUXES!!

								// we should convert it to MUX and "supp_condition_enode" should serve as the SEL of the MUX (either as is or negated!)
								//assert(!consumer->isMux);
								// the following function converts the Phi enode to a MUX 
								if(!consumer->isMux)
									convertToMux(network_flag, consumer, supp_condition_enode, is_negated_cond);
								
							}
							
						} else {
							// if you are here then the producer and the consumer are in different loops but connected through a backward edge
								// So, start from the loop of the producer and identify all the loops in the nest all the way up to
								// the loop of the consumer
							std::vector<Loop*> loops_in_nest;
							std::vector<BBNode*> loop_exits;
							ENode* new_prod = producer;
							Loop* current_loop = (BBMap->at(producer->BB))->loop;
							do {
								loops_in_nest.push_back(current_loop);
								current_loop = current_loop->getParentLoop();
							} while(current_loop!=nullptr && current_loop!= (BBMap->at(consumer->BB))->loop);

							if(current_loop == (BBMap->at(consumer->BB))->loop) {
								loops_in_nest.push_back(current_loop);
							}

							for(int i = 0; i < loops_in_nest.size(); i++) {
								loop_exits.clear();
								modif_consBB_deps.clear();
								FindLoopExits(loops_in_nest.at(i), loop_exits);
								for(int k = 0; k < loop_exits.size(); k++) {
									modif_consBB_deps.push_back(loop_exits.at(k)->Idx);
								}
								std::vector<std::vector<pierCondition>> br_cond;
								constructSOP(dbg_file_2, BBMap->at(producer->BB), consumerBB, modif_consBB_deps, br_cond);

								// 26/06/2022: do a check on br_cond to make sure that it includes the loop exit condition only once if the loop has a single loop exit!!
								if(loop_exits.size() == 1) {
									if(br_cond.size() > 1) {
										dbg_file_5 << "\n\tISSUE: need to fix the calculated loop exit condition!!\n";
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

										// 1) if it is the loop of the the consumer, we should suppress
											// if the loop is done iterating, so the negation should be depending on the loop exit's successor that is outside the loop!!

										// 2) But, if it is not the loop of the consumer (i.e., intermediate loop in the nest)
											// suppress if the loop is still not done iterating, so the negation should be depending on the loop exit's successor that is inside the loop!!
										if(bbnode_dag->at(check_cond_idx)->loop == BBMap->at(consumer->BB)->loop) {
											if(!bbnode_dag->at(check_cond_idx)->loop->contains(bbnode_dag->at(check_cond_idx)->CntrlSuccs->at(0)->BB)) {
												// sanity check, the loop must contain the false successor!!
												assert(bbnode_dag->at(check_cond_idx)->loop->contains(bbnode_dag->at(check_cond_idx)->CntrlSuccs->at(1)->BB));

												// this means the true successor is outside the loop, so I should suppress when the condition is true!
												corrected_cond.is_negated = true;  // since this represents the opposite of suppression!!
											} else {
												// sanity check, the loop must NOT contain the false successor!!
												assert(!bbnode_dag->at(check_cond_idx)->loop->contains(bbnode_dag->at(check_cond_idx)->CntrlSuccs->at(1)->BB));
												// this means the false successor is outside the loop, so I should not suppress when the condition is false!!
												corrected_cond.is_negated = false;
											}
										} else {
											// intermediate loop in the nest
											if(!bbnode_dag->at(check_cond_idx)->loop->contains(bbnode_dag->at(check_cond_idx)->CntrlSuccs->at(0)->BB)) {
												// sanity check, the loop must contain the false successor!!
												assert(bbnode_dag->at(check_cond_idx)->loop->contains(bbnode_dag->at(check_cond_idx)->CntrlSuccs->at(1)->BB));

												// this means the true successor is outside the loop, so I should suppress when the condition is false!!
												corrected_cond.is_negated = false;
											} else {
												// sanity check, the loop must NOT contain the false successor!!
												assert(!bbnode_dag->at(check_cond_idx)->loop->contains(bbnode_dag->at(check_cond_idx)->CntrlSuccs->at(1)->BB));
												// this means the false successor is outside the loop, so I should suppress when the condition is true!!
												corrected_cond.is_negated = true;
											}
										}

										std::vector<pierCondition> corrected_cond_product;
										corrected_cond_product.push_back(corrected_cond);
										br_cond.push_back(corrected_cond_product);

									}
								} 
								//////////////////////////////////////////////////////////////////////////////////////////

								/////////////////// Printing for debugging!! 
								dbg_file_5 << "\n\t\t Printing the SOP of the SUPP in this case! The condition is opposite to SUPPRESS!!:\n";
								if(br_cond.size() == 0) {
									dbg_file_5 << " 1 ";
								} 
								for(int u = 0; u < br_cond.size(); u++) {
									for(int v = 0; v < br_cond.at(u).size(); v++) {
										if(br_cond.at(u).at(v).is_negated)
											dbg_file_5 << " not C";
										else
											dbg_file_5 << " C";

										dbg_file_5 << br_cond.at(u).at(v).pierBB_index + 1 << " & ";
									}
									dbg_file_5 << " + ";
								}
								dbg_file_5 << "\n\n";
								/////////////////////////////////////

								bool is_negated_cond;
								ENode* supp_condition_enode = applyShannon(dbg_file_6, dbg_file_5, producer->BB, br_cond, is_negated_cond);
								///////////// 
								if(is_negated_cond) {
									is_negated_cond = false;
								} else {
									is_negated_cond = true;
								}

								ENode* new_branch = insertBranch(network_flag, supp_condition_enode, new_prod, consumer, is_negated_cond);
								assert(new_branch != nullptr);
								new_prod = new_branch;


								// AYAA: 03/10/2022!!!
								// TEMPORARILY COMMENT THIS IF YOU WANT TO LEAVE MERGES AT LOOP HEADERS INSTEAD OF MUXES!!
								if(i == loops_in_nest.size() - 1) {
									assert(!consumer->isMux);
									// we should be at the consumer's loop now!!
									assert(loops_in_nest.at(i) == (BBMap->at(consumer->BB))->loop);
									// convert the phi_ to MUX once in the end with the conditions of the consumer's loop (not the producer's)!!
									convertToMux(network_flag, consumer, supp_condition_enode, is_negated_cond);
								}

							}

							
							/*
							// producer must be in an inner loop and the consumer in an outer loop directly above the inner loop in the loop-nest
							assert(BBMap->at(consumer->BB)->loop == BBMap->at(producer->BB)->loop->getParentLoop());
							// need to insert two branches: 1 per loop

							//////// Insert the First branch between the producer and the consumer with the conditions of the Producer's loop
							bool is_negated_cond;
							std::vector<std::vector<pierCondition>> br_cond;
							for(auto& bbnode: *bbnode_dag) {
								if(BBMap->at(producer->BB)->loop->isLoopExiting(bbnode->BB) && BBMap->at(producer->BB)->loop->contains(bbnode->BB))
									modif_consBB_deps.push_back(bbnode->Idx);
							}
							constructSOP(dbg_file_2, BBMap->at(producer->BB), consumerBB, modif_consBB_deps, br_cond); 

							ENode* supp_condition_enode = applyShannon(dbg_file_6, dbg_file_5, producer->BB, br_cond, is_negated_cond);
							///////////// TEMPORARILY TESTING SOMETHING
							if(is_negated_cond) {
								is_negated_cond = false;
							} else {
								is_negated_cond = true;
							}
							//////////////////////////////////////////////////////

							ENode* first_branch = insertBranch(const_trigger_network_flag, supp_condition_enode, producer, consumer, is_negated_cond);
							assert(first_branch != nullptr);
							// we should convert it to MUX and "supp_condition_enode" should serve as the SEL of the MUX (either as is or negated!)
							assert(!consumer->isMux);

							/////// Insert the Second branch between the first branch and the consumer with the conditions of the Consumer's loop
							bool is_negated_cond_2;
							std::vector<std::vector<pierCondition>> br_cond_2;
							std::vector<int> modif_consBB_deps_2;
							modif_consBB_deps_2.clear();
							for(auto& bbnode: *bbnode_dag) {
								if(BBMap->at(consumer->BB)->loop->isLoopExiting(bbnode->BB) && BBMap->at(consumer->BB)->loop->contains(bbnode->BB))
									modif_consBB_deps_2.push_back(bbnode->Idx);
							}
							constructSOP(dbg_file_2, BBMap->at(producer->BB), consumerBB, modif_consBB_deps_2, br_cond_2); 

							ENode* supp_condition_enode_2 = applyShannon(dbg_file_6, dbg_file_5, producer->BB, br_cond_2, is_negated_cond_2);
							///////////// TEMPORARILY TESTING SOMETHING
							if(is_negated_cond_2) {
								is_negated_cond_2 = false;
							} else {
								is_negated_cond_2 = true;
							}
							//////////////////////////////////////////////////////
							insertBranch(const_trigger_network_flag, supp_condition_enode_2, first_branch, consumer, is_negated_cond_2);
			
							// we should convert it to MUX and "supp_condition_enode" should serve as the SEL of the MUX (either as is or negated!)
							assert(!consumer->isMux);

							/////////////////////////////////////////////////////////////////////////////////////////////////////////////
							// convert the phi_ to MUX once in the end with the conditions of the consumer's loop (not the producer's)!!
							convertToMux(const_trigger_network_flag, consumer, supp_condition_enode_2, is_negated_cond_2);
							*/
						}

						llvm_phi_case = true;
						
					} else {
						dbg_file_5 << "\n\t\t Triggering the normal non-loop addSuppress case!!!!\n";
						// do the standard non-loop addSuppress algorithm 
						eliminateCommonControlAncestors(dbg_file, consumerBB, BBMap->at(producer->BB), modif_consBB_deps, modif_prodBB_deps); 
						constructSOP(dbg_file_2, bbnode_dag->at(0), consumerBB, modif_consBB_deps, f_cons); 
						constructSOP(dbg_file_2, bbnode_dag->at(0), BBMap->at(producer->BB), modif_prodBB_deps, f_prod);
					}
				}
			}

			///////////////////////////////////////////////////////////////////////
			// some prints for debugging!
			if(!llvm_phi_case) {
				dbg_file_5 << "\n\t The finalized f_prod = ";
				if(f_prod.size() == 0) {
					dbg_file_5 << " 1 ";
				} 
				for(int u = 0; u < f_prod.size(); u++) {
					for(int v = 0; v < f_prod.at(u).size(); v++) {
						if(f_prod.at(u).at(v).is_negated)
							dbg_file_5 << " not C";
						else
							dbg_file_5 << " C";

						dbg_file_5 << f_prod.at(u).at(v).pierBB_index + 1 << " & ";
					}
					dbg_file_5 << " + ";
				}
				dbg_file_5 << "\n\n";

				dbg_file_5 << "\n\t The finalized f_cons = ";
				if(f_cons.size() == 0) {
					dbg_file_5 << " 1 ";
				} 
				for(int u = 0; u < f_cons.size(); u++) {
					for(int v = 0; v < f_cons.at(u).size(); v++) {
						if(f_cons.at(u).at(v).is_negated)
							dbg_file_5 << " not C";
						else
							dbg_file_5 << " C";

						dbg_file_5 << f_cons.at(u).at(v).pierBB_index + 1 << " & ";
					}
					dbg_file_5 << " + ";
				}
				dbg_file_5 << "\n\n";
			} else {
				dbg_file_5 << "\n\t Not printing the Boolean expressions since we are in a case of LLVM PHI!!";
			}
			
			std::vector<std::vector<pierCondition>> f_supp;
			bool suppress_flag = false;

			// added the following variables to manage the case of adding a generate
			std::vector<std::vector<pierCondition>> f_gen;
			bool generate_flag = false;

			// this condition is true only in the normal non loop count mismatch case!
			if(!llvm_phi_case) {
				suppress_flag = calculate_F_Supp(dbg_file_7, f_prod, f_cons, f_supp);

				///////////////////////////////////////////////////////////////
				// 04/09/2022: IN the mission of adding a Generate where needed:
					// 1_ TODO: complete the implementation of 'if(generate_flag && !llvm_phi_case)'
					// 2) TODO: test the completeness and correctness of my calculate_F_Gen!!!!
					// 3) TODO: do the necessary fixes in Memory.cpp to follow the new algorithm!!
					// 4) TODO: check that I'm correctly handling the following case!
					// In case of a combination of a token count mismatch and the need for Generate or SUppress, my implementation should
						// FIrst manage the stuff of token count mismatch (i.e., adding SUppress and Generate where needed) THEN, Add the necessary components
				/////////////////////////////////////////////////////////////

				// 04/09/2022: ADD HERE A FUNCTION TO CALCULATE F_GEN
				//generate_flag = calculate_F_Gen(dbg_file_7, f_prod, f_cons, f_gen);

			}
			//////////////////////////////// Debugging output ///////////////////////////////////
			// Add a branch only if suppress_flag is true!!
			if(suppress_flag && !llvm_phi_case) {
				dbg_file_5 << "\n\t Add a SUPPRESS with f_supp = \n";
				for(int u = 0; u < f_supp.size(); u++) {
					for(int v = 0; v < f_supp.at(u).size(); v++) {
						if(f_supp.at(u).at(v).is_negated)
						dbg_file_5 << " not C";
						else
						dbg_file_5 << " C";

						dbg_file_5 << f_supp.at(u).at(v).pierBB_index + 1 << " & ";
					}
					dbg_file_5 << " + ";
				}
				dbg_file_5 << "\n\n";
			} else {
				if(!suppress_flag && !llvm_phi_case)
					dbg_file_5 << "\n\t Don't need to add a SUPPRESS!!\n\n";
			}
			///////////////////////////////////////////////////////////////////////////////
			if(suppress_flag && !llvm_phi_case) {
				// create a new Branch object and insert it between the producer and consumer!
				// run Shannon's expansion on f_supp to correctly calculate the condition
				bool is_negated_cond;

				dbg_file_5 << "\n\tRight before calling applyShannon, the size of f_supp is: " << f_supp.size() << "\n";
				ENode* supp_condition_enode = applyShannon(dbg_file_6, dbg_file_5, producer->BB, f_supp, is_negated_cond);

				ENode* new_branch = insertBranch(network_flag, supp_condition_enode, producer, consumer, is_negated_cond);
				
				// AYAA: 03/10/2022!!!
				// TEMPORARILY COMMENT THIS IF YOU WANT TO LEAVE MERGES AT LOOP HEADERS INSTEAD OF MUXES!!

				// 15/06/2022: if the producer == consumer == type Phi_n (or Phi_c) 
				// we should convert it to MUX and "supp_condition_enode" should serve as the SEL of the MUX (either as is or negated!)
				if( (producer == consumer && (producer->type == Phi_n || producer->type == Phi_c)) ) {
					// Aya: 23/06/2022: commented the following condition since I now take care of converting the LLVM PHIs to Muxes above!!!!
					// ||(consumer->type == Phi_ && BBMap->at(consumer->BB)->is_loop_header) ) {
					assert(!consumer->isMux);
					// the following function converts the Phi enode to a MUX and
					convertToMux(network_flag, consumer, supp_condition_enode, is_negated_cond);

				} 

				// 30/10/2022: added extra code to check if the consumer is of type Phi_c and is_non_loop_gsa_mux and we happened to add a Branch. In which case, we should invert the negation state corresponding to the connection between this Branch and the PHi_c
					// this is because this Branch should only deliver the token to the Phi_c if the BB containing the Phi_c doesn't execute!!!! (Stuff related to smart LSQs!)
				if(consumer->type == Phi_c && consumer->is_non_loop_gsa_mux) {
					// sanity check
					assert(consumer->isMux);
					// the goal is to invert the is_negated_input->at(1) of this branch which is corresponding to the condition of this branch
					assert(new_branch->is_negated_input->size() == 2);
					if(new_branch->is_negated_input->at(1) == true) {
						new_branch->is_negated_input->at(1) = false;
					} else {
						assert(new_branch->is_negated_input->at(1) == false);
						new_branch->is_negated_input->at(1) = true;
					}
					
				}
			} 

			///////////////////////////////////////////////// Added what is enclosed here on 04/09/2022
			/*if(generate_flag && !llvm_phi_case) {
				// create a new MUX object and insert it between the producer and consumer!
				// run Shannon's expansion on f_supp to correctly calculate the condition
				bool is_negated_cond;

				dbg_file_5 << "\n\tRight before calling applyShannon, the size of f_supp is: " << f_supp.size() << "\n";
				ENode* supp_condition_enode = applyShannon(dbg_file_6, dbg_file_5, producer->BB, f_supp, is_negated_cond);

				insertBranch(network_flag, supp_condition_enode, producer, consumer, is_negated_cond);
			
				// 15/06/2022: if the producer == consumer == type Phi_n (or Phi_c) 
				// we should convert it to MUX and "supp_condition_enode" should serve as the SEL of the MUX (either as is or negated!)
				if( (producer == consumer && (producer->type == Phi_n || producer->type == Phi_c)) ) {
					// Aya: 23/06/2022: commented the following condition since I now take care of converting the LLVM PHIs to Muxes above!!!!
					// ||(consumer->type == Phi_ && BBMap->at(consumer->BB)->is_loop_header) ) {
					assert(!consumer->isMux);
					// the following function converts the Phi enode to a MUX and
					convertToMux(network_flag, consumer, supp_condition_enode, is_negated_cond);

				}
			} */
			///////////////////////////////////////////////////////////////////////////////////////////

			if( (consumer->type == Phi_) && !consumer->isMux && !(BBMap->at(consumer->BB)->is_loop_header && 
				(BBMap->at(consumer->BB)->Idx <= BBMap->at(cons_preds->at(0)->BB)->Idx 
						|| BBMap->at(consumer->BB)->Idx <= BBMap->at(cons_preds->at(1)->BB)->Idx)					
			) ) {
				BasicBlock *llvm_predBB = (dyn_cast<PHINode>(consumer->Instr))->getIncomingBlock(h);
				assert(virtualBB!=nullptr);
				removeVirtualBB(consumer, virtualBB, llvm_predBB); 
			}
		}
	} 
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief 
 * @param 
 */
void CircuitGenerator::convertToMux(networkType network_flag, ENode* phi, ENode* supp_cond, bool is_negated_cond) {	
	// need to know if the supp_cond should be inverted or not
	// supp_cond produces a 1 when we should suppress the token.. 
	// In our case here (i.e., loop regeneration or loop-carried dependency with a Phi consumer)
		// suppressing happens when there are no longer any further loop iterations 
	// So, the input of the phi .at(1) should be coming from a producer outside the loop
	std::vector<ENode*>* phi_preds;

	switch(network_flag) {
		case data:
			phi_preds = phi->CntrlPreds;	
		break;

		case constCntrl:
			phi_preds = phi->JustCntrlPreds;	
		break;

		case memDeps:
			phi_preds = phi->CntrlOrderPreds;	
	}

	int cstValue = 1;
	std::string cst_name = std::to_string(1); 
	// check if the pred.at(1) is contained inside the loop or not 
	if(BBMap->at(phi->BB)->loop->contains(phi_preds->at(1)->BB)) {
		// sanity check: the pred.at(0) must not be coming from inside the loop
		//assert(BBMap->at(phi_preds->at(0)->BB)->loop != BBMap->at(phi->BB)->loop);
		assert(!BBMap->at(phi->BB)->loop->contains(phi_preds->at(0)->BB));
		// swap the two inputs
		ENode* tmp = phi_preds->at(1);
		phi_preds->at(1) = phi_preds->at(0);
		phi_preds->at(0) = tmp;

		//assert(BBMap->at(phi_preds->at(1)->BB)->loop != BBMap->at(phi->BB)->loop && BBMap->at(phi_preds->at(0)->BB)->loop == BBMap->at(phi->BB)->loop);

		// extra sanity check to make sure that the swap was correct and did waht we want!
		assert(!BBMap->at(phi->BB)->loop->contains(phi_preds->at(1)->BB) && BBMap->at(phi->BB)->loop->contains(phi_preds->at(0)->BB));

	} else {
		// sanity check: the pred.at(0) must be coming from inside the loop
		assert(!BBMap->at(phi->BB)->loop->contains(phi_preds->at(0)->BB));
		// do nothing!
	}

	// then, should insert a Merge and connect the supp_cond to it 
	// before adding a new Merge and constant, search for an existing Merge and connect to it!!
	bool merge_exists_flag;
	ENode* merge_for_sel = nullptr;
	ENode* temp = new_MergeExists(merge_exists_flag, phi->BB, supp_cond);
	if(merge_exists_flag) {
		// the function should return the existing Merge!
		assert(temp != nullptr);
		merge_for_sel = temp;
	} else {
		// add a new INIT component.. All of the connections around it should be through the data (CntrlSuccs and CntrlPreds) network since it transfers a SEL that is at least 1 bit
		merge_for_sel = new ENode(Phi_n, "phi", phi->BB);   // mimics the INIT component
		merge_for_sel->id = phi_id++;

		merge_for_sel->is_mux_for_cond = true;

		// AYA: 19/03/2022: mark this as a special Merge node added for initialization..
		merge_for_sel->is_init_token_merge = true;

		merge_for_sel->is_merge_init = true; // I use this flag to not convert this Merge to MUX later and to skip it in the
									// connections of prod, cons in addSuppress

		enode_dag->push_back(merge_for_sel);

		bool found_init_const = false;
		ENode* existing_init_const = nullptr;
		for(auto& ee : *enode_dag) {
			if(ee->type == Cst_ && ee->is_init_token_const) {
				found_init_const = true;
				existing_init_const = ee;
				break;
			}
		} 

		if(found_init_const) {
			assert(existing_init_const != nullptr);
			merge_for_sel->CntrlPreds->push_back(existing_init_const);
			existing_init_const->CntrlSuccs->push_back(merge_for_sel);
		} else {
			// create a new constant
			ENode* cst_condition = new ENode(Cst_, cst_name.c_str(), bbnode_dag->at(0)->BB);
			cst_condition->is_init_token_const = true;
			// this is very important to specify the value of the constant which should be 1 here to make the branch true all the time!!
			cst_condition->cstValue = cstValue;
			enode_dag->push_back(cst_condition);
			cst_condition->id = cst_id++;

			cst_condition->is_const_for_cond = true;
			
			merge_for_sel->CntrlPreds->push_back(cst_condition);
			cst_condition->CntrlSuccs->push_back(merge_for_sel);

			// trigger the constant from START
			// a direct connection to START since the initialization token should arrive to the mux at the loop header regardless of any control structure!!!
			for(auto& cnt_node : *bbnode_dag->front()->Cntrl_nodes) {
				if(cnt_node->type == Start_) {
					cst_condition->JustCntrlPreds->push_back(cnt_node);
					cnt_node->JustCntrlSuccs->push_back(cst_condition);
					break; // found the start node, no need to continue looping
				}
			} 
		}

		// feed the Merge with the supp_cond
		merge_for_sel->CntrlPreds->push_back(supp_cond);
		supp_cond->CntrlSuccs->push_back(merge_for_sel);
		// there are chances now that the SEL input of the merge_for_sel is negated
			// so mark it to be checked in the DOT file!
		merge_for_sel->is_advanced_component = true;
		// the first input is the constant and we don't invert need!
		merge_for_sel->is_negated_input->push_back(false);
		// the second input is supp_cond. Its negation is dependent on the passed is_negated_cond
		merge_for_sel->is_negated_input->push_back(is_negated_cond);
		assert(merge_for_sel->CntrlPreds->size() == merge_for_sel->is_negated_input->size());
	}

	// let the newly created Merge feed the SEL of the phi node
	assert(merge_for_sel != nullptr);
	phi->isMux = true;

	switch(network_flag) {
		case data:
			phi->CntrlPreds->push_back(phi->CntrlPreds->at(1));
			phi->CntrlPreds->at(1) = phi->CntrlPreds->at(0);
			// the SEL signal of the mux must be .at(0)
			phi->CntrlPreds->at(0) = merge_for_sel;
			merge_for_sel->CntrlSuccs->push_back(phi);
		break;

		case constCntrl:
		case memDeps:
			// even if we are in any of the two control networks, the SEL should be connected over the data network!!
			assert(phi->CntrlPreds->size() == 0);
			phi->CntrlPreds->push_back(merge_for_sel);
			merge_for_sel->CntrlSuccs->push_back(phi);
	}

}

/**
 * @brief check if the hacky Merge for the intiialization token is already present in the BB
 * @param 
 */
ENode* CircuitGenerator::new_MergeExists(bool&merge_exists, BasicBlock* target_bb, ENode* cond) {
	for(auto& enode:*enode_dag) {
		if(enode->type == Phi_n && !enode->isMux && enode->BB == target_bb) {
			// it needs to be a having the first predecessor a MUX and a successor as another Phi_n or Phi_!
			if(enode->CntrlPreds->at(0)->type == Cst_ && (enode->CntrlSuccs->at(0)->type == Phi_ || enode->CntrlSuccs->at(0)->type == Phi_n || enode->CntrlSuccs->at(0)->type == Phi_c ) ) {
				if(enode->CntrlPreds->at(1) == cond) {
					merge_exists = true;
					return enode;
				}
			}

		}
	} 
	merge_exists = false;
	return nullptr;
}


/**
 * @brief  
 * @param 
 */
ENode* CircuitGenerator::insertBranch(networkType network_flag, ENode* branch_condition, ENode* producer, ENode* consumer, bool is_negated_cond) {
	std::vector<ENode*>* producer_succs;
	// 19/06/2022: in the special case of the consumer being LLVM's phi_, the backward edge could be emerging from an inner loop,
		// in which case we insert multiple branches, thus we have the producer being a Branch
	// Note that: Given that we construct Suppressors (i.e., a Branch with a sink in its trueSuccs, we should find the original consumer in the falseSucc!!!)
	std::vector<ENode*>* producer_false_succs;
	std::vector<ENode*>* producer_true_succs;
	
	std::vector<ENode*>* consumer_preds;

	node_type branch_type; // shall we create a Branch_c or Branch_n
	std::string branch_name; 

	switch(network_flag) {
		case data:
			producer_succs = producer->CntrlSuccs;
			consumer_preds = consumer->CntrlPreds;

			producer_false_succs = producer->false_branch_succs;  // applicable in case of producers of type Branch only!! (only happening in case of loops for regenration)
			producer_true_succs = producer->true_branch_succs;  // applicable in case of producers of type Branch only!! (only happening in case of loops for regenration)

			branch_type = Branch_n;
			branch_name = "branch";
		break;

		case constCntrl:
			producer_succs = producer->JustCntrlSuccs;

			producer_false_succs = producer->false_branch_succs_Ctrl;  // applicable in case of producers of type Branch only!! (only happening in case of loops for regenration)
			producer_true_succs = producer->true_branch_succs_Ctrl;

			consumer_preds = consumer->JustCntrlPreds;
			branch_type = Branch_c;
			branch_name = "branchC";
		break;

		case memDeps:
			producer_succs = producer->CntrlOrderSuccs;

			producer_false_succs = producer->false_branch_succs_Ctrl_lsq;  // applicable in case of producers of type Branch only!! (only happening in case of loops for regenration)
			producer_true_succs = producer->true_branch_succs_Ctrl_lsq;  // applicable in case of producers of type Branch only!! (only happening in case of loops for regenration)

			consumer_preds = consumer->CntrlOrderPreds;
			branch_type = Branch_c;
			branch_name = "branchC";
	}
	

	ENode* branch = new ENode(branch_type, branch_name.c_str(), producer->BB);  // we insert the Branch in the producer BB 
	branch->id = branch_id++;
	enode_dag->push_back(branch);

	branch->producer_to_branch = producer;
	branch->true_branch_succs = new std::vector<ENode*>;
	branch->false_branch_succs = new std::vector<ENode*>;
	branch->true_branch_succs_Ctrl = new std::vector<ENode*>;
	branch->false_branch_succs_Ctrl = new std::vector<ENode*>;

	branch->true_branch_succs_Ctrl_lsq = new std::vector<ENode*>;
	branch->false_branch_succs_Ctrl_lsq = new std::vector<ENode*>;

	// - Data should be .at(0)
	// find the index of the consumer in the producer's successors to replace it with the Branch
	// AYA: 16/09/2022: changed the following condition to be checking on the type of Branch depending on the network flag!
	//if(producer->type == Branch_n) {
	if(producer->type == branch_type) {
		// AYA: 16/09/2022: Commented the following and  made it generic here that the producer could have its consumer on its true or false succs (not blindly the false as expected by FPL's SUPPRESS) because now I add branches for GSA in the smart interfacing with LSQ and those branches could have the consumer connected on either of its true or false succs!!
		// Given that we construct Suppressors (i.e., a Branch with a sink in its trueSuccs, we should find the original consumer in the falseSucc!!!)
		/*assert(producer_false_succs->size() != 0);

		auto pos_1 = std::find(producer_false_succs->begin(), producer_false_succs->end(), consumer);
		assert(pos_1 != producer_false_succs->end());   
		auto index_1 = pos_1 - producer_false_succs->begin();
		producer_false_succs->at(index_1) = branch;*/
		// start by searching for it in the false
		auto pos_false = std::find(producer_false_succs->begin(), producer_false_succs->end(), consumer);
		if(pos_false != producer_false_succs->end()) {
			auto index_1 = pos_false - producer_false_succs->begin();
			producer_false_succs->at(index_1) = branch;
		} else {
			// search for it in the true
			auto pos_true = std::find(producer_true_succs->begin(), producer_true_succs->end(), consumer);
			assert(pos_true != producer_true_succs->end());
			auto index_1 = pos_true - producer_true_succs->begin();
			producer_true_succs->at(index_1) = branch;
		}
		

		switch(network_flag) {
			case data:
				branch->CntrlPreds->push_back(producer);
			break;

			case constCntrl:
				branch->JustCntrlPreds->push_back(producer);
			break;

			case memDeps:
				branch->CntrlOrderPreds->push_back(producer);
		}


	} else {
		auto pos_1 = std::find(producer_succs->begin(), producer_succs->end(), consumer);
		assert(pos_1 != producer_succs->end());
		auto index_1 = pos_1 - producer_succs->begin();
		producer_succs->at(index_1) = branch;

		switch(network_flag) {
			case data:
				branch->CntrlPreds->push_back(producer);
			break;

			case constCntrl:
				branch->JustCntrlPreds->push_back(producer);
			break;

			case memDeps:
				branch->CntrlOrderPreds->push_back(producer);
		}
	}
	
	// find the index of the producer in the consumer's predecessors to replace it with the Branch
	auto pos_2 = std::find(consumer_preds->begin(), consumer_preds->end(), producer);
	assert(pos_2 != consumer_preds->end());
	auto index_2 = pos_2 - consumer_preds->begin();
	consumer_preds->at(index_2) = branch;

	// the consumer should be at the False side of the Branch (i.e., SUPRESS if the branch_condition is True)
	switch(network_flag) {
		case data:
			branch->false_branch_succs->push_back(consumer);
		break;

		case constCntrl:
			branch->false_branch_succs_Ctrl->push_back(consumer);
		break;

		case memDeps:
			branch->false_branch_succs_Ctrl_lsq->push_back(consumer);
	}

	// the branch condition is at least 1 bit so we ALWAYS connect the condition over the data network even if it's a Branch_c
	branch_condition->CntrlSuccs->push_back(branch);
	branch->CntrlPreds->push_back(branch_condition);
	

	// Fill information about whether either of the two inputs of the Branch are negated or not
	branch->is_advanced_component = true;
	// the data input of the Branch should not be negated!!
	branch->is_negated_input->push_back(false); 
	// the passed is_negated_cond suggests if the condition of the Branch should be negated or not
	branch->is_negated_input->push_back(is_negated_cond);

	return branch;
}

/**
 * @brief  
 * @param 
 * @return 
 */
ENode* CircuitGenerator::applyShannon(std::ofstream& dbg_file, std::ofstream& dbg_file_5, BasicBlock* target_bb, const std::vector<std::vector<pierCondition>>& f, bool& is_condition_negated, bool print_flag) {
	std::vector<int> condition_variables;
	std::vector<std::string> f_string;

	dbg_file_5 << "\n\tInside applyShannon\n";

	new_get_boolean_function_variables(f, condition_variables);  // fills the condition_variables with different conditions constituting the function
	new_get_binary_string_minterms_Shannons(dbg_file, f, condition_variables, f_string);//, print_flag);

	dbg_file_5 << "\n\tPrinting the f_supp after converting it to string\n";
	for(int i = 0; i < f_string.size(); i++) {
		dbg_file_5 << f_string.at(i) << " + ";
	}
	dbg_file_5 << "\n\n";


	Shannon_Expansion shannon;
	Shannon_Expansion::MUXrep f_mux;
	f_mux = shannon.apply_shannon(f_string, &condition_variables, dbg_file);

	ENode* condition_enode = nullptr; // can be a MUX node that I create or an already existing BB condition

	if(f_mux.is_valid) {
		// then f is calculated by one or more MUXes
			// call the following function to create the necessary MUXes in the target_bb

		dbg_file_5 << "\nF_SUPP is a big condition!!\n";
		// check the f_mux expression
		shannon.printMUX(dbg_file_5, f_mux);

		// TODO: MAKE SURE YOU ADD A CONDITION TO SEE IF THE INPUT OF THE MUX IS A CONDITION OR ANOTHER MUX!!

		///////////// TODO: clean this part!!
		// Before adding a new MUX, check if an identical one already exists!!
		bool found = false;
		ENode* exisitng_mux = nullptr;
		for(int k = 0; k < enode_dag->size(); k++) {
			if(enode_dag->at(k)->BB != target_bb || enode_dag->at(k)->type != Phi_n || !enode_dag->at(k)->isMux)
				continue;  // skip if the enode is not in the target_BB
			else {
				// then it must be an enode MUX in the target_bb. Check the SEL and the two inputs of the MUX
				assert(enode_dag->at(k)->CntrlPreds->size() == 3);
				if((BBMap->at(enode_dag->at(k)->CntrlPreds->at(0)->BB))->Idx == f_mux.cond_var_index) {
					// TODO: for now I support sharing only if the inputs of the MUX are not other MUXes!!
					if(f_mux.in0_preds_muxes.size() == 0 && f_mux.in1_preds_muxes.size() == 0) {
						assert(f_mux.in0.size() == 1 && f_mux.in1.size() == 1);
						assert(f_mux.in0.at(0).size() == 1 && f_mux.in1.at(0).size() == 1);
						if(f_mux.in0.at(0).at(0).is_const) {
							if(enode_dag->at(k)->CntrlPreds->at(1)->type == Cst_ && enode_dag->at(k)->CntrlPreds->at(1)->cstValue == f_mux.in0.at(0).at(0).const_val) {
								// check the second input of the MUX, it can't be also a constant!!!
								assert(!f_mux.in1.at(0).at(0).is_const);
								if((BBMap->at(enode_dag->at(k)->CntrlPreds->at(2)->BB))->Idx == f_mux.in1.at(0).at(0).var_index && enode_dag->at(k)->is_negated_input->at(2) == f_mux.in1.at(0).at(0).is_negated) {
									exisitng_mux = enode_dag->at(k);
									found = true;
									break;
								}
							}
						} else {
							// then in0 must be a condition
							if((BBMap->at(enode_dag->at(k)->CntrlPreds->at(1)->BB))->Idx == f_mux.in0.at(0).at(0).var_index && enode_dag->at(k)->is_negated_input->at(1) == f_mux.in0.at(0).at(0).is_negated) {
								// check the second input of the MUX, 
								if(f_mux.in1.at(0).at(0).is_const) {
									if(enode_dag->at(k)->CntrlPreds->at(2)->type == Cst_ && enode_dag->at(k)->CntrlPreds->at(2)->cstValue == f_mux.in1.at(0).at(0).const_val) {
										exisitng_mux = enode_dag->at(k);
										found = true;
										break;
									}
								} else {
									// maybe the second input happens to be a condition as well!!
									if((BBMap->at(enode_dag->at(k)->CntrlPreds->at(2)->BB))->Idx == f_mux.in1.at(0).at(0).var_index && enode_dag->at(k)->is_negated_input->at(2) == f_mux.in1.at(0).at(0).is_negated) {
										exisitng_mux = enode_dag->at(k);
										found = true;
										break;
									}
							
								}

							}
						}
					}
				}
			}
		}
		//////////////////////////////////////////////////////

		if(found) {
			assert(exisitng_mux != nullptr);
			condition_enode = exisitng_mux;
		} else {
			// create a new MUX
			condition_enode = addNewMuxes_wrapper(f_mux, target_bb);
		}

		assert(condition_enode != nullptr);
		is_condition_negated = false;


	} else {
		// Shannon's expnasion didn't return any MUX, then SEL_ALWAYS must be just a SINGLE condition of an existing BB. It may or may not be an inverter..
			// we need to identify this condition and retrieve the enode that computes this condition
		// sanity check

		assert(f.size() == 1 && f.at(0).size() == 1);
		bool is_negated;
		condition_enode = returnSingleCondENode(f, is_negated);
		// sanity checks
		assert(condition_enode != nullptr);
		assert(is_negated == true || is_negated == false);

		is_condition_negated = is_negated;

		dbg_file_5 << "\nF_SUPP is a single condition!!\n";
		if(is_condition_negated)
			dbg_file_5 << "\tThe condition is inverted!!\n";
		else
			dbg_file_5 << "\tThe condition is not inverted!!\n";
		
		
	}

	return condition_enode;  // this is the enode implementing the passed f boolean function
}

/**
 * @brief  
 * @param 
 * @return 
 */
ENode* CircuitGenerator::returnSingleCondENode(const std::vector<std::vector<pierCondition>>& f, bool& is_negated) {
	// first identify the BB of the single condition of f
	int BB_idx = f.at(0).at(0).pierBB_index;

	// search for LLVM's branch inside this BB and find the ENode feeding its condition
	llvm::BranchInst* BI = nullptr;
	ENode* llvm_branch = nullptr;
	for(auto& enode_ : *enode_dag) {
		if(enode_->type == Branch_ && enode_->BB == bbnode_dag->at(BB_idx)->BB) { 
			BI = dyn_cast<llvm::BranchInst>(enode_->Instr);
			llvm_branch = enode_;
			break; // no need to continue looping, 1 llvm branch is enough
		}
	}

	is_negated = f.at(0).at(0).is_negated;

	return llvm_branch->CntrlPreds->at(0);
}

/**
 * @brief  
 * @param 
 * @return True if we will insert a SUPPRESS and False if we do not need a SUPPRESS
 */
bool CircuitGenerator::calculate_F_Supp(std::ofstream& dbg_file, const std::vector<std::vector<pierCondition>>& f_prod, const std::vector<std::vector<pierCondition>>& f_cons, std::vector<std::vector<pierCondition>>& f_supp_simplified) {
	// TODO think if we need to do anything more to detect if f_supp = 1 or 0????

	// the naive cases: 
	// 1) if the token is guaranteed to be consumed, we don't need a SUPPRESS
	if(f_cons.size() == 0)
		return false;

	//////////////////////////////////
	dbg_file << "\n\nConsidering f_prod = ";
	if(f_prod.size() == 0) {
		dbg_file << " 1 ";
	} 
	for(int u = 0; u < f_prod.size(); u++) {
		for(int v = 0; v < f_prod.at(u).size(); v++) {
			if(f_prod.at(u).at(v).is_negated)
				dbg_file << " not C";
			else
				dbg_file << " C";

			dbg_file << f_prod.at(u).at(v).pierBB_index + 1 << " & ";
		}
		dbg_file << " + ";
	}
	dbg_file << "\n\n";

	dbg_file << "\n\nAnd f_cons = ";
	if(f_cons.size() == 0) {
		dbg_file << " 1 ";
	} 
	for(int u = 0; u < f_cons.size(); u++) {
		for(int v = 0; v < f_cons.at(u).size(); v++) {
			if(f_cons.at(u).at(v).is_negated)
				dbg_file << " not C";
			else
				dbg_file << " C";

			dbg_file << f_cons.at(u).at(v).pierBB_index + 1 << " & ";
		}
		dbg_file << " + ";
	}
	dbg_file << "\n\n";
	/////////////////////////////////

	// 2) if the token is guaranteed to be produced, we only need to SUPPRESS at the negation of f_cons
	if(f_prod.size() == 0) {
		std::vector<std::vector<pierCondition>> f_supp;
		NegateApplydeMorgans(dbg_file, f_cons, f_supp);
		Simplify_Quine_McCluskey(dbg_file, f_supp, f_supp_simplified, false);
		return true;
	}

	// the normal case f_supp = f_prod AND NOT(f_cons)
	std::vector<std::vector<pierCondition>> negated_f_cons;
	NegateApplydeMorgans(dbg_file, f_cons, negated_f_cons);
	// AND f_prod with the negated_f_cons
	std::vector<std::vector<pierCondition>> f_supp;
	std::vector<pierCondition> one_merged_product;
	for(int i = 0; i < f_prod.size(); i++) {
		for(int j = 0; j < negated_f_cons.size(); j++) {
			one_merged_product.clear();
			one_merged_product.reserve(f_prod.at(i).size() + negated_f_cons.at(j).size());
			one_merged_product.insert(one_merged_product.end(), f_prod.at(i).begin(), f_prod.at(i).end());
			one_merged_product.insert(one_merged_product.end(), negated_f_cons.at(j).begin(), negated_f_cons.at(j).end());
			f_supp.push_back(one_merged_product);
		}
	}

	// simplify f_supp where possible using Quine McCluskey
	Simplify_Quine_McCluskey(dbg_file, f_supp, f_supp_simplified, false);

	return true; 
}



// added on 04/09/2022
/**
 * @brief  
 * @param 
 * @return True if we will insert a GENERATE and False if we do not need a GENERATE
 */
bool CircuitGenerator::calculate_F_Gen(std::ofstream& dbg_file, const std::vector<std::vector<pierCondition>>& f_prod, const std::vector<std::vector<pierCondition>>& f_cons, std::vector<std::vector<pierCondition>>& f_gen_simplified) {
	// the naive cases: 
	// 1) if the token is guaranteed to be produced (i.e., f_prod = 1), we don't need a GENERATE
	if(f_prod.size() == 0)
		return false;

	// OLD DEBUGGING FROM THE F_SUPP FUNCTION!!
	/*dbg_file << "\n\nConsidering f_prod = ";
	if(f_prod.size() == 0) {
		dbg_file << " 1 ";
	} 
	for(int u = 0; u < f_prod.size(); u++) {
		for(int v = 0; v < f_prod.at(u).size(); v++) {
			if(f_prod.at(u).at(v).is_negated)
				dbg_file << " not C";
			else
				dbg_file << " C";

			dbg_file << f_prod.at(u).at(v).pierBB_index + 1 << " & ";
		}
		dbg_file << " + ";
	}
	dbg_file << "\n\n";

	dbg_file << "\n\nAnd f_cons = ";
	if(f_cons.size() == 0) {
		dbg_file << " 1 ";
	} 
	for(int u = 0; u < f_cons.size(); u++) {
		for(int v = 0; v < f_cons.at(u).size(); v++) {
			if(f_cons.at(u).at(v).is_negated)
				dbg_file << " not C";
			else
				dbg_file << " C";

			dbg_file << f_cons.at(u).at(v).pierBB_index + 1 << " & ";
		}
		dbg_file << " + ";
	}
	dbg_file << "\n\n";
	*/
	/////////////////////////////////

	// 2) if the token is guaranteed to be consumed, we only need to GENERATE at the negation of f_prod
	if(f_cons.size() == 0) {
		std::vector<std::vector<pierCondition>> f_gen;
		NegateApplydeMorgans(dbg_file, f_prod, f_gen);
		Simplify_Quine_McCluskey(dbg_file, f_gen, f_gen_simplified, false);
		return true;
	}

	// the normal case f_gen = NOT(f_prod) AND f_cons
	std::vector<std::vector<pierCondition>> negated_f_prod;
	NegateApplydeMorgans(dbg_file, f_prod, negated_f_prod);
	// AND f_cons with the negated_f_prod
	std::vector<std::vector<pierCondition>> f_gen;
	std::vector<pierCondition> one_merged_product;
	for(int i = 0; i < f_cons.size(); i++) {
		for(int j = 0; j < negated_f_prod.size(); j++) {
			one_merged_product.clear();
			one_merged_product.reserve(f_cons.at(i).size() + negated_f_prod.at(j).size());
			one_merged_product.insert(one_merged_product.end(), f_cons.at(i).begin(), f_cons.at(i).end());
			one_merged_product.insert(one_merged_product.end(), negated_f_prod.at(j).begin(), negated_f_prod.at(j).end());
			f_gen.push_back(one_merged_product);
		}
	}

	// simplify f_gen where possible using Quine McCluskey
	Simplify_Quine_McCluskey(dbg_file, f_gen, f_gen_simplified, false);

	return true; 
}



/**
 * @brief  
 * @param 
 */
void CircuitGenerator::Simplify_Quine_McCluskey(std::ofstream& dbg_file, const std::vector<std::vector<pierCondition>>& f, std::vector<std::vector<pierCondition>>& f_simplified, bool print_flag) {
	Quine_McCluskey boolean_simplifier;
	std::vector<std::string> minterms_only_in_binary;

	// convert the passed boolean expression f into the std::vector<std::string> minterms_only_in_binary structure
		// the implementation of this function is inside AddCtrl.cpp

	std::vector<int> condition_variables;
	new_get_boolean_function_variables(f, condition_variables);  // fills the condition_variables with different conditions constituting the function
	new_get_binary_string_minterms(dbg_file, f, condition_variables, minterms_only_in_binary, print_flag);

	///////////////////////////////////////////////////////////
	if(print_flag) {
		dbg_file << "\n\tThe minterms_only_in_binary is = \n";
		for(int i = 0; i < minterms_only_in_binary.size(); i++) {
			dbg_file << minterms_only_in_binary.at(i) << "\n";
		}
	}
	////////////////////////////////////////////////////////// 

	int number_of_bits = minterms_only_in_binary.at(0).length();
	int number_of_minterms = minterms_only_in_binary.size();
	// We can't tell anything about dont_cares so consider them as 0 and pass minterms_only_in_binary in place of minterms_plus_dont_cares_in_binary
	int number_of_dont_cares = 0;

	boolean_simplifier.initialize(number_of_bits, number_of_minterms, number_of_dont_cares, &minterms_only_in_binary, &minterms_only_in_binary);
	boolean_simplifier.solve(dbg_file, false);

	std::vector<std::string> essential_f = boolean_simplifier.getEssentialPrimeImpl();
	new_binary_string_to_pierCondition_vec(essential_f, condition_variables, f_simplified);
}


/**
 * @brief  
 * @param 
 */
void CircuitGenerator::new_binary_string_to_pierCondition_vec(const std::vector<std::string>& f_string, const std::vector<int>& condition_variables, std::vector<std::vector<pierCondition>>& f_vec) {
	std::vector<pierCondition> one_product;

	for(int i = 0; i < f_string.size(); i++) {
		one_product.clear();
		for(int j = 0; j < f_string.at(i).length(); j++) {
			// skip if there is a '-' in place of that condition!
			if(f_string.at(i).at(j) == '-') {
				continue;
			} else {
				pierCondition cond;
				cond.pierBB_index = condition_variables.at(j);
				if(f_string.at(i).at(j) == '0') {
					cond.is_negated = true;
				} else {
					cond.is_negated = false;
				}

				one_product.push_back(cond);
			}
		}
		f_vec.push_back(one_product);
	}  
}


/**
 * @brief This function is identical to new_get_binary_string_minterms except that here we do not replace the dontcares!!
 * @param 
 */
void CircuitGenerator::new_get_binary_string_minterms_Shannons(std::ofstream& dbg_file, const std::vector<std::vector<pierCondition>>& funct, const std::vector<int>& condition_variables, std::vector<std::string>& minterms_only_in_binary, bool print_flag) {
	// loop over the different products constituting the function
	for(int i = 0; i < funct.size(); i++) {
		// the first step is to check if there are any repeated conditions (of the same or opposite negation sign) per product
		if(simplify_if_repeated_conds(funct.at(i))) { // it takes a single product as a parameter!!
			continue;  // this product evaluates to 0 so don't account for it
		}  

		// for each product, check which variables are present and which are not
		std::string one_binary_f_product = "";
		for(int j = 0; j < condition_variables.size(); j++) {
			// search for each of these conditions in the current product
				// to make this find work, I overloaded a == operator that compares the struct object with an integer
			auto pos_ = std::find(funct.at(i).begin(), funct.at(i).end(), condition_variables.at(j));

			if(pos_!= funct.at(i).end()) {
				auto index = pos_ - funct.at(i).begin();
				if(funct.at(i).at(index).is_negated) {
					one_binary_f_product.push_back('0');
				} else {
					one_binary_f_product.push_back('1');
				} 

			} else {
				// if this condition_variable is not found, then it is a don't-care so mark it as x to create two entries out of it in the end!
				one_binary_f_product.push_back('x');
			}
		}
		minterms_only_in_binary.push_back(one_binary_f_product);
	}

	if(print_flag) {
		dbg_file << "\n********* Inside get_binary_string_minterms *******************\n";
		dbg_file << "\tPrinting the minterms_only_in_binary with dontcare bits:\n";
		for(int i = 0; i < minterms_only_in_binary.size(); i++) {
			dbg_file << minterms_only_in_binary.at(i) << "\n";
		}
	}

}


/**
 * @brief  
 * @param 
 */
void CircuitGenerator::new_get_binary_string_minterms(std::ofstream& dbg_file, const std::vector<std::vector<pierCondition>>& funct, const std::vector<int>& condition_variables, std::vector<std::string>& minterms_only_in_binary, bool print_flag) {
	// loop over the different products constituting the function
	for(int i = 0; i < funct.size(); i++) {
		// the first step is to check if there are any repeated conditions (of the same or opposite negation sign) per product
		if(simplify_if_repeated_conds(funct.at(i))) { // it takes a single product as a parameter!!
			continue;  // this product evaluates to 0 so don't account for it
		}  

		// for each product, check which variables are present and which are not
		std::string one_binary_f_product = "";
		for(int j = 0; j < condition_variables.size(); j++) {
			// search for each of these conditions in the current product
				// to make this find work, I overloaded a == operator that compares the struct object with an integer
			auto pos_ = std::find(funct.at(i).begin(), funct.at(i).end(), condition_variables.at(j));

			if(pos_!= funct.at(i).end()) {
				auto index = pos_ - funct.at(i).begin();
				if(funct.at(i).at(index).is_negated) {
					one_binary_f_product.push_back('0');
				} else {
					one_binary_f_product.push_back('1');
				} 

			} else {
				// if this condition_variable is not found, then it is a don't-care so mark it as x to create two entries out of it in the end!
				one_binary_f_product.push_back('x');
			}
		}
		minterms_only_in_binary.push_back(one_binary_f_product);
	}

	if(print_flag) {
		dbg_file << "\n********* Inside get_binary_string_minterms *******************\n";
		dbg_file << "\tPrinting the minterms_only_in_binary with dontcare bits:\n";
		for(int i = 0; i < minterms_only_in_binary.size(); i++) {
			dbg_file << minterms_only_in_binary.at(i) << "\n";
		}
	}
	
	std::vector<int> indices_to_replicate;  // replicate the don't-care variables
	for(int i = 0; i < minterms_only_in_binary.size(); i++) {
		indices_to_replicate.clear();
		// get the dont-care variables for that product
		for(int j = 0; j < minterms_only_in_binary.at(i).length(); j++) {
			if(minterms_only_in_binary.at(i).at(j) == 'x') {
				indices_to_replicate.push_back(j);
			}
		}

		for(int k = 0; k < indices_to_replicate.size(); k++) {
			// replace this don't-care with "0" and make another copy of it with "1"
			minterms_only_in_binary.at(i).at(indices_to_replicate.at(k)) = '1';
			minterms_only_in_binary.push_back(minterms_only_in_binary.at(i));
			minterms_only_in_binary.at(i).at(indices_to_replicate.at(k)) = '0';
		}
	}

}


/**
 * @brief  
 * @param 
 * @return True if the product should vanish because it simplifies to a 0
 */
bool CircuitGenerator::simplify_if_repeated_conds(const std::vector<pierCondition>& one_product) {
	// check every condition in the passed product against all other conditions in the product
	for(int i = 0; i < one_product.size(); i++) {
		for(int j = i+1; j < one_product.size(); j++) {
			if(one_product.at(i).pierBB_index == one_product.at(j).pierBB_index) {
				// check the signs 
				if(one_product.at(i).is_negated != one_product.at(j).is_negated) {
					// a condition and its negation are present, so the product should evaluate to 0
					return true;
				} 
			}
		}
	}

	return false;
}


/**
 * @brief  
 * @param 
 */
void CircuitGenerator::new_get_boolean_function_variables(const std::vector<std::vector<pierCondition>>& f, std::vector<int>& condition_variables) {
	for(int j = 0; j < f.size(); j++) {
		for(int k = 0; k < f.at(j).size(); k++) {
			int pierBB_index = f.at(j).at(k).pierBB_index;
			auto pos = std::find(condition_variables.begin(), condition_variables.end(), pierBB_index);
			// push to condition_variables only if you didn't push this value before!!
			if(pos == condition_variables.end()) {
				condition_variables.push_back(pierBB_index);
			}
		}
	}
	// sort the condition_variables to eas your life in calculating the decimal_f0_products 
	std::sort(condition_variables.begin(), condition_variables.end());
}

/**
 * @brief 
 * @param 
 */
std::vector<std::vector<CircuitGenerator::pierCondition>> CircuitGenerator::NegateProduct(const std::vector<pierCondition>& one_product) {
	std::vector<std::vector<pierCondition>> negated_one_product_sop;
	std::vector<pierCondition> temp_negated_product;
	for(int i = 0; i < one_product.size(); i++) {
		temp_negated_product.clear();
		pierCondition cond = one_product.at(i);
		if(cond.is_negated) {
			cond.is_negated = false;
		} else {
			cond.is_negated = true;
		}	
		temp_negated_product.push_back(cond);
		negated_one_product_sop.push_back(temp_negated_product);  // a single element of it is composed of just a single condition
								// But since each condition is ORed with the other conditions, I'm putting them as a vector of vectors to be consistent with the structure of a SOP
	}
	return negated_one_product_sop;
}


void CircuitGenerator::myPushBack(std::vector<pierCondition>& product_to_push_to, const std::vector<pierCondition>& product_to_push) {
	for(int i = 0; i < product_to_push.size(); i++) {
		product_to_push_to.push_back(product_to_push.at(i));
	}

}

/**
 * @brief takes a sum of products boolean expression that is negated and expands it by applying deMorgan's 
 * @param 
 */
void CircuitGenerator::NegateApplydeMorgans(std::ofstream& dbg_file, const std::vector<std::vector<pierCondition>>& f, std::vector<std::vector<pierCondition>>& negated_f) {
	// one element of the big vector represents a single negated product which is made up by 
				// ORing the conditions of the original product
	std::vector<std::vector<std::vector<pierCondition>>> all_negated_products; 
	// loop over each product, negate it and push to the all_negated_products
	for(int i = 0; i < f.size(); i++) {
		all_negated_products.push_back(NegateProduct(f.at(i)));
		dbg_file << "The outcome of negating the product number " << i << "is: \t";
		for(int j = 0; j < all_negated_products.at(all_negated_products.size()-1).size(); j++) {
			assert(all_negated_products.at(all_negated_products.size()-1).at(j).size() == 1);
			if(all_negated_products.at(all_negated_products.size()-1).at(j).at(0).is_negated)
				dbg_file << " not C";
			else
				dbg_file << " C";
			dbg_file << all_negated_products.at(all_negated_products.size()-1).at(j).at(0).pierBB_index + 1 << " + ";
			
		}

		dbg_file << " & ";
	}

	dbg_file << "\n\n";
	///////////////////////////////////////////////
	// loop over the different blocks starting at block 1
	for(int i = 1; i < all_negated_products.size(); i++) {
		// loop over every product in all_negated_products.at(0)
		int j = 0;
		while (j < all_negated_products.at(0).size()){
			// replicate each product at all_negated_products.at(0).at(j) to be repeated with the size of products in all_negated_products.at(i)
			auto iter = all_negated_products.at(0).begin();
			all_negated_products.at(0).insert(iter + j, all_negated_products.at(i).size() - 1, all_negated_products.at(0).at(j));

			// push_back each product in all_negated_products.at(i) to the corresponding copy we inserted in all_negated_products.at(0)
			for(int k = 0; k < all_negated_products.at(i).size(); k++) {
				myPushBack(all_negated_products.at(0).at(k + j), all_negated_products.at(i).at(k));
				// all_negated_products.at(0).at(k + j).push_back(all_negated_products.at(i).at(k));
			}
			// j should be incremented by the size 
			j = j + all_negated_products.at(i).size();
		}
	}
	
	// the correct sum of products after negating the condition is now in all_negated_products.at(0) 
	std::copy(all_negated_products.at(0).begin(), all_negated_products.at(0).end(), std::back_inserter(negated_f));

}

/**
 * @brief 
 * @param 
 */
void CircuitGenerator::eliminateCommonControlAncestors(std::ofstream& dbg_file, BBNode* consumer_bb, BBNode* producer_bb, std::vector<int>& modif_consBB_deps, std::vector<int>& modif_prodBB_deps) {
	std::copy(consumer_bb->BB_deps->begin(), consumer_bb->BB_deps->end(), std::back_inserter(modif_consBB_deps));
	std::copy(producer_bb->BB_deps->begin(), producer_bb->BB_deps->end(), std::back_inserter(modif_prodBB_deps));

	std::sort(modif_consBB_deps.begin(), modif_consBB_deps.end());
	std::sort(modif_prodBB_deps.begin(), modif_prodBB_deps.end());
// compare the BB_deps fields of the two passed BBs

	dbg_file << "\n\n============================================\n\n";
	dbg_file << "The control dependencies before modifications are:  \n";
	dbg_file << "ProducerBB: BB" << producer_bb->Idx + 1 << "  has the following modified control dependencies:     ";
	if(modif_prodBB_deps.size() == 0) {
		dbg_file << "Has no modified control dependencies!!\n\n";
	} else {
		for(int k = 0; k < modif_prodBB_deps.size(); k++) {
			dbg_file << modif_prodBB_deps.at(k) + 1<< ",";
		}
		dbg_file << "\n\n";
	}

	dbg_file << "ConsumerBB: BB" << consumer_bb->Idx + 1 << "  has the following modified control dependencies:     ";
	if(modif_consBB_deps.size() == 0) {
		dbg_file << "Has no modified control dependencies!!\n\n";
	} else {
		for(int k = 0; k < modif_consBB_deps.size(); k++) {
			dbg_file << modif_consBB_deps.at(k) + 1<< ",";
		}
		dbg_file << "\n\n";
	}

	int size = (modif_consBB_deps.size() > modif_prodBB_deps.size())? modif_consBB_deps.size(): modif_prodBB_deps.size();
	std::vector<int> common_ancestors(size);

	if(modif_consBB_deps.size() > 0 && modif_prodBB_deps.size() > 0) {
		std::set_intersection(modif_consBB_deps.begin(), modif_consBB_deps.end(), modif_prodBB_deps.begin(), modif_prodBB_deps.end(), std::back_inserter(common_ancestors));
		modif_consBB_deps.erase(std::set_difference(modif_consBB_deps.begin(), modif_consBB_deps.end(), common_ancestors.begin(), common_ancestors.end(), modif_consBB_deps.begin()), modif_consBB_deps.end());
		modif_prodBB_deps.erase(std::set_difference(modif_prodBB_deps.begin(), modif_prodBB_deps.end(), common_ancestors.begin(), common_ancestors.end(), modif_prodBB_deps.begin()), modif_prodBB_deps.end());
	}
	
    dbg_file << "\n\n*****************************************\n\n";
	dbg_file << "\n\tNew (Producer,Consumer) pair:\n";
	dbg_file << "ProducerBB: BB" << producer_bb->Idx + 1 << "  has the following modified control dependencies:     ";
	if(modif_prodBB_deps.size() == 0) {
		dbg_file << "Has no modified control dependencies!!\n\n";
	} else {
		for(int k = 0; k < modif_prodBB_deps.size(); k++) {
			dbg_file << modif_prodBB_deps.at(k) + 1<< ",";
		}
		dbg_file << "\n\n";
	}

	dbg_file << "ConsumerBB: BB" << consumer_bb->Idx + 1 << "  has the following modified control dependencies:     ";
	if(modif_consBB_deps.size() == 0) {
		dbg_file << "Has no modified control dependencies!!\n\n";
	} else {
		for(int k = 0; k < modif_consBB_deps.size(); k++) {
			dbg_file << modif_consBB_deps.at(k) + 1<< ",";
		}
		dbg_file << "\n\n";
	}
}

/**
 * @brief constructs a boolean expression representing the execution of the passed bbnode in terms of the conditions of the BBs in the passed dependency set 
 * @param 
 * @note remember that enumerateBBpaths_dfs enumerates paths in a backward manner!
 */
void CircuitGenerator::constructSOP(std::ofstream& dbg_file, BBNode* startBB, BBNode* bbnode, const std::vector<int>& modif_BB_deps, std::vector<std::vector<pierCondition>>& f_sop, bool experiment_flag) {
/* Plan:
1) I need to extract the SOP expression in terms of the modified control dependencies
2) To do so, one straight forward approach is to enumerate all paths from START to the node and consider ONLY the BBs in the modified control dependency not the other BBs!
3) Then, I need to see the logic of calculating the SEL of any MUX (which is resolved in the paper by the GSA!)
*/
	//BBNode* startBB = bbnode_dag->at(0); // will pass it as a parameter!!

	int cfg_size = bbnode_dag->size();
	bool *visited = new bool[cfg_size];
	int *path = new int[cfg_size];
	int path_index = 0; 
	std::vector<std::vector<int>>* final_paths = new std::vector<std::vector<int>>;
	// enumerate paths from startBB to our bbnode 

	enumerateBBpaths_dfs(bbnode, startBB, visited, path, path_index, final_paths, false);

	std::vector<pierCondition> one_product;

	dbg_file << "\n*******************************************\n";
	dbg_file << "\nFound " << final_paths->size() << " paths\n";
	// loop over the final_paths and identify the BBs of the dependency with their conditions
	for(int kk = 0; kk < final_paths->size(); kk++) {
		dbg_file << "Printing path number " << kk << " details\n";
		for(int hh = 0; hh < final_paths->at(kk).size(); hh++) {
			dbg_file << final_paths->at(kk).at(hh) + 1 << " -> ";
		}
		dbg_file << "\n";

		// Create a product in terms of the conditions of the BBs in the modif_BB_deps for each path
			// and push to the f_sop
		one_product.clear();

		// for each BB in modif_BB_deps, search for it in the current path
		for(int i = 0; i < modif_BB_deps.size(); i++) {
			auto pos = std::find(final_paths->at(kk).begin(), final_paths->at(kk).end(), modif_BB_deps.at(i));
			if(pos != final_paths->at(kk).end()) {
				int index = pos - final_paths->at(kk).begin();  // find the index of the BB in the dependency list in the final_path

				pierCondition cond;
				cond.pierBB_index = modif_BB_deps.at(i);

				// sanity check: the bb in the control dependency must have 2 successors!
				assert(bbnode_dag->at(final_paths->at(kk).at(index))->CntrlSuccs->size() == 2);

				///////////////////////////////////////////////
				// In the situation of loop regeneration, we have the producerBB == consumerBB because
					// phi_n (or phi_c) happen to be both the producer and consumer
						// If the loop exit happens to be = the loop header, we will have the control dependency at index = 0
								// and it will be the only BB in the path
									// To identify if the condition is negated or not in this case, we need to search for the BB in its own successors! 
				//////////////////////////////////////////////
				if(index == 0) {
					// extra sanity checks
					assert(startBB == bbnode && bbnode->is_loop_exiting || bbnode->is_loop_header); 
					// to decide if the condition is negated or not, search for the successor of this BB that is inside the loop 
							// (i.e., the successor that should have been detected by enumerate paths to lead you to the backward edge)
								// But, it was not detected this way because enumerate_paths return from the very beginning since the producerBB is = to the consumerBB
					
					// first make sure that the BB has two succs
					BBNode* dep_bbnode = bbnode_dag->at(final_paths->at(kk).at(index));
					assert(dep_bbnode->CntrlSuccs->size() == 2);
					assert(dep_bbnode->loop != nullptr);

					if(dep_bbnode->loop->contains(dep_bbnode->CntrlSuccs->at(0)->BB)) {
						// sanity check: the other successor must be outside the loop!
						assert(!dep_bbnode->loop->contains(dep_bbnode->CntrlSuccs->at(1)->BB));
						index = 0;
					} else {
						// sanity check: the other successor must be inside the loop!
						assert(dep_bbnode->loop->contains(dep_bbnode->CntrlSuccs->at(1)->BB));
						index = 1;
					}
					/*BBNode* dep_bbnode = bbnode_dag->at(final_paths->at(kk).at(index));
					auto pos_2 = std::find(dep_bbnode->CntrlSuccs->begin(), dep_bbnode->CntrlSuccs->end(), dep_bbnode);
					assert(pos_2 != dep_bbnode->CntrlSuccs->end());
					index = pos_2 - dep_bbnode->CntrlSuccs->begin();*/

				} else {
					// to decide if the condition is negated or not, check which successor of the BB is this path considering
					BBNode* dep_bbnode = bbnode_dag->at(final_paths->at(kk).at(index));
					BBNode* dep_succ_bbnode = bbnode_dag->at(final_paths->at(kk).at(index - 1));


					// AYA: 23/08/2022: replaced the following 3 lines with the logic of searching for the LLVM Branch inside the dependency BB
							// and identifying if the path is taking the BB in its true side or the false side
					/*
					auto pos_2 = std::find(dep_bbnode->CntrlSuccs->begin(), dep_bbnode->CntrlSuccs->end(), dep_succ_bbnode);
					assert(pos_2 != dep_bbnode->CntrlSuccs->end());
					index = pos_2 - dep_bbnode->CntrlSuccs->begin();
					*/

					// search for LLVM branch in the BB of the dependency list (i.e., dep_bbnode)
					llvm::BranchInst* BI;
					ENode* llvm_branch;
					for(auto& enode_ : *enode_dag) {
						if(enode_->type == Branch_ && enode_->BB == dep_bbnode->BB) { 
							BI = dyn_cast<llvm::BranchInst>(enode_->Instr);
							llvm_branch = enode_;
							break; // no need to continue looping 1 llvm branch is enough
						}
					}

					// sanity check: the Branch is inside a dependency BB so should not be unconditional
					assert(!BI->isUnconditional());

					BasicBlock *falseBB, *trueBB;
					trueBB = BI->getSuccessor(0);
					falseBB = BI->getSuccessor(1);

					// check if dep_succ_bbnode->BB is equivalent to the trueBB or the falseBB and adjust "index" accordingly because it is used below!!
					if(dep_succ_bbnode->is_virtBB) {
						// since it's a virtual BB, we should compare its successor BB (not itself) with the trueBB and the falseBB of LLVM's Branch
						if(dep_succ_bbnode->CntrlSuccs->at(0)->BB == trueBB){
							index = 0;
						} else {
							assert(dep_succ_bbnode->CntrlSuccs->at(0)->BB == falseBB);
							index = 1;
						}
					} else {
						if(dep_succ_bbnode->BB == trueBB){
							index = 0;
						} else {
							assert(dep_succ_bbnode->BB == falseBB);
							index = 1;
						}
					}

					/////////////////////// ENd of things added on 23/08/2022
				}

				
				if(index == 0) {

					if(experiment_flag) {
						// true side
						cond.is_negated = false;//true;
					} else {
						// true side
						cond.is_negated = false;
					}

				} else {
					assert(index == 1);
					if(experiment_flag) {
						// false side
						cond.is_negated = true;//false;  
					} else {
						// false side
						cond.is_negated = true;  // AYA: 23/08/2022: the way I decide when to negate here is wrong in some cases?!
					}
					
				}
				
				one_product.push_back(cond);

			} else {
				// 18/06/2022: the following part is not clean. TODO clean the logic here!!
					// in cases where the producer and the consumer are both Phi_n (or Phi_c) that we inserted for regeneration,
						// the fact that they are in the same BB would imply that enumerate paths will return from the beginning so will not enumerate the paths involving the loop exit (which is in a BB different from the loop header BB)
				if(startBB == bbnode) {
					if(bbnode->loop->contains(bbnode_dag->at(modif_BB_deps.at(i))->BB) && bbnode_dag->at(modif_BB_deps.at(i))->is_loop_exiting) {
						pierCondition cond;
						cond.pierBB_index = modif_BB_deps.at(i);

						// on a side note, keep in mind that if the loop has the loop exit different from the loop tail, this loop exit will not be a predecessor of th eloop header 
							// the goal is to identify if the loop exit starts a new iteration when its condition is negated or not 
						BBNode* dep_bbnode = bbnode_dag->at(modif_BB_deps.at(i));
						////////////////////////////////////////////////////////////
						if(dep_bbnode->loop->contains(dep_bbnode->CntrlSuccs->at(0)->BB)) {
							// sanity check: the other successor must be outside the loop!
							assert(!dep_bbnode->loop->contains(dep_bbnode->CntrlSuccs->at(1)->BB));
							cond.is_negated = false;
						} else {
							// sanity check: the other successor must be inside the loop!
							assert(dep_bbnode->loop->contains(dep_bbnode->CntrlSuccs->at(1)->BB));
							cond.is_negated = true;
						}
						//////////////////////////////////////////////////////////
						one_product.push_back(cond);
					} /*else {  // AYA: 10/11/2022: the following else is added to check if the modif_BB_deps.at(i) corresponds to an outer loop enclosing the startBB and the bbnode!
						if(experiment_flag && bbnode_dag->at(modif_BB_deps.at(i))->loop->contains(bbnode->BB) && bbnode_dag->at(modif_BB_deps.at(i))->is_loop_exiting) {
							pierCondition cond;
							cond.pierBB_index = modif_BB_deps.at(i);

							// on a side note, keep in mind that if the loop has the loop exit different from the loop tail, this loop exit will not be a predecessor of the loop header 
								// the goal is to identify if the loop exit starts a new iteration when its condition is negated or not 
							BBNode* dep_bbnode = bbnode_dag->at(modif_BB_deps.at(i));
							////////////////////////////////////////////////////////////
							if(dep_bbnode->loop->contains(dep_bbnode->CntrlSuccs->at(0)->BB)) {
								// sanity check: the other successor must be outside the loop!
								assert(!dep_bbnode->loop->contains(dep_bbnode->CntrlSuccs->at(1)->BB));
								cond.is_negated = false;// true;
							} else {
								// sanity check: the other successor must be inside the loop!
								assert(dep_bbnode->loop->contains(dep_bbnode->CntrlSuccs->at(1)->BB));
								cond.is_negated = true;//false;
							}
							//////////////////////////////////////////////////////////
							one_product.push_back(cond);
						}
					} */
				}
			}
		}

		if(one_product.size() > 0)
			f_sop.push_back(one_product);
	}  // end of loop on all paths between START and the bbnode

	// 12/11/2022: with the smart way of REGENERATing tokens in cases where the consumer is inside one or more loops that the consumer is not inside, 
				// the outer loops enclosing the consumer in a loop nest will not be found in any path because the producer and consumer are in the same BB (i.e., Phi of our insertion)
	// check if one of the modif_BB_deps was not found in any of the paths
	one_product.clear();
	bool found = false;
	for(int i = 0; i < modif_BB_deps.size(); i++) {
		found = false;
		for(int kk = 0; kk < final_paths->size(); kk++) {
			auto pos = std::find(final_paths->at(kk).begin(), final_paths->at(kk).end(), modif_BB_deps.at(i));
			if(pos != final_paths->at(kk).end()) {
				found = true;
			}
		}

		if(!found && experiment_flag) {   // AYA: to think! IN some cases, we could have some modiBB_deps not in any path but for different reasons
											// than the case of smart REGENERATE; therefore, I do the following only if I'm in the case of smart REGENERATE (i.e., if experiment_flag is true!)
			assert(bbnode_dag->at(modif_BB_deps.at(i))->loop->contains(bbnode->BB) && bbnode_dag->at(modif_BB_deps.at(i))->is_loop_exiting);
			pierCondition cond;
			cond.pierBB_index = modif_BB_deps.at(i);

			// on a side note, keep in mind that if the loop has the loop exit different from the loop tail, this loop exit will not be a predecessor of the loop header 
				// the goal is to identify if the loop exit starts a new iteration when its condition is negated or not 
			BBNode* dep_bbnode = bbnode_dag->at(modif_BB_deps.at(i));
			////////////////////////////////////////////////////////////
			if(dep_bbnode->loop->contains(dep_bbnode->CntrlSuccs->at(0)->BB)) {
				// sanity check: the other successor must be outside the loop!
				assert(!dep_bbnode->loop->contains(dep_bbnode->CntrlSuccs->at(1)->BB));
				cond.is_negated = false;// true;
			} else {
				// sanity check: the other successor must be inside the loop!
				assert(dep_bbnode->loop->contains(dep_bbnode->CntrlSuccs->at(1)->BB));
				cond.is_negated = true;//false;
			}
			//////////////////////////////////////////////////////////
			one_product.push_back(cond);
			f_sop.push_back(one_product);

		}
	}

	// THE Other issue is that probably the smart way of REGENERATing tokens does not fit very well in the idea of prod and not cons assuming that prod is 1!
		// TO THINK!!!

}