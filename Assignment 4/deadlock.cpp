#include <string.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <set>
#include <list>
#include <algorithm>

using namespace std;

/**
 * Assignment 4
 * CPSC 457 - L01 - T05
 * Eddy Qiang - 30058191
 */
 
//vector of all graphs
vector<vector<string> > vectorOfAllGraphs;

//vector of sub graphs
vector<string> vec;

int main(int argc, char *argv[])
{
	//read input
	string line;
	while (getline (cin,line))
	{
		//keep adding each line to their own vector until you see a #
		if (line.find("#") != string::npos)
		{
			//when you see the hash add that subgraph to the vector of all graphs
			vector<string> temp(vec);
			vectorOfAllGraphs.push_back(temp);
			//clear the vector so it can load another graph after the #
			vec.clear();
		}
		else
		{
			vec.push_back(line);
		}
	}
	//push the last graph because there is no # after the last line
	vectorOfAllGraphs.push_back(vec);

	//for each graph g, find the deadlocked processes
	for (int g = 0; g < vectorOfAllGraphs.size(); g++)
	{
		//this is the graph we are currently working on
		vector<string> graph(vectorOfAllGraphs[g]);
		
		//this map contains the mappings of the processes and resources to their outgoing nodes
		//the key is the process or resource number, the value is a list of connected node numbers
		unordered_map<int, vector<int> > mymap; 
		
		//this vector will contain the keys to the map, with the key being the number
		vector<int> processesAndResources;
		
		//this loop goes through each line of input that was read in and parses it
		for(int i = 0; i < graph.size(); i++)
		{
			//split string by space to get process number, the arrow direction, and resource number
			stringstream ss(graph[i]);
			int process, resource;
			string arrow;
			ss >> process >> arrow >> resource;

			//put all the processes and resources into a vector
			//the resource is multipled by -1 so we can distinguish them from processes
			//that have the same number
			processesAndResources.push_back(process);
			processesAndResources.push_back(-resource-1);
		}
		
		//remove the duplicates by converting to set and converting back to vector
		set<int> set( processesAndResources.begin(), processesAndResources.end() );
		processesAndResources.assign(set.begin(), set.end());

		//loop through and create a mapping for each process and resource
		for(int i = 0; i < processesAndResources.size(); i++)
		{
			//this step only initializes the key, and assigns a empty list as the value
			vector<int> outgoing;
			mymap[processesAndResources[i]] = outgoing;
		}
			
		//this loop will update the list for each key, adding the connections for each node
		for(int i = 0; i < graph.size(); i++)
		{
			//split string by space to get process number, the arrow direction, and resource number
			stringstream ss(graph[i]);
			int process, resource;
			string arrow;
			ss >> process >> arrow >> resource;
			
			//the resource is multipled by -1 so we can distinguish them from processes
			//that have the same number
			//depending on the direction of the arrow assign the connection
			if (arrow.find("<-") != string::npos)
			{
				mymap[-resource-1].push_back(process);
			}
			else if (arrow.find("->") != string::npos)
			{
				mymap[process].push_back(-resource-1);
			}
		}
		
		//when this flag is false program will print answer list
		int noDeadlock = 0;
		//this list stores the deadlocked process numbers
		list<int> answer;

		//main loop that runs my algorithm to find if there are deadlocked processes
		while(true)
		{
			//initialized assumming process and resource numbers will not exceed 1 million
			int lastRemoved = -1000001;
			bool flag = true;

			//if the node has no outgoing edges then it can't be part a cycle, so erase and remove it 
			for ( auto it = mymap.begin(); it != mymap.end(); ++it )
			{
				if(it -> second.size() == 0)
				{
					lastRemoved = it -> first;
					mymap.erase(it -> first);
					flag = false;
					break;
				}
			}

			if(flag)
			{
				//get the deadlocked processes and add to the list
				for ( auto it = mymap.begin(); it != mymap.end(); ++it )
				{
					//we only want positive numbers because they are processes
					//the negative keys are resources and we don't want to print those
					if (it -> first > -1)
					{
						answer.push_back(it -> first);
					}
				}
				//since there is a deadlock, we can stop and break from the loop
				break;
			}
			
			//edges going to a removed node can be removed since it can't be part of a cycle by going there
			for ( auto it = mymap.begin(); it != mymap.end(); ++it )
			{
				for(int j = 0; j < it -> second.size(); j++)
				{
					if (it -> second[j] == lastRemoved)
					{
						it -> second.erase(remove(it -> second.begin(), it -> second.end(), lastRemoved), it -> second.end());
					}
				}
			}
			
			//after backtracking and everything is erased then we know there are not any cycles
			if (mymap.size() == 0)
			{
				//print the message and break from loop
				cout << "Deadlocked processes: none" << endl;
				noDeadlock = 1;
				break;
			}
		}
		//print the processes for when a deadlock was found
		if(!noDeadlock)
		{
			answer.sort();
			cout << "Deadlocked processes: ";
			for(auto x : answer) 
			{
				cout << x << " ";
			}
			cout << endl;
		}
		answer.clear();
	}
	return 0;
}
