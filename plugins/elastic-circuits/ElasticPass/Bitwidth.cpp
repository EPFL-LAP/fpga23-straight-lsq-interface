#include "ElasticPass/CircuitGenerator.h"
#include "ElasticPass/Utils.h"
#include "OptimizeBitwidth/OptimizeBitwidth.h"

#define CONTROL_SIZE 0
#define DATA_SIZE 32


//---------------------------------------------
// defined in Utils.h :

// Aya: this function seems to not be called anywhere so I don't need to modify it!!!
int getSuccIdxInPred(const ENode* node, const ENode* pred) {
    // search for node in predecessor's ctrlSuccs
    int idx = indexOf(pred->CntrlSuccs, node);
    if (idx != -1)
        return idx;
    
    idx = indexOf(pred->JustCntrlSuccs, node);
    if (idx != -1)
        return pred->CntrlSuccs->size() + idx;

    return -1;
}
int getPredIdxInSucc(const ENode* node, const ENode* succ) {
    int idx = indexOf(succ->CntrlPreds, node);
    if (idx != -1)
        return idx;
    
    idx = indexOf(succ->JustCntrlPreds, node);
    if (idx != -1)
        return succ->CntrlPreds->size() + idx;
    
    return -1;
}

// I try to identify patterns that occur when an edge carries a condition
//eg. a Branch with predecessor a CmpInst (or a Fork with predecessor a CmpInst) 

bool isBranchConditionEdge(const ENode* from, const ENode* branch) {
    assert (branch->type == Branch_n || branch->type == Branch_c);
    // original idea : use fixed indices as displayed in the .dot output
    //however, this indexing is not respected in node generation

    // int idxInBranch = getPredIdxInSucc(from, branch);
    // return idxInBranch == BRANCH_CONDITION_IN;
    //////////////////////////////////////////////////////////
    /*switch (from->type)
    {
    case Fork_:
        // cheat : as if from == fork.preds[0]
        return isBranchConditionEdge(from->CntrlPreds->front(), branch);

    case Branch_:	
        return true;

	case Cst_:  // AYA: 03/11/2021: Since some conditions are constants
		// depending on the index of this predecessor we will decide if it's a condition or not!!
		{auto it = std::find(branch->CntrlPreds->begin(), branch->CntrlPreds->end(), from);
		assert(it != branch->CntrlPreds->end());
        int ind = distance(branch->CntrlPreds->begin(), it);
		if(ind == 1)
			return true;

		else
			return false;
		}

    case Inst_: {
        unsigned int opcode = from->Instr->getOpcode();
        return opcode == Instruction::ICmp || opcode == Instruction::FCmp;
    }

    case Fork_c:
    default:
        return false;
    } */

    // AYA: 07/11/2022: commented the above code and added the following instead
        // if it is pred 1 not pred 0, then it is the condition
    // search for the pred in the CntrlPreds network 
    auto pos_datapath = std::find(branch->CntrlPreds->begin(), branch->CntrlPreds->end(), from);
    if(pos_datapath != branch->CntrlPreds->end()) {  // I do it as an if condition not an assertion because if it's a Branch_c, and if the pred is in the data side not the condiiton side, then the pred will not be found in the CntrlPreds!
        int idx = pos_datapath - branch->CntrlPreds->begin();
        if(idx == 1) {
            assert(branch->type == Branch_n);
            return true;
        }
        else {
            assert(idx == 0);
            // if the type is Branch_c, return true, else return false!
            if(branch->type == Branch_c) {
                return true;
            } else {
                assert(branch->type == Branch_n);
                return false;
            }
        }
    } 
}

bool isMuxConditionEdge(const ENode* from, const ENode* mux) {
	// AYA: 08/10/2021: added also the irredundant Phi_c in the conditions!!!
    assert (mux->type == Phi_ || mux->type == Phi_n || (mux->type == Phi_c && !mux->is_redunCntrlNet));
    assert (mux->isMux);

    /*// int idxInMux = getPredIdxInSucc(from, mux);
    // return idxInMux == MUX_CONDITION_IN;

    switch (from->type)
    {
    case Fork_:
        return isMuxConditionEdge(from->CntrlPreds->front(), mux);

    case Phi_c: // TODO probably miss a condition here
        return from->isCntrlMg;
    
    default:
        return false;
    }*/
    // AYA: 08/11/2022: commented the above code and replaced it with the following code because I have a fixed place for the SEL of the MUX at index 0 of the CntrlPreds network no matter if the Mux is Phi_ or Phi_c
        // identify the position of the from in the mux CntrlPreds
    auto pos = std::find(mux->CntrlPreds->begin(), mux->CntrlPreds->end(), from);
    if(pos != mux->CntrlPreds->end()) { // I do it as an if condition not an assertion because if it's a Phi_c, and if the pred is in the data side not the SEL side, then the pred will not be found in the CntrlPreds!
        int idx = pos - mux->CntrlPreds->begin();
        if(idx == 0) {
            return true;
        }
        else {
            return false;
        }
    }


}
bool isCMergeConditionEdge(const ENode* cmerge, const ENode* to) {
    assert (cmerge->type == Phi_c);
    assert (cmerge->isCntrlMg);

    // int idxInCMerge = getSuccIdxInPred(to, cmerge);
    // return idxInCMerge == CMERGE_CONDITION_OUT;

    switch (to->type)
    {
    case Fork_:
        // like for branches/muxes, Forks are transparent here
        //if any successor is a condition, this output must be a condition
        for (const ENode* succ : *to->CntrlSuccs)
            if (isCMergeConditionEdge(cmerge, succ))
                return true;
        return false;

    case Phi_:
    case Phi_n:
        return to->isMux;
    
    default:
        return false;
    }
}


