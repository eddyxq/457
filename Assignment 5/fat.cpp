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

struct DEntry 
{
    SS fname = SS( 4096, 0);
    int size = 0;
    int ind = 0;
    bool tooManyBlocks = true;
    bool tooFewBlocks = false;
    bool hasCycle = true;
    bool sharesBlocks = true;
    set<int> traversedblocks;
};

static SS join( const VS & toks, const SS & sep) 
{
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

int checkConsistency( int blockSize, std::vector<DEntry> &files, std::vector<int> &fat)
{
	//keeps track of the blocks used from all the files
    set<int> blockUsed;
    //for each file
    for (int i = 0; i < files.size(); i++)
    {
        //set everything to false to start
        files[i].hasCycle = false;
        files[i].tooFewBlocks = false;
        files[i].tooManyBlocks = false;
        files[i].sharesBlocks = false;

        //calc number of blocks
        int numBlocks = ceil(float(files[i].size) / float(blockSize));

        //calc how many blocks used in table
        int current = files[i].ind;
		
		//initialize array to -1, used to keep track if a frame has been used
		int arr [200000];
		for (int i = 0; i <= 200000; i++) 
		{
			arr[i] = -1;
		}
		
		//count the number of blocks a file uses
		int blockCount = 0;

		//flag
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
		//when the file is not empty
        while(notEmpty)
        {
            //if it points to -1 break because you are at end of file
            if (fat[current] == -1)
            {
				//increment count and mark block as used
				blockCount++;
				arr[current] = 1;
                files[i].traversedblocks.insert(current);
                blockUsed.insert(current);
                break;
            }
            else 
            {
                //check if current index is in the set
                if (arr[fat[current]] == 1|| current == fat[current])
                {
                    //if already in set, means there is cycle, so break
                    files[i].hasCycle = true;
					blockCount++;
					arr[current] = 1;
                    files[i].traversedblocks.insert(current);
                    blockUsed.insert(current);
                    break;
                }
                //if not in set, then add it into set to keep track of if there is a cycle
                else
                {
                    files[i].traversedblocks.insert(current);
					blockCount++;
					arr[current] = 1;
                    blockUsed.insert(current);
                }
                //change pointer
                current = fat[current];
            }
        }

        //check if too many blocks or too few blocks
        int min = (blockCount-1) * blockSize + 1;
        int max = (blockCount * blockSize);

        if (files[i].ind != -1 && files[i].size < min)
        {
            files[i].tooManyBlocks = true;
        }
        else if (files[i].ind != -1 &&files[i].size > max)
        {
            files[i].tooFewBlocks = true;
        }
    }

    //for each file
    for (int i = 0; i < files.size(); i++)
    {
        //checking if any of the sets of blocks intersect
        for (int j = 0; j < files.size(); j++)
        {
            if ((i != j) && blocksIntersect(files[i].traversedblocks, files[j].traversedblocks))
            {
                files[i].sharesBlocks = true;
                break;
            }
        }
    }

    // finally return the number of free blocks
    return fat.size() - blockUsed.size();
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
