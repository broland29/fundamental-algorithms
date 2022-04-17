/**
 *
 * @author Bálint Roland
 * @group  30424
 *
 * Theme: Dynamic Order Statistics (1/1)
 *
 * Majority of the code is written based on the online presentation of the lab.
 *
 * Some things that might be personalized/ need more explanation:
 *
 *  - i chose the operation counting variables to be floats, since we are talking
 *      about very small values - i did the division with n inside the code, before
 *      putting the data in the output file
 *
 *  - when creating the random index, i needed to make sure it is a valid one. i
 *      used the formula: random number between f and t = f + rand() % (t - f + 1).
 *      in our case, we need a random number in the interval 1 and n-i, since at
 *      each iteration we deleted i elements previously, and the search works with
 *      orders, so basically the maximum order decreases by the number of deleted
 *      nodes each time. After applying formula, we get 1 + rand() % (n - i + 1)
 *
 *  - when fixing the subSize (number of nodes is node's subtree, including itself)
 *      after deleting, as commented in the code, i decreased it after entering the
 *      deletion function. If a node is processed as a root in the deleting process,
 *      no matter if it will be deleted or some of its successors, its subSize will
 *      decrease by one.
 *
 *  - general observation: both in OS-Select and OS-Delete, when we go on the left
 *      or right subtree, we ignore the rest of the tree. This can be done due to lines like
 *              osSelect(root->right,i-r);
 *      the only thing that can change when descending into a subtree is the order,
 *      this happens when we go on the right subtree. To use the same algorithm, we
 *      just pretend that instead of searching for the i'th element of the whole tree,
 *      we have to get the i-r'th of the right subtree. This is what i meant by "skipping"
 *
 * Complexity:
 *  By testing the algorithm the way it was asked, and making the graph from the data,
 *  we can see that it has O(logn) - which seems logical to me, since we are talking
 *  about operations in a balanced tree
 *              n	        select	    delete
 *              100	        15.9	    19.044
 *              1000	    25.9294	    29.0366
 *              10000	    35.8844	    39.1466
 *  It is obvious from the plot that it is logarithmic, but i also checked the differences
 *  between the values shown above, and i put them in a table (see: the excel file)
 *
 * Note: instead of size i used subSize, because i hate using reserved words
 *
**/

#include <iostream>
#include <fstream>
#include <time.h>   //for rand()
#include <cstdlib>  //for excel output

using namespace std;

float opSelect;
float opDelete;

///Type for a node of the Order Statistic Tree
typedef struct _node
{
    int key;
    int subSize;   //number of nodes is node's subtree, including itself
    struct _node *left;
    struct _node *right;
}node;


///Builds a (balanced) OS Tree with nodes having keys in range [s,e]
///Divide and Conquer approach
node *buildTree(int s, int e)
{
    if (s > e) return NULL;     //stopping condition, repeat until in a valid range

    int mid = (s + e) / 2;      //middle key

    node *toAdd = new node;
    toAdd->key = mid;               //key of root will be the middle element
    toAdd->subSize = e - s + 1;     //size of subtree = itself and all nodes below = number of nodes in the interval

    toAdd->left = buildTree(s, mid-1);      //continue building left subtree = in range [s,mid-1]
    toAdd->right = buildTree(mid+1, e);     //continue building right subtree = in range [mid+1,e]

    return toAdd;   //in the last step it will return the root itself
}


///Pretty Print to check if buildTree works
void preorder(node *root, int spaceCount)
{
    if (root != NULL)
    {
        for (int i=0; i<spaceCount; i++) cout<<"  ";
        cout<<root->key<<"-"<<root->subSize<<endl;
        preorder(root->left,spaceCount+1);
        preorder(root->right,spaceCount+1);
    }
    return;
}

