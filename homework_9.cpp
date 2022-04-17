#include <iostream>
#include <algorithm>

using namespace std;

struct node
{
    int key;        //key of node
    int parent;     //key of the parent of the node
    int rankOf;     //number of nodes below the node (including itself)
};

struct edge
{
    int x;      //from
    int y;      //to
    int weigth;
};

node *makeSet(int key)
{
    node *toAdd = new node;

    toAdd->key = key;
    toAdd->parent = key;
    toAdd->rankOf = 1;

    return toAdd;
}

void initSets(node *arr[], int n)
{
    for (int i=0; i<n; i++)
        arr[i] = makeSet(i);

    return;
}

void printSets(node *arr[], int n)
{
    cout<<"Key  -  Parent  -  Rank"<<endl;

    for (int i=0; i<n; i++)
        cout<<arr[i]->key<<" - "<<arr[i]->parent<<" - "<<arr[i]->rankOf<<endl;
    cout<<endl;

    return;
}

int findSet(node arr[], int key)
{
    if (key != arr[key].parent)
    {
        arr[key].parent = findSet(arr,arr[key].parent);  //continue searching on parent, and also put result (root) as parent of all nodes we go through
        return arr[key].parent;   //so we return a value on each branch
    }
    else    //we got the representative, since parent of node is itself = it is root
    {
        return key;
    }
}

void unionSet(node arr[], int x, int y)
{
    int xRoot = findSet(arr,x);
    int yRoot = findSet(arr,y);

    if (arr[xRoot].rankOf < arr[yRoot].rankOf)
        arr[xRoot].parent = yRoot;
    else if (arr[xRoot].rankOf > arr[yRoot].rankOf)
        arr[yRoot].parent = xRoot;
    else
    {
        arr[xRoot].parent = yRoot;
        arr[yRoot].rankOf++;
    }
}

bool compare(edge &e1, edge &e2)
{
    return e1.weigth < e2.weigth;
}

void kruskal(edge arrEdges[], int edgeNum, node nodeSets[])
{
    sort(arrEdges,arrEdges+edgeNum,compare);    //sort the edges

    for (int i=0; i<edgeNum; i++)
        if (findSet(nodeSets,arrEdges[i].x) != findSet(nodeSets,arrEdges[i].y))
        {
            unionSet(nodeSets,arrEdges[i].x,arrEdges[i].y);
            cout<<arrEdges[i].x<<" "<<arrEdges[i].y<<" "<<arrEdges[i].weigth<<endl;
        }

    return;
}

int main()
{
    cout<<"Demo based on lab example:\n";
    node *demo[5];
    initSets(demo,5);
    printSets(demo,5);
    return 0;
}
