/* @Author
Student Name: 
Student ID : 
Date: 
*/
#include <iostream>
#include <stdio.h>
#include <string.h>
// #include "node.h"
#include "ProcessRep.h"
#include "FIFORep.h"
#include "CPURep.h"

using namespace std;

CPURep::CPURep()
{
    this->mFinishedProcess = new FIFORep();
}

CPURep::~CPURep()
{
    delete mFinishedProcess;
}

ProcessRep* CPURep::runCPU(ProcessRep* p, int time)
{   

    if(p!=NULL){ 
        p->remainingTime--;
        /*Procces eğer biterse o anki zamandan bi fazlası endtime olarak belirlenir
        Çünkü schedulerRep classında totaltimeı arttırma fonsiyonu runCPUdan sonra
        geliyor yani 8 saniyede biten bir procces totaltimeı 7yken runCPUya geliyor
        bu yüzden endtimeı bir arttırdım*/
        if(p->remainingTime == 0){
            p->endTime = time+1;
            this->getFinishedProcess()->queue(p);
            return NULL;
        }
        else{
            return p;
        }
    }
    else return NULL;
    
    
    
}

FIFORep* CPURep::getFinishedProcess()
{
    return this->mFinishedProcess;
}

void CPURep::setFinishedProcess(FIFORep* finishedProcess)
{
    this->mFinishedProcess = finishedProcess;
}