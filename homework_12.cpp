/**
 *
 * @author Bálint Roland
 * @group  30424
 *
 *
 * Theme: Depth- First Search, Topological Sort, Tarjan's algorithm
 *
 *
 * Explanations, notes for future me:
 *
 *  Depth- first search
 *   - well-known algorithm
 *   - it uses a stack
 *   - in this implementation, it is recursive => it uses the default stack
 *
 *  Topological sort
 *   - ex: an order in which tasks can be made
 *       - tasks rely on each other
 *       - arcs show this dependence (a->b => a has to be made before b)
 *
 *   - by looking at end times of nodes being processed in a graph, in reverse order,
 *     we can find a possible topological order
 *   - algorithm takes DFS and stores nodes when processing is done
 *   - at the end it prints nodes in reverse order
 *   - if there are cycles, there is no topological order (it introduces a paradox)
 *
 *   - NOTE: the algorithm does not run in infinite loop if there is a cycle, since it
 *       checks the colors, but the outcome is not a solution, since there is no
 *       valid solution for such graphs. I chose to print the outcome anyways, and
 *       notify the user that the graph has a cycle. It could be easily modified by
 *       de-commenting the lines marked with (*) so that the algorithm stops at the
 *       first cycle and does not print the (otherwise useless) outcome
 *
 *  Tarjan's algorithm
 *   - goal: find strongly connected components
 *
 *   - approach: further augmenting the node structure : we have an index and a low-link
 *   - when we run Tarjan, we focus on the indexes of nodes - the time they were first processed by DFS
 *       - like starting time but since we have no end time, it is incremented only when nodes are being
 *         processed, while in DFS/topological, we incremented when nodes turned black as well
 *   - "The low-link value of a node is the smallest node ID reachable from that node when doing
 *      a depth-first search (DFS), including itself"
 *       - this will not result in one strongly connected component having same low-link for each of its nodes!
 *           - highlighting the part "when doing a depth-first search" - nodes are processed only once
 *       - it will rather result in an interesting behavior, giving us the result
 *   - in each strongly connected component, at the end, we will have one node with its key equal to its low-link
 *       - except when a node connects to an already processed SCC (meaning "it can go there, but can't come back")
 *           - we address this particular case by ignoring elements which are already processed (black),
 *              but in other component
 *           - this is where the implementation with stack comes in handy
 *
 *   - final implementation:
 *       - we run DFS, putting nodes in a stack and updating low-links
 *       - when we encounter a black node with its index and low-link equal, we pop the elements (from the stack)
 *          until we meet this node
 *
 *   - why DFS is not enough to find strongly connected components? well it just "flows" in one direction, it
 *      does not check if we can get back as well; in one connected component there may be several strongly
 *      connected components
 *
 *
 * Complexity: from the values given from running the evaluation, it is obvious that in both cases (when
 *  varying number of arcs and when varying the nodes) we got a linear complexity. These complexities
 *  should be added up instead of getting multiplied, for similar reasons as in BFS: the two dominant
 *  parts of operation, giving the previously mentioned complexities, are not embedded in each other,
 *  they come one after the other (first nodes are processed, then edges are processed, each one only once).
 *
 * In conclusion, the complexity is O(V+E)
 *
**/


#include <iostream>
#include <time.h>   //for random
#include <fstream>  //for output
#include <cstdlib>  //for output

using namespace std;


///For elegant node coloring
enum
{
    COLOR_WHITE,
    COLOR_GRAY,
    COLOR_BLACK
};


///Representation of a node of the graph, used by each algorithm
typedef struct _Node
{
    ///for DFS
    int key;                    //identifier of the node
    int adjSize;                //number of neighbors
    struct _Node *adj[300];     //array of (pointers to) neighbors
    int color;                  //for the coloring of nodes

    ///for Tarjan
    int index;      //order in which nodes were processed
    int lowlink;    //lowest index reachable from the node
    bool onStack;   //is it inside the stack currently
}Node;


