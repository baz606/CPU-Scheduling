//
// Created by shehbaz on 2/6/18.
//

#include "LinkedList.h"

using namespace std;

// Constructor: Sets null values for head, tail and firstHead
LinkedList::LinkedList()
{
    head = nullptr;
    tail = nullptr;
    firstHead = nullptr;
}

// Deconstructor: Calls the makeListEmpty() function to deallocate memory in the linked list
LinkedList::~LinkedList()
{
    makeListEmpty();
}

// Inserts a request node to the linked list in the appropriate location depending on the requestType
void LinkedList::insert(int time, requestType reqType, int processNumber)
{
    request *newRequest = new request(time, reqType, processNumber);

    if(isEmpty() && newRequest->type == NEW)
    {
        head = newRequest;
        tail = newRequest;
        firstHead = newRequest;
        newRequest->nextProcess = nullptr;
        newRequest->nextRequest = nullptr;
    }
    else if(newRequest->type == NEW)
    {
        head->nextProcess = newRequest;
        newRequest->nextProcess = nullptr;
        head = newRequest;
        tail = newRequest;
    }
    else
    {
        tail->nextRequest = newRequest;
        newRequest->nextRequest = nullptr;
        tail = newRequest;
    }
}

// Prints all of the linked list element, used for debugging
void LinkedList::printAll()
{
    head = firstHead;
    tail = firstHead;

    while(head != nullptr)
    {
        cout << "NEW " << head->requestTime << "  " << head->processNumber << "    ";
        tail = tail->nextRequest;

        while(tail != nullptr)
        {
            cout << tail->type << "  " << tail->requestTime << "   ";
            tail = tail->nextRequest;
        }

        cout << endl;
        head = head->nextProcess;
        tail = head;
    }
}

// Checks whether the linked list is empty
bool LinkedList::isEmpty()
{
    return (head == nullptr && tail == nullptr && firstHead == nullptr);
}

// Iterates through the linked list elements and deletes the nodes to deallocate memory
void LinkedList::makeListEmpty()
{
    request *temp = nullptr;
    while(firstHead != nullptr)
    {
        request *reqTemp = firstHead;
        firstHead = firstHead->nextProcess;

        while(reqTemp != nullptr)
        {
            temp = reqTemp;
            reqTemp = reqTemp->nextRequest;
            delete temp;
        }
    }
}

// Gets the pointer of the request node of NEW type given a process number
request* LinkedList::getProcessAddressAt(int processNumber)
{
    request *temp = firstHead;
    int i= 0;

    while(i < processNumber)
    {
        temp = temp->nextProcess;
        i++;
    }

    return temp;
}