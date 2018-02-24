//
// Created by shehbaz on 2/3/18.
//

#ifndef OS_HW1_LINKEDLIST_H
#define OS_HW1_LINKEDLIST_H


#include "request.h"
#include <iostream>


/*  The LinkedList class implements a two-dimensional linked list for storing each process and its
 *  corresponding requests. It is used by the eventList vector to traverse through the linked list so
 *  order is preserved..
 */
class LinkedList
{
public:
    LinkedList();
    ~LinkedList();
    void insert(int time, requestType reqType, int proceeNumber);
    bool isEmpty();
    void printAll();
    void makeListEmpty();

    request *getProcessAddressAt(int i);

private:
    request *head;
    request *tail;
    request *firstHead;
};


#endif //OS_HW1_LINKEDLIST_H
