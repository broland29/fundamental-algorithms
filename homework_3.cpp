/**
 *
 * @author Bálint Roland
 * @group  30424
 *
 * Theme: Implementation and analysis of heap-building
 *        functions, in bottom-up and top-down fashion
 *
 * Personal interpretation of complexity:
 *  - By running the code, i got these values in the excel file:
 *      array_size_n      op_nb_bottom_up    op_nb_top_down
 *          100                  421             449
 *          1000                4374            5072
 *          10000               44225           50992
 *  - One can see that if we divide the op numbers by n, we get close values
 *  - So, from this, and from the graph i could deduce that both have O(n)
 *
 * Instructions:
 *  - Run code and scroll up for demos of Heap Sort and heap-building functions
 *  - Check output file "comparison_td_bu.csv" for algorithm complexity evaluation
 *  - For easier checking, i made a drawing of the outputs of the last two in form of a tree
 *
 * Attempt for extra credit:
 *  - Worst case for both algorithms, in my opinion, is when we
 *     need a max heap as output, but we get a min heap as input
 *  - In this case, every input breaks our heap in such a way, that
 *     we need the most amount of operations possible to fix it
 *  - For simplicity, lets assume that
 *          - we get a min heap with all leaves on the same level
 *          - N is the highest level (levels go from 0 to N)
 *  - Bottom-up:
 *          - we skip level n, as leaves by themselves are already heaps
 *          - on all the other levels we need to compare and swap
 *          - we also need to fix after swap
 *          - on level n-1 we need 2^(n-1) swaps (number of leaves on level n-1)
 *          - on level n-2 we need 2 * 2^(n-2) swaps
 *          - on level n-3 we need 3 * 2^(n-3) swaps
 *          - on level 0 we need 0 swaps
 *          - in total, we have sum of (n-i+1) * 2^i, where i goes from 1 to n
 *  - Top-down:
 *          - we start with one node, which is apparently the minimum
 *          - each time we add an element, it has to bubble all the way up
 *          - on level 1 we need 2 swaps
 *          - on level 2 we need 4*2 swaps
 *          - on level n we need (2^n)*n swaps
 *          - in total, we have sum of (2^i)*i where i goes from 1 to n
 *  - If we look at both formulas (which i hope are correct), we see that they are basically the same
 *  - So we can say that in the worst case, both algorithms perform the same
**/

#include <iostream> //for output of demos
#include <fstream>  //for file management
#include <cstdlib>  //for excel file
#include <time.h>   //for random values

using namespace std;

int opBottomUp;     //number of operations for building heap bottom-up style
int opTopDown;      //same, but for top-down

/** Used when building a max heap in bottom-up style
     - Takes a node i and its children
     - Puts biggest out of these three as parent
    Parameters:
     - arr  :   array to build a heap out of
     - i    :   index of (original) parent
     - n    :   total number of elements (to check if node exists)
    Note: this only works in one particular case,
     if removing the root would result in 2 heaps
**/
void maxHeapify(int arr[], int i, int n)
{
    int maxi = i;           //initialize maximum element as parent
    int left = i * 2;       //index of left child
    int right = i * 2 + 1;  //index of right child

    //check if node exists and if it is bigger than original parent
    if (left <= n && arr[left] > arr[right])
        maxi = left;
    if (left <= n) opBottomUp ++;  //comparison

    //check if node exists and if it is bigger than previous max
    if (right <= n && arr[right] > arr[maxi])
        maxi = right;
    if (left <=n) opBottomUp ++;  //comparison

    if (maxi != i)
    {
        swap(arr[i],arr[maxi]);
        opBottomUp += 3;    //swapping
        //since the swap may mess up the heaps below, we must apply the function recursively
        maxHeapify(arr,maxi,n);     //the "potential problem" is now at index maxi
    }
    return;
}


/// Build a max heap bottom-up style using the previous function
void buildMaxHeap(int arr[], int n)
{
    for (int i=n/2; i>=1; i--)  //we start from n/2 in order to skip nodes- they are already heaps
        maxHeapify(arr,i,n);    //we stop at 1 since we start indexing there

    return;
}


