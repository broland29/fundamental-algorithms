/**
 *
 * @author Bálint Roland
 * @group  30424
 *
 * Theme: Merge k Ordered Lists Efficiently (1/2)
 *
 * Instructions: For better visibility i split the task in two different
 * files. This one will contain the implementation and demos, with extra
 * comments. For the code generating the graphs, and for further explanations
 * regarding complexity, please check "homework_fa_cases"
 *
**/

#include <iostream>
#include <list>         //for using the list container class
#include <time.h>       //for generating random numbers


using namespace std;


///print one single list
void printList (list <int> a)
{
    list <int> :: iterator it;                  //we need an iterator
    for (it = a.begin(); it != a.end(); it++)   //traverse from beginning to end
        cout<<*it<<"\t";                        //print value, not address
    cout<<endl;

    return;
}


///fill list of length m with random elements in order
void createRandomList(list <int> &a, int m)
{
    for (int i=0; i<m; i++)     //repeat m times
        a.push_back(rand());    //add a random elements
    a.sort();                   //sort the list

    return;
}


/**Heapify function for a min heap of lists (array of integer lists)
    Modified maxHeapify from previous homework
    Idea:   - same as what we did with an array of numbers
            - we have a heap except root might not be in the right position
            - put the smallest element as root
**/
void minHeapify(list <int> arr[], int i, int n)
{
    int mini = i;           //suppose root is minimum
    int left = i * 2;       //left child of root
    int right = i * 2 + 1;  //right child of root

    if (left <= n && arr[left].front() < arr[i].front())        //if child exists and
        mini = left;

    if (right <= n && arr[right].front() < arr[mini].front())
        mini = right;

    if (mini != i)
    {
        swap(arr[i],arr[mini]);
        //arr[i].swap(arr[mini]);  i thought this was a source of error but no, regular swap works aswell
        minHeapify(arr,mini,n);
    }

    return;
}


///Builds a heap from array of lists, like in previous homework
void buildMinHeap(list <int> arr[], int n)
{
    for (int i=n/2; i>=1; i--)
        minHeapify(arr,i,n);    //use our new function

    return;
}


void printListArray(list <int> arr[], int listCount);   //needed for mergeLists


///The algorithm itself, using the previous functions - resembles heapsort
void mergeLists(list <int> arr[], int heapSize, list <int> &result)
{
    buildMinHeap(arr,heapSize);
    while (heapSize > 0)    //as long as there are elements in the array of lists
    {
        if (arr[1].size() > 1)  //if first list has more than one elements
        {
            result.push_back(arr[1].front());   //put first element (aka min) in result
            arr[1].pop_front();                 //pop the element since already processed
            printListArray(arr,heapSize);       //print, for debug purposes
            minHeapify(arr,1,heapSize);         //fix the heap
            printListArray(arr,heapSize);       //print, for debug purposes
        }
        else    //if first list has one element <=> it is gonna become empty
        {
            result.push_back(arr[1].front());   //put first element in result
            swap(arr[1],arr[heapSize]);         //swap first element with last element
            heapSize --;                        //decrease heap size <=> last element is "removed"
            printListArray(arr,heapSize);       //print, for debug purposes
            minHeapify(arr,1,heapSize);         //fix the heap
            printListArray(arr,heapSize);       //print, for debug purposes
        }
    }

    return;
}


///Creates an array of random lists
void createRandomArray(list <int> arr[], int listCount, int listLength)
{
    for (int i=1; i<=listCount; i++)
        createRandomList(arr[i],listLength);

    return;
}


///Prints an array of lists
void printListArray(list <int> arr[], int listCount)
{
    for (int i=1; i<=listCount; i++)
        printList(arr[i]);
    cout<<endl<<"------------------"<<endl;     //for aesthetic reasons

    return;
}

int main()
{
    srand (time(NULL));

    cout<<"Demo of createRandomList, using array one:\n";
    list <int> demoArrayOne[4];             //demo of array of 3 lists, but indexing from 1 so heapify works properly
    createRandomList(demoArrayOne[1],5);    //create the list on index 1, of length 5
    printList(demoArrayOne[1]);             //print it
    cout<<endl;

    cout<<"Demo of createRandomArray, using array two:\n";
    list <int> demoArrayTwo[4];
    createRandomArray(demoArrayTwo,3,5);
    printListArray(demoArrayTwo,3);
    cout<<endl;

    cout<<"Demo of buildMinHeap (includes minHeapify), using array two:\n";
    buildMinHeap(demoArrayTwo,3);           //build a heap out of the 3 lists based on their first index
    printListArray(demoArrayTwo,3);
    cout<<endl;

    cout<<"Demo of mergeLists (includes all of above), using a new array:\n";
    list <int> demoArrayThree[4];               //create
    createRandomArray(demoArrayThree,3,5);      //and
    printListArray(demoArrayThree,3);           //print
    list <int> demoResult;
    mergeLists(demoArrayThree,3,demoResult);    //actually use
    printList(demoResult);                      //and print result
    return 0;
}
