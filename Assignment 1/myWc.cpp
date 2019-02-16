/**********************************************
 * Last Name:   Qiang
 * First Name:  Eddy
 * Student ID:  30058191
 * Course:      CPSC 457
 * Tutorial:    2
 * Assignment:  5
 * Question:    Q5
 *
 * File name: myWc.cpp
 *********************************************/

#include <unistd.h>
#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

int main (int argc, char * const argv[])
{
  // get the file name from command line
  string filename;
  if (argc != 2) 
  {
    cerr << "Usage: readFile <input file> " << endl;
    return -1;
  }
  else 
  {
    filename = argv[1];
  }
  
  // open the file for reading
  int fd = open( filename.c_str(), O_RDONLY);
  if( fd < 0) 
  {
    cerr << "Could not open file " << filename << "\n";
    exit(-1);
  }
 
  int count = 0;
  while(1)
  {
    int size = 1000;
    char c[size];
    if(read(fd, &c, size) < 1) break;

    int e = 0;
    int i = 0;
    while(e == 0 && i < size)
    {
	if(c[i] == '\n') //when you encounter a new line, add 1 to count
	{
		count += 1;
	}
	else if (c[i] == '\0') //when reach end update flag to leave loop
	{
		e = 1;
	}
	i += 1; //increment i
    }
  }

  // close file and report results
  close(fd);
  cout << count << " " << filename << "\n";
  return 0;
}