///Prepare nodes for DFS
void initNodes(Node *nodes[], int nodeCount)
{
    for (int i=0; i<nodeCount; i++)
    {
        nodes[i] = new Node;    //allocate space

        nodes[i]->key = i;              //set key - we suppose we have one node for each integer in range [0,nodeCount]
        nodes[i]->adjSize = 0;          //no neighbors initially
        nodes[i]->color = COLOR_WHITE;  //node initially unprocessed - it is white
    }
}


///Used if we use same graph multiple times
void reInitNodes(Node *nodes[], int nodeCount)
{
    for (int i=0; i<nodeCount; i++)
        nodes[i]->color = COLOR_WHITE;
}


/*************************** Depth-First Search ***************************/


///Does one DFS, starting from the node initially called on
void dfsVisit(Node *node)
{
    cout<<node->key<<" ";    //for debugging/ demo

    //start of processing ("in stack") - color it gray
    node->color = COLOR_GRAY;

    //for each neighbor
    for (int i=0; i<node->adjSize; i++)
    {
        Node *neighbor = node->adj[i];  //current neighbor processed

        if (neighbor->color == COLOR_WHITE) //if unprocessed
        {
            dfsVisit(neighbor);    //descend
        }
    }

    //end of processing ("popped out of stack") - color it black
    node->color = COLOR_BLACK;
}


///Does DFS repeatedly up until whole graph is covered
void dfs(Node *nodes[], int nodeCount)
{
    for (int i=0; i<nodeCount; i++)
    {
        if (nodes[i]->color == COLOR_WHITE)
        {
            dfsVisit(nodes[i]);
            cout<<"  "; //for debugging/ demo
        }
    }
    cout<<endl<<endl; //for debugging/ demo
}


/*************************** Topological Sorting ***************************/


int topologicalArray[100];  //for storing end times
int topCount;               //counter for array "topologicalArray"
bool hasCycle;              //set true if graph has cycle => cannot have topological sort


///Gets topological sort for one DFS
void topologicalVisit(Node *node)
{
    node->color = COLOR_GRAY;
    for (int i=0; i<node->adjSize; i++)
    {
        Node *neighbor = node->adj[i];
        if (neighbor->color == COLOR_WHITE)
        {
            topologicalVisit(neighbor);
        }
        //if condition below is met, the graph has a cycle
        else if (neighbor->color == COLOR_GRAY)
        {
            hasCycle = true;
            //return; (*)
        }
    }
    node->color = COLOR_BLACK;

    //processing done => end time reached => store node
    topologicalArray[topCount] = node->key;
    topCount++;
}


///Does DFS repeatedly up until whole graph has a topological sort
void topological(Node *nodes[], int nodeCount)
{
    hasCycle = false;
    topCount = 0;

    for (int i=0; i<nodeCount; i++)
    {
        if (nodes[i]->color == COLOR_WHITE)
            topologicalVisit(nodes[i]);
    }

    if (hasCycle)
        cout<<"Graph contains a cycle!\n";
    //else (*)
    //print w.r.t end times, in reverse order, if topological sort possible
    for (int i=topCount-1; i>=0; i--)
        cout<<topologicalArray[i]<<" ";

    cout<<endl<<endl;
}


/*************************** Tarjan's Algorithm ***************************/


Node *nodeStack[100];
int stackSize;
int currentIndex;


///Does one Tarjan, started at node "node"
void tarjanVisit(Node *node)
{
    //initialize index and low-link
    node->index = currentIndex;
    node->lowlink = currentIndex;
    currentIndex++;

    //put node on stack
    nodeStack[stackSize] = node;
    stackSize++;
    node->onStack = true;

    //run DFS
    node->color = COLOR_GRAY;

    for (int i=0; i<node->adjSize; i++)
    {
        Node *neighbor = node->adj[i];
        if (neighbor->color == COLOR_WHITE)
        {
            //call a new visit
            tarjanVisit(neighbor);
            //unprocessed/white neighbor => low-link = index
            //update low-link if neighbors have smaller low-link
            if (neighbor->lowlink < node->lowlink)
            {
                node->lowlink = neighbor->lowlink;
            }
        }
        else //gray or black neighbor
        {
            if (neighbor->onStack)//in the same component
            {
                //update low-link if neighbors have smaller index and on stack
                if (neighbor->index < node->lowlink)
                    node->lowlink = neighbor->index;
            }
        }
    }

    node->color = COLOR_BLACK;

    //if condition below is met, we finished a strongly connected component
    if (node->index == node->lowlink)
    {
        //pop until we reach current node
        while (nodeStack[stackSize-1] != node)
        {
            cout<<nodeStack[stackSize-1]->key<<" ";
            nodeStack[stackSize-1]->onStack = false;
            stackSize--;
        }
        cout<<nodeStack[stackSize-1]->key<<"   ";
        nodeStack[stackSize-1]->onStack = false;
        stackSize--;
    }
}


