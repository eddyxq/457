#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm> 
#include <iterator> 
#include <unordered_map>

using namespace std;

//method returns true if given integer is exists inside array
int inArray(int n, int a[], int size)
{
    for (int i = 0; i<size; i++)
    {
        if(n == a[i])
        {
            return 1;
        }
    }
    return 0;
}

//optimal page replacement algorithm
void optimal(int numFrames, vector<int> v)
{
    unordered_map<int, int> mymap;
    int numPageFaults = 0;
	int count = 0;
    int frame[numFrames];
	
    //initialize all array values to -1
    for (int i = 0; i<numFrames; i++)
    {
        frame[i] = -1;
    }

    //for each number in vector
    for (int i = 0; i<v.size(); i++)
    {
        int currentIndex = i;

        //if can find -1, not full
        if (inArray(-1, frame, numFrames))
        {
            //if it's not already there
            if (!inArray(v[i], frame, numFrames))
            {
                //add to frame at index count
                frame[count] = v[i];
                numPageFaults += 1;
            }
			//if it's a hit, do do not increase count
			else
			{
				count -= 1;
			}
        }
		else if (inArray(v[i], frame, numFrames))
		{
			//do nothing when it is a hit
		}
		//else determine where to swap 
        else
        {
			//look forward
            if (!inArray(v[i], frame, numFrames))
            {
                //for each of the numbers in the frame, find the index of the next occruence
                for (int i = 0; i < numFrames; i++)
                {
                    int isInFuture = 0;
					for (int j = v.size()-1; j > currentIndex; j--)
                    {
                        if (frame[i] == v[j])
                        {
                            mymap[frame[i]] = j;
                            isInFuture = 1;
                        }
                    }
                    if (isInFuture == 0)
                    {
                        mymap[frame[i]] = 99999;
                    }
                }
                int indexThatHasMax;
                int max = 0;
                //find the highest index found, which is the one we should replace
                for (int i = 0; i < numFrames; i++)
                {
                    int temp = mymap[frame[i]];

                    if (temp > max)
                    {
                        max = temp;
                        indexThatHasMax = i;
                    } 
                }
                //replace the index that has max
                frame[indexThatHasMax] = v[i];
                numPageFaults += 1;
            }
        }
        count += 1;
    }

    string frameOutput;
    //get the state of frames at the end
    for (int i = 0; i<numFrames; i++)
    {
        frameOutput += to_string(frame[i]) + " ";
    }
	//display output
    cout << "Optimal:" << endl;
    cout << " - frames: " << frameOutput << endl;
    cout << " - page faults: " << numPageFaults << endl;
}

//least recently used page replacement algorithm
void lru(int numFrames, vector<int> v)
{
    unordered_map<int, int> mymap;
    int numPageFaults = 0;
	int count = 0;
    int frame[numFrames];
	
    //initialize all array values to -1
    for (int i = 0; i<numFrames; i++)
    {
        frame[i] = -1;
    }

    //for each number in vector
    for (int i = 0; i<v.size(); i++)
    {
        int currentIndex = i;

        //if can find -1, not full
        if (inArray(-1, frame, numFrames))
        {
            //if it's not already there
            if (!inArray(v[i], frame, numFrames))
            {
                //add to frame at index count
                frame[count] = v[i];
                numPageFaults += 1;
            }
			else
			{
				count -= 1;
			}
        }
		else if (inArray(v[i], frame, numFrames))
		{
			//do nothing when it is a hit
		}
		//else determine where to swap 
        else
        {
			//look backward
            if (!inArray(v[i], frame, numFrames))
            {
                
                //for each of the numbers in the frame, find the index of the previous occruence
                for (int i = 0; i<numFrames; i++)
                {
                    for (int j = 0; j<currentIndex; j++)
                    {
                        if (frame[i] == v[j])
                        {
                            mymap[frame[i]] = j;
                        }
                    }
                }
                int indexThatHasMin;
                int min = 99999;
                //find the lowest index found, which is the one we should replace
                for (int i = 0; i<numFrames; i++)
                {
                    int temp = mymap[frame[i]];

                    if (temp < min)
                    {
                        min = temp;
                        indexThatHasMin = i;
                    } 
                }
                //replace the index that has max
                frame[indexThatHasMin] = v[i];
                numPageFaults += 1;
            }
        }
        count += 1;
    }

    string frameOutput;
    //get the state of frames at the end
    for (int i = 0; i<numFrames; i++)
    {
        frameOutput += to_string(frame[i]) + " ";
    }
	//display output
    cout << "LRU:" << endl;
    cout << " - frames: " << frameOutput << endl;
    cout << " - page faults: " << numPageFaults << endl;
}

//method that increments the pointer to wrap around like a clock head
int incrementClockPointer(int current, int max)
{
    return (current + 1) % max;
}

//clock replacement (aka second chance) algorithm
void clock(int numFrames, vector<int> v)
{
	//array containing the frame numbers
    int frame[numFrames];
	//array that tracks the clock bit
    int flag[numFrames];
	//index of the clock pointer
    int pointer = 0;
	//variable tracking the number of page faults
	int numPageFaults = 0;

    //initialize array values
    for (int i = 0; i < numFrames; i++)
    {
		//-1 means frame location is empty
        frame[i] = -1;
		//initial bit is 0
        flag[i] = 0;
    }

    //for each number in vector
    for (int i = 0; i < v.size(); i++)
    {
        //if array location is empty (marked by -1)
        if (inArray(-1, frame, numFrames))
        {
            //if it's not already there
            if (!inArray(v[i], frame, numFrames))
            {
                //add to frame, increment page fault, and set bit to 1
                frame[pointer] = v[i];
                flag[pointer] = 1;
                numPageFaults += 1;
				//move pointer to next location
                pointer = incrementClockPointer(pointer, numFrames);
            }
        }
		else if (inArray(v[i], frame, numFrames))
		{
			//do nothing when it is a hit
		}
		//else determine where to swap 
		else
		{
			while(true)
			{
				//find the next frame with bit counter 0
				if (flag[pointer] == 1)
				{
					//if it's 1, decrement and move to next
					flag[pointer] = 0;
					pointer = incrementClockPointer(pointer, numFrames);
				}
				//if it's 0, replace this frame and increment pagefault
				else
				{
					//add to frame, increment page fault, and set bit to 1
					frame[pointer] = v[i];
					flag[pointer] = 1;
					numPageFaults += 1;
					//move pointer to next location
					pointer = incrementClockPointer(pointer, numFrames);
					break;
				}
			}
		}
    }

    string frameOutput;
    //get the state of frames at the end
    for (int i = 0; i<numFrames; i++)
    {
        frameOutput += to_string(frame[i]) + " ";
    }
	//display output
    cout << "Clock:" << endl;
    cout << " - frames: " << frameOutput << endl;
    cout << " - page faults: " << numPageFaults << endl;
}

int main(int argc, char *argv[])
{
	//vector that holds each of the numbers read in from text file
	vector<int> v;
    int numFrames = 0;

    if(argc != 2) 
    {
        cout << "Usage: pagesim <number of frames> < filename.txt" << endl;
        exit(-1);
    }
	else
    {
        numFrames = atoi(argv[1]);
    }

	//read input file 
	string line;
	while (getline (cin,line))
	{
        //store each integer into array
        istringstream in(line);
        string t;
        while (in >> t) 
        {
            v.push_back(stoi(t));
        }
	}    
	
	//run each algorithm
    optimal(numFrames, v);
    lru(numFrames, v);
    clock(numFrames, v);

	return 0;
}
