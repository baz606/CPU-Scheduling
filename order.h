//
// Created by shehbaz on 2/23/18.
//

#ifndef OS_HW1_ORDER_H
#define OS_HW1_ORDER_H

#include <iostream>
#include <string>

using namespace std;

struct order
{
    string message;
    int processID;

    order(string &mess, int pID)
    {
        message = mess;
        processID = pID;
    }
};


#endif //OS_HW1_ORDER_H
