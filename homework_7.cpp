/**
 *
 * @author Bálint Roland
 * @group  30424
 *
 * Theme: Multi-way Trees (1/1)
 *
 * Ideas, explanations, designing choices:
 *
 *  R1:  When implementing with parent representation, it was easy to guess that i should use a fixed length
 *      array, called the parent array/ vector, which in itself is also the representation, since it holds
 *      all the nodes and all the data. I did not need a dedicated data type for this, since the only
 *      data stored in our example is the key, and this can be solved with a simple array of integers
 *       Pretty print was easy, the challenge was implementing it recursively since i knew previously that it
 *      is possible to do so. First we search for value -1, indicating the root. Then, from the position of -1,
 *      we get the key of the root. Now we search for each node having the root as parent, aka we search for
 *      the previously found value in the parent vector. Repeat this as many times as needed.
 *
 *  R2:  The problem quickly got hard when i reached this point. I knew that the transformation needed to have
 *      O(n) time complexity, meaning that we are allowed to run through the parent vector only once, and when
 *      doing so, creating all the nodes and linkages between them. There needs to be a way of remembering which
 *      nodes were created, so, inspired from the hash tables, i created an array of nodes which has on index i
 *      the node i. This takes extra O(n) space, but at the same time can be considered R2, the multi-way representation,
 *      since it holds all nodes, all data. To make it even more convenient (knowing that we have to use this
 *      later on) i put the root on index 0 as well.
 *       Pretty print is kind of self- explanatory.
 *
 *  R3:  Since i already have a dedicated data type for the multi way representation and i know which one is the
 *      root instantly, for the third representation i can make a "real" dynamic tree. I just need to change the
 *      linkage between the nodes, but i also have to make new nodes since they differ from the previous type. So,
 *      it is easier said than done.
 *       Each time we work with one node (ex: start from root), and with its sons. The first son needs to become
 *      the node's left child, while the other sons the left child's brothers. We also have to take into consideration
 *      that each son, even if left child or brother, might serve as a new root, aka might have a left child and that
 *      left child might have one more more brothers.
 *       Pretty print has a similar approach as the transformation itself.
 *
 *  T1:  The transformation from parent to multi way representation has a complexity of O(n), since it runs through
 *      parent node once, parent node having the length of n (number of nodes)
 *
 *  T2:  The transformation from multi way to binary representation has a complexity of O(n), since it runs through
 *      the multi way representation only once (which also has a length of n)
 *
**/

#include <iostream>
#define MAXSONS 20

using namespace std;


///Type for a node in multi-way representation
struct mulNode
{
    int key;                    //key of node, could store any data like this
    mulNode *sons[MAXSONS];     //array of sons (pointers to sons)
    int sonCount;               //number of sons
};


///Type for a node in binary representation
struct binNode
{
    int key;                    //key of node, could store any data like this
    binNode *leftChild;         //linkage to left child
    binNode *brother;           //linkage to brother ("on right")
};


/**Pretty print for parent representation
    Recursive implementation
    -parent[]   :   parent vector
    -parentSize :   size of parent vector
    -toSearch   :   value we are searching for in the parent vector = we search for its children
    -spaceNum   :   number of spaces in order to have a "pretty" output
**/
void prettyPrintR1(int parent[], int parentSize, int toSearch, int spaceNum)
{
    for (int i=1; i<parentSize; i++)    //for explanation of limits, check extra comments
    {
        if (parent[i] == toSearch)      //if value found = found a node which has parent toSearch
        {
            for (int j=0; j<spaceNum; j++) cout<<" ";       //print
            cout<<i<<endl;                                  //node
            prettyPrintR1(parent,parentSize,i,spaceNum+1);      //descend = search for "sons of the son"
        }
    }

    return;
}


///Create a new mulNode
mulNode *newMulNode(int key)
{
    mulNode *newNode = new mulNode;

    newNode->key = key;
    newNode->sonCount = 0;

    return newNode;
}


