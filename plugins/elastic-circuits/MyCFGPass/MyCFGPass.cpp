/**
 * Elastic Pass
 *
 * Forming a netlist of dataflow components out of the LLVM IR
 *
 */

#include <cassert>

#include "llvm/ADT/Statistic.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/RandomNumberGenerator.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/Transforms/Utils/Cloning.h"

#include "ElasticPass/CircuitGenerator.h"
#include "ElasticPass/Utils.h"
#include "ElasticPass/Head.h"
#include "ElasticPass/Nodes.h"
#include "ElasticPass/Pragmas.h"
#include "OptimizeBitwidth/OptimizeBitwidth.h"


#include <sys/time.h>
#include <time.h>

static cl::opt<bool> opt_useLSQ("use-lsq", cl::desc("Emit LSQs where applicable"), cl::Hidden,
                                cl::init(true), cl::Optional);

static cl::opt<std::string> opt_cfgOutdir("cfg-outdir", cl::desc("Output directory of MyCFGPass"),
                                          cl::Hidden, cl::init("."), cl::Optional);

static cl::opt<bool> opt_buffers("simple-buffers", cl::desc("Naive buffer placement"), cl::Hidden,
                                cl::init(false), cl::Optional);

static cl::opt<std::string> opt_serialNumber("target", cl::desc("Targeted FPGA"), cl::Hidden,
                                cl::init("default"), cl::Optional);

struct timeval start, time_end;

std::string fname;
static int indx_fname = 0;

void set_clock() { gettimeofday(&start, NULL); }

double elapsed_time() {
    gettimeofday(&time_end, NULL);

    double elapsed = (time_end.tv_sec - start.tv_sec);
    elapsed += (double)(time_end.tv_usec - start.tv_usec) / 1000000.0;
    return elapsed;
}

using namespace llvm;

//class ControlDependencyGraph;

namespace {
class MyCFGPass : public llvm::FunctionPass {

public:
    static char ID;

    MyCFGPass() : llvm::FunctionPass(ID) {}

    virtual void getAnalysisUsage(AnalysisUsage& AU) const {
        // 			  AU.setPreservesCFG();
        AU.addRequired<OptimizeBitwidth>();
		OptimizeBitwidth::setEnabled(false);  // pass true to it to enable OB	

        AU.addRequired<MemElemInfoPass>();

		AU.addRequired<LoopInfoWrapperPass>();

    }

