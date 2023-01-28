#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "../include/include.hpp"
#include "../include/SJF.hpp"

int main(int argc, char **argv){
    std::vector<std::string> input;

    if (argc > 1){
        input = parseDat(argv[1]);
    }

    std::vector<processBlock> processQ;
    
    for (std::string line : input){
        
        processBlock blk(line);
        processQ.push_back(blk);
        
    }



    return 0;

}