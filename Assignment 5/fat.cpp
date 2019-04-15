// CPSC457 University of Calgary
// Skeleton C++ program for Q7 of Assignment 5.
//
// The program reads in the input, then calls the (wrongly implemented) checkConsistency()
// function, and finally formats the output.
//
// You only need to reimplement the checkConsistency() function.
//
// Author: Pavol Federl (pfederl@ucalgary.ca or federl@gmail.com)
// Date: April 1, 2019
// Version: 5

#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <math.h>
#include <algorithm>
#include <set>

using namespace std;

typedef std::string SS;
typedef std::vector<SS> VS;

struct DEntry {
    SS fname = SS( 4096, 0);
    int size = 0;
    int ind = 0;
    bool tooManyBlocks = true;
    bool tooFewBlocks = false;
    bool hasCycle = true;
    bool sharesBlocks = true;

    set<int> traversedblocks;

};

static SS join( const VS & toks, const SS & sep) {
    SS res;
    bool first = true;
    for( auto & t : toks) { res += (first ? "" : sep) + t; first = false;}
    return res;
}


int blocksIntersect(set<int> s1, set<int> s2)
{
    set<int> intersect;
    set_intersection(s1.begin(),s1.end(),s2.begin(),s2.end(), inserter(intersect,intersect.begin()));

    return intersect.size() > 0 ? 1 : 0;
}

// re-implement this function
//
// Parameters:
//   blockSize - contains block size as read in from input
//   files - array containing the entries as read in from input
//   fat - array representing the FAT as read in from input
// Return value:
//   the function should return the number of free blocks
//   also, for ever entry in the files[] array, you need to set the appropriate flags:
//      i.e. tooManyBlocks, tooFewBlocks, hasCycle and sharesBlocks
int checkConsistency( int blockSize, std::vector<DEntry> &files, std::vector<int> &fat)
{
    set<int> blockUsed;

    //for each file
    for (int i = 0; i < files.size(); i++)
    //for (int i = 6; i < 7; i++)
    {
		
		cout << "file size: " << files[i].size << endl;
		
        //set everything to false to start
        files[i].hasCycle = false;
        files[i].tooFewBlocks = false;
        files[i].tooManyBlocks = false;
        files[i].sharesBlocks = false;


        //calc number of blocks
        int numBlocks = ceil(float(files[i].size) / float(blockSize));

        //calc how many blocks used in table
        int current = files[i].ind;
        set<int> set; 
		
		int setSize = 0;


        int notEmpty = 1;
        //check if file is empty
        if(files[i].ind == -1)
        {
            notEmpty = 0;
            if (files[i].size == 0)
            {
                //good no problem
            }
            else if (files[i].size > 0)
            {
                //not enough blocks
                files[i].tooFewBlocks = true;
            }

        }

        while(notEmpty)
        {

            //if it points to -1 break
            if (fat[current] == -1)
            {
                set.insert(current);
				setSize++;
                files[i].traversedblocks.insert(current);
                blockUsed.insert(current);

                //cout << "BROKEN OUT OF LOOP HERE" << endl;
                break;
            }
            else 
            {
                //check if current index is in the set
                if (find(set.begin(), set.end(), fat[current]) != set.end() || current == fat[current])
                {
                    //if already in set, means there is cycle, so break
                    files[i].hasCycle = true;
                    set.insert(current);
					setSize++;
                    files[i].traversedblocks.insert(current);
                    blockUsed.insert(current);
                    break;
                }
                //if not in set, then add it into set to keep track of if there is a cycle
                else
                {
                    files[i].traversedblocks.insert(current);
                    set.insert(current);
					setSize++;
                    blockUsed.insert(current);
                }
                //change pointer
                current = fat[current];


            }
        }


        //cout <<" this set has :" << endl;
        //for (auto it=set.begin(); it != set.end(); ++it) 
        //cout << ' ' << *it; 
		
		cout << "size of set: " << set.size() << endl;
		cout << "new set num: " << setSize << endl;

        //check if too many blocks or too few blocks
        int min = (set.size()-1) * blockSize + 1;
        int max = (set.size() * blockSize);

        cout << "min " << min << endl;
        cout << "max " << max << endl;


        if (files[i].ind != -1 && files[i].size < min)
        {
            files[i].tooManyBlocks = true;
        }
        else if (files[i].ind != -1 &&files[i].size > max)
        {
            files[i].tooFewBlocks = true;
        }
    }

    //now check if any of the sets intersect
    //for each file
    for (int i = 0; i < files.size(); i++)
    {
        //if any of the files intersect with another file break
        for (int j = 0; j < files.size(); j++)
        {
            if ((i != j) && blocksIntersect(files[i].traversedblocks, files[j].traversedblocks))
            {
                files[i].sharesBlocks = true;
                break;
            }
        }
    }

    // finally, return the number of free blocks
    return fat.size() - blockUsed.size();
    //return 0;
}

int main()
{
    try {
        // read in blockSize, nFiles, fatSize
        int blockSize, nFiles, fatSize;
        if( 3 != scanf( "%d %d %d", & blockSize, & nFiles, & fatSize))
            throw "cannot read blockSize, nFiles and fatSize";
        if( blockSize < 1 || blockSize > 1024) throw "bad block size";
        if( nFiles < 0 || nFiles > 50) throw "bad number of files";
        if( fatSize < 1 || fatSize > 200000) throw "bad FAT size";
        // read in the entries
        vector<DEntry> entries;
        for( int i = 0 ; i < nFiles ; i ++ ) {
            DEntry e;
            if( 3 != scanf( "%s %d %d", (char *) e.fname.c_str(), & e.ind, & e.size))
                throw "bad file entry";
            e.fname = e.fname.c_str();
            if( e.fname.size() < 1 || e.fname.size() > 16)
                throw "bad filename in file entry";
            if( e.ind < -1 || e.ind >= fatSize) throw "bad first block in fille entry";
            if( e.size < 0 || e.size > 1073741824) throw "bad file size in file entry";
            entries.push_back( e);
        }
        // read in the FAT
        vector<int> fat( fatSize);
        for( int i = 0 ; i < fatSize ; i ++ ) {
            if( 1 != scanf( "%d", & fat[i])) throw "could not read FAT entry";
            if( fat[i] < -1 || fat[i] >= fatSize) throw "bad FAT entry";
        }

        // run the consistency check
        int nFreeBlocks = checkConsistency( blockSize, entries, fat);

        // format the output
        size_t maxflen = 0;
        for( auto & e : entries ) maxflen = max( maxflen, e.fname.size());
        SS fmt = "  %" + to_string( maxflen) + "s: %s\n";

        printf( "Issues with files:\n");
        for( auto & e : entries ) {
            VS issues;
            if( e.tooFewBlocks) issues.push_back( "not enough blocks");
            if( e.tooManyBlocks) issues.push_back( "too many blocks");
            if( e.hasCycle) issues.push_back( "contains cycle");
            if( e.sharesBlocks) issues.push_back( "shares blocks");
            printf( fmt.c_str(), e.fname.c_str(), join( issues, ", ").c_str());
        }
        printf( "Number of free blocks: %d\n", nFreeBlocks);
    }
    catch( const char * err) {
        fprintf( stderr, "Error: %s\n", err);
    }
    catch( ... ) {
        fprintf( stderr, "Errro: unknown.\n");
    }
    return 0;
}
