/*
 * Created by Shehbaz Khan
 *
 * Description: The program simulates the execution of processes of a tablet having multiple cores, 1 SSD and 1 display (input).
 *              Time is simulated using an event based simulation system where each event denotes when a process has started and
 *              ended depending on the number of requests completed. At each start/termination of a process, the number of
 *              RUNNING, READY and BLOCKED processes are printed. At the end of the simulation, a summary of data is displayed.
 *              The data includes: Number of processes completed, Total number of SSD accesses, Average SSD access time (rounded to 2 DP),
 *              Total elapsed time, Core utilization (rounded to 2 DP), SSD utiliazation (rounded to 2 DP).
 */

#include <iostream>
#include <queue>
#include <string>
#include "event.h"
#include "LinkedList.h"
#include <algorithm>
#include <iomanip>
#include "coreProcessor.h"

using namespace std;

#define DEBUG 0

// Macro for debugging
#if DEBUG
#define LOG(x) cout << (x) << endl
#else
#define LOG(x)
#endif

// Protype declarating of each function used
// Each of their functionality is described in their definitions
int checkProcessType(const string &process);
bool compareEvents(const event &ev1, const event &ev2);
bool isCoreFree(coreProcessor *core, int numOfCores);
void setCoreStatus(coreProcessor *coreArr, bool status, int numOfCores, int processNumber);
void checkNextRequest(vector<event> &eventList, coreProcessor *cores, int numOfCores, double &totalTimeCore, deque<int> &processCompleted, deque<int> &processStarted, vector<event>::size_type i, double &totalTimeSSD, int &ssdAccess, double &deltaSSDTime, bool &ssdFree, bool &inputFree, deque<request *> &readyQueue, deque<request *> &ssdQueue, deque<request *> &inputQueue, int numOfProcesses);
void printRunningProcesses(coreProcessor *coreArr, int numOfCores);
void printReadyProcesses(deque<request *> &rQueue);
void printBlockedProcesses(deque<request *> &rQueue, coreProcessor *coreArr, deque<int> &processCompleted, deque<int> &processStarted, int numOfProcesses, int numOfCores);