///Does Tarjan repeatedly up until whole graph is covered
void tarjan(Node *nodes[], int nodeCount)
{
    currentIndex = 0; //in case of using more than once, we need re-initialization
    stackSize = 0;    //of the global variables

    for (int i=0; i<nodeCount; i++)
    {
        //nodes[i]->color = COLOR_WHITE;    since done inside initNodes and reInitNodes,
        nodes[i]->onStack = false;        //there is no need to re-color the nodes
    }

    for (int i=0; i<nodeCount; i++)
        if (nodes[i]->color == COLOR_WHITE)
            tarjanVisit(nodes[i]);

    cout<<endl<<endl;
}


/*************************** Auxiliary Functions ***************************/


///Adds an arc from node with key "from" to node with key "to"
void addArc(Node *nodes[], int from, int to)
{
    Node *fromNode = nodes[from];
    Node *toNode = nodes[to];

    fromNode->adj[fromNode->adjSize] = toNode;
    fromNode->adjSize++;
}


///Check if there exists an arc from node with key "from" to node with key "to"
bool isConnected(Node *nodes[], int from, int to)
{
    Node *fromNode = nodes[from];
    Node *toNode = nodes[to];

    for (int i=0; i<fromNode->adjSize; i++)
        if (fromNode->adj[i] == toNode) return true;

    return false;
}


///Print adjacency matrix - can check easily if we coded the demo graph we want
void printMatrix(Node *nodes[], int nodeCount)
{
    for (int i=0; i<nodeCount; i++)
    {
        for (int j=0; j<nodeCount; j++)
        {
            if (i == j) cout<<"0 ";
            else if (isConnected(nodes,i,j)) cout<<"1 ";
            else cout<<"0 ";
        }
        cout<<endl;
    }
    cout<<endl;
}


///Function reInitNodes, but also resets structure
void reInitNodes2(Node *nodes[], int nodeCount)
{
    for (int i=0; i<nodeCount; i++)
    {
        nodes[i]->color = COLOR_WHITE;
        nodes[i]->adjSize = 0;
    }
}


///Generates a random graph with nodeCount nodes and arcCount arcs
void generateRandomGraph(Node *nodes[], int nodeCount, int arcCount)
{
    reInitNodes2(nodes,nodeCount);
    for (int i=0; i<arcCount; i++)
    {
        int from = rand() % nodeCount;
        int to = rand() % nodeCount;

        //requirements on moodle said: "make sure you don’t
        //generate the same edge twice for the same graph"
        //i also avoided self-loops since we did not talk about them
        while (to == from || isConnected(nodes,from,to))
        {
            /*need to reset both, since, especially in case of graphs with many edges, one may encounter
                an impossible case and crash the program. for example, i tested 5 nodes and 10 arcs.
                i got value of "from" as 4, and arcs 4->0 4->1 4->2 4->3, i.e. all possible arcs were
                already generated. the program crashed, since it is impossible to find another new arc
            */
            from = rand() % nodeCount;
            to = rand() % nodeCount;
        }

        addArc(nodes,from,to);
    }
}


int opCount;


///Function dfsVisit without comments and printing, with handling opCount, further optimised
void evalDfsVisit(Node *node)
{
    node->color = COLOR_GRAY;
    opCount++;                              //assignment

    int adjSize = node->adjSize;
    opCount++;                              //assignment

    for (int i=0; i<adjSize; i++)
    {
        Node *neighbor = node->adj[i];
        opCount++;                          //assignment

        if (neighbor->color == COLOR_WHITE)
        {
            opCount++;                      //comparison
            evalDfsVisit(neighbor);
        }
    }

    node->color = COLOR_BLACK;
    opCount++;                              //assignment
}


