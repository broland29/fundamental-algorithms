/**
 *
 * @author Bálint Roland
 * @group  30424
 *
 * Themes: Comparison of Quick Sort and Heap Sort.
 *         Comparison of best and worst case of Quick sort
 *
 *
 * Instructions: This is the second part of the lab task. If you did not
 * do already, you should check "homework_fa_4" for demos of Quick Sort,
 * Quick Select. For better visibility, I deleted the explanations from
 * codes that were written and used previously
 *
 *
 * Personal interpretation of complexity:
 *
 * In the average case, as proved by rigorous mathematical equations which i do not really
 * understand, Quick Sort has a complexity of n*log(n). As for me, i can check this from the
 * outputs i got by running the code and counting the operations. I can do the same for Heap Sort
 *          array_size_n      op_quick        op_heap
 *              100           	2298	        2755
 *              1000            32096	        43867
 *              10000           466601	        605416
 * If we look at the graphs, we can find out that it is either linear or n log n. By comparing the
 * operation numbers at n = 100, 1000, 10000, we can see that the difference between the values
 * is almost the same (roughly 10 for Quick Sort and 20 for Heap Sort). So we can conclude that
 * both sorting algorithms have a complexity of n log n in average case.
 *
 *
 * The best and worst case of Quick Sort entirely depends on the implementation, more exactly
 * on the way we choose the pivot. The best case occurs when each pivot divides its array in
 * two partitions of the same length. In this case, we get the complexity of O(n * log n),
 * similarly to other DEI algorithms (we get to do n operations log n times).
 *
 * We may find ourselves in the worst case when after partitioning we end up with an array of
 * 1 size and an array or n-1 size. We have to do the operation n times, leaving us with a
 * complexity of O(n^2).
 *
 * Looking at our partitioning function, we can see that the pivot is always the rightmost
 * element of the array.
 *
**/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <time.h>

using namespace std;

int opQuick;    //number of operations for Quick Sort
int opHeap;     //number of operations for Heap Sort

int opBest;     //number of operations for best case, for Quick Sort
int opWorst;    //number of operations for worst case, for Quick Sort

///Start of Quick Sort code
int partitionBook(int A[], int p, int r)
{
    int x = A[r];
    opQuick ++;                                 //assignment
    int i = p-1;
    for (int j=p; j<r; j++)
    {
        opQuick ++;                             //comparison
        if (A[j] <= x)
        {
            i++;
            swap(A[i],A[j]);
            opQuick += 3;                       //swap
        }
    }
    swap(A[i+1],A[r]);
    opQuick += 3;                               //swap
    return i+1;
}

void quickSort (int arr[], int p, int r)
{
    if (p < r)
    {
        int q = partitionBook(arr,p,r);
        quickSort(arr,p,q-1);
        quickSort(arr,q+1,r);
    }

    return;
}
///End of Quick Sort code


///Start of Heap Sort code
void maxHeapify(int arr[], int i, int n)
{
    int maxi = i;
    int left = i * 2;
    int right = i * 2 + 1;

    if (left <= n && arr[left] > arr[right])
        maxi = left;
    if (left <= n) opHeap ++;                       //comparison

    if (right <= n && arr[right] > arr[maxi])
        maxi = right;
    if (right <=n) opHeap ++;                       //comparison

    if (maxi != i)
    {
        swap(arr[i],arr[maxi]);
        opHeap += 3;                                //swap

        maxHeapify(arr,maxi,n);
    }
    return;
}

void buildMaxHeap(int arr[], int n)
{
    for (int i=n/2; i>=1; i--)
        maxHeapify(arr,i,n);

    return;
}

void heapSort(int arr[], int n)
{
    buildMaxHeap(arr,n);

    while (n > 1)
    {
        swap(arr[1],arr[n]);
        opHeap+=3;                                  //swap
        n--;
        maxHeapify(arr,1,n);
    }

    return;
}
///End of Heap Sort code


///Start of Auxiliary Functions
void copyArray (int dest[], int src[], int n)
{
    for (int i=0; i<n; i++)
        dest[i] = src[i];

    return;
}
void printArray (int arr[], int n)
{
    for (int i=0; i<n; i++)
        cout<<arr[i]<<" ";
    cout<<endl<<endl;

    return;
}

void sortedArray(int arr[], int n)
{
    for (int i=0; i<n; i++)
        arr[i] = i;

    return;
}
///End of Auxiliary Functions



/** A function to simulate the best case scenario when using the previous partitioning function. This one
    feels like cheating but after long thinking i still could not come up with better solution. My idea
    was to build up this kind of array, but this function is a shortcut for that. Works if the array is
    already in increasing order, so for simplicity, it will be 0, 1, 2 ... n-1. It will always put the middle
    element at the last index, and since the middle element is also the middle element of sorted array (it is
    sorted), it will perfectly divide the array in two.
    Implementation inspired from my friend Varga Zoltán
*/
int partitionCheat (int arr[], int i, int j)
{
    swap(arr[(i+j)/2],arr[j]);      //puts middle element to last position to simulate best case
    return partitionBook(arr,i,j);  //now uses the "regular" partitioning function
}

///Quick Sort best case using the previous partitioning function
void generateBestCase (int arr[], int p, int r)
{
    if (p < r)
    {
        int q = partitionCheat(arr,p,r);
        generateBestCase(arr,p,q-1);
        generateBestCase(arr,q+1,r);
    }

    return;
}

/** As stated before, the worst case is when we get very uneven partitions, like 1 element on one side
    and n-1 on the other. This can easily be achieved by feeding an already sorted array to our algorithm.
    Since the pivot in my implementation is always the last item, it will have a very hard time finishing
    the sorting. I will also reuse the sortedArray function.
*/
void generateWorstCase (int arr[], int n)
{
    sortedArray(arr,n);
    quickSort(arr,0,n-1);
    return;
}


int main()
{
    ///*****analysis of general case*****//
    int randomArray[10000];
    int randomCopy[10000];

    ofstream fOutTable("comparison_qs_hs.csv");
    fOutTable<<"n , Quick Sort , Heap Sort"<<endl;

    srand(time(NULL));

    int i, j;
    for (int n=100; n<=10000; n+=100)
    {
        opQuick = 0;
        opHeap = 0;

        for (i=0; i<5; i++)
        {
            for (j=0; j<n; j++)
                randomArray[j] = rand();
            copyArray(randomCopy,randomArray,n);

            quickSort(randomArray,0,n);
            heapSort(randomCopy,n);
        }

        opQuick /= 5;
        opHeap /= 5;

        fOutTable<<n<<" , "<<opQuick<<" , "<<opHeap<<endl;
        cout<<"..."<<endl;
    }

    fOutTable.close();


    ///***analysis of best and worst case***///
    ofstream fOutQuick("quicksort_best_worst.csv");

    fOutQuick<<"n , Best Case , Worst Case"<<endl;
    int testArray[10000];

    for (int n=100; n<=10000; n+=100)
    {
        opBest = 0;
        opWorst = 0;

        for (i=0; i<5; i++)
        {
            opQuick = 0;
            sortedArray(testArray,n);
            generateBestCase(testArray,0,n-1);
            opBest += opQuick;

            opQuick = 0;
            generateWorstCase(testArray,n);
            opWorst += opQuick;
        }

        opBest /= 5;
        opWorst /= 5;

        fOutQuick<<n<<" , "<<opBest<<" , "<<opWorst<<endl;
        cout<<"..."<<endl;
    }

    fOutQuick.close();
    return 0;
}