    void compileAndProduceDOTFile(Function& F , LoopInfo& LI) {
        fname = F.getName().str();

        // main is used for frequency extraction, we do not use its dataflow graph
        if (fname != "main") {

            bool done = false;

            auto M = F.getParent();

            pragmas(0, M->getModuleIdentifier());

            std::vector<BBNode*>* bbnode_dag = new std::vector<BBNode*>;
            std::vector<ENode*>* enode_dag   = new std::vector<ENode*>;
            OptimizeBitwidth* OB             = &getAnalysis<OptimizeBitwidth>();
            MemElemInfo& MEI                 = getAnalysis<MemElemInfoPass>().getInfo();

            //----- Internally constructs elastic circuit and adds elastic components -----//

            // Naively building circuit
            CircuitGenerator* circuitGen = new CircuitGenerator(enode_dag, bbnode_dag, OB, MEI);//, CDGP);//, CDG);

	    // calling functions implemented in `AddComp.cpp`
            circuitGen->buildDagEnodes(F);
            circuitGen->fixBuildDagEnodes();
            circuitGen->removeRedundantBeforeElastic(bbnode_dag, enode_dag);
            circuitGen->setGetelementPtrConsts(enode_dag);

#if 1
			circuitGen->buildPostDomTree(F);
			circuitGen->constructDomTree(F);


			// AYA: 01/10/2021
			circuitGen->Fix_LLVM_PhiPreds();

			// AYA: 21/10/2022: temporarily calling this function here to trigger all constants from source
			//circuitGen->addSourceForConstants();

	    	circuitGen->addStartC();


	    	// Aya: 12/09/2022: Added a flag to choose a mode of operation for the CntrlOrder network for interfacing with the LSQ:
			/*
				- If false: Make the CntrlOrder network strictly follow the sequential control flow,
				Or - If True: Make the CntrlOrder network implement the FPGA'23 algorithm
			*/
			// depending on the experiment I want, I manually change the flag to true or false.
			bool is_smart_cntrlOrder_flag = true;

			bool lazy_fork_flag = false;

			circuitGen->printDebugDominatorTree();

			
			if(!is_smart_cntrlOrder_flag) {
				circuitGen->buildRedunCntrlNet();  
			}


			// 12/09/2022: moved this function call to happen earlier because I need to check the conditions of execution inside the addMemoryInterfaces function!!
			circuitGen->identifyBBsForwardControlDependency();

			circuitGen->FindLoopDetails(LI);

			std::ofstream general_dbg_file;
			general_dbg_file.open("general_MYCFG_dbg.txt");

			general_dbg_file << "\n HIIII before addMemoryInterfaces\n";


			// This function (implemented in Memory.cpp) takes care of all the memory stuff; mainly operates on the CntrlOrder network
			// Aya: 12/09/2022 added an extra parameter to tell the function if we will interface with thr LSQ the conservative or the smart way!
			circuitGen->addMemoryInterfaces(opt_useLSQ, is_smart_cntrlOrder_flag, lazy_fork_flag);
			general_dbg_file << "\nAfter addMemoryInterfaces\n";



			/////////////////////////////////////////// TEMPORARILY FOR DEBUGGING!!!
			std::ofstream dbg_file_15;
    		dbg_file_15.open("check_CntrlOrderNetwork.txt");
			for(auto& enode: *enode_dag) {
				if(enode->CntrlOrderPreds->size() > 0) {
					dbg_file_15 << "\n\nNew node of type: " << getNodeDotTypeNew(enode);
					if(enode->BB != nullptr)
						dbg_file_15 << " and BB" << circuitGen->BBMap->at(enode->BB)->Idx  + 1 << " has the following preds in the CntrlOrderPreds\n";
					else 
						dbg_file_15 << " and BB_NULL " << " has the following preds in the CntrlOrderPreds\n";
						
				}
				// loop over the control order network to check its predecessors and successors
				for(auto& pred: *enode->CntrlOrderPreds) {
					if(pred->BB != nullptr)
						dbg_file_15 << getNodeDotTypeNew(pred) << " in BB" << circuitGen->BBMap->at(pred->BB)->Idx  + 1 << ", ";
					else
						dbg_file_15 << getNodeDotTypeNew(pred) << " in BB_NULL, ";
				}
				dbg_file_15 << "\n\n";
			}
			///////////////////////////////////////////////////////////////////////////////

			/* Aya: 23/05/2022: Applying FPL's algorithm
			 */
					
			circuitGen->printCDFG();
			std::ofstream dbg_file_5;
    		dbg_file_5.open("check_final_loops_details.txt");
			circuitGen->debugLoopsOutputs(dbg_file_5);
			dbg_file_5.close();

			// AYA: 08/10/2022: added the following to trigger the return of void functions
			circuitGen->connectReturnToVoid_irredundant();


			// the following function is responsible for adding Merges for re-generation!
			circuitGen->checkLoops(F, LI, CircuitGenerator::data);
			circuitGen->checkLoops(F, LI, CircuitGenerator::constCntrl);

			if(is_smart_cntrlOrder_flag) {
				// 10/08/2022: added the following copy of checkLoops to operate on the CntrlOrder network
				circuitGen->checkLoops(F, LI, CircuitGenerator::memDeps);
			}

			// AYAA: TODO: 09/11/2022: add a new function here to delete the extra PHIs added from above..
				// MY GOAL is to have a single PHI of my insertion ONLY in the innermost loop that the consumer is inside and the producer is not inside
					// SINCE IT IS STILL EXPERIMENTAL AND IN ORDER TO NOT MESS UP WITH OTHER STUFF, I WILL NOT CHANGE ANYTHING IN THE ABOVE FUNCTION OF INSERTING PHIS AND WILL DO THE CHANGES I WANT IN A NEW FUNCTION THAT DELETES THE EXTRA ADDED STUFF
			//circuitGen->TEMP_deleteExtraPhis();

			general_dbg_file << "\nAfter the 3 calls of checkLoops!\n";

			
			//////// TEMPORARILY FOR DEBUGGING!!
			std::ofstream dbg_file;
    		dbg_file.open("PROD_CONS_BBs.txt");

			for (auto& enode : *enode_dag) {
				for (auto& succ : *enode->CntrlSuccs) {
					dbg_file << "\n---------------------------------------------------\n";
					if(enode->BB!=nullptr && succ->BB!=nullptr)
						dbg_file << "Producer in BB" << aya_getNodeDotbbID(circuitGen->BBMap->at(enode->BB)) << " to a Consumer in BB " << aya_getNodeDotbbID(circuitGen->BBMap->at(succ->BB)) << "\n";
				}
			}
			
			circuitGen->removeExtraPhisWrapper(CircuitGenerator::data);
			circuitGen->removeExtraPhisWrapper(CircuitGenerator::constCntrl);
			if(is_smart_cntrlOrder_flag) {
				circuitGen->removeExtraPhisWrapper(CircuitGenerator::memDeps);
			}
			
			general_dbg_file << "\nAfter the 3 calls of removeExtraPhisWrapper!\n";


			// AYA: 06/10/2021: added this to make sure the predecessors of any Phi we added above is compatible with the order of the LLVM convention to be able to convert them to muxes later!
			circuitGen->Fix_my_PhiPreds(LI, CircuitGenerator::data);  // to fix my phi_
			circuitGen->Fix_my_PhiPreds(LI, CircuitGenerator::constCntrl);  // to fix my irredundant phi_c

			if(is_smart_cntrlOrder_flag) {
				circuitGen->Fix_my_PhiPreds(LI, CircuitGenerator::memDeps);
			}

			//general_dbg_file << "\nAfter the 3 calls of Fix_my_PhiPreds!\n";

			
			/////////////////////////////////////////////////

			circuitGen->addSuppress_with_loops(CircuitGenerator::data); // connect producers and consumers of the datapath
			circuitGen->addSuppress_with_loops(CircuitGenerator::constCntrl);  // trigger constants from START through the data-less network

			if(is_smart_cntrlOrder_flag) {
				circuitGen->addSuppress_with_loops(CircuitGenerator::memDeps);
			}

			//general_dbg_file << "\nAfter the 3 calls of addSuppress_with_loops!\n";

			circuitGen->removeExtraBranchesWrapper(CircuitGenerator::data);
			circuitGen->removeExtraBranchesWrapper(CircuitGenerator::constCntrl);
			if(is_smart_cntrlOrder_flag) {
				circuitGen->removeExtraBranchesWrapper(CircuitGenerator::memDeps);
			}
			
			//general_dbg_file << "\nAfter the 3 calls of removeExtraBranchesWrapper!\n";

			// Aya: 16/06/2022: final version of setMuxes that converts only the Merges at the confluence points not at the loop headers into MUXes
				// this is why internally it does not need to operate on Phi_c because those are never inserted except at loop headers (for regeneration)
			circuitGen->setMuxes_nonLoop(); 
			//general_dbg_file << "\nAfter setMuxes_nonLoop!\n";


			circuitGen->deleteLLVM_Br();
			//general_dbg_file << "\nAfter deleteLLVM_Br!\n";


			// AYA: 15/11/2021: Trigger all constants (except those triggering an unconditional branch) through a Source component in the same BB
			//circuitGen->addSourceForConstants();

			//////// TEMPORARILY FOR DEBUGGING!!
			std::ofstream dbg_file_8;
    		dbg_file_8.open("check_Pred_succs_of_gsa_non_loop_mux.txt");
    		for (auto& enode : *enode_dag) {
    			if(enode->is_non_loop_gsa_mux) {
					assert(enode->type == Phi_c);
					dbg_file_8 << "\n\nFound 1 non_loop_gsa_mux with " << enode->CntrlOrderPreds->size() << " CntrlOrderPreds, " <<  enode->JustCntrlPreds->size() << " JustCntrlPreds, " << enode->CntrlOrderSuccs->size() << " CntrlOrderSuccs, "  << enode->JustCntrlSuccs->size() << " JustCntrlSuccs" << "\n";
					if(enode->CntrlOrderSuccs->size() > 0) {
						assert(enode->CntrlOrderSuccs->size() == 1);
						dbg_file_8 << "\n\tThe enode->CntrlOrderSuccs->at(0) is of type " << enode->CntrlOrderSuccs->at(0)->type << "\n";
					}
				}

			}


			// Aya: 13/09/2022: added the following function (implemented in Memory.cpp) to optimize the Bx components that are having no succs thus fed to a sink!!
	      	if(is_smart_cntrlOrder_flag)
	      		circuitGen->removeUselessBxs(lazy_fork_flag);

			//general_dbg_file << "\nAfter removeUselessBxs!\n";


	      	dbg_file_8 << "\nAfter calling removeUselessBxs!\n ";
	      	for (auto& enode : *enode_dag) {
    			if(enode->is_non_loop_gsa_mux) {
					assert(enode->type == Phi_c);
					dbg_file_8 << "\n\nFound 1 non_loop_gsa_mux with " << enode->CntrlOrderPreds->size() << " CntrlOrderPreds, " <<  enode->JustCntrlPreds->size() << " JustCntrlPreds, " << enode->CntrlOrderSuccs->size() << " CntrlOrderSuccs, "  << enode->JustCntrlSuccs->size() << " JustCntrlSuccs" << "\n";
					if(enode->CntrlOrderSuccs->size() > 0) {
						assert(enode->CntrlOrderSuccs->size() == 1);
						dbg_file_8 << "\n\tThe enode->CntrlOrderSuccs->at(0) is of type " << enode->CntrlOrderSuccs->at(0)->type << "\n";

						if(enode->CntrlOrderSuccs->at(0)->type == LSQ_) {
							// search for the enode in the predecessors of the LSQ!!
							auto pos_check = std::find(enode->CntrlOrderSuccs->at(0)->CntrlOrderPreds->begin(), enode->CntrlOrderSuccs->at(0)->CntrlOrderPreds->end(), enode);
							assert(pos_check != enode->CntrlOrderSuccs->at(0)->CntrlOrderPreds->end());
							dbg_file_8 << "\n\tThe index of the non_loop_gsa_mux in the CntrlOrderPreds of the LSQ is  " << pos_check - enode->CntrlOrderSuccs->at(0)->CntrlOrderPreds->begin() << "\n";
						}
					}
				}

			}


		// IMP Note: this function is also important to connect the predecessors of all branches!!!
			circuitGen->addFork();
			//general_dbg_file << "\nAfter addFork!\n";

			// Aya: 05/11/2022: added the following function to make sure to transfer any fork feeding the LSQ to a LazyFork
				// this is needed only if we use the naive interface with the LSQ through the ordered network of control merges because in case of the smart network, the type of the fork is defined from the moment we insert it inside the Memory.cpp
			if(!is_smart_cntrlOrder_flag && lazy_fork_flag) {
				circuitGen->convert_LSQ_fork_to_lazy();  // implemented in Memory.cpp
			}

			// AYA: 27/11/2022: converting the merges in the ordered only control network to CMerges to check if it will matter with buffers
			/*if(!is_smart_cntrlOrder_flag) {
				circuitGen->convert_Merges_to_CMerges();
			}*/


			// TEMPORARILY FOR DEBUGGING!
			/*dbg_file_9 << "\n\tPrinting again after addFork!!\n\n";
			for (auto& enode : *enode_dag) {
				if(enode->is_non_loop_gsa_mux) {
					assert(enode->type == Phi_c);
					dbg_file_9 << "\n---------------------------------------------------\n";
					dbg_file_9 << "Found 1 non_loop_gsa_mux with " << enode->CntrlOrderPreds->size() << " CntrlOrderPreds\n";
					if(enode->JustCntrlPreds->size() == 0) {
						dbg_file_9 << "It has no JustCntrlPreds!!\n\n";
					} else {
						dbg_file_9 << "It has " << enode->JustCntrlPreds->size() << " JustCntrlPreds!!\n\n";
					}

					dbg_file_9 << " Printing the CntrlOrderPreds..\n";
					for (auto& pred : *enode->CntrlOrderPreds) {
						dbg_file_9 << "Node name is " << getNodeDotNameNew(pred) << "of type " << pred->type << " in BB" << circuitGen->BBMap->at(pred->BB)->Idx + 1 << ", ";
					}

					dbg_file_9 << "\nPrinting the index of the non_loop_gsa_mux in the 2nd predecessor: ";
					auto pos__ = std::find(enode->CntrlOrderPreds->at(1)->true_branch_succs_Ctrl_lsq->begin(), enode->CntrlOrderPreds->at(1)->true_branch_succs_Ctrl_lsq->end(), enode);
					if(pos__ != enode->CntrlOrderPreds->at(1)->true_branch_succs_Ctrl_lsq->end()) {
						int idx = pos__ - enode->CntrlOrderPreds->at(1)->true_branch_succs_Ctrl_lsq->begin();
						dbg_file_9 << " in true Succs " << idx << "\n";
					} else {
						auto pos_ = std::find(enode->CntrlOrderPreds->at(1)->false_branch_succs_Ctrl_lsq->begin(), enode->CntrlOrderPreds->at(1)->false_branch_succs_Ctrl_lsq->end(), enode);
						assert(pos_ != enode->CntrlOrderPreds->at(1)->false_branch_succs_Ctrl_lsq->end());
						int idx = pos__- enode->CntrlOrderPreds->at(1)->false_branch_succs_Ctrl_lsq->begin();
						dbg_file_9 << " in false Succs " << idx << "\n";

					}
				}
			}*/

			//circuitGen->remove_SUPP_COND_Negation();

	   // call a function to create the exit node, NEED TO CHECK RETURN LOGIC!!
	      	circuitGen->addExitC();
			//general_dbg_file << "\nAfter addExitC!\n";

	      	
			if (opt_buffers)
				circuitGen->addBuffersSimple_OLD();

			//general_dbg_file << "\nAfter addBuffersSimple_OLD!\n";

			general_dbg_file.close();



	// Aya: The following is meant for optimizing bit width
       		if (OptimizeBitwidth::isEnabled()) {
				circuitGen->setSizes();   // called in Bitwidth.cpp
			}
	
	////////////////////////////////////////////////// DONEEE!

	// These were in the original Dynamatic, not sure if I need to fix and add them!!
		// circuitGen->removeRedundantAfterElastic(enode_dag);

	// this is important in filling the frequency field of BBs needed later in buffers
			circuitGen->setFreqs(F.getName());
						
	        aya_printDotDFG(enode_dag, bbnode_dag, opt_cfgOutdir + "/" + fname + "_graph.dot", opt_serialNumber);
	        printDotCFG(bbnode_dag, (opt_cfgOutdir + "/" + fname + "_bbgraph.dot").c_str());
#endif

          //  circuitGen->sanityCheckVanilla(enode_dag);
        }
    }

    bool runOnFunction(Function& F) override {
		LoopInfo &LI = getAnalysis<LoopInfoWrapperPass>().getLoopInfo();
		this->compileAndProduceDOTFile(F, LI); 
    }

    void print(llvm::raw_ostream& OS, const Module* M) const override {}
};
} // namespace

char MyCFGPass::ID = 1;

static RegisterPass<MyCFGPass> Z("mycfgpass", "Creates new CFG pass",
                                 false, // modifies the CFG!
                                 false);

/* for clang pass registration
 */
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

static void registerClangPass(const PassManagerBuilder&, legacy::PassManagerBase& PM) {
    PM.add(new MyCFGPass());
}

static RegisterStandardPasses RegisterClangPass(PassManagerBuilder::EP_EarlyAsPossible,
                                                registerClangPass);

bool fileExists(const char* fileName) {
    FILE* file = NULL;
    if ((file = fopen(fileName, "r")) != NULL) {
        fclose(file);

        return true;

    } else {

        return false;
    }
}
