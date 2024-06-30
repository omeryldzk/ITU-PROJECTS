//--- 2022-2023 Fall Semester Data Structure Assignment 2 ---//
//--------------------------//
//---Name & Surname:Ömer YILDIZ
//---Student Number:150200060
//--------------------------//

//-------------Do Not Add New Libraries-------------//
//-------------All Libraries Needed Were Given-------------//
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string.h>
#include <bits/stdc++.h>

#include "huffman.h"
#include "structs.h"

using namespace std;

//-------------Complete The Functions Below-------------//

//-----------------------------------------------------//
//-----------Reads the key from text file--------------//
//------------DO NOT CHANGE THIS FUNCTION--------------//
//-----------------------------------------------------//
void Huffman::readKey(const char* argv){
    ifstream readKeyFile;
    readKeyFile.open(argv);

    if(readKeyFile.is_open()){
        while(!readKeyFile.eof()){
            readKeyFile >> key;
        }
    }
    readKeyFile.close();
};
//-----------------------------------------------------//

//-----------------------------------------------------//
//---------Sorts the key in an alpabetic order---------//
//------------DO NOT CHANGE THIS FUNCTION--------------//
//-----------------------------------------------------//
void Huffman::sortKey(){
    sortedKey = key;
    sort(sortedKey.begin(), sortedKey.end());
};
//-----------------------------------------------------//


//-----------------------------------------------------//
//-------Finds the frequency of the characters---------//
//-----------------------------------------------------//
void Huffman::findFrequency(){
    //DO NOT CHANGE THIS
    sortKey();
    //DO NOT CHANGE THIS
    string ::iterator it = sortedKey.begin();
    Token newToken ;
    newToken.symbol = *it;
    int freq = 1;
    while(it!=sortedKey.end()){
        if(*it != *(it+1) && *(it+1) != '\0'){
            newToken.val = freq;
            Node* temp = new Node();
            temp->token = newToken;
            this->queue.enque(temp);
            newToken.symbol = *(it+1);
            freq = 0;
        }
        else if(*(it+1) == '\0'){
            Node* temp = new Node();
            newToken.val = freq;
            temp->token = newToken;
            this->queue.enque(temp);
        }
        freq++;
        it++;
    }
};
//-----------------------------------------------------//


//-----------------------------------------------------//
//----------------Creates Huffman Tree-----------------//
//-----------------------------------------------------//
void Huffman::createHuffmanTree(){
    this->findFrequency();
    while(queue.head->next != NULL){
        Node* temp1 = queue.dequeue();
        Node* temp2 = queue.dequeue();
        queue.enque(this->huffmanTree.mergeNodes(temp1,temp2));
   }
   this->huffmanTree.root = this->queue.dequeue();
};
//-----------------------------------------------------//

//-----------------------------------------------------//
//---------------Prints the Huffman Tree---------------//
//------------DO NOT CHANGE THIS FUNCTION--------------//
//-----------------------------------------------------//
void Huffman::printHuffmanTree(){
    huffmanTree.printTree(huffmanTree.root, 0);
};
//-----------------------------------------------------//

//-----------------------------------------------------//
//-------------Finds and returns the binary------------//
//----------------value to given character-------------//
//-----------------------------------------------------//
string Huffman::getTokenBinary(char tokenChar, Node* traverse, string tokenBinary){
    string findleft = "";
    string findright = "";
    string oldTokenBin = tokenBinary;
    if(traverse != NULL){
        if(traverse->left){
            tokenBinary += "0";
            findleft = this->getTokenBinary(tokenChar,traverse->left,tokenBinary);
            }
        if(findleft == "" && (traverse->right) && (findright == "")){
//--------------------Sol subtreede tokeni bulamazsa tokenbinaryı eski hale gelir---------------------------------//
            tokenBinary = oldTokenBin;
            tokenBinary += "1";
            findright = this->getTokenBinary(tokenChar,traverse->right,tokenBinary);
        }
//--------------------Leafleri kontrol eder Aranan tokeni bulursa recursiondan çıkar---------------------------------//
        if((traverse->left == NULL) && (traverse->right == NULL)){
                if(traverse->token.symbol[0] == tokenChar){
                    return tokenBinary;
                }
                return "";
        }
        if((findleft != "") ){
            return findleft;
        }
        else if((findright != ""))
        {
            return findright;
        }
    }
    return "";
    


    
};
//-----------------------------------------------------//

//-----------------------------------------------------//
//--------------Encodes the given password-------------//
//-----------------------------------------------------//
void Huffman::encodePassword(string password){
    string empty = "";
    string binary,depth;
    int i = 0;
    while(password[i] != '\0'){
        binary = this->getTokenBinary(password[i],this->huffmanTree.root,empty);
        depth = to_string(binary.size());
        encodedBinaryPassword += binary;
        encodedValPassword += depth;
        i++;
    }
};
//-----------------------------------------------------//

//-----------------------------------------------------//
//-------------Prints the encoded password-------------//
//------------DO NOT CHANGE THIS FUNCTION--------------//
//-----------------------------------------------------//
void Huffman::printEncodedPassword(){
    cout << "Encoded Password Binary: " << encodedBinaryPassword << endl;
    cout << "Encoded Password Value: " << encodedValPassword << endl;
};

//-----------------------------------------------------//
//--------------Decodes the given password-------------//
//-----------------------------------------------------//
void Huffman::decodePassword(string encodedValPassword, string encodedBinaryPassword){
    string::iterator itBin = encodedBinaryPassword.begin();
    string::iterator itVal = encodedValPassword.begin();
    while(itVal != encodedValPassword.end()){
        string temp = "";
        int tmp = (*itVal) - '0'; 
        for(int i=0;i<tmp;i++){
            temp += *itBin;
            itBin++; 
        } 
        this->decodeToken(temp);
        itVal++;
    }
};

//-----------------------------------------------------//

//-----------------------------------------------------//
//---------------Decodes the given binary--------------//
//-----------------------------------------------------//
void Huffman::decodeToken(string encodedToken){
    Node* traverse = this->huffmanTree.root;
    int i = 0;
    while(encodedToken[i] != '\0'){
        if(encodedToken[i] == '0'){
            traverse = traverse->left;
        }
        else if(encodedToken[i] == '1'){
            traverse = traverse->right;
        }
        i++;
    }
    this->decodedPassword += traverse->token.symbol;
};
//-----------------------------------------------------//

//-----------------------------------------------------//
//-------------Prints the decoded password-------------//
//------------DO NOT CHANGE THIS FUNCTION--------------//
//-----------------------------------------------------//
void Huffman::printDecodedPassword(){
    cout << "Decoded Password: " << decodedPassword << endl;
};