// Start of the program
int main()
{
    int time = 0;
    int newTime = 0;
    int numOfCores = 0;
    int processNumber = 0;
    string process;

    coreProcessor *cores;
    bool ssdFree = true;
    bool inputFree = true;

    deque<request *> readyQueue;
    deque<request *> ssdQueue;
    deque<request *> inputQueue;
    deque<int> processCompleted;
    deque<int> processStarted;

    vector<event> eventList;
    string message;

    cin >> process >> numOfCores;
    cores = new coreProcessor[numOfCores];

    for (int i = 0; i < numOfCores; i++)
    {
        cores[i].isFree = true;
        cores[i].processNumber = -1;
    }

    LinkedList linkedList;

    // Getting the process and the request time from the standard input
    while (cin >> process >> time)
    {
        if (process == "NEW")
        {
            linkedList.insert(time, (requestType)(checkProcessType(process)), processNumber);
            eventList.emplace_back(time, START, linkedList.getProcessAddressAt(processNumber));
            processNumber++;
        }
        else
        {
            linkedList.insert(time, (requestType)(checkProcessType(process)), processNumber - 1);
        }
    }

    int ssdAccess = 0;
    double deltaSSDTime = 0.0;
    double totalTimeCore = 0.0;
    double totalTimeSSD = 0.0;

    // Process scheduling starts here
    // The loop iterates through the event vector and processes each event according to its type.
    // At each END process, it checks the necessary queue whether it is empty and makes a new event and adds it to the eventList vector.
    for (vector<event>::size_type i = 0; i < eventList.size(); i++)
    {

        if (eventList[i].type == START)
        {
            cout << "\nProcess " << eventList[i].reqAddress->processNumber << " starts at time " << eventList[i].time << " ms." << endl;
            printRunningProcesses(cores, numOfCores);
            printReadyProcesses(readyQueue);
            printBlockedProcesses(readyQueue, cores, processCompleted, processStarted, processNumber, numOfCores);
            cout << endl;

            processStarted.push_back(eventList[i].reqAddress->processNumber);
            checkNextRequest(eventList, cores, numOfCores, totalTimeCore, processCompleted, processStarted, i, totalTimeSSD, ssdAccess, deltaSSDTime, ssdFree, inputFree, readyQueue, ssdQueue, inputQueue, processNumber);
        }
        else if (eventList[i].type == END)
        {
            if (eventList[i].reqAddress->type == CORE)
            {
                setCoreStatus(cores, true, numOfCores, eventList[i].reqAddress->processNumber);

                if (!readyQueue.empty())
                {
                    request *coreReq = readyQueue.front();
                    newTime = eventList[i].time + coreReq->requestTime;
                    eventList.emplace_back(newTime, END, coreReq);
                    stable_sort(eventList.begin(), eventList.end(), compareEvents);
                    readyQueue.pop_front();

                    setCoreStatus(cores, false, numOfCores, coreReq->processNumber);
                    totalTimeCore += coreReq->requestTime;
                }

                checkNextRequest(eventList, cores, numOfCores, totalTimeCore, processCompleted, processStarted, i, totalTimeSSD, ssdAccess, deltaSSDTime, ssdFree, inputFree, readyQueue, ssdQueue, inputQueue, processNumber);
            }
            else if (eventList[i].reqAddress->type == SSD)
            {
                ssdFree = true;
                deltaSSDTime = deltaSSDTime + eventList[i].time;

                if (!ssdQueue.empty())
                {
                    ssdAccess++;
                    request *ssdReq = ssdQueue.front();
                    newTime = eventList[i].time + ssdReq->requestTime;
                    eventList.emplace_back(newTime, END, ssdReq);
                    totalTimeSSD += ssdReq->requestTime;
                    deltaSSDTime = deltaSSDTime - eventList[i].time;

                    stable_sort(eventList.begin(), eventList.end(), compareEvents);
                    ssdFree = false;
                    ssdQueue.pop_front();
                }

                checkNextRequest(eventList, cores, numOfCores, totalTimeCore, processCompleted, processStarted, i, totalTimeSSD, ssdAccess, deltaSSDTime, ssdFree, inputFree, readyQueue, ssdQueue, inputQueue, processNumber);
            }
            else if (eventList[i].reqAddress->type == INPUT)
            {
                inputFree = true;

                if (!inputQueue.empty())
                {
                    request *inputReq = inputQueue.front();
                    newTime = eventList[i].time + inputReq->requestTime;
                    eventList.emplace_back(newTime, END, inputReq);
                    stable_sort(eventList.begin(), eventList.end(), compareEvents);
                    inputQueue.pop_front();
                    inputFree = false;
                }

                checkNextRequest(eventList, cores, numOfCores, totalTimeCore, processCompleted, processStarted, i, totalTimeSSD, ssdAccess, deltaSSDTime, ssdFree, inputFree, readyQueue, ssdQueue, inputQueue, processNumber);
            }
        }
    }

    int totalTimeElapsed = eventList[eventList.size() - 1].time;

    //Checking divide by zero
    double coreUtilization = 0.0;
    double ssdUtilization = 0.0;
    double averageSSDTime = 0.0;

    // Checks divide by zero condition
    if (totalTimeElapsed > 0)
    {
        coreUtilization = (totalTimeCore / totalTimeElapsed) * 100;
        ssdUtilization = (totalTimeSSD / totalTimeElapsed) * 100;
    }

    if (ssdAccess > 0)
    {
        averageSSDTime = deltaSSDTime / ssdAccess;
    }

    // Outputs a summary for the process scheduling
    cout << "\nSUMMARY:" << endl;
    cout << "Number of processes that completed: " << processCompleted.size() << endl;
    cout << "Total number of SSD accesses: " << ssdAccess << endl;
    cout << "Average SSD access time: " << setprecision(2) << fixed << averageSSDTime << " ms" << endl;
    cout << "Total elapsed time: " << totalTimeElapsed << " ms" << endl;
    cout << "Core utilization: " << setprecision(2) << fixed << coreUtilization << " percent" << endl;
    cout << "SSD utilization: " << setprecision(2) << fixed << ssdUtilization << " percent" << endl;

    delete[] cores;
    return 0;
}
// Takes the core array, number of cores and the process id and sets the status, either true or false.
void setCoreStatus(coreProcessor *coreArr, bool status, int numOfCores, int processNumber)
{
    if (!status)
    {
        for (int i = 0; i < numOfCores; i++)
        {
            if (coreArr[i].isFree)
            {
                coreArr[i].isFree = false;
                coreArr[i].processNumber = processNumber;
                return;
            }
        }
    }
    else
    {
        for (int i = 0; i < numOfCores; i++)
        {
            if (!coreArr[i].isFree && coreArr[i].processNumber == processNumber)
            {
                coreArr[i].isFree = true;
                coreArr[i].processNumber = -1;
                return;
            }
        }
    }
}

// Checks whether a core is free
bool isCoreFree(coreProcessor *core, int numOfCores)
{
    for (int i = 0; i < numOfCores; i++)
    {
        if (core[i].isFree)
        {
            return true;
        }
    }

    return false;
}

// Compare function to sort the vector
bool compareEvents(const event &ev1, const event &ev2)
{
    return ev1.time < ev2.time;
}

// Checks what process is being passed and returns a value to denote the type.
// The type is used later as an enum.
int checkProcessType(const string &process)
{
    if (process == "NEW")
    {
        return 0;
    }
    else if (process == "CORE")
    {
        return 1;
    }
    else if (process == "SSD")
    {
        return 2;
    }
    else if (process == "INPUT")
    {
        return 3;
    }

    return -1;
}

// Prints the RUNNING processes by checking all processes that are being used by the coreArr.
void printRunningProcesses(coreProcessor *coreArr, int numOfCores)
{
    for (int i = 0; i < numOfCores; i++)
    {
        if (!coreArr[i].isFree)
        {
            cout << "Process " << coreArr[i].processNumber << " is RUNNING" << endl;
        }
    }
}

