/**
 *
 * @author Bálint Roland
 * @group  30424
 *
 * Theme: Implementation and Demos for Quick Sort and Quick Select
 *
 * Instructions: For better visibility and simplicity i split the task in
 * two different files. This one will contain only the implementation and
 * demos of the ones mentioned above. For the code generating the graphs,
 * and for further explanations, please check "homework_fa_cases"
 *
**/

#include <iostream>

using namespace std;


/**Puts the pivot in its right place, returns its index
        i = end index of partition with smaller elements
        j = end index of partition with bigger elements
        p = start index of array to sort
        r = end index of array to sort
    Implementation based on pseudo code from the book.
    Contrary to what we said at lab, in this implementation
    both i and j start from left
**/
int partitionBook(int A[], int p, int r)
{
    int x = A[r];   //select pivot
    int i = p-1;    //start with first element in "smaller" partition
    for (int j=p; j<r; j++)     //j reaches r <=> all elements were evaluated
    {
        if (A[j] <= x)  //if element is smaller than pivot, put it in "smaller" partition
        {
            i++;                //increase size of "smaller" partition
            swap(A[i],A[j]);    //swap first element of "bigger" (now in "smaller"), with last element of "bigger" (should be in "smaller")
        }
    }
    swap(A[i+1],A[r]);  //put pivot on the right index; A[i+1] is in "bigger"
    return i+1;
}


///Quick sort between indexes p and r
void quickSort (int arr[], int p, int r)
{
    if (p < r)  //if there still is something to sort
    {
        int q = partitionBook(arr,p,r); //set q on its right place
        quickSort(arr,p,q-1);           //sort left side of q
        quickSort(arr,q+1,r);           //sort right side of q
    }

    return;
}


/**Finds the i'th smallest element of array A[p,r] (without sorting)
   Or in other words, the i'th element of array A if it was sorted
        p - first index of array
        r - last index of array
        i - index i as explained above
    Implementation is a mixture of what i found in the book and
    what i thought i should write
**/
int quickSelect(int A[], int p, int r, int i)
{
    if (p == r)         //base condition, not sure if needed tho
        return A[p];    //since for me it worked even without it

    int q = partitionBook(A,p,r);   //puts A[r] in the right place, which is at index q

    if (i == q)         //if pivot was the searched element
        return A[q];
    if (i < q)
        return quickSelect(A,p,q-1,i);  //cont. searching on left partition
    if (i > q)
        return quickSelect(A,q+1,r,i);  //cont. searching on right partition
}


void printArray (int arr[], int n)
{
    for (int i=0; i<n; i++)
        cout<<arr[i]<<" ";
    cout<<endl<<endl;

    return;
}


void copyArray (int dest[], int src[], int n)
{
    for (int i=0; i<n; i++)
        dest[i] = src[i];

    return;
}


int main()
{
    cout<<"The demo array used:\n";
    int demoArray[] = {3,7,2,5,4,1,10,8,9,6};
    int demoCopy[10];
    printArray(demoArray,10);

    cout<<"Demo of Quick Sort:\n";
    copyArray(demoCopy,demoArray,10);
    quickSort(demoCopy,0,9);
    printArray(demoCopy,10);

    cout<<"Demo of Quick Select:\n";
    int userChoice;
    cout<<" Insert i and i will show you the i'th smallest element!\n";
    cout<<" Insert -1 to stop demo!\n";
    cin>>userChoice;
    while (userChoice > -1 && userChoice < 10)
    {
        copyArray(demoCopy,demoArray,10);
        cout<<"Element on index "<<userChoice<<" in the sorted array would be "<<quickSelect(demoCopy,0,10,userChoice);
        cout<<"\n\n Next i: ";
        cin>>userChoice;
    }

    return 0;
}
