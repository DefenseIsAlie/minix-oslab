#include <vector>

class RR{

public:
    std::vector<processBlock> processQ;
    int currTime = 0;
    std::vector<processBlock> readyQ;
    int isdone = 0;

    int timeSlice = 50; // Time slice for RR

    std::vector<processBlock> IoQ;

    RR(std::vector<processBlock> processQue);
    
    void doCpuJob();

    void updateReadyQ();

    void doIoJob();

    void simulate();
};

RR::RR(std::vector<processBlock> processQue){
    this->processQ = processQue;
}

void RR::doCpuJob(){

    // get first job from readyQ
    processBlock job("-1");
    if(!this->readyQ.empty()){
        job = this->readyQ[0];
        this->readyQ.erase(this->readyQ.begin());
    } else if(this->readyQ.empty() && (!this->processQ.empty() || !this->IoQ.empty())){
        this->currTime += 10;
        return;
    }


    // do the job
    int timeSlice = this->timeSlice;
    bool isJobcomplete = true;
    bool isBurst = true;
    for (int& timeleft : job.timeComplete){
        if (timeleft > 0){

            if (!isBurst){
                this->currTime += timeleft;
                job.IoStart = this->currTime;
                timeleft=0;
                this->IoQ.push_back(job);
                return;
            }


            if (timeleft >= timeSlice){
                timeleft-=timeSlice;
                this->currTime+=timeSlice;
                this->readyQ.push_back(job);
                return;
            }

            timeSlice-=timeleft; // if job is done before time slice then blocked by I/o
            this->currTime+=timeleft;
            timeleft = 0;

            isJobcomplete = isJobcomplete && false;
        }

        isJobcomplete = isJobcomplete && true;
        isBurst = !isBurst;
    }

    if (isJobcomplete){
        return;
    }else{
        this->readyQ.push_back(job);
    }
    
};

void RR::updateReadyQ(){
    
    if (this->readyQ.empty() && this->IoQ.empty() && this->processQ.empty()){
        this->isdone = true;
    }
    
    if (!this->processQ.empty()){
        std::vector<processBlock> newProcessQ;
        processBlock job("12 34 34 -1");
        
        for (int i = this->processQ.size() -1; i >= 0 ; i--){
            job = this->processQ[i];
            this->processQ.pop_back();

            if (job.arrivalTime <= this->currTime){
                this->readyQ.push_back(job);
            } else{
                newProcessQ.push_back(job);
            }
        
        }

        this->processQ = newProcessQ;   
    }
    
}


// Assume Io Job is independent of Cpu
void RR::doIoJob(){


    if (this->IoQ.empty()){
        return;
    }
    

    std::vector<processBlock> newIoQ;
    for (int i = this->IoQ.size() - 1; i >=0; i--){

        processBlock IoJob("-1");
        IoJob = this->IoQ[i];
        this->IoQ.pop_back();

        if(IoJob.IoStart <= this->currTime){
            this->processQ.push_back(IoJob);
            continue;
        }

        newIoQ.push_back(IoJob);
    }

    this->IoQ = newIoQ;
}

void RR::simulate(){

    // std::cout << "started rr sim" << "\n";
    this->updateReadyQ();

    // std::cout << "updated readyQ for first time" << "\n";
    while (!this->isdone){

        // std::cout << "doing cpu job rq size: " << this->readyQ.size() << "\n";
        this->doCpuJob();
        // std::cout << "doing IO job ioq size: " << this->IoQ.size() << "\n";
        this->doIoJob();

        // std::cout << "updateing readyQ pq size: " << this->processQ.size() << "\n";
        this->updateReadyQ();   

    }
    
}