// returns the Value* held by the ENode, if not null (one in Instr, Arg, CI, CF)
static Value* getOutput(const ENode* e);

static unsigned getInputSize(const OptimizeBitwidth& OB, const ENode* node, const ENode* pred);
static unsigned getOutputSize(const OptimizeBitwidth& OB, const ENode* node, const ENode* succ);

void CircuitGenerator::setSizes() {

    if (!OptimizeBitwidth::isEnabled())
        return;

    for (ENode* node : *enode_dag) {
        //std::cout << node->Name << " (type=" << node->type << ") - " 
        //    << node->CntrlPreds->size() << ", " << node->JustCntrlPreds->size() << " - " 
        //    << node->CntrlSuccs->size() << ", " << node->JustCntrlSuccs->size() << std::endl;

        // add inputs one by one
        for (const ENode* pred : *node->CntrlPreds)
            node->sizes_preds.push_back(getInputSize(*OB, node, pred));

        for (const ENode* pred : *node->JustCntrlPreds)
            node->sizes_preds.push_back(getInputSize(*OB, node, pred));

		// Aya: 01/11/2021
		for (const ENode* pred : *node->CntrlOrderPreds)
            node->sizes_preds.push_back(getInputSize(*OB, node, pred));

        // add outputs one by ones
        for (const ENode* succ : *node->CntrlSuccs)
            node->sizes_succs.push_back(getOutputSize(*OB, node, succ));

        for (const ENode* succ : *node->JustCntrlSuccs)
            node->sizes_succs.push_back(getOutputSize(*OB, node, succ));

		// Aya: 01/11/2021
		for (const ENode* succ : *node->CntrlOrderSuccs)
            node->sizes_succs.push_back(getOutputSize(*OB, node, succ));

        // add at least one I/O to the nodes
        //eg. even for an End_ node with no successor,
        //an output ("out1:0") is still expected to be printed
        if (node->sizes_preds.empty())
            node->sizes_preds.push_back(CONTROL_SIZE);
        if (node->sizes_succs.empty()){
            //node->sizes_succs.push_back(CONTROL_SIZE);
            // Lana (6.6.2021): fix for End_ (in case of data predecessor (data return), take pred size)
            if (node->CntrlPreds->empty())
                node->sizes_succs.push_back(CONTROL_SIZE);
            else
                node->sizes_succs.push_back(getInputSize(*OB, node, node->CntrlPreds->front()));
        }
    }
}

