/* @Author
Student Name: 
Student ID : 
Date: 
*/
#include <iostream> 
#include <stdio.h>
#include <string.h>
#include "SchedulerRep.h"

using namespace std;

SchedulerRep::SchedulerRep()
{
    /*dinamik olarak FIFORep objesi oluşturma*/
    for(int i = 0;i<3;i++){
        this->mpProcessFIFO[i] = new FIFORep();
    }
    this->mpRunningProcess = NULL; 
    this->timeSliceCount = 0;
    this->pCpuObj = new CPURep();
}
SchedulerRep::~SchedulerRep()
{
    for(int i = 0;i<3;i++){
        delete this->mpProcessFIFO[i] ;
    }
    delete mpRunningProcess;
    delete pCpuObj;
}

FIFORep* SchedulerRep::getProcessFIFO(int index)
{
    
    return this->mpProcessFIFO[index];

}

void SchedulerRep::setProcessFIFO(FIFORep* fifo, int index)
{
    this->mpProcessFIFO[index] = fifo;
}

void SchedulerRep::setRunningProcess(ProcessRep* p)
{
    this->mpRunningProcess = p;
    /*Eğer yeni bir process setRunningProcesse gelirse quantum time sıfırlanır
    CPUdaki procces quantum timeı dolmadan geri CPUya dönerse bu method çalıştırılmaz
    this->mpRunningProcess = x yazılır*/
    if(mpRunningProcess != NULL){
        this->timeSliceCount = 0;
        /*process ilk defa CPUya gelirse startimeı totaltime olarak belirler*/
        if(p->getProcessTime() == p->remainingTime){
            p->startTime = this->totalTime;
        }
    }
    
    
}

ProcessRep* SchedulerRep::getRunningProcess()
{
    return this->mpRunningProcess;
}

void SchedulerRep::pushProcess(ProcessRep* p)
{
    /* pushProcess gelen processi uygun leveldeki FIFOya sondan ekler */
    if(p->getProcessType() == "A"){
        this->getProcessFIFO(0)->queue(p);
    }
    if(p->getProcessType() == "B"){
        this->getProcessFIFO(1)->queue(p);
    }
    if(p->getProcessType() == "C"){
        this->getProcessFIFO(2)->queue(p);
    }

}

ProcessRep* SchedulerRep::popProcess()
{
    /*popProcess FIFOda en yüksek level de hangi procces varsa onu en baştan alır
    (process FIFOdan silinir) FIFO boşsa geri null gönderir */
    for(int i=0;i<3;i++){
        if(this->getProcessFIFO(i) != NULL){
            if(this->getProcessFIFO(i)->getHead() != NULL){
                return this->getProcessFIFO(i)->dequeue();
            }
        }
    }
    return NULL;
}