/// Print and array arr of length n
void printArray(int arr[], int n)
{
    for (int i=1; i<=n; i++)    //since we started indexing from 1
        cout<<arr[i]<<" ";
    cout<<endl<<endl;

    return;
}


/// Heap sorting algorithm on an array arr of length n
void heapSort(int arr[], int n)
{
    buildMaxHeap(arr,n);    //make whole array a heap

    while (n > 1)           //while we have more than one element
    {
        swap(arr[1],arr[n]);    //since array is heap, put first element (maximum) at end
        n--;                    //decrease heap size
        maxHeapify(arr,1,n);    //"fix" the remaining array in order to have a heap again
    }

    return;
}


/** Build a heap from an array, in top-down fashion
     - Takes the elements one by one, in their original order
     - By moving i to the right, we add element to the bottom of heap
     - We bubble up the element to its right place (if needed)
     - What we have on the left of i will become a heap
     - By reaching i = n and fixing, we get a heap containing all elements
**/
void buildMaxHeapTD(int arr[], int n)
{
    for (int i=2; i<=n; i++)    //we add all elements; start from 2 since one element by itself is heap
    {
        int j = i;          //to not ruin i
        int p = i / 2;      //index of newly added node's parent

        //fixing the heap by bubbling up new element, if needed
        while (j > 1 && arr[j] > arr[p])    //until we reach the root or find the right place
        {
            opTopDown++;   //comparison if j>1
            swap (arr[j],arr[p]);
            opTopDown += 3; //swapping
            j = p;                  //continue process by updating variables
            p = p / 2;              //parent becomes new son
        }
    }

    return;
}


/// copy source array into destination array, both of length n
void copyArray(int dest[], int src[], int n)
{
    dest[0] = 0;    //not necessarily needed
    for (int i=1; i<=n; i++)    //indexing from 1
        dest[i] = src[i];

    return;
}

int main()
{
    ///demo of heap sort
    cout<<"Demo of heapSort:"<<endl;
    int demoArray[] = {0, 17, 13, 22, 28, 11, 83, 5, 30, 31};     //hard-coded example; we actually do not use first index
    int demoCopy[10];
    copyArray(demoCopy,demoArray,9);

    heapSort(demoCopy,9);    //number of elements is 9, we ignore the first element
    printArray(demoCopy,9);


    ///demo of building heap bottom-up
    cout<<"Demo of buildMaxHeap:"<<endl;
    copyArray(demoCopy,demoArray,9);
    buildMaxHeap(demoCopy,9);
    printArray(demoCopy,9);


    ///demo of building heap top-down
    cout<<"Demo of buildMaxHeapTD:"<<endl;
    copyArray(demoCopy,demoArray,9);
    buildMaxHeapTD(demoCopy,9);
    printArray(demoCopy,9);


    ///analysis of functions
    int randomArray[10000];
    int randomCopy[10000];

    ofstream fOutTable("comparison_td_bu.csv");
    fOutTable<<"n , bottom-up , top-down"<<endl;

    srand(time(NULL));

    int i, j;
    for (int n=100; n<=10000; n+=100)   //counting operations for arrays of length n
    {
        opBottomUp = 0;
        opTopDown = 0;

        for (i=0; i<5; i++)     //repeat 5 times and make average, for more precise result
        {
            for (j=1; j<=n; j++)    //generate random array, indexing from 1!
                randomArray[j] = rand();
            copyArray(randomCopy,randomArray,n);  //put it away for top-down

            buildMaxHeap(randomArray,n);
            buildMaxHeapTD(randomCopy,n);
        }

        opBottomUp /= 5;
        opTopDown /= 5;

        fOutTable<<n<<" , "<<opBottomUp<<" , "<<opTopDown<<endl;
        cout<<"..."<<endl;
    }

    fOutTable.close();
    return 0;
}
