//--- 2022-2023 Fall Semester Data Structure Assignment 2 ---//
//--------------------------//
//---Name & Surname:Ömer YILDIZ
//---Student Number:150200060
//--------------------------//

//-------------Do Not Add New Libraries-------------//
//-------------All Libraries Needed Were Given-------------//
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <fstream>

#include "structs.h"

using namespace std;

//-------------Complete The Functions Below-------------//

//-------------Initialize Priority Queue-------------//
PriorityQueue::PriorityQueue(){
   this->head = NULL;
};

//-------------Insert New Node To Priority Queue-------------//
void PriorityQueue::enque(Node* newnode){
    /* queue boş ise */
    if(this->head == NULL){
        this->head = newnode;
    }
    else{
        Node* searchNode = this->head;
        /* yeni node head olcakak ise */
        if(newnode->token.val < this->head->token.val){
            searchNode = NULL;
        }
        else{
            while(searchNode->next){
                if(searchNode->next->token.val > newnode->token.val ){
                    break;
                }
                searchNode = searchNode->next;
            }
        }
//-------------Yeni Node yeni head olcak ise-------------//
        if(searchNode == NULL){
            newnode->next = this->head;
            this->head = newnode;
        }
//-------------Yeni node en sonda ise-------------//
        else if(searchNode->next == NULL){
            searchNode->next = newnode;
        }
        else{
            newnode->next = searchNode->next;
            searchNode->next = newnode;
        }
    }
};

//-------------Remove Node From Priority Queue-------------//
Node* PriorityQueue::dequeue(){
    Node* p = this->head;
    if(this->head == NULL){
        return NULL;
    }
    else if(this->head->next == NULL){
        this->head = NULL;
        return p;
    }
    else{
        this->head = p->next;
        return p;
    }
};

//-------------Initialize Tree-------------//
Tree::Tree(){
    this->root = NULL;
};

//-------------Delete Tree Nodes From Memory-------------//
Tree::~Tree(){
    if(this->root != NULL)
        this->deleteTree(this->root);
    this->root=NULL;
};

//-------------Delete Tree From Memory-------------//
void Tree::deleteTree(Node* node){
    if (node == NULL) return;
 
    /* first delete both subtrees */
    deleteTree(node->left);
    deleteTree(node->right);
     
    delete node;
}

//-------------Merges Two Node Into One-------------//
Node* Tree::mergeNodes(Node* temp1, Node* temp2){
    Node* mergeN = new Node();
    mergeN->token.val = temp1->token.val + temp2->token.val;
    mergeN->token.symbol = temp1->token.symbol + temp2->token.symbol;
    mergeN->left = temp1;
    mergeN->right = temp2;
    return mergeN;
 };

void Tree::printTree(Node* traverse, int level){
    cout << level << "\t";
    for (int i = 0; i < level; i++)
        cout << "\t"; 

    cout << traverse->token.symbol << "(" << traverse->token.val << ")" << endl;

    if(traverse->left)
        printTree(traverse->left, level + 1);  
    if(traverse->right)
        printTree(traverse->right, level + 1);
};