/**Transformation from parent to multi way representation = T1
    -parent[]   -   the parent vector, from previous representation
    -parentSize -   size of parent vector
    -R2         -   the new representation will be created here
**/
void transParToMul(int parent[], int parentSize, mulNode* R2[])
{
    for (int i=1; i<parentSize; i++)
    {
        //check if node i exists - if not, create it
        if (R2[i] == NULL)
        {
            R2[i] = newMulNode(i);
        }

        if (parent[i] == -1)//if node i root - set R2[0] as a pointer to (pointer to) root, for easy access
        {
            R2[0] = R2[i];
        }
        else                //if node i not root
        {
            if (R2[parent[i]] == NULL)                              //check if the parent of node i exists - if not, create it
                R2[parent[i]] = newMulNode(parent[i]);

            R2[parent[i]]->sons[R2[parent[i]]->sonCount] = R2[i];   //add node i to sons of node parent[i]

            R2[parent[i]]->sonCount ++;                             //increase son counter in parent
        }
    }

    return;
}


///Pretty print for multi way representation
void prettyPrintR2(mulNode *node, int spaceNum)
{
    for (int i=0; i<spaceNum; i++) cout<<" ";   //print
    cout<<node->key<<endl;                      //the node

    for (int i=0; i<node->sonCount; i++)
    {
        prettyPrintR2(node->sons[i],spaceNum+1);    //repeat for each son
    }

    return;
}


///Create new binNode
binNode *newBinNode(int key)
{
    binNode *toAdd = new binNode;

    toAdd->key = key;
    toAdd->brother = NULL;
    toAdd->leftChild = NULL;

    return toAdd;
}


/**Transformation from multi way to binary representation = T2
    node    -   the current node we are working on
    R2      -   the multi way representation
**/
void transMulToBin(binNode *node, mulNode *R2[])
{
    if (R2[node->key]->sonCount > 0)        //if the node (in previous representation) has children
    {
        //set first son as left child
        binNode *child = newBinNode(R2[node->key]->sons[0]->key);
        node->leftChild = child;
        transMulToBin(node->leftChild,R2);  //repeat for left child on the way

        //set rest of sons as brothers
        if (R2[node->key]->sonCount > 1)    //if there are such nodes
        {
            binNode *prev = node->leftChild;
            binNode *bro;
            for (int i=1; i<R2[node->key]->sonCount; i++)
            {
                bro = newBinNode(R2[node->key]->sons[i]->key);
                prev->brother = bro;
                transMulToBin(prev->brother,R2);    //repeat for each brother on the way
                prev = bro;
            }
        }
    }

    return;
}


///Pretty print for binary representation
void prettyPrintR3(binNode *node, int spaceNum)
{
    //print node
    for (int i=0; i<spaceNum; i++) cout<<" ";
    cout<<node->key<<endl;

    //call function for left child
    if (node->leftChild != NULL)
        prettyPrintR3(node->leftChild,spaceNum+1);

    //print brothers
    while (node->brother != NULL)
    {
        //print brother
        for (int i=0; i<spaceNum; i++) cout <<" ";
        cout<<node->brother->key<<endl;
        //call function for left child of brother
        if (node->brother->leftChild != NULL)
            prettyPrintR3(node->brother->leftChild,spaceNum+1);
        //continue traversing brothers
        node = node->brother;
    }

    return;
}


int main()
{
    ///Demo on the example from the lab

    ///Implementation and print - R1
    //              1   2   3   4   5   6   7   8   9
    int R1[] = {0 , 2 , 7 , 5 , 2 , 7 , 7 ,-1 , 5 , 2};     //hard coded parent vector
    prettyPrintR1(R1,10,-1,0);
    cout<<endl;


    ///Implementation and print - T1, R2
    mulNode *R2[10];            //vector to hold pointers to created nodes
    for (int i=1; i<10; i++)
        R2[i] = NULL;
    transParToMul(R1,10,R2);
    prettyPrintR2(R2[0],0);
    cout<<endl;


    ///Implementation and print - T2, R3
    binNode *R3;    //root of binary representation
    R3 = newBinNode(R2[0]->key);    //since R2[0] is the root
    transMulToBin(R3,R2);
    prettyPrintR3(R3,0);

    return 0;
}
