//
// Created by shehbaz on 2/1/18.
//

#ifndef OS_HW1_REQUEST_H
#define OS_HW1_REQUEST_H

enum requestType {NEW, CORE, SSD, INPUT};

// Structure for requests that is used as a node in the linked list.
// It consists of requestTime that denotes the time required by that request and the process number to store the process IDs.
// Also, it containes two pointers, nextProcess which points to the next process and
// nextRequest that points to the next request for the process.
struct request
{
    int requestTime;
    int processNumber;
    requestType type;

    request(int time, requestType type, int processNumber)
    {
        requestTime = time;
        this->type = type;
        this->processNumber = processNumber;

        nextProcess = nullptr;
        nextRequest = nullptr;
    }

    request *nextProcess;
    request *nextRequest;
};


#endif //OS_HW1_REQUEST_H
