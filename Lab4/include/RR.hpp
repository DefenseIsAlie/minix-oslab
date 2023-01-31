#include <vector>
#include <algorithm>
#include <fstream>

class RR{

public:
    std::vector<processBlock> processQ;
    int currTime = 0;
    std::vector<processBlock> readyQ;
    int isdone = 0;

    int timeSlice = 50; // Time slice for RR

    std::vector<processBlock> IoQ;

    int prevIoTime = 0;

    RR(std::vector<processBlock> processQue);
    
    void doCpuJob();

    void updateReadyQ();

    void doIoJob();

    void simulate();

    void printStatistics();

    std::vector<processBlock> doneJobs;
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

    if (job.burstIo.size() == 0 || job.pid == 0){
        return;
    }
    


    // do the job
    int timeSlice = this->timeSlice;

    bool isBurst = true;
    for (int& timeleft : job.timeComplete){        

        if (timeleft > 0){

            if (!isBurst){
                job.CurrIoTime = timeleft;
                job.Iostart = std::max(this->prevIoTime, this->currTime);
                this->prevIoTime = job.Iostart + timeleft;
                timeleft=0;
                this->IoQ.push_back(job);
                return;
            }

            if (!job.first_executed){
                job.first_executed = true;
                job.first_execution = this->currTime;
            }
            
            std::pair<int,int> sched;
            sched.first = this->currTime;

            if (timeleft >= timeSlice){
                timeleft-=timeSlice;
                this->currTime+=timeSlice;
                sched.second = this->currTime;
                job.scheduleTimes.push_back(sched);
                this->readyQ.push_back(job);
                return;
            }

            timeSlice-=timeleft; // if job is done before time slice then blocked by I/o
            this->currTime+=timeleft;
            sched.second = this->currTime;
            job.scheduleTimes.push_back(sched);
            timeleft = 0;

            bool isJobcomplete = true;

    for (int& time : job.timeComplete){
        if (time>0)
        {
            isJobcomplete = false;
            break;
        }   
    }

    if (isJobcomplete){
        processBlock jobbb("12 123 -1");
        jobbb = job;

        jobbb.job_done_time = this->currTime;
        this->doneJobs.push_back(jobbb);

        return;
    }

        
        }

        
        isBurst = !isBurst;
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

        if(IoJob.Iostart + IoJob.CurrIoTime <= this->currTime){
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
    int count = 0;
    while (!this->isdone){
        count +=1;
       // std::cout << "doing cpu job rq size: " << this->readyQ.size() << "\n";
        this->doCpuJob();
       // std::cout << "doing IO job ioq size: " << this->IoQ.size() << "\n";
        this->doIoJob();

       // std::cout << "updateing readyQ pq size: " << this->processQ.size() << "\n";
        this->updateReadyQ();   

        if (count > 1000)
        {   
            std::cout << this->processQ.size() << "\n";
            
            std::cout << this->processQ[0].burstIo.size() << "\n";

            return;
        }
        
    }

    std::cout << "printing stats" << "\n";
    this->printStatistics();
    
}

void RR::printStatistics(){
    
    for (processBlock& job : this->doneJobs){
        job.turnAroundtime = job.job_done_time - job.arrivalTime;
        job.responsetime = job.first_execution - job.arrivalTime;
        job.penaltyratio =  (double)job.TotalBurst / (double)job.turnAroundtime;
        job.waitingtime = job.job_done_time - job.TotalBurst - job.arrivalTime;
    }

    // for each job

    /**
     * pid
     * time arrival
     * total burst
     * total io
     * time complete
     * time firstexec
     * turnaround
     * tresp
     * penalty
     * twaait
     * tsched
    */

    
    for (processBlock& job : this->doneJobs){
        std::cout << "Job" << job.pid << "\n";
        std::string outfile = "RR_process" + std::to_string(job.pid) + ".txt";
        std::ofstream OUT(outfile);

        OUT << job.pid << "\n";
        OUT << job.arrivalTime << "\n";
        OUT << job.TotalBurst << "\n";
        OUT << job.TotalIoTime << "\n";

        OUT << job.job_done_time << "\n";
        OUT << job.first_execution << "\n";
        OUT << job.turnAroundtime << "\n";
        OUT << job.responsetime << "\n";
        OUT << job.penaltyratio << "\n";
        OUT << job.waitingtime << "\n";
        
        OUT << "\n";
        for ( std::pair x : job.scheduleTimes){
            OUT << x.first << " " << x.second << "\n";   
        }

        OUT.close();

    }
    
}