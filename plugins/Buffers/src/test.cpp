#include <iostream>
//#include "SatSolver.h"
//#include "Cats.h"
#include "ErrorManager.h"
//#include "Circuit.h"
#include "Dataflow.h"
#include "MILP_Model.h"
//#include "Dataflow.h"
#include "DFnetlist.h"
#include <sstream>


using namespace std;

using namespace Dataflow;

using vecParams = vector<string>;

string exec;
string command;



int main_buffers(const vecParams& params)
{
    if (params.size() != 5) {
        cerr << "Usage: " + exec + ' ' + command + " period buffer_delay solver infile outfile" << endl;
        return 1;
    }

    DFnetlist DF(params[3]);

    if (DF.hasError()) {
        cerr << DF.getError() << endl;
        return 1;
    }

    // Lana 07.03.19. Removed graph optimizations (temporary)
    //DF.optimize();
    double period = atof(params[0].c_str());
    double delay = atof(params[1].c_str());

    cout << "Adding elastic buffers with period=" << period << " and buffer_delay=" << delay << endl;
    DF.setMilpSolver(params[2]);

    bool stat = DF.addElasticBuffers(period, delay, true);

    // Lana 05/07/19 Instantiate if previous step successful, otherwise just print and exit
    if (stat) {
        DF.instantiateElasticBuffers();
    }
    //DF.instantiateElasticBuffers();
    DF.writeDot(params[4]);
    return 0;
}


int main_help()
{
    cerr << "Usage: " + exec + " cmd params" << endl;
    cerr << "Available commands:" << endl;
    cerr << "  dataflow:      handling dataflow netlists." << endl;
    cerr << "  buffers:       add elastic buffers to a netlist." << endl;
    cerr << "  async_synth:   synthesize an asynchronous circuit." << endl;
    cerr << "  solveCSC:      solve state encoding in an asynchronous circuit." << endl;
    cerr << "  hideSignals:   hide signals in an asynchronous specification." << endl;
    cerr << "  syncprod:      calculate the synchronous product of two LTSs." << endl;
    cerr << "  bisimilar:     check whether two LTSs are weakly bisimilar." << endl;
    cerr << "  bbg:           unit test for Basic Block graphs." << endl;
    cerr << "  circuit:       unit test to read and write a circuit." << endl;
}

#include <regex>

struct user_input {
    string graph_name;
    string solver;
    double period;
    double delay;
    double first;
    int timeout;
    bool set;
};

void clear_input(user_input& input) {
    input.graph_name = "dataflow";
    input.set = true;
    input.first = false;
    input.delay = 0.0;
    input.period = 5;
    input.timeout = 180;
    input.solver = "cbc";
}

void print_input(const user_input& input) {
    cout << "****************************************" << endl;
    cout << "dataflow graph name: " << input.graph_name << endl;
    cout << "milp solver: " << input.solver << endl;
    cout << "delay: " << input.delay << ", period: " << input.period << endl;
    cout << "timeout: " << input.timeout << endl;
    cout << "set optimization: " << (input.set ? "true" : "false") << endl;
    cout << "first MG optimization: " << (input.first ? "true" : "false") << endl;
    cout << "****************************************" << endl;
}
void parse_user_input(const vecParams& params, user_input& input) {
    clear_input(input);
    regex period_regex("(-period=)(.*)");
    regex delay_regex("(-delay=)(.*)");
    regex name_regex("(-filename=)(.*)");
    regex timeout_regex("(-timeout=)(.*)");
    regex set_regex("(-set=)(.*)");
    regex solver_regex("(-solver=)(.*)");
    regex first_regex("(-first=)(.*)");
    for (auto param: params) {
        if (regex_match(param, period_regex)) {
            input.period = atof(param.substr(param.find("=") + 1).c_str());
        } else if (regex_match(param, delay_regex)) {
            input.delay = atof(param.substr(param.find("=") + 1).c_str());
        } else if (regex_match(param, name_regex)) {
            input.graph_name = param.substr(param.find("=") + 1);
        } else if (regex_match(param, set_regex)) {
            string tmp = param.substr(param.find("=") + 1);
            input.set = (tmp == "false") ? false : true;
        } else if (regex_match(param, timeout_regex)) {
            input.timeout = atoi(param.substr(param.find("=") + 1).c_str());
        } else if (regex_match(param, solver_regex)) {
            input.solver = param.substr(param.find("=") + 1);
        } else if (regex_match(param, first_regex)) {
            string tmp = param.substr(param.find("=") + 1);
            input.first = (tmp == "false") ? false : true;
        } else {
            cout << param << " is invalid argument" << endl;
            assert(false);
        }
    }
}

