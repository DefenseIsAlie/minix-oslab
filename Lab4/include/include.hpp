#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>

std::vector<std::string> parseDat(std::string filePath);

class processBlock{
public:
    int arrivalTime;
    std::vector<int> burstIo;

    /*
     * DS for SJF 
    */

    int TotalBurst;
    int TotalIoTime;

    /*
        DS for RR
    */

   std::vector<int> timeComplete;
   int IoStart = 0;


processBlock(std::string processdat);

void operator = (const processBlock& rhs){
    arrivalTime = rhs.arrivalTime;
    burstIo = rhs.burstIo;
    TotalBurst = rhs.TotalBurst;
    TotalIoTime = rhs.TotalIoTime;
    timeComplete = rhs.timeComplete;
    IoStart = rhs.IoStart;
}


};

std::vector<std::string> parseDat(std::string filePath){
    std::ifstream input(filePath);
    std::vector<std::string> ret;

    while (!input.eof()){
        std::string line;
        getline(input, line);

        if (line == "<html>" || line == "<body>" || line == "<pre>" || line == "</pre></body></html>"){
            continue;
        } else{
            ret.push_back(line);
        }
        
    }
    
    input.close();

    return ret;
}

processBlock:: processBlock(std::string processdat){
    std::stringstream dat(processdat);
    this->TotalBurst = 0;
    int isBurst = 0;

    int arTime;
    dat >> arTime;
    this->arrivalTime = arTime;

    while (!dat.eof()){
        
        int burstdata;
        dat >> burstdata;

        int burstcopy = burstdata;
        
        if (burstdata == -1){
            break;
        }

        if(isBurst%2 == 0){ this->TotalBurst += burstdata; }
        if(isBurst%2 != 0){ this->TotalIoTime += burstdata; }       

        this->burstIo.push_back(burstdata);
        this->timeComplete.push_back(burstdata);
        

        isBurst+=1;
    }

}
