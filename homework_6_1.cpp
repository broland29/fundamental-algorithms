/**
 *
 * @author Bálint Roland
 * @group  30424
 *
 * Theme: Search Operation in Hash Tables (1/2)
 *
 * Instructions: For better visibility i split the task in two different
 * files. This one will contain the implementation and demos, with extra
 * comments. For further explanations, please check "homework_fa_6_graphs"
 *
**/

#include <iostream>
#include <string.h>

using namespace std;


///Type for an Entry (ex: a student)
typedef struct
{
    int id;
    char name[30];
}Entry;


/**Gets index of Entry based on its id
    - this is basically the hash function
    - quadratic probing is used - formula resembles this
    - open addressing is used: if we find a taken slot, we increment "try"
**/
int getIndex(int id, int tryNum, int hashLen)
{
    return (id % hashLen + tryNum + tryNum * tryNum) % hashLen;
}


/**Adds an Entry to hash table
    Why do we try N times? (N = hashLen = length of hash table)
    - indexes will always repeat themselves, it is only a matter of time
    - for prime numbers it takes more tries
    -  if all of the repeated indexes are taken, an infinite loop occurs
    - to avoid this, we try N times
        - if we cannot succeed, we assume hashing failed, we cannot insert
**/
void insertElement(Entry *hashTable[], int hashLen, int newId, char newName[])
{
    for(int tryNum=0; tryNum<hashLen; tryNum++)     //try N times
    {
        int p = getIndex(newId, tryNum, hashLen);   //get desired index
        if (hashTable[p] == NULL)       //if slot empty
        {
            hashTable[p] = new Entry;
            hashTable[p]->id = newId;
            strcpy(hashTable[p]->name,newName);
            return;
        }
    }
    cout<<"Could not insert \""<<newName<<"\"!\n";
    return;
}


/**Search for an Entry in the hash table
    - if we try N times and Entry is still not found, we can be sure
      that Entry is not in there (since we also hashed N times)
    - if Entry not found, this function returns NULL
**/
Entry *searchElement(Entry *hashTable[], int hashLen, int id)
{
    for (int tryNum=0; tryNum<hashLen; tryNum++)
    {
        int p = getIndex(id,tryNum,hashLen);

        if (hashTable[p] == NULL)       //hashTable[p]->id creates error if hashTable[p]==NULL
            return NULL;                //so, it is important to put this condition first

        if (hashTable[p]->id == id)
            return hashTable[p];
    }

    return NULL;    //tried N times
}


///prints a hash table (fancy)
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


///Used for testing (processing output of) searchElement
void testSearch(Entry *searchResult, int id)
{
    if (searchResult == NULL)
        cout<<"Person with id "<<id<<" not in table!\n";
    else
        cout<<"Person with id "<<id<<" is "<<searchResult->name<<endl;

    return;
}


int main()
{
    cout<<"Demo of getIndex - hash table of length 10 for value 7"<<endl;
    for (int i=0; i<20; i++)
        cout<<getIndex(7,i,10)<<" ";
    cout<<endl<<endl;

    cout<<"Demo of getIndex - hash table of length 13 for value 7"<<endl;
    for (int i=0; i<20; i++)
        cout<<getIndex(7,i,13)<<" ";
    cout<<endl<<endl;


    cout<<"Demo of insertion\n";
    Entry *hashDemo[10];

    for (int i=0; i<10; i++)        //fillNull fails for array of pointers with unknown values
        hashDemo[i] = NULL;         //do i included my fillNull function only in the other file
    printHash(hashDemo,10);

    insertElement(hashDemo,10,7,"Roli");
    printHash(hashDemo,10);

    insertElement(hashDemo,10,3,"Szaby");
    insertElement(hashDemo,10,5,"Zolika");
    printHash(hashDemo,10);

    insertElement(hashDemo,10,17,"Kinga");
    printHash(hashDemo,10);

    insertElement(hashDemo,10,17,"Enemy");
    printHash(hashDemo,10);

    insertElement(hashDemo,10,1212,"Ben");
    printHash(hashDemo,10);


    cout<<"Demo of search\n";
    testSearch(searchElement(hashDemo,10,1),1);
    testSearch(searchElement(hashDemo,10,3),3);
    testSearch(searchElement(hashDemo,10,7),7);

    return 0;
}