void show_help_shab() {
    cout << "-filename: <filename>_graph.dot and <filename>_bbgraph.dot should contain DFC and CFG respectively" << endl;
    cout << "\toptimized output will be writen to <filename>_graph_buf.dot and <filename>_bbgraph_buf.dot" << endl;
    cout << "\tdefault value is \"dataflow\"" << endl;
    cout << "-period: the clock period" << endl;
    cout << "\tdefault value is 3" << endl;
    cout << "-delay: the units' delay" << endl;
    cout << "\tdefault value is 0.0" << endl;
    cout << "-solver: the milp solver" << endl;
    cout << "\tdefault value is cbc" << endl;
    cout << "-timeout: the milp timeout. If -1, no limit will be applied" << endl;
    cout << "\tdefault value is -1" << endl;
    cout << "-set: whether set optimization should be applied or not" << endl;
    cout << "\tdefault value is false" << endl;
    cout << "-first: whether the milp should only consider the throughput for the first MG or not" << endl;
    cout << "\tdefault value is false" << endl;
}

int main_shab(const vecParams& params){

	cout << "\nAya: INSIDE main_shab!\n";

    if (params.size() == 1 && params[0] == "-help") {
        show_help_shab();
        return 1;
    }

    user_input input{};

	cout << "\nAya: before parse_user_input!\n";
    parse_user_input(params, input);
	cout << "\nAya: after parse_user_input!\n";
    print_input(input);
	cout << "\nAya: after print_input!\n";

	cout << "\nAya: Before constructing a DF object!\n";
    DFnetlist DF(input.graph_name + ".dot", input.graph_name + "_bbgraph.dot");
    //DF.cleanElasticBuffers();
	cout << "\nAya: After constructing a DF object!\n";

    if (DF.hasError()) {
        cerr << DF.getError() << endl;
        return 1;
    }

    cout << "Adding elastic buffers with period=" << input.period << " and buffer_delay=" << input.delay << endl;
    cout << endl;

	cout << "\nAya: Before calling DF.setMilpSolver!\n";
    DF.setMilpSolver(input.solver);
	cout << "\nAya: After returning from DF.setMilpSolver!\n";

    bool stat;

    if (input.set) {
		cout << "\nAya: Before calling DF.addElasticBuffersBB_sc!\n";
       stat = DF.addElasticBuffersBB_sc(input.period, input.delay, true, 1, input.timeout, input.first);
		cout << "\nAya: After calling DF.addElasticBuffersBB_sc!\n";
    } else {
		cout << "\nAya: Before calling DF.addElasticBuffersBB!\n";
        stat = DF.addElasticBuffersBB(input.period, input.delay, true, 1, input.timeout, input.first);
		cout << "\nAya: After calling DF.addElasticBuffersBB!\n";
   }
    if (stat) {
        DF.instantiateElasticBuffers();
    }
    DF.writeDot(input.graph_name + "_graph_buf.dot");
    DF.writeDotBB(input.graph_name + "_bbgraph_buf.dot");
    return 0;
}

int main_test(const vecParams& params){
    DFnetlist DF(params[0], params[1]);
//    DF.computeSCC(false);
    //  DF.printBlockSCCs();
}

int main(int argc, char *argv[])
{
    //return main_persistence(argc, argv);
    //return main_csv(argc, argv);
    //return main_nodal(argc, argv);
    //return main_windows(argc, argv);

    exec = basename(argv[0]);
    if (argc == 1) return main_help();

    command = argv[1];
    if (command == "help") return main_help();

    vecParams params;
    for (int i = 2; i < argc; ++i) params.push_back(argv[i]);

    if (command == "buffers_old") return main_buffers(params);
    if (command == "buffers") return main_shab(params);
    if (command == "test") return main_test(params);

#if 0
    if (command == "dataflow") return main_dataflow(params);
    if (command == "dflib") return main_dflib(params);
    if (command == "syncprod") return main_syncprod(params);
    if (command == "bisimilar") return main_bisimilar(params);
    if (command == "hideSignals") return main_hideSignals(params);
    if (command == "solveCSC") return main_solveCSC(params);
    if (command == "async_synth") return main_async_synth(params);
    if (command == "milp") return main_milp(params);
    if (command == "bbg") return main_bbg();
    if (command == "circuit") return main_circuit(params);
#endif
    cerr << command << ": Unknown command." << endl;
    return main_help();
}
