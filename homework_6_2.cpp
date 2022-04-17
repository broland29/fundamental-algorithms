/**
 *
 * @author Bálint Roland
 * @group  30424
 *
 * Theme: Search Operation in Hash Tables (2/2)
 *
 * Instructions: This file serves the pure purpose of generating the
 *  tables and testing the search function. For detailed explanations
 *  of the functions used and demos, check "homework_fa_6_demos"
 *
 * Additional explanations:when using FillRandomArray, for the
 *  first time i put the unique parameter true, while on other two
 *  i put it false. This is because the first array contains id-s
 *  to be inserted (i do not see why two different Entries would
 *  have the same identifier), while the other two are used for
 *  search purposes - randomIdOutside to search for non-existing id-s,
 *  while getRandomIndex as indexes of randomIdInside (we may want to
 *  search for the same Entry/ same id more than once)
 *
 *
 *      Filling factor      Avg. Effort found       Max. Effort found       Avg. Effort not-found       Max. Effort not-found
 *          0.8	                1.74853	                27	                    5.96627	                    52
 *          0.85	            1.88613	                31	                    8.16293             	    67
 *          0.9	                2.1544	                58	                    13.1347	                    96
 *          0.95	            2.8744	                131	                    27.0132	                    168
 *          0.99	            4.2416	                277	                    105.095	                    927
 *
 *
 * Interpretation of results: as expected, the search function finds
 *  the desired element faster for hash tables with less elements.
 *  The filling factor determines the number of elements in the hash
 *  table, so the bigger alpha is, the more elements we have. We can see
 *  that the effort increases together with the filling factor, as well
 *  as maximum effort (which represents the worst-case scenario encountered)
 *
 * The values of the table may vary on a lost of factors, such as the
 *  size of the hash table, the filling factor, the hash function. It
 *  is interesting that although these were the same, my dear colleague
 *  found very different values. We think that this is because of the ranges
 *  chosen for the indexes (existing, non-existing, and probably even the
 *  "gap" between these two).
 *
 * Another observation one can make is that it takes far less effort to find
 *  an existing Entity than a non-existing one. The explanation is pretty
 *  simple: big filling factor => lots of elements => few NULLs. Why does this
 *  matter? Well, because searching ends when a NULL is found. So we have to
 *  go through all the possible slots AND find a NULL. When searching for an
 *  existing Entry, we also go through some elements due to collision, but
 *  eventually it will be found before finding a NULL (we only go through all
 *  collisions in the worst case)
 *
 *
**/


#include <iostream>
#include <string.h>
#include <fstream>
#include "Profiler.h"

#define N 10007

using namespace std;

float opFoundAvg;
int   opFoundMax;
float opNotFoundAvg;
int   opNotFoundMax;

int opOne;  //used to count operations in one single search


///Start of code from previous file
typedef struct
{
    int id;
    char name[30];
}Entry;

int getIndex(int id, int tryNum, int hashLen)
{
    return (id % hashLen + tryNum + tryNum * tryNum) % hashLen;
}

void insertElement(Entry *hashTable[], int hashLen, int newId, char newName[])
{
    for(int tryNum=0; tryNum<hashLen; tryNum++)
    {
        int p = getIndex(newId, tryNum, hashLen);
        if (hashTable[p] == NULL)
        {
            hashTable[p] = new Entry;
            hashTable[p]->id = newId;
            strcpy(hashTable[p]->name,newName);
            return;
        }
    }
    //cout<<"Could not insert \""<<newName<<"\"!\n"; here it has a very low chance to occur
    return;
}

Entry *searchElement(Entry *hashTable[], int hashLen, int id)
{
    for (int tryNum=0; tryNum<hashLen; tryNum++)
    {
        opOne++;    //increment each time we try, each time we check a slot

        int p = getIndex(id,tryNum,hashLen);

        if (hashTable[p] == NULL)
            return NULL;

        if (hashTable[p]->id == id)
            return hashTable[p];
    }

    return NULL;
}

void printHash (Entry *hashTable[], int hashSize)
{
    for (int i=0; i<hashSize; i++)
        if (hashTable[i] == NULL)
            cout<<"slot "<<i<<"  -  Empty slot"<<endl;
        else
            cout<<"slot "<<i<<"  -  id: "<<hashTable[i]->id<<", name: "<<hashTable[i]->name<<endl;
    cout<<endl;
    return;
}
///End of code from previous file


///Makes the hash table "empty" - important when reusing
void fillNull(Entry *hashTable[], int hashSize)
{
    for (int i=0; i<hashSize; i++)
    {
        if (hashTable[i] != NULL)
        {
            delete hashTable[i];
            hashTable[i] = NULL;
        }
    }
    return;
}


///Does the testing for a given filling factor
void fillAndTest(Entry *hashTable[],  float alfa, ofstream& f)
{
    int elemNum = N * alfa; //number of elements to be inserted (integer, since we cannot have half element)

    int randomIdInside[N];      //all id-s to be inserted
    int randomIdOutside[1500];  //1500 non-existing id-s
    int getRandomIndex[1500];   //1500 indexes for choosing 1500 random existing elements

    opFoundAvg = 0;
    opFoundMax = 0;
    opNotFoundAvg = 0;
    opNotFoundMax = 0;

    for (int i=0; i<5; i++) //perform 5 runs
    {
        ///fill hash table
        FillRandomArray(randomIdInside,elemNum,0,20000,true,0);     //existing id-s in range [0,20000] - no need to generate N elements

        FillRandomArray(randomIdOutside,1500,20001,40000,false,0);   //non-existing id-s in range [20001,40000]

        FillRandomArray(getRandomIndex,1500,0,elemNum,false,0);      //1500 indexes in proper ranges

        for (int i=0; i<elemNum; i++)
        {
            insertElement(hashTable,N,randomIdInside[i],"demoName");
        }

        ///test search
        //search for existing
        for (int i=0; i<1500; i++)
        {
            opOne = 0;
            searchElement(hashTable,N,randomIdInside[getRandomIndex[i]]);
            if (opOne > opFoundMax)
                opFoundMax = opOne;
            opFoundAvg += opOne;
        }

        //search for non-existing
        for (int i=0; i<1500; i++)
        {
            opOne = 0;
            searchElement(hashTable,N,randomIdOutside[i]);
            if (opOne > opNotFoundMax)
                opNotFoundMax = opOne;
            opNotFoundAvg += opOne;
        }

        fillNull(hashTable,N);
    }

    opFoundAvg /= 7500;     //divide with
    opNotFoundAvg /= 7500;  //5 * 1500

    f<<alfa<<" , "<<opFoundAvg<<" , "<<opFoundMax<<" , ";
    f<<opNotFoundAvg<<" , "<<opNotFoundMax<<endl;

    return;
}


int main()
{
    Entry *hashTable[N];

    for(int i=0; i<N; i++)
        hashTable[i] = NULL;

    ofstream f("hash_quadratic.csv");
    f<<"Filling factor , Avg. Effort found , Max. Effort found";
    f<<" , Avg. Effort not-found , Max. Effort not-found"<<endl;

    fillAndTest(hashTable,0.8, f);
    fillAndTest(hashTable,0.85,f);
    fillAndTest(hashTable,0.9, f);
    fillAndTest(hashTable,0.95,f);
    fillAndTest(hashTable,0.99,f);

    f.close();

    return 0;
}
