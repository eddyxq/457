#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm> 
#include <iterator> 
#include <unordered_map>

using namespace std;

/**
 * Assignment 5
 * CPSC 457 - L01 - T05
 * Eddy Qiang - 30058191
 */
 
vector<int> v;

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
		//when frame is full
        else
        {
			//look forward
            if (!inArray(v[i], frame, numFrames))
            {
                //for each of the numbers in the frame, find the index of the next occruence
                for (int i = 0; i<numFrames; i++)
                {
                    int isInFuture = 0;
                    for (int j = currentIndex; j<v.size(); j++)
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
                for (int i = 0; i<numFrames; i++)
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

    cout << "Optimal:" << endl;
    cout << "  - frames: " << frameOutput << endl;
    cout << "  - page faults: " << numPageFaults << endl;
}

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

    cout << "LRU:" << endl;
    cout << "  - frames: " << frameOutput << endl;
    cout << "  - page faults: " << numPageFaults << endl;
}


int incrementClockPointer(int current, int max)
{
    if (current < max)
    {
        return current + 1;
    }
    return 0;
}

void clock(int numFrames, vector<int> v)
{
    int numPageFaults = 0;
	int count = 0;
    int frame[numFrames];
    int flag[numFrames];
    int pointer = 0;

    //initialize all array values to -1
    for (int i = 0; i<numFrames; i++)
    {
        frame[i] = -1;
        flag[i] = 0;
    }

    //for each number in vector
    for (int i = 0; i<v.size(); i++)
    {
        // cout << "index is pointing to: " << pointer << endl;




        //if can find -1, not full
        if (inArray(-1, frame, numFrames))
        {
            //if it's not already there
            if (!inArray(v[i], frame, numFrames))
            {
                //add to frame at index count
                frame[count] = v[i];
                flag[count] = 1;
                numPageFaults += 1;

                if (pointer < numFrames-1)
                {
                    pointer++;
                }
                else
                {
                    pointer = 0;
                }

                
            }
        }
        else
        {
            if (!inArray(v[i], frame, numFrames))
            {

                int notReplaced = 1;
                
                while(notReplaced)
                {
                    //look if flag is 0
                    //for each frame
                    for (int i = 0; i<numFrames; i++)
                    {   
                        // cout << "flag pointer value " << flag[pointer] << endl;


                        //if 0, replace
                        if (flag[pointer] == 0)
                        {
                            // cout << "replaced " << frame[pointer] << " with " << v[i] << endl;

                            frame[pointer] = v[i];
                            numPageFaults += 1;
                            notReplaced = 0;
                            flag[pointer] = 1; 
                        }
                        //if 1, decrease to 0
                        else if (flag[pointer] == 1)
                        {
                            flag[pointer] = 0;
                        }


                        //move the pointer
                        // cout << "index: " << pointer << endl;
                        //pointer = incrementClockPointer(pointer, numFrames-1);
                        //pointer ++;
                        if (pointer < numFrames-1)
                        {
                            pointer++;
                        }
                        else
                        {
                            pointer = 0;
                        }


                        if (notReplaced = 0)
                        {
                            break;
                        }
                    }
                }


                


            }
        }


        cout << "current state after seeing " << v[i] << endl;

        
        for (int i = 0; i<numFrames; i++)
        {
            cout << frame[i] << endl;
        }


        count += 1;
    }


    string frameOutput;// = "4 5 2 3";
    //numPageFaults = 10;
    //get the state of frames at the end
    for (int i = 0; i<numFrames; i++)
    {
        frameOutput += to_string(frame[i]) + " ";
    }

    cout << "Clock:" << endl;
    cout << "  - frames: " << frameOutput << endl;
    cout << "  - page faults: " << numPageFaults << endl;
}

int main(int argc, char *argv[])
{
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

    //optimal(numFrames, v);
    //lru(numFrames, v);
    clock(numFrames, v);

	return 0;
}
