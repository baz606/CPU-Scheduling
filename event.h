//
// Created by shehbaz on 2/1/18.
//

#ifndef OS_HW1_EVENT_H
#define OS_HW1_EVENT_H

#include "request.h"

enum eventType {START, END};

// Structure for the event that is used in the vector for event-driven processing.
// Keeps track of the request address to the linked list so it can process the next request accordingly
// Stores the time at which it happens.
struct event
{
    int time;
    eventType type;
    request *reqAddress;

    event(int time, eventType type, request *req)
    {
        this->time = time;
        this->type = type;
        reqAddress = req;
    }
};


#endif //OS_HW1_EVENT_H
