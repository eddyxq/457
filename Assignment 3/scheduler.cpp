#include <string.h>
#include <iostream>
#include <fstream>
#include <queue>

/**
 * This program simulates the execution of processes using two different scheduling algorithms: 
 * non-preemptive shortest-job-first and preemptive round-robin. Reference Ben. The program will
 * start by reading the description of the processes from a cofiguration file, and then run the
 * simulation. During the simulation your program will output the state of the processes at every
 * simulated time step. The scheduling algorithm will be specified on the command line.
 */
 
using namespace std;

int main(int argc, char *argv[])
{
	//error checking
	if (argc > 4)
	{
		cout << "Too many arguments" << endl;
		return 0;
	}
	else if (argc < 3)
	{
		cout << "Insufficient arguments" << endl;
		return 0;
	}
	else
	{
		string str = argv[2];
		ifstream file(argv[1]);
		if (!file)
		{
			cout << "File not found" << endl;
			return 0;
		}
		cout << argc << endl;
		if (str == "RR" && !(argc == 4))
		{
			cout << "You need 3 arguments for RR" << endl;
			return 0;
		}
		else if (str == "SJF" && argc != 3)
		{
			cout << "You need 2 arguments for SJF" << endl;
			return 0;
		}
	}
	char state[30];
	int array [30][2];  
	int countWait[30];
	int currP = -1;   
	int index = 0;
	int numP = 0;
	int totalTime = 0; 
	queue<int> readyQueue;
	//initialize arrays with default values
	for(int i = 0; i < 30; i++)
    {
        state[i] = ' ';
		countWait[i] = 0;
    }
	//first arg is the file name
    string fileName = argv[1];
    ifstream file(fileName);
	//second arg is the scheduler choice
    string option = argv[2];
	//third arg is the quantum time (only applicable for RR)
	int quantumTime;
	if(option == "RR")
	{
		quantumTime = atoi(argv[3]);
	}
	//read numbers from file
    int arrival, burst;
    while (file >> arrival && file >> burst) 
	{
		//store numbers into vectors
		array[numP][0] = arrival;
		array[numP][1] = burst;
		//count the total time it will take all processes to finish
		totalTime += burst;
		//count the number of processes given
		numP += 1;
    }
	//print the header 
    cout << "Time\t";
    for(int i = 0; i < numP; i++)
    {
        cout << "P" + to_string(i) + "\t";
    }
	cout << "\n------------------------------------------------------------" << endl;
	//start SJF algorithm
	if(option == "SJF")
	{
		for(int i = 0; totalTime > 0; i++)
		{
			while(index < numP)
			{
				if(array[index][0] == i)
				{    
					state[index] = '+';
				}
				else
				{
					break;
				}
				index += 1;
			}
			if(array[currP][1] == 0 || (currP == -1))
			{  
				if(array[currP][1] == 0)
				{
					state[currP] = ' ';
					currP = -1;
				}
				int num = -1;
				for (int i = 0 ; i < index; i++)
				{
					bool prevReady = array[i][1] < array[num][1] && state[i] == '+';
					if((num == -1 && state[i] =='+') || prevReady)
					{
						num = i;
					}
				}
				if(num != -1)
				{
					currP = num;
					state[currP] = '.';
					array[currP][1] -= 1;
				}
			}
			else
			{
				array[currP][1] -= 1;
			}
			cout << i;
			for(int i = 0; i < numP; i++)
			{
				cout << "\t" << state[i];
				if(state[i] == '+')
				{
					countWait[i] += 1;
				}
				else if(state[i] == '.')
				{
					totalTime -= 1;
				}
			}
			cout << "\n";
		}
	}
	//start RR algorithm
	if(option == "RR")
	{   
		int time = quantumTime;   
		for(int i = 0; totalTime > 0; i++)
		{
			while(index < numP)
			{
				if(array[index][0] == i)
				{   
					readyQueue.push(index);
					state[index] = '+';
				}
				else
				{
					break;
				}
				index++;
			}
			if(currP != -1)
			{
				if(time != 0)
				{
					if(array[currP][1] != 0)
					{
						array[currP][1] -= 1;
						time -= 1;
					}
					else
					{
						state[currP] = ' ';
						currP = -1;
						if(readyQueue.size() > 0) 
						{
							currP = readyQueue.front();
							readyQueue.pop();
							time = quantumTime;
							array[currP][1] -= 1;
							state[currP] = '.';
							time -= 1;
						}
						else
						{
							cout << i;
							for(int i=0; i < numP; i++)
							{
								cout << "\t" << state[i];
								if(state[i] == '+')
								{
									countWait[i] += 1;
								}
								else if(state[i] == '.')
								{
									totalTime -= 1;
								}
							}
							cout << "\n";
						}
						
					}
				}
				else
				{
					
					if(array[currP][1] != 0)
					{
						state[currP] = '+';
						readyQueue.push(currP);
						currP = readyQueue.front();
						readyQueue.pop();
						time = quantumTime;
						array[currP][1] -= 1;
						state[currP] = '.';
						time -= 1;
					}
					else
					{
						state[currP] = ' ';
						currP = -1;
						if(readyQueue.size() == 0)
						{
							cout << i;
							for(int i = 0; i < numP; i++)
							{
								cout << "\t" << state[i];
								if(state[i] == '+')
								{
									countWait[i] += 1;
								}
								else if(state[i] == '.')
								{
									totalTime -= 1;
								}
							}
							cout << "\n";
						}
						else 
						{
							currP = readyQueue.front();
							readyQueue.pop();
							time = quantumTime;
							array[currP][1] -= 1;
							state[currP] = '.';
							time -= 1;
						}
					}
				}
				
			}
			else
			{
				if(readyQueue.size() != 0)
				{
					currP = readyQueue.front();
					readyQueue.pop();
					time = quantumTime;
					array[currP][1] -= 1;
					state[currP] = '.';
					time -= 1;
				}
				else
				{
					cout << i;
					for(int i = 0; i < numP; i++)
					{
						cout << "\t" << state[i];
						if(state[i] == '+')
						{
							countWait[i] += 1;
						}
						else if(state[i] == '.')
						{
							totalTime -= 1;
						}
					}
					cout << "\n";
				}
			} 
			cout << i;
			for(int i = 0; i < numP; i++)
			{
				cout << "\t" << state[i];
				if(state[i] == '+')
				{
					countWait[i] += 1;
				}
				else if(state[i] == '.')
				{
					totalTime -= 1;
				}
			}
			cout << "\n";
		}
	}
	//print wait times
	cout << "------------------------------------------------------------" << endl;
    double total = 0;
    for (int i = 0; i < numP; i++) 
	{
        cout << "P" + to_string(i) + " waited " + to_string(countWait[i]) + " sec." << endl;
        total += countWait[i];
    }
    cout << "Average waiting time = " + to_string(total/numP) + " sec." << endl;
	
	return 0;
}