#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <algorithm>
#include <string>
#include <vector>

using namespace std;

/*
 * Assignment 4
 * CPSC 457 - L01 - T05
 * Eddy Qiang - 30058191
 *
 * This program is an implementation of the Banker's Algorithm. The program will determine whether
 * there is a safe execution sequence for a given set of processes, and a request. The process
 * information will be provided in a configuration file, specified as a command-line argument. 
 */
class Banker
{
private:
    int numProc;      // the number of processes
    int numResources; // the number of resources
    int * available;  // number of available instances of each resource
    int ** allocation;// the number of resource instances already allocated
    int ** need;      // the number of resource isntances needed by each process
	int ** max;       // the max demand of each process, e.g., max[i][j] = k
                      // means process i needs at most k instances of resource j
public:

    /* 
	 * Initializing the vectors and matrixes for the Banker's Algorithm. Takes ownership of all arrays.
     * @param avail  The available vector
     * @param m      The max demand matrix
     * @param alloc  The allocation matrix
     * @param p      The number of processes
     * @param r      The number of resources
     */
    Banker (int * avail, int ** m, int ** alloc, int p, int r) 
	{
        numProc = p;
        numResources = r;
        // Setup the available vector, the max matrix, and the allocation matrix
        available = avail;
        max = m;
        allocation = alloc;
        // Initialize the need matrix
        need = new int*[numProc];
        for (int i = 0; i < numProc; i++)
		{
            need[i] = new int[numResources];
		}
    }

    // Destroy the vectors and matrixes
    ~Banker() 
	{
        numProc = 0;
        numResources = 0;
        // Free all allocated memory space
        delete[] available;
        for (int i = 0; i < numProc; i++)
        {
            delete[] need[i];
            delete[] max[i];
            delete[] allocation[i];
        }
        delete[] need;
        delete[] max;
        delete[] allocation;
    }

    /* 
	 * Check whether it is safe to grant the request
     * @param pid    The process that is making the request
     * @param req    The request
     * @param sequenceOrReason  The safe execution sequence returned by the algorithm
     * @return Whether granting the request will lead to a safe state.
     */
    bool isSafe (int pid, int* req, string &sequenceOrReason) 
	{	
        //calculate the need matrix with formula: need = max - alloc
		for(int i = 0; i < numProc; i++) 
		{
			for (int j = 0; j < numResources; j++) 
			{
				need[i][j] = max[i][j] - allocation[i][j];
			}
		}
		//reject if request is larger than the amount avaliable
		for(int i = 0; i < numResources; i++)
		{
			if(req[i] > available[i])
			{
				sequenceOrReason = "not enough resources available.";
				return false;
			}				
		}
		//reject if request is larger than the max amount
		for (int j = 0; j < numResources; j++)
		{
			if (req[j] > need[pid][j])
			{
				sequenceOrReason = "request is invalid (exceeding declared max for process).";
				return false;
			}		
		}
		//update the allocation and need matrix
		for (int i = 0; i < numResources; i++)
		{
			available[i] -= req[i];
			allocation[pid][i] += req[i];
			need[pid][i] -= req[i];
		}	
		//initialize an array that tracks if each process has finished
        int finish[numProc];
        for (int i = 0; i < numProc; i++)
		{
            finish[i] = 0;
        }
		//this vector stores a safe execution sequence in order
        vector<int> processSequence;
		//initialize flags for detecting the state of the algorithm
        int notFinished = 1;
		int unsafeState = 1;
		//main loop that runs until it determines if an request is safe
        while(notFinished)
        {
			//loop through each process i 
            for (int i = 0; i < numProc; i++)
            {
				//counter to determine if need <= available
                int counter = 0;
				//for each resource j
                for (int j = 0; j < numResources; j++)
                {
					//check process if it has not been ran yet and has enough resources
                    if ((finish[i] == 0) && (need[i][j] <= available[j]))
                    {
                        counter += 1;
                    }
                }
				//if all resources are available then execute it
                if (counter == numResources)
                {
					//add that processes' allocation to avaliable
					for (int k = 0; k < numResources; k++)
					{
						available[k] += allocation[i][k];
					}
					//mark process as finished
					finish[i] = 1;
					//add to vector so the execution order is tracked
					processSequence.push_back(i);
					//since a process was able to run, we are currently not in a unsafe state
					unsafeState = 0;
					//start again from the beginning and look for another process
					break;
                }
            }
			//if an unsafe state was detected, then exit loop
			if(unsafeState)
			{
				break;
			}
			//this flag is true when all processes have ran, false otherwise
			notFinished = 0;
			//check each processes to see if they have been ran
			for (int i = 0; i < numProc; i++)
			{
				//if any of them is still 0 then we need to check all the remaining processes again
				if (finish[i] == 0)
				{
					notFinished = 1;
				}
			}
        }
		//set the reasoning for granting or rejecting a request
		if(unsafeState)
		{
			sequenceOrReason = "request would result in an unsafe state.";
			return false;
		}
		else
		{
			string answer;
			//construct the sequence 
			for (int i = 0; i < processSequence.size()-1; i++)
			{
				answer += "P" + to_string(processSequence[i]) + ", ";
			}
			answer += "P" + to_string(processSequence[processSequence.size()-1]);
			
			sequenceOrReason = answer;
		}
		return true;
    }
};

