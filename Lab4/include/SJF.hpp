#include <vector>


class SJF{

public:
    std::vector<processBlock> processQ;
    int currTime = 0;
    int isdone = 0;


    SJF(std::vector<processBlock> processQue);  

    void doJob(processBlock& job);

    processBlock pickNextJob();

    void simulate();
};

SJF::SJF(std::vector<processBlock> processQue){
    this->processQ = processQue;
}

void SJF::doJob(processBlock& job){
    this->currTime+= job.TotalBurst + job.TotalIoTime;

    for (int i = 0; i < this->processQ.size(); i++){
        
        if (this->processQ[i].arrivalTime == job.arrivalTime && this->processQ[i].TotalBurst == job.TotalBurst && this->processQ[i].TotalIoTime == job.TotalIoTime) {
            this->processQ.erase(this->processQ.begin() + i);
            return;
        }
        
    }

}

// 0 for successfully picking next job
// 1 for empty processQ
// -1 fail due to no arrival never returns -1
processBlock SJF::pickNextJob(){


    processBlock ret("-1");

    if (this->processQ.empty()){
       this->isdone = 1;
       return ret;
    }
    

    this->isdone = -1;

    processBlock minJob = this->processQ[0];

    schedule:
    for (int i = 0; i < this->processQ.size(); i++){
        
        if (this->currTime > this->processQ[i].arrivalTime && (this->processQ[i].TotalBurst + this->processQ[i].TotalIoTime) <= (minJob.TotalBurst + minJob.TotalIoTime)){
            minJob = this->processQ[i];
            this->isdone = 0;
        }
    }

    if (this->isdone ==-1 && !this->processQ.empty()){
        this->currTime+=500;
        goto schedule;
    }
    
    return minJob;
}

void SJF::simulate(){
    processBlock currentJob = this->pickNextJob();

    while (!isdone){
        this->doJob(currentJob);

        currentJob = this->pickNextJob();
    }
    
}