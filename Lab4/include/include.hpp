#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>

std::vector<std::string> parseDat(std::string filePath);

class processBlock{
public:
    int arrivalTime;
    std::vector<int> burstIo;
    int pid;
    /*
     * DS for SJF 
    */

    int TotalBurst;
    int TotalIoTime;

    /*
        DS for RR
    */

   std::vector<int> timeComplete;
   int CurrIoTime = 0;
   int Iostart=0;

    // Statistics
    bool first_executed = false;
    int first_execution = 0; // execution start time
    int job_done_time = 0;

    int turnAroundtime = 0;
    int responsetime = 0;
    double penaltyratio = 0;
    int waitingtime = 0;
    std::vector<std::pair<int,int>> scheduleTimes;


processBlock(std::string processdat);

void operator = (const processBlock& rhs){
    pid = rhs.pid;
    arrivalTime = rhs.arrivalTime;
    burstIo = rhs.burstIo;
    TotalBurst = rhs.TotalBurst;
    TotalIoTime = rhs.TotalIoTime;
    timeComplete = rhs.timeComplete;
    CurrIoTime = rhs.CurrIoTime;
    Iostart = rhs.Iostart;
    job_done_time = rhs.job_done_time;

    first_executed = rhs.first_executed;
    first_execution = rhs.first_execution;
    turnAroundtime = rhs.turnAroundtime;
    responsetime = rhs.responsetime;
    penaltyratio = rhs.penaltyratio;
    waitingtime = rhs.waitingtime;
    scheduleTimes = rhs.scheduleTimes;
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
