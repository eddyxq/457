// This class is a multithreaded implementation of counting prime numbers.

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <pthread.h>
#include <iostream> 
#include <vector>

using namespace std;

int arr_index;
int64_t count = 0;
int n;
int k[3];
vector<int> vec;
pthread_mutex_t index_mutex;
pthread_mutex_t count_mutex;
long long * arr;

// method that returns true if a number is prime
int isPrime(int64_t n, int inc, int num, int numt)
{
    if( n <= 1) return 0; // small numbers are not primes
    if( n <= 3) return 1; // 2 and 3 are prime
    if( n % 2 == 0 || n % 3 == 0) return 0; // multiples of 2 and 3 are not primes
    int64_t i = inc;
    int64_t max = sqrt(n);
    while((i <= max)) 
    {
        if (n % i == 0 || n % (i+2) == 0)
        {
            return 0;
        } 
        // separate logic for extreamly large numbers
        if (n > 1000000000000000000)
        {
            i += 6*numt;
        }
        else
        {
            i += 6;
        }
    }
    return 1;
}

// method that is passed to the thread
void* countPrimes(void* arg)
{
    int* a = (int*)arg;
    int i;
    while(1) 
    {
        // lock down critical section
        pthread_mutex_lock(&index_mutex);
        i = arr_index++;
        pthread_mutex_unlock(&index_mutex);
        if(i >= n) break;
        long long num = arr[i];
        int is_prime = isPrime(num, a[0], a[1], a[2]);
        // mark the vector slot corresponding to the number
        if (is_prime) 
        {
            vec[i] += 1;
        }
    }
    return NULL;
}

// starting main method
int main( int argc, char ** argv)
{
    // parse command line arguments
    int nThreads = 1;
    if( argc != 1 && argc != 2) 
    {
        printf("Uasge: countPrimes [nThreads]\n");
        exit(-1);
    }
    if( argc == 2) nThreads = atoi( argv[1]);

    // print error message if invalid number of arguments are given
    if( nThreads < 1 || nThreads > 256) 
    {
        printf("Bad arguments. 1 <= nThreads <= 256!\n");
    }

    //10000 is the max amount of numbers this program will hold
    arr = new long long[10000];

    // display counting primes message
    printf("Counting primes using %d thread%s.\n", nThreads, nThreads == 1 ? "" : "s");

    n = 0;
    // read in all the numbers from text file
    while(1) 
    {
        int64_t num;
        if( 1 != scanf("%lld", &arr[n])) break;
        n++;
    }
    // initialize vector to all zeros
    for(int i = 0; i < n; i++)
    {
        vec.push_back(0);
    }
    // initialize threads
    pthread_t threads[nThreads];
    pthread_mutex_init(&index_mutex, NULL);
    pthread_mutex_init(&count_mutex, NULL);
    // create threads
    for(int i = 0; i < nThreads; i++)
    {
        // get numbers to pass into thread
        int inc = 5 + (i * 6);
        int num = arr[i];
        int numThreads = nThreads;
        k[0] = inc;
        k[1] = num;
        k[2] = numThreads;
        pthread_create(&threads[i], NULL, countPrimes, &k);
    }
    // join the threads
    for(int i = 0; i < nThreads; i++)
    {
        pthread_join(threads[i], NULL);
    }
    // check if number is prime
    for(int i = 0; i < n; i++)
    {
        if (vec[i] > 0)
        {
            count++;
        }
    }
    // print results
    printf("Found %lld primes.\n", count);

    return 0;
}
