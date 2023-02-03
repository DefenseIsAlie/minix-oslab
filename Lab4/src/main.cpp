#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "../include/include.hpp"
#include "../include/SJF.hpp"
#include "../include/RR.hpp"

int main(int argc, char **argv){
    std::vector<std::string> input;

    if (argc > 1){
        input = parseDat(argv[1]);
    }

    int pid = 1;
    std::vector<processBlock> processQ;
    std::vector<processBlock> processQ2;
    for (std::string line : input){
        
        processBlock blk(line);
        blk.pid = pid;
        processQ.push_back(blk);
        pid+=1;
    }
    processQ2 = processQ;


 std::cout << "Started sjf" << "\n";    
    SJF sjf(processQ);

    sjf.simulate();
        std::cout << "Finished sjf" << "\n";

    
    std::cout << "Started rr" << "\n";

    RR rr(processQ2);
    rr.simulate();

    std::cout << "Finished rr" << "\n";
    
    // std::vector<processBlock> processQ3;
    // processQ3.push_back(processQ2[0]);

    // RR rr(processQ3);
    // rr.simulate();

    return 0;

}