///Function dfs without comments and printing, with handling opCount
void evalDfs(Node *nodes[], int nodeCount)
{
    for (int i=0; i<nodeCount; i++)
        if (nodes[i]->color == COLOR_WHITE)
        {
            opCount++;                      //comparison
            evalDfsVisit(nodes[i]);
        }

    //cout<<"...";//to see if program is still running
}


int main()
{

/*************************** Demos ***************************/

    ///Set up first graph - as first seen in presentation, page 7

    Node *demoNodes[9];     //9 nodes, with indexes [0,8]
    initNodes(demoNodes,9);

    //printMatrix(demoNodes,9);

    addArc(demoNodes,0,3);
    addArc(demoNodes,1,0);
    addArc(demoNodes,1,5);
    addArc(demoNodes,2,1);
    addArc(demoNodes,3,2);
    addArc(demoNodes,4,1);
    addArc(demoNodes,5,1);
    addArc(demoNodes,5,8);
    addArc(demoNodes,7,6);
    addArc(demoNodes,8,5);

    //printMatrix(demoNodes,9);

    ///Set up second graph - as first seen in presentation, page 32

    Node *demoNodes2[6];     //6 nodes, with indexes [0,5]
    initNodes(demoNodes2,6);

    //printMatrix(demoNodes2,6);

    addArc(demoNodes2,5,0);
    addArc(demoNodes2,0,1);
    addArc(demoNodes2,1,2);
    addArc(demoNodes2,1,3);
    addArc(demoNodes2,1,4);

    //printMatrix(demoNodes2,6);


    ///Run demos
    cout<<"Demo of Depth-First Search - on first graph:\n";
    dfs(demoNodes,9);

    cout<<"Demo of Topological Sort - on second graph:\n";
    topological(demoNodes2,6);

    cout<<"Demo of Topological Sort - on first graph:\n";
    reInitNodes(demoNodes,9);
    topological(demoNodes,9);

    cout<<"Demo of Tarjan's Algorithm - on first graph:\n";
    reInitNodes(demoNodes,9);
    tarjan(demoNodes,9);

/*************************** Evaluation ***************************/

    ///Preparation
    int numberOfNodes;
    int numberOfArcs;
    Node *evalNodes[200];
    initNodes(evalNodes,200);

    srand(time(NULL));

    ofstream fOutTable("dfs.csv");

    /*
    ///Testing generateRandomGraph
    generateRandomGraph(evalNodes,5,10);
    printMatrix(evalNodes,5);
    generateRandomGraph(evalNodes,5,10);
    printMatrix(evalNodes,5);
    generateRandomGraph(evalNodes,5,10);
    printMatrix(evalNodes,5);
    */

    ///Vary number of arcs
    fOutTable<<"Arc Number , Operation Count"<<endl;

    numberOfNodes = 100;
    for(numberOfArcs = 1000; numberOfArcs <= 5000; numberOfArcs+=100)
    {
        opCount = 0;

        for (int i=0; i<5; i++)
        {
            generateRandomGraph(evalNodes,numberOfNodes,numberOfArcs);
            evalDfs(evalNodes,numberOfNodes);
        }
        fOutTable<<numberOfArcs<<" , "<<opCount/5<<endl;
    }

    ///Vary number of nodes
    fOutTable<<"Node Number , Operation Count"<<endl;

    numberOfArcs = 9000;
    for(numberOfNodes = 100; numberOfNodes <= 200; numberOfNodes+=10)
    {
        opCount = 0;

        for (int i=0; i<5; i++)
        {
            generateRandomGraph(evalNodes,numberOfNodes,numberOfArcs);
            evalDfs(evalNodes,numberOfNodes);
        }

        fOutTable<<numberOfNodes<<" , "<<opCount/5<<endl;
    }

    cout<<"\n\nCheck output file!\n";

    return 0;
}