// AYA: 07/11/2022: I'm debugging this function step by step until I make it compatible with FPL'22!!
static unsigned getInputSize(const OptimizeBitwidth& OB, const ENode* node, const ENode* pred) {

    // for special nodes, (that may not have a corresponding value)
    //we just call getOutput of predecessor
    switch (node->type) {
    case Branch_n:
    case Branch_c:
        // bits needed for condition input : log2(#outs)
        if (isBranchConditionEdge(pred, node))   // AYA: 07/11/2022: DONE!!
            return op::ceil_log2(node->CntrlSuccs->size() + node->JustCntrlSuccs->size() + node->CntrlOrderSuccs->size());   // Aya: 01/11/2021: added also the size of CntrlOrder!!
        // else, same as Fork
    case Fork_:
    case Fork_c: // input_size = output_size of pred
    case LazyFork_:
        return getOutputSize(OB, pred, node);

    case Phi_:
    case Phi_n:
    case Phi_c:
        // if node is Mux, bits needed for condition is log2(#ins - 1) 
        //-1 because the condition itself is an input, and shouldn't be counted
        if (node->isMux && isMuxConditionEdge(pred, node))
            return op::ceil_log2(node->CntrlPreds->size() + node->JustCntrlPreds->size() + node->CntrlOrderPreds->size() - 1); // Aya: 01/11/2021: added also the size of CntrlOrder!!
        return getOutputSize(OB, pred, node);

    case Start_:
    case Source_:
    case Bx_Join_wrapper_:
    case Bx_Buffer_wrapper_:  // AYA: 08/11/2022 added two extra cases for the two new components that are added in the smart LSQ! 
        return CONTROL_SIZE; // no inputs for these nodes

    case End_:
    //case Sink_:
        return getOutputSize(OB, pred, node);

	// AYA: 03/11/2021: Moved it to a separate case to deal with the stupid case of a Branch_c having a wrong size_preds!!
	case Sink_:
		// A sink always has a branch predecessor! It can be Branch_ or Branch_c
		if(pred->type == Branch_c)
			return CONTROL_SIZE;
		else
			return getOutputSize(OB, pred, node);

    case Branch_:
        return 1;

    case Cst_: // constant nodes only relay its value : input_size == output_size
        return getOutputSize(OB, node, nullptr);

    case Argument_:
    case Buffera_:
    case Bufferi_:
    case Fifoa_:
    case Fifoi_: {
        const Value* out = getOutput(node);
        return out != nullptr ? OB.getSize(out) : getOutputSize(OB, pred, node);
    }

    case Inst_:
		// AYA: 03/11/2021: Need to check if the pred is a fork, get its pred!
		// I'm doing it blindly on Fork_ and data network because it should be the case that any Inst_ has its predecessor(s) in the data network!!
		if(pred->type == Fork_) {
			if (getOutput(pred->CntrlPreds->at(0)) != nullptr)
				return OB.getSize(getOutput(pred->CntrlPreds->at(0)));
			return DATA_SIZE; // because it's an Inst_, why would it have control size????
			//return CONTROL_SIZE;
		} else {
			if (getOutput(pred) != nullptr)
				return OB.getSize(getOutput(pred));
			return CONTROL_SIZE;
		}

		//////////// ENd of stuff added by AYA!!
        //if (getOutput(pred) != nullptr)
          //  return OB.getSize(getOutput(pred));
        //return CONTROL_SIZE;

    case MC_:
    case LSQ_:
    default:
        // return contains(node->JustCntrlPreds, pred) ? CONTROL_SIZE : DATA_SIZE;
		// AYA: 01/11/2021: added CntrlOrderPreds along with JustCntrlPreds!!
		return (contains(node->CntrlOrderPreds, pred) || contains(node->JustCntrlPreds, pred)) ? CONTROL_SIZE : DATA_SIZE;
    }
}
static unsigned getOutputSize(const OptimizeBitwidth& OB, const ENode* node, const ENode* succ) {

    // if we have some output (any Value* somehow inherited), use it
    const Value* out = getOutput(node);

    switch (node->type)
    {
    // AYA: 08/11/2022: added two cases for the two new components used in the smart interface of the LSQ!
    case Bx_Buffer_wrapper_:
    case Bx_Join_wrapper_:
        return CONTROL_SIZE;
    break;

    case Fork_:   // if the node is of type fork, the size of any output (succ) should be equivalent to the size of the input of the fork 
    case Fork_c:
    case LazyFork_:  
        if (!node->CntrlPreds->empty())   // identify in which network is the input of the fork 
            return getInputSize(OB, node, node->CntrlPreds->front());
        else {
			if (!node->JustCntrlPreds->empty())
            	return getInputSize(OB, node, node->JustCntrlPreds->front());

			// AYA: 01/11/2021: added the following line!
			else {
                assert(!node->CntrlOrderPreds->empty());
				return getInputSize(OB, node, node->CntrlOrderPreds->front());
            }
		}
			
    case Branch_n: // output_size = data input_size.. need to extract the data input not the condition input
    case Branch_c: 
        for (int i = 0; i < node->CntrlPreds->size(); i++)  // loops to search for the data input of the branch that could be in any of the 3 networks
            if (!isBranchConditionEdge(node->CntrlPreds->at(i), node) /*|| !(node->CntrlPreds->at(i)->type == Cst_ && i == 1)*/)
                return getInputSize(OB, node, node->CntrlPreds->at(i));


        for (const ENode* pred : *node->JustCntrlPreds)
            if (!isBranchConditionEdge(pred, node))
                return getInputSize(OB, node, pred);

		// AYA: 02/11/2021: added this for CntrlOrder
		for (const ENode* pred : *node->CntrlOrderPreds)
            if (!isBranchConditionEdge(pred, node))
                return getInputSize(OB, node, pred);


        assert (false && "Illegal state : Branch should have a non-condition input");

    case Phi_:
    case Phi_n:
    case Phi_c:  
        // AYA: 08/11/2022: I never have CntrlMg in my circuits even if I use the naive LSQ interface because I no longer use them to feed the SEL of my MUXes! 
            // so I commented the following condition
        //if (node->isCntrlMg && isCMergeConditionEdge(node, succ))  
			//return op::ceil_log2(node->CntrlPreds->size() + node->JustCntrlPreds->size() + node->CntrlOrderPreds->size());  // AYA: 01/11/2021: added CntrlOrderPreds as well!!
        if (out != nullptr)
            return OB.getSize(out);
        else 
            return node->type == Phi_c ? CONTROL_SIZE : DATA_SIZE;
            
    case Start_:
        return CONTROL_SIZE;
    case Source_:  
        if (node->CntrlSuccs->empty())
            return CONTROL_SIZE;
        else if (node->CntrlSuccs->front()->type == Cst_)
            return getInputSize(OB, node->CntrlSuccs->front(), node);
        else
            return out != nullptr ? OB.getSize(out) : DATA_SIZE;
        
    case End_:
    case Sink_:
        if (node->CntrlSuccs->empty() || node->CntrlPreds->empty())
            return CONTROL_SIZE;
        else
            return getInputSize(OB, node, node->CntrlPreds->front());

    case Branch_:
        return 1;
    case Cst_:
        return out != nullptr ? OB.getSize(out) : (op::findLeftMostBit(node->cstValue) + 1);

    case Argument_:
    case Buffera_:
    case Bufferi_:
    case Fifoa_:
    case Fifoi_:
        if (out != nullptr)
            return OB.getSize(out);
        else if (!node->CntrlPreds->empty())
            return getInputSize(OB, node, node->CntrlPreds->front());

        else if (!node->JustCntrlPreds->empty())
            return getInputSize(OB, node, node->JustCntrlPreds->front());

		// AYA: 02/11/2021: added the following for CntrlOrderPreds
		else
			return getInputSize(OB, node, node->CntrlOrderPreds->front());

    case Inst_:
        if (node->Instr->getOpcode() == Instruction::Ret) {
            if (node->Instr->getNumOperands() != 0)
                return OB.getSize(node->Instr->getOperand(0));
            else
                return CONTROL_SIZE;
        }
        else
            return OB.getSize(out);

    case MC_:
    case LSQ_:
    default:
        // return contains(node->JustCntrlSuccs, succ) ? CONTROL_SIZE : DATA_SIZE;
		// AYA: 01/11/2021: 
		return (contains(node->CntrlOrderSuccs, succ) || contains(node->JustCntrlSuccs, succ)) ? CONTROL_SIZE : DATA_SIZE;
    }
}

