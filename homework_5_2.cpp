/**
 *
 * @author Bálint Roland
 * @group  30424
 *
 * Theme: Merge k Ordered Lists Efficiently (2/2)
 *
 * Instructions: This file serves the pure purpose of generating
 *  the tables and checking complexity. For detailed explanation
 *  of the functions used and demos, check "homework_fa_5_demos"
 *
 * Personal interpretation of complexity: As the problem statement suggested,
 *  this algorithm depends on two variables: n (the total number of elements
 *  or in other words, the number of integers, the length of result as well),
    respectively k (the number of lists, each having n/k length). By running
 *  the code and counting the operations for fixed k, then fixed n, we can
 *  get the complexity based on variable n, then k. The tables which can also
 *  be found in the excel file, look as follows:
 *
 *      k = 5			        k = 10			    k = 100	    	    n = 10000
 *      n 	    op		        n 	    op		    n 	    op		    k 	    op
 *      100     839		        100	    1224		100	    2907		10	    122095
 *      1000	8840		    1000	12476		1000	30123		70	    243592
 *      10000	86740		    10000	122525		10000	272854		500	    400206
 *
 * Using the tricks of Complexity Guide, we can see that for fixed k-s, we get
 *  O(n), since op/n is close to each other for variable n. For fixed n, i did
 *  not have the same testing dimension, max value of k was 500, so i tried to
 *  find a value between 10 and 500 (lets denote x), that satisfies the condition
 *  x/10 = 500/x, just like 1000/100 = 10000/1000. I hope this is a correct analogy.
 *  The data does not really speak for itself, but from the graph it is obvious that
 *  we have O(log k). Since the algorithm depends both on k and n, to get the "total"
 *  big oh, we have to multiply these two values.
 *
 * So, final result : O(n log k) - just as asked.
 *
**/

#include <iostream>
#include <list>
#include <time.h>

#include <fstream>
#include <cstdlib>

int opCount;

using namespace std;


///Start of old code from demo
void createRandomList(list <int> &a, int m)
{
    for (int i=0; i<m; i++)         //i do not count the operations needed
        a.push_back(rand());        //to create the lists themselves, since
    a.sort();                       //these are considered already given

    return;
}

void minHeapify(list <int> arr[], int i, int n)
{
    int mini = i;
    int left = i * 2;
    int right = i * 2 + 1;

    if (left <= n && arr[left].front() < arr[i].front())
        mini = left;
    if (left <=n)
        opCount++;      //comparison

    if (right <= n && arr[right].front() < arr[mini].front())
        mini = right;
    if (right <=n)
        opCount++;      //comparison

    if (mini != i)
    {
        swap(arr[i],arr[mini]);
        opCount += 3;   //swap
        minHeapify(arr,mini,n);
    }

    return;
}

void buildMinHeap(list <int> arr[], int n)
{
    for (int i=n/2; i>=1; i--)
        minHeapify(arr,i,n);

    return;
}

void mergeLists(list <int> arr[], int heapSize, list <int> &result)
{
    buildMinHeap(arr,heapSize);
    while (heapSize > 0)
    {
        if (arr[1].size() > 1)
        {
            result.push_back(arr[1].front());
            opCount++;              //push
            arr[1].pop_front();
            opCount++;              //pop
            minHeapify(arr,1,heapSize);
        }
        else
        {
            result.push_back(arr[1].front());
            opCount++;                      //push
            swap(arr[1],arr[heapSize]);
            opCount+=3;                     //swap
            heapSize --;
            minHeapify(arr,1,heapSize);
        }
    }

    return;
}

void createRandomArray(list <int> arr[], int listCount, int listLength)
{
    for (int i=1; i<=listCount; i++)
        createRandomList(arr[i],listLength);

    return;
}
///End of old code from demo


///Generates the evaluation for a fixed k
void generateCaseK (int k, ofstream& f)
{
    list <int> listsToMerge[101];   //max value of k is 100, i index from 1
    list <int> result;              //to store merged result

    int c;  //list count, number of lists

    f<<"Analysis for k = "<<k<<endl;
    f<<" n , operations"<<endl;
    for (int n=100; n<=10000; n+=100)
    {
        opCount = 0;
        c = n / k;

        createRandomArray(listsToMerge,k,c);    //create an array of k lists of length c
        mergeLists(listsToMerge,k,result);      //merge the k lists (elements of listsToMerge) into result

        f<<n<<" , "<<opCount<<endl;
    }
    return;
}


///Generates the evaluation for a fixed n
void generateCaseN (int n, ofstream& f)
{
    list <int> listsToMerge[501];   //max value of k is 500, i index from 1
    list <int> result;              //to store merged result

    int c;  //list count, number of lists

    f<<"Analysis for n = "<<n<<endl;
    f<<" k , operations"<<endl;
    for (int k=10; k<=500; k+=10)
    {
        opCount = 0;
        c = n / k;

        createRandomArray(listsToMerge,k,c);    //create an array of k lists of length c
        mergeLists(listsToMerge,k,result);      //merge the k lists (elements of listsToMerge) into result

        f<<k<<" , "<<opCount<<endl;
    }
    return;
}

int main()
{
    srand (time(NULL));

    ofstream f("merge_lists.csv");

    generateCaseK(5,f);
    f<<endl;
    generateCaseK(10,f);
    f<<endl;
    generateCaseK(100,f);
    f<<endl;

    generateCaseN(10000,f);

    f.close();
    return 0;
}