//problem always simplifies on working with one tree we do not care about what is outside
///Gets the i'th smallest element using the properties of OS Trees
node *osSelect(node *root, int i)
{
    if (root == NULL)   //we would get here if a non-existent node was searched
        return NULL;    //should not occur in our testing/ evaluating phases

    ///get order of root
    int r;  //the order of the root we are currently working on

    opSelect++;
    if (root->left == NULL) //no predecessors
        r = 1;              //node has order 1
    else
    {
        r = root->left->subSize + 1;    //since it is a binary tree, all nodes on left are smaller
        opSelect++;                     //x elements smaller than root = root is the x+1'th element
    }

    ///check cases, knowing order of root
    if (i == r)
        return root;                        //found the desired node
    else if (i < r)
    {
        opSelect++;
        return osSelect(root->left,i);      //continue searching on left subtree
    }
    else
    {
        opSelect++;                         //continue searching on right subtree
        return osSelect(root->right,i-r);   //i-r since r nodes "skipped" on the left
    }
}


///Deletes the i'th smallest element from the tree
void osDelete(node* &root, int i)
{
    //deal with fixing size - easiest to do it at the beginning, since if we
    if (root != NULL)               //got to this point, it means the node is
            root->subSize--;        //already affected by the delete operation

    ///find the element - same approach as in osSelect
    if (root == NULL)   //deleting inexistent node
        return;         //should not reach

    int r;
    if (root->left == NULL)             //no predecessors
    {
        opDelete++;
        r = 1;
    }
    else                                //predecessors
    {
        opDelete += 2;//also count previous comparison
        r = root->left->subSize + 1;
    }


    if (i < r)      //not found, continue searching on left
    {
        opDelete++;
        return osDelete(root->left,i);
    }
    else if (i > r)      //not found, continue searching on right
    {
        opDelete++;
        return osDelete(root->right,i-r);
    }
    else if (i == r)
    {
        ///found node, delete it
        if (root->left != NULL) opDelete++;
            else opDelete+=2;
        if (root->left == NULL && root->right == NULL)  //case 1 : no children - replace node with null
            root = NULL;
        else if (root->right == NULL)                   //case 2 : only left child - replace with left child
        {
            opDelete++;
            root = root->left;
        }
        else if (root->left == NULL)                    //case 2 : only right child - replace with right child
        {
            opDelete+=2;//1 from previous 1 from here
            root = root->right;
        }
        else                                            //case 3 : both left and right child
        {
            opDelete+=5;//2 from previous 3 from here
            node *successor = osSelect(root->right,1);  //find successor = element with order 1 in the right subtree
            root->key = successor->key;                 //replace, but only the key!
            osDelete(root->right,1);                    //actually delete the successor (the one before modifying)
        }
    }
}

int main()
{
    cout<<"Demo of buildTree:\n";
    node *root = buildTree(1,11);
    preorder(root,0);
    cout<<endl;

    cout<<"Demo of osSelect:\n";
    cout<<osSelect(root,1)->key<<endl;
    cout<<osSelect(root,3)->key<<endl;
    cout<<osSelect(root,6)->key<<endl;
    cout<<osSelect(root,11)->key<<endl<<endl;

    cout<<"Demo of osDelete:\n";
    //osDelete(root,1);     //i recommend to use
    //osDelete(root,2);     //just one at a time
    //osDelete(root,6);     //out of these three
    //preorder(root,0);     //to make sure it works

    osDelete(root,1);   //original node 1 (deleted 0 nodes previously)
    preorder(root,0);
    osDelete(root,2);   //original node 3 (deleted 1 node previously)
    preorder(root,0);
    osDelete(root,4);   //original node 6 (deleted 2 nodes previously)
    preorder(root,0);
    osDelete(root,8);   //original node 11 (deleted 3 nodes previously)
    preorder(root,0);

    ///Evaluation
    srand(time(NULL));
    ofstream f("ostree_eval.csv");
    f<<"n , select , delete"<<endl;
    for (int n=100; n<=10000; n+=100)
    {
        opSelect = 0;
        opDelete = 0;
        for (int j=0; j<5; j++) //repeat 5 times for each n
        {
            node *testRoot = buildTree(1,n);
            //for (int i=0; i<n; i++) //repeat n times
            for (int i=0; i<n; i++)
            {
                int randomIndex = 1 + rand() % (n - i + 1);   //see explanations why
                osSelect(testRoot,randomIndex);
                osDelete(testRoot,randomIndex);
            }

        }
        opSelect /= (5 * n);    //since in each iteration
        opDelete /= (5 * n);    //we repeated n times
        f<<n<<" , "<<opSelect<<" , "<<opDelete<<endl;
    }
    return 0;
}