int main (int argc, char * const argv[])
{
    ifstream config;       // Configuration file
    string conffile;       // The configuration file name
    int numProc;           // The number of processes
    int numResources;      // The number of resources
    string sequenceOrReason;       // The execution sequence returned by the Banker's Algorithm
    int i, j, index;       // Indices for the vectors and matrixes
    int pid;               // The ID of the process that is making the request
    string reqStr;         // The request vector in string format

    // Read in the config file name from the commanda-line arguments
    if (argc < 2)
    {
        cout << "Usage: banker <config file>\n";
        return 0;
    }
    else
    {
        conffile = argv[1];
    }

    // Open the file
    config.open(conffile.c_str());

    // Get the number of process and the number of resources
    string line, var, equal;    		// strings for parsing a line in the config file
    getline(config, line);
    istringstream iss(line);
    iss >> var >> equal >> numProc;     // Get the number of processes
    iss.clear();

    getline(config, line);
    iss.str(line);
    iss >> var >> equal >> numResources;// Get the number of resources
    iss.clear();

    // Create the available vector, the max matrix, and the allocation matrix
    // according to the number of processes and the number of resources
    int * available = new int[numResources];
    int ** max = new int*[numProc];
    int ** allocation = new int*[numProc];
    for (int i = 0; i < numProc; i++)
    {
        max[i] = new int[numResources];
        allocation[i] = new int[numResources];
    }

    // Get the available vector
    getline(config, line);
    replace(line.begin(), line.end(), '<', ' ');  // Remove "<" and ">"
    replace(line.begin(), line.end(), '>', ' ');
    iss.str(line);
    iss >> var >> equal;
	// Read in the "available" vector
    for (j = 0; j < numResources; j++) 
	{		
        iss >> available[j];
	}
    iss.clear();

    // Get the max matrix and the allocation matrix
    for (i = 0; i < numProc; i++)
    {
        getline(config, line);
        replace(line.begin(), line.end(), '<', ' ');
        replace(line.begin(), line.end(), '>', ' ');
        iss.str(line);
        iss >> var;
        index = atoi(&var.at(1));            // Get the process ID
        if (index < 0 || index >= numProc)
        {
            cerr << "Invalid process ID: " << var << endl;
            return 0;
        }
        // Get the number of resources allocated to process "index".
        for (j = 0; j < numResources; j++)
		{
            iss >> allocation[index][j];
		}
        // Get the max allocation to process "index".
        for (j = 0; j < numResources; j++)
		{
            iss >> max[index][j];
		}
        iss.clear();
    }

    // Get the request vector
    int * request = new int[numResources];
    getline(config, line);
    reqStr = line.substr(line.find('<'), line.find('>') - line.find('<') + 1);
    replace(line.begin(), line.end(), '<', ' ');
    replace(line.begin(), line.end(), '>', ' ');
    iss.str(line);
    iss >> var >> pid >> equal;
	// Read in the "request" vector
    for (j = 0; j < numResources; j++)
	{		
        iss >> request[j];
	}
    iss.clear();

    // Check the request using the Banker's algorithm.
    Banker * banker = new Banker(available, max, allocation, numProc, numResources);
    if (banker -> isSafe(pid, request, sequenceOrReason))
	{
        cout << "Grant request " << reqStr << " from P" << pid << ".\n"
             << "Sequence: " << sequenceOrReason << ".\n";
	}
    else
	{
        cout << "Reject request " << reqStr << " from P" << pid << ".\n"
             << "Reason: " << sequenceOrReason << "\n";
	}
}
