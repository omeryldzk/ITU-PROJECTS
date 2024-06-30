/* @Author
Student Name: 
Student ID : 
Date: 
*/
#include <iostream> 
#include <stdio.h>
#include <string.h>

#include "ProcessRep.h"
#include "FIFORep.h"

using namespace std;

FIFORep::FIFORep()
{
    mpHead = NULL;
    mpTail = NULL;
}

FIFORep::FIFORep(ProcessRep* head)
{
    ProcessRep* tmp = head;
    this->mpHead = head;
    /*taili bulma algoritması*/
    while(tmp->getNext()!= NULL){
        tmp=tmp->getNext();
    }
    this->mpTail = tmp;
}


FIFORep::~FIFORep()
{
    if(mpHead!=NULL){
        delete mpHead;
    }
}

void FIFORep::setHead(ProcessRep* head)
{
    this->mpHead = head;
}

ProcessRep* FIFORep::getHead()
{
    
    return this->mpHead;
    
}

void FIFORep::setTail(ProcessRep* tail)
{
    this->mpTail = tail;
}

ProcessRep* FIFORep::getTail()
{
    return this->mpTail;
}

void FIFORep::queue(ProcessRep* p)
{
    /*Linked list boş ise */
    if(mpTail == NULL){
        mpHead = p;
        mpTail = p; 
    }
    /*Linked list boş değil ise */
    else{
        mpTail->setNext(p);
        p->setNext(NULL);
        mpTail = p;
    }
}

ProcessRep* FIFORep::dequeue()
{
    ProcessRep* p = mpHead;
    /*Linked list boş ise */
    if(mpHead==NULL){
        return NULL;
    } 
    /*Linked list tek elemana sahip ise*/
    else if(p->getNext()==NULL){
        mpHead = NULL;
        mpTail = NULL;
        return p;
    }
    /*Linked list birden fazla elemana sahip ise*/
    else{
        ProcessRep* newhead = p->getNext();
        p->setNext(NULL);
        mpHead = newhead;
        return p;
    }


}

ProcessRep* FIFORep::searchID(int id)
{
        ProcessRep* node = mpHead;
        if(id == mpHead->getProcessID()){
            return mpHead;
        }
        while(node->getNext()){
            if(node->getNext()->getProcessID() == id){
                return node->getNext();
            }
            node = node->getNext();
        }
        return NULL;

    
}

void FIFORep::printFIFO()
{
    if(mpHead != NULL){
        ProcessRep* node = mpHead;
        while(node!= NULL){
            cout<<node->getProcessType()<<node->getProcessID()<<" "<<node->startTime
            <<" "<<node->endTime<<" "; 
            node=node->getNext();
        }
    }

}