bool SchedulerRep::checkTimeSlice()
{
    /*Eğer CPU boş değilse quantum timeı kontrol eder*/
    if(this->getRunningProcess() != NULL){
        if(mpRunningProcess->getProcessType() == "A"){
        if (timeSliceCount == 2){
                return true;
        } 
        else{
            return false;
        }
        }
        else if(mpRunningProcess->getProcessType() == "B"){
        if (timeSliceCount == 4){
                return true;
        } 
        else{
            return false;
        }
        }
        else if(mpRunningProcess->getProcessType() == "C"){
        if (timeSliceCount == 8){
                return true;
        } 
        else{
            return false;
        }
        }
        else return false;
    }
    else {
        return false;
        }

}
ProcessRep* SchedulerRep::sendProcessToCPU(ProcessRep* p){
    return pCpuObj->runCPU(p,this->totalTime);
}
void SchedulerRep::schedule(string type, int id, int arrivalTime, int processTime)
{   
    ProcessRep* job = new ProcessRep(type,id,arrivalTime,processTime);
    /* Schedulerın ve cpunun boş olma durumu */
    if(this->getRunningProcess() == NULL && this->popProcess() == NULL){
        this->setRunningProcess(job);
    }
    /* Quantum timeı kontrol eder eğer quantum time dolduysa FIFOyla yeni gelen işi
    karşılaştırır FIFOda bekleyen iş önceliklidir*/
    else if(this->checkTimeSlice()){
        ProcessRep* oldjob = this->getRunningProcess();
        /*popProcess yerine getHead methodu kullandım 
        Çünkü popProcess ile gelen procces geri aynı yerine dönemez
        FIFOnun sonuna döner onun yerine processi hiç FIFOdan 
        çıkarmadan sadece kontrol edip sadece CPUya geçeceği durumda popProcessi kullandım */
        if(mpProcessFIFO[0]->getHead() != NULL){
            this->setRunningProcess(this->popProcess());
            this->pushProcess(job);
        }
        else if(job->getProcessType() == "A"){
            this->setRunningProcess(job);
        }
        else if(mpProcessFIFO[1]->getHead() != NULL){
            this->setRunningProcess(this->popProcess());
            this->pushProcess(job);

        }
        else if(job->getProcessType() == "B"){
            this->setRunningProcess(job);
        }
        else if(mpProcessFIFO[2]->getHead() != NULL){
            this->setRunningProcess(this->popProcess());
            this->pushProcess(job);
        }
        else {
             this->setRunningProcess(job);
        }
        /*CPUdan gelen process her durumda FIFOya döner */
        this->pushProcess(oldjob);
    }
        
    //   
    /* quantum süreci dolmadan yeni gelen işle CPUdaki processin öncelik karşılatırması*/
    else{
        ProcessRep* oldjob = this->getRunningProcess();
        this->setRunningProcess(NULL);
        if(oldjob->getProcessType() == "A"){
            this->pushProcess(job);
            /*setProcess yerine kullandım çünkü quantum timeın sıfırlanmaması gerekiyor*/
            this->mpRunningProcess = (oldjob);
        }
        /*Eğer yeni gelen iş öncelikliyse CPUdaki process FIFOya döner 
        yeni iş CPUya geçer quantum time sıfırlanır*/
        else if(job->getProcessType() == "A"){
            this->pushProcess(oldjob);
            this->setRunningProcess(job);
        }
        else if(oldjob->getProcessType() == "B"){
            this->pushProcess(job);
            this->mpRunningProcess = (oldjob);
        }
        else if(job->getProcessType() == "B"){
            this->pushProcess(oldjob);
            this->setRunningProcess(job);
        }
        else{
            this->pushProcess(job);
            this->mpRunningProcess = (oldjob);
        }

    }
    /*Process CPU ya gider remaining time bir azalır*/
    this->mpRunningProcess = this->sendProcessToCPU(this->getRunningProcess());
    this->totalTime++;
    /*Eğer CPUdaki procces biterse mpRunnigProcess nulla eşit olur
    bu durumda CPUya FIFOda en öncelikli process gelir.Quantum timeı bir azalttım çünkü
    processin quantum timeı bir saniye sonra başlaması gerekiyor*/
    if(mpRunningProcess == NULL){
        this->setRunningProcess(popProcess());
        this->timeSliceCount-=1;
    }
    this->timeSliceCount+=1;
        
    
}
void SchedulerRep::schedule(ProcessRep* p)
{
    if(p==NULL){
        /*İlk schedule fonksiyonu ile aynı durumnlar geçerlidir*/
        if(this->checkTimeSlice()){
            ProcessRep* job = this->getRunningProcess();
            if(mpProcessFIFO[0]->getHead() != NULL){
                this->setRunningProcess(this->popProcess());
                this->pushProcess(job);
            }
            else if(job->getProcessType() == "A"){
                this->setRunningProcess(job);
            }
            else if(mpProcessFIFO[1]->getHead() != NULL){
                this->setRunningProcess(this->popProcess());
                this->pushProcess(job);

            }
            else if(job->getProcessType() == "B"){
                this->setRunningProcess(job);
            }
            else if(mpProcessFIFO[2]->getHead() != NULL){
                this->setRunningProcess(this->popProcess());
                this->pushProcess(job);
            }
            else {
                this->mpRunningProcess = (job);

            }
        }
        
    

    }
    this->mpRunningProcess = this->sendProcessToCPU(this->getRunningProcess());
    this->totalTime++;
    if(this->mpRunningProcess == NULL){
        this->setRunningProcess(this->popProcess());
        this->timeSliceCount-=1;
    }
    this->timeSliceCount+=1;

}
