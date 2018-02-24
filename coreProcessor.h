//
// Created by shehbaz on 2/11/18.
//

#ifndef OS_HW1_COREPROCESSOR_H
#define OS_HW1_COREPROCESSOR_H

// Core process struct used to create a dynamic array for multi-core processing
// Also keeps track of what processes are using the core.
struct coreProcessor
{
    bool isFree;
    int processNumber;
};

#endif //OS_HW1_COREPROCESSOR_H