// Prints the READY processes by checking all the processes that are in the ready queue.
void printReadyProcesses(deque<request *> &rQueue)
{
    for (auto it : rQueue)
    {
        cout << "Process " << it->processNumber << " is READY" << endl;
    }
}

// Prints the blocked process by checking what processes are NOT READY AND RUNNING.
// This is done by first checking whether a process is not ready by iterating through the ready queue
// Then it iterates through the coreArr to check if that process is executing on a core.
// Finally it checks whether that process is terminated.
// If all checks pass, then the process is printed as BLOCKED.
void printBlockedProcesses(deque<request *> &rQueue, coreProcessor *coreArr, deque<int> &processCompleted, deque<int> &processStarted, int numOfProcesses, int numOfCores)
{
    bool isBlocked;

    for (int i = 0; i < numOfProcesses; i++)
    {
        isBlocked = true;
        for (auto request : rQueue)
        {
            if (request->processNumber == i)
            {
                isBlocked = false;
                break;
            }
        }

        if (isBlocked)
        {
            for (int j = 0; j < numOfCores; j++)
            {
                if (i == coreArr[j].processNumber)
                {
                    isBlocked = false;
                    break;
                }
            }
        }

        if (isBlocked)
        {
            for (auto proc : processCompleted)
            {
                if (i == proc)
                {
                    isBlocked = false;
                    break;
                }
            }

            bool isThere = false;
            for(auto proc : processStarted)
            {
                if(i == proc)
                {
                    isThere = true;
                    break;
                }
            }
            if(!isThere)
            {
                isBlocked = false;
            }
        }

        if (isBlocked)
        {
            cout << "Process " << i << " is BLOCKED" << endl;
        }
    }
}

// Checks what is the next request in the linked list and makes a new event based on that.
// First it checks whether the node in the linked list for that process has reached the nullptr, if it did then terminates that process.
// If the next request is not a nullptr, then checks what the type is and makes a new event based on that.
void checkNextRequest(vector<event> &eventList, coreProcessor *cores, int numOfCores, double &totalTimeCore, deque<int> &processCompleted, deque<int> &processStarted, vector<event>::size_type i, double &totalTimeSSD, int &ssdAccess, double &deltaSSDTime, bool &ssdFree, bool &inputFree, deque<request *> &readyQueue, deque<request *> &ssdQueue, deque<request *> &inputQueue, int numOfProcesses)
{
    int newTime;

    if (eventList[i].reqAddress->nextRequest == nullptr)
    {
        cout << "\nProcess " << eventList[i].reqAddress->processNumber << " terminates at time " << eventList[i].time << " ms." << endl;
        cout << "Process " << eventList[i].reqAddress->processNumber << " is TERMINATED" << endl;
        processCompleted.push_back(eventList[i].reqAddress->processNumber);

        printRunningProcesses(cores, numOfCores);
        printReadyProcesses(readyQueue);
        printBlockedProcesses(readyQueue, cores, processCompleted, processStarted, numOfProcesses, numOfCores);
        cout << endl;
    }
    else if (eventList[i].reqAddress->nextRequest->type == CORE)
    {
        if (isCoreFree(cores, numOfCores))
        {
            setCoreStatus(cores, false, numOfCores, eventList[i].reqAddress->nextRequest->processNumber);
            totalTimeCore += eventList[i].reqAddress->nextRequest->requestTime;

            newTime = eventList[i].time + eventList[i].reqAddress->nextRequest->requestTime;
            eventList.emplace_back(newTime, END, eventList[i].reqAddress->nextRequest);
            stable_sort(eventList.begin(), eventList.end(), compareEvents);
        }
        else
        {
            readyQueue.push_back(eventList[i].reqAddress->nextRequest);
        }
    }
    else if (eventList[i].reqAddress->nextRequest->type == SSD)
    {
        if (ssdFree)
        {
            ssdAccess++;
            deltaSSDTime = deltaSSDTime - eventList[i].time;
            totalTimeSSD += eventList[i].reqAddress->nextRequest->requestTime;

            ssdFree = false;
            newTime = eventList[i].time + eventList[i].reqAddress->nextRequest->requestTime;
            eventList.emplace_back(newTime, END, eventList[i].reqAddress->nextRequest);
            stable_sort(eventList.begin(), eventList.end(), compareEvents);
        }
        else
        {
            ssdQueue.push_back(eventList[i].reqAddress->nextRequest);
        }
    }
    else if (eventList[i].reqAddress->nextRequest->type == INPUT)
    {
        if (inputFree)
        {
            inputFree = false;
            newTime = eventList[i].time + eventList[i].reqAddress->nextRequest->requestTime;
            eventList.emplace_back(newTime, END, eventList[i].reqAddress->nextRequest);
            stable_sort(eventList.begin(), eventList.end(), compareEvents);
        }
        else
        {
            inputQueue.push_back(eventList[i].reqAddress->nextRequest);
        }
    }
}