// AYA: 08/11/2022: I assume those fields are field initially inside buildDagEnodes, so I will leave it as is
static Value* getOutput(const ENode* e) {
    Value* res = nullptr;

    if (e->CI != nullptr)
        res = e->CI;
    else if (e->CF != nullptr)
        res = e->CF;
    else if (e->A != nullptr)
        res = e->A;
    else if (e->Instr != nullptr)
        res = e->Instr;
        
    return res;
}


/*
switch (node->type)
{
case Fork_:
case Fork_c:
case Branch_n:
case Branch_c:
case Phi_:
case Phi_n:
case Phi_c:

case Inst_:
case Argument_:
case Cst_:
case Branch_:

case Start_:
case End_:
case Source_:
case Sink_:

case Bufferi_:
case Buffera_:

case LSQ_:
case MC_:
default:
    break;
}

*/

/* JustCntrl <-> size == 0 IS FALSE : 
eg. Fork_->Branch_C node uses JustCntrlPred to communicate the condition input

ie. THIS DOESN'T HOLD
for (ENode* node : *enode_dag) {
    for (unsigned i = 0 ; i < node->JustCntrlPreds->size() ; ++i)
        if (node->sizes_preds.at(node->CntrlPreds->size() + i) != 0) {
            std::cout << "node " << node->type << ", " << node->Name << ", idx=" << i
                << "(#CntrlPreds=" << node->CntrlPreds->size() << ") ; "
                << "from=" << node->JustCntrlPreds->at(i)->Name << std::endl;
            assert (false);
        }

    for (unsigned i = 0 ; i < node->JustCntrlSuccs->size() ; ++i)
        if (node->sizes_succs.at(node->CntrlSuccs->size() + i) != 0) {
            std::cout << "node " << node->type << ", " << node->Name << ", idx=" << i
                << "(#CntrlSuccs=" << node->CntrlSuccs->size() << ") ; " 
                << "to=" << node->JustCntrlSuccs->at(i)->Name << std::endl;
            assert (false);
        }
}
*/
