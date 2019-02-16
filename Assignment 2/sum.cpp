#include <iostream> 
#include <pthread.h> 
#include <iomanip>
#include <fstream>
#include <vector>
  
/*
 * This program is a multi-threaded solution for computing the sum of a
 * array of integers. You will have to speficy two arguments. The first 
 * argument is the name of the file and the second is the number of threads.
 */

using namespace std; 

int total = 0;
int threadNumber = 1;
int n = 0;
int x;
vector<int> arr;

/*
 * This method will sum of the numbers in the array
 */
void* sum_array(void* arg) 
{
	int* a = (int*)arg;
	int subtotal = 0;
	//starting and ending index
	int start = a[0];
	int end = a[1];
	//update total and subtotal
	for(int i = start; i < end; i++)
	{
		total += arr[i];
		subtotal += arr[i];
	}
	//display subtotal
	cout << "Thread " + to_string(threadNumber) + ": ";
	threadNumber += 1;
	cout << subtotal << endl;
	return NULL;
} 

int main( int argc, char ** argv) 
{ 
	//first argument is the file suffix
	string fileName = argv[1];
	//second argument is the number of files displayed
	int t = stoi(argv[2]);
	//open file
	ifstream inFile;
    inFile.open("text.txt");
	//error checking
    if (!inFile) 
	{
        cout << "File Error";
        exit(1); 
    }
    //read each integer to vector
    while (inFile >> x) 
	{
        n += 1;
		arr.push_back(x);
    }
	//close file
    inFile.close();
	//calculations for the number of threads in each group
	int numFirstgroup = n % t;
	int numSecGroup = t - (n%t);
	//calculations for the number of elements added from each thread
	int numElementsFirstGroup = n / t + 1;
	int numElementsSecondGroup = n/t;
	//initial starting and ending array index
	int start = 0;
	int end = start + numElementsFirstGroup;
	//this array will contain the specified starting and ending indices
	int k[2];
	//create, join, and run each of the threads in the first group
	for (int i = 0; i < numFirstgroup; i++) 
	{
		k[0] = start;
		k[1] = end;
		pthread_t mythread;
		pthread_create(&mythread, NULL, sum_array, &k); 
		pthread_join(mythread, NULL);
		start += numElementsFirstGroup;
		end = start + numElementsFirstGroup;
	}
	//update indices for second group
	int start2 = numFirstgroup * numElementsFirstGroup;
	int end2 = start2 + numElementsSecondGroup;
	//create, join, and run each of the threads in the second group
	for (int i = 0; i < numSecGroup; i++) 
	{
		k[0] = start2;
		k[1] = end2;
		pthread_t mythread;
		pthread_create(&mythread, NULL, sum_array, &k); 
		pthread_join(mythread, NULL);
		start2 += numElementsSecondGroup;
		end2 = start2 + numElementsSecondGroup;
	}
	//display the total sum
	cout <<"Sum = ";
	cout << total << endl;
  
    return 0; 
} 
