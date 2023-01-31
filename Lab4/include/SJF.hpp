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

    std::vector<processBlock> doneJobs;

    void printStatistics();
};

SJF::SJF(std::vector<processBlock> processQue){
    this->processQ = processQue;
}

void SJF::doJob(processBlock& job){

    if (!job.first_executed){
        job.first_executed = true;
        job.first_execution = this->currTime;
    }
    
    std::pair<int,int> sched;
    sched.first = this->currTime;

    this->currTime+= job.TotalBurst + job.TotalIoTime;
    job.job_done_time = this->currTime;

    sched.second = this->currTime;
    job.scheduleTimes.push_back(sched);

    processBlock done_job = job;
    this->doneJobs.push_back(job);

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
        
        if (this->currTime >= this->processQ[i].arrivalTime && (this->processQ[i].TotalBurst + this->processQ[i].TotalIoTime) <= (minJob.TotalBurst + minJob.TotalIoTime)){
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

    this->printStatistics();
    
}


void SJF::printStatistics(){
    
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
        std::string outfile = "SJF_process" + std::to_string(job.pid) + ".txt";
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