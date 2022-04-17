/**
 * @author Bálint Roland
 * @group 30424
 *
 * Comparison of direct sorting methods
 *
 *                    Best Case    Average Case    Worst Case
 *   Bubble Sort       O(n)          O(n^2)          O(n^2)
 *   Selection Sort    O(n^2)        O(n^2)          O(n^2)
 *   Insertion Sort    O(n)          O(n^2)          O(n^2)
 *
 * Personal choice: I prefer Bubble Sort since i am more familiar
 *    with this algorithm, i use it all the time and i learned it
 *    a long time ago
 */

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <time.h>

using namespace std;

int asg, com;

///bubble sort algorithm
void bubbleSort (int n, int A[])
{
    asg = 0;
    com = 0;

    bool swapMade = 1;
    int i;      //so that we initialize only once
    n--;        //so that it does not evaluate a lot of times in the header of for loop
    while (swapMade)
    {
        swapMade = 0;
        for (i = 0; i < n; i++)
        {
            com ++;
            if (A[i] > A[i+1])
            {
                asg += 3;           //1 swap is basically equal to 3 assignments
                swap(A[i],A[i+1]);
                swapMade = 1;
            }
        }
        n--;
    }
    return;
}

///insertion sort algorithm
void insertionSort (int n, int A[])
{
    asg = 0;
    com = 0;

    int i, j, aux;
    for (i=1; i<n; i++)
    {
        asg++;
        aux = A[i];
        j = i-1;
        while (j >= 0 && A[j] > aux)
        {
            com++;
            asg++;
            A[j+1] = A[j];
            j--;
        }
        A[j+1] = aux;
    }
    return;
}

///selection sort algorithm
void selectionSort(int n, int A[])
{
    asg = 0;
    com = 0;

    int i, j, mini;
    for (i=0; i<n-1; i++)
    {
        mini = i;
        for (j=i+1; j<n; j++)
        {
            com++;
            if (A[j] < A[mini])
                mini = j;
        }
        if (mini != i)
        {
            asg+=3;
            swap (A[mini],A[i]);
        }

    }
    return;
}

///print an array
void printArray (int n, int A[])
{
    for (int i=0; i<n; i++)
        cout<<A[i]<<" ";
    cout<<endl;
    return;
}

///copy array B into array A
void copyArray (int n, int A[], int B[])
{
    for (int i=0; i<n; i++)
        A[i] = B[i];
    return;
}


int main()
{
    int hardCodeArr[] = {4,2,7,4,9,0,1,6,2,5};  //hard coded array with 10 elements, for demo
    int hardArrCopy[10];

    ///demo of bubble sort
    copyArray(10,hardArrCopy,hardCodeArr);
    bubbleSort(10,hardArrCopy);
    printArray(10,hardArrCopy);

    ///demo of insertion sort
    copyArray(10,hardArrCopy,hardCodeArr);
    insertionSort(10,hardArrCopy);
    printArray(10,hardArrCopy);

    ///demo of selection sort
    copyArray(10,hardArrCopy,hardCodeArr);
    selectionSort(10,hardArrCopy);
    printArray(10,hardArrCopy);


    srand(time(NULL));

    ofstream f("avg.csv");
    f<<"n , bubAsg , insAsg , selAsg , bubCom , insCom , selCom , bubTot , insTot , selTot"<<endl;
    int randomArray[10000];
    int randomCopy[10000];

    int j;
    int n;
    int bubAsg, bubCom, insAsg, insCom, selAsg, selCom;
    int bubTot, insTot, selTot;
    for (n=100; n<=10000; n+=100)
    {
        bubAsg = 0;
        bubCom = 0;
        insAsg = 0;
        insCom = 0;
        selAsg = 0;
        selCom = 0;
        for (int i=0; i<5; i++)
        {
            for (j=0; j<n; j++)
            randomArray[j] = rand();

            copyArray(n,randomCopy,randomArray);
            bubbleSort(n,randomCopy);
            bubAsg += asg;
            bubCom += com;

            copyArray(n,randomCopy,randomArray);
            insertionSort(n,randomCopy);
            insAsg += asg;
            insCom += com;

            copyArray(n,randomCopy,randomArray);
            selectionSort(n,randomCopy);
            selAsg += asg;
            selCom += com;
        }
        bubAsg /= 5;
        bubCom /= 5;
        insAsg /= 5;
        insCom /= 5;
        selAsg /= 5;
        selCom /= 5;

        bubTot = bubAsg + bubCom;
        insTot = insAsg + insCom;
        selTot = selAsg + selCom;
        f<<n<<" , "<<bubAsg<<" , "<<insAsg<<" , "<<selAsg<<" , "<<bubCom<<" , "<<insCom<<" , "<<selCom<<" , "<<bubTot<<" , "<<insTot<<" , "<<selTot<<endl;
        cout<<"..."<<endl;
    }
    f.close();


    ofstream g("best.csv");
    g<<"n , bubAsg , insAsg , selAsg , bubCom , insCom , selCom , bubTot , insTot , selTot"<<endl;
    int bestArray[10000];
    for (n=100; n<=10000; n+=100)
    {
        bubAsg = 0;
        bubCom = 0;
        insAsg = 0;
        insCom = 0;
        selAsg = 0;
        selCom = 0;

        for (j=0; j<n; j++)
            bestArray[j] = j;

        bubbleSort(n,bestArray);
        bubAsg = asg;
        bubCom = com;

        insertionSort(n,bestArray);
        insAsg = asg;
        insCom = com;

        selectionSort(n,bestArray);
        selAsg = asg;
        selCom = com;

        bubTot = bubAsg + bubCom;
        insTot = insAsg + insCom;
        selTot = selAsg + selCom;
        g<<n<<" , "<<bubAsg<<" , "<<insAsg<<" , "<<selAsg<<" , "<<bubCom<<" , "<<insCom<<" , "<<selCom<<" , "<<bubTot<<" , "<<insTot<<" , "<<selTot<<endl;
        cout<<"..."<<endl;
    }
    g.close();


    ofstream h("worst.csv");
    h<<"n , bubAsg , insAsg , selAsg , bubCom , insCom , selCom , bubTot , insTot , selTot"<<endl;
    int worstArray[10000];
    for (n=100; n<=10000; n+=100)
    {
        bubAsg = 0;
        bubCom = 0;
        insAsg = 0;
        insCom = 0;
        selAsg = 0;
        selCom = 0;

        for (j=0; j<n; j++)
            worstArray[j] = n-j;

        bubbleSort(n,worstArray);
        bubAsg = asg;
        bubCom = com;

        for (j=0; j<n; j++)
            worstArray[j] = n-j;

        insertionSort(n,worstArray);
        insAsg = asg;
        insCom = com;

        for (j=0; j<n; j++)
            worstArray[j] = n-j;    //internet says SS is not an adaptive sorting algorithm, so it does not have a "worst case"

        selectionSort(n,worstArray);
        selAsg = asg;
        selCom = com;

        bubTot = bubAsg + bubCom;
        insTot = insAsg + insCom;
        selTot = selAsg + selCom;
        h<<n<<" , "<<bubAsg<<" , "<<insAsg<<" , "<<selAsg<<" , "<<bubCom<<" , "<<insCom<<" , "<<selCom<<" , "<<bubTot<<" , "<<insTot<<" , "<<selTot<<endl;
        cout<<"..."<<endl;
    }
    h.close();
    return 0;
}
