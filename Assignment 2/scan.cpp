#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <map>
#include <iostream>
#include <algorithm>

/*
 * This class will scan your file directory for files ending in
 * your speficied suffix. The file path and size of each file will 
 * be displayed, as well as the total size of all the result files.
 * The number of results is also specified by the user, and will be 
 * display in decending file size order.
 */

using namespace std;
#define MAX_FNAME_SIZE 512
#define MAX_FILES 1000

//this method check if a string ends with another string
bool endsWith(const std::string &mainStr, const std::string &toMatch)
{
	return mainStr.size() >= toMatch.size() && mainStr.compare(mainStr.size() - toMatch.size(), toMatch.size(), toMatch) == 0;
}

int main( int argc, char ** argv) 
{
	//first argument is the file suffix
	string suffix = argv[1];
	//second argument is the number of files displayed
	int num = stoi(argv[2]);
	//initialize variables  
	int fileSize[MAX_FILES];
	map<int, string> mymap; 
	long long totalSize = 0;
	struct stat st;
	int counter = 0;

	// open 'find...'
	FILE * fp = popen( "find . -type f", "r");
	//FILE * fp = popen( "./myFind", "r");
	if( fp == NULL) 
	{
		perror("popen failed:");
		exit(-1);
	}
	// read in all filenames
	char buff[MAX_FNAME_SIZE];
	int nFiles = 0;
	char * files[MAX_FILES];
	while(fgets(buff,MAX_FNAME_SIZE,fp)) 
	{
		int len = strlen(buff) - 1;
		files[nFiles] = strndup(buff,len);
		nFiles++;
	}
	fclose(fp);

	for(int i = 0 ; i < nFiles ; i ++ ) 
	{
		if(0 != stat(files[i],&st)) 
		{
			perror("stat failed:");
			exit(-1);
		}
		//arary that stores the file sizes
		fileSize[i] = stoi(to_string(st.st_size));
		//map each file to its file size
		mymap[stoi(to_string(st.st_size))] = files[i]; 
	}
	int SIZE = nFiles;
	//sort sizes 
	sort(fileSize, fileSize + SIZE);
	//reverse to get decending order
	reverse(fileSize, fileSize + SIZE);

    for (size_t i = 0; i != SIZE; i++)
	{
		//if file name ends with given suffix and less than num
		if(endsWith(mymap.at(fileSize[i]), suffix) && (counter < num))
		{
		  //display the file name and its size
		  cout << mymap.at(fileSize[i]);
		  cout << " ";
		  cout << fileSize[i] << endl;
		  //increment counter to keep track of how many files displayed thus far
		  counter += 1;
		  //update total size
		  totalSize += fileSize[i];
		}
	}
	//display total size
	string total = totalSize > 0 ? "Total size: " + to_string(totalSize) : "Total size: ";
	cout << total << endl;
  
	//clean up
	for(int i = 0; i < nFiles ; i ++ ) 
	{
		free(files[i]);
	}
	//return success
	return 0;
}
