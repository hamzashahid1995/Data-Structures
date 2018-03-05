//  bst.c
//  Project03 - Auto-Complete
//
//  Created by Hamza Shahid on 2/6/17.
//  Copyright © 2017 Hamza Shahid. All rights reserved.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "bst.h"


//==========================================================================================================
// BSTCreate: dynamically creates and returns an empty
// binary search tree:
BST *BSTCreate()
{
    BST *tree;
    
    tree = (BST *)malloc(sizeof(BST));
    tree->Root = NULL;
    tree->Count = 0;
    
    return tree;
}


//==========================================================================================================
// BSTCompareKeys: compares key1 and key2, returning
//   value < 0 if key1 <  key2
//   0         if key1 == key2
//   value > 0 if key1 >  key2
int BSTCompareKeys(BSTKey key1, BSTKey key2)
{
    if (strcmp(key1, key2) < 0)
        return -1;
    else if (strcmp(key1, key2) == 0)
        return 0;
    else
        return 1;
}


//==========================================================================================================
int _BSTCount(BSTNode *root)
{
    
    if (root == NULL)
        return 0;
    else
        return 1 + _BSTCount(root->Left) + _BSTCount(root->Right);
}


//==========================================================================================================
int BSTCount(BST *tree)
{
    return _BSTCount(tree->Root);
}


//==========================================================================================================
// BSTSearch: searches the binary search tree for a node containing the
// same key.  If a match is found, a pointer to the node is returned,
// otherwise NULL is returned.
BSTNode *BSTSearch(BST *tree, BSTKey key)
{
    BSTNode *cur = tree->Root;
    
    // search the tree to see if it contains a matching key:
    while (cur != NULL)
    {
        if (BSTCompareKeys(key, cur->Key) == 0)                    // found!
            return cur;
        else if (BSTCompareKeys(key, cur->Key) < 0)                // smaller, go left:
        {
            cur = cur->Left;
        }
        else                                                    // larger, go right:
        {
            cur = cur->Right;
        }
    }
    // if we get here, we fell out of the tree, and didn't find it:
    return NULL;
}


//==========================================================================================================
BSTNode *BSTSearchPrefix(BSTNode *tree, BSTKey key)
{
    // search the tree to see if it contains a matching key:
    if (tree == NULL)
    {
            return NULL;
    }
    if (strncmp(tree->Key, key, strlen(key)) == 0)
    {
            return tree;
    }
    else if (strncmp(tree->Key, key, strlen(key)) < 0)
    {                                                           // smaller, go left:
            return BSTSearchPrefix(tree->Right, key);
    }
    else
    {
            return BSTSearchPrefix(tree->Left, key);
    }
}


//==========================================================================================================
int LookForSubTree(BSTNode *Root, BSTKey key)
{
    BSTNode *root = Root;
    int counter = 0;
    // search the tree to see if it contains a matching key:
    while (root != NULL)
    {
        if (strncmp(root->Key, key, strlen(key)) < 0)
        {                                                   // smaller, go left:
            root= root->Right;
        }
        else if (strncmp(root->Key, key, strlen(key)) > 0)
        {
            root=root->Left;
        }
        
        else
        {
            counter = _BSTCount(root);
            break;                          //break out
        }
    }
    return counter;
}


//==========================================================================================================
int _max2(int x, int y)
{
    return (x > y) ? x : y;
}


//==========================================================================================================
int _BSTHeight(BSTNode* root)
{
    if (root == NULL)
    {
        return -1;
    }
    else
    {
        return 1 + _max2(_BSTHeight(root->Left), _BSTHeight(root->Right));
    }
}


//==========================================================================================================
int BSTHeight (BST* tree)
{
    return _BSTHeight(tree->Root);
}


//==========================================================================================================
// BSTInsert: inserts the given (key, value) pair into the binary search
// tree.  Returns true (non-zero) if the insert was successful, returns
// false (0) if the given key is already in the tree -- in which case the
// given (key, value) pair is not inserted.
int BSTInsert(BST *tree, BSTKey key, BSTValue value)
{
    BSTNode *prev = NULL;
    BSTNode *cur = tree->Root;
    
    // first we search the tree to see if it already contains key:
    while (cur != NULL)
    {
        if (BSTCompareKeys(key, cur->Key) == 0)  // already in tree, failed:
            return 0;
        else if (BSTCompareKeys(key, cur->Key) < 0)  // smaller, go left:
        {
            prev = cur;
            cur = cur->Left;
        }
        else  // larger, go right:
        {
            prev = cur;
            cur = cur->Right;
        }
    }
    
    // If we get here, tree does not contain key, so insert new node
    // where we fell out of tree:
    BSTNode *T = (BSTNode *)malloc(sizeof(BSTNode));
    T->Key = key;
    T->Value = value;
    T->Left = NULL;
    T->Right = NULL;
    
    // link T where we fell out of tree -- after prev:
    if (prev == NULL)  // tree is empty, insert @ root:
    {
        tree->Root = T;
    }
    else if (BSTCompareKeys(key, prev->Key) < 0)  // smaller, insert to left:
    {
        prev->Left = T;
    }
    else  // larger, insert to right:
    {
        prev->Right = T;
    }
    
    tree->Count++;
    
    return 1;  // success:
}


//==========================================================================================================
// BSTPrintInorder: prints the nodes of the given binary search
// tree inorder to the console.
//
// NOTE: "pf" is a print function that must be declared like this
// (though the name "pf" doesn't really matter):
//
//  void pf(BSTNode *cur)
//  { ...}
//
// When you call, pass pf and then you'll be able to control
// what gets printed when a node is "visited".
void _BSTPrintInorder(BSTNode *root, void (*pf)(BSTNode*))
{
    if (root == NULL)  // base case: empty tree
        return;
    else  // recursive case: non-empty tree
    {
        _BSTPrintInorder(root->Left, pf);
        pf(root);
        _BSTPrintInorder(root->Right, pf);
    }
}


//==========================================================================================================
void BSTPrintInorder(BST *tree, void(*pf)(BSTNode*))
{
    printf(">>Inorder: %d node(s)\n", tree->Count);
    
    _BSTPrintInorder(tree->Root, pf);
    
    printf(">><<\n");
}


//==========================================================================================================
// BSTPrintPreorder: prints the nodes of the given binary search
// tree pre-order to the console.
//
// NOTE: "pf" is a print function that must be declared like this
// (though the name "pf" doesn't really matter):
//
//  void pf(BSTNode *cur)
//  { ...}
//
// When you call, pass pf and then you'll be able to control
// what gets printed when a node is "visited".
void _BSTPrintPreorder(BSTNode *root, void(*pf)(BSTNode*))
{
    if (root == NULL)  // base case: empty tree
        return;
    else  // recursive case: non-empty tree
    {
        pf(root);
        _BSTPrintPreorder(root->Left, pf);
        _BSTPrintPreorder(root->Right, pf);
    }
}


//==========================================================================================================
void BSTPrintPreorder(BST *tree, void(*pf)(BSTNode*))
{
    printf(">>Preorder: %d node(s)\n", tree->Count);
    _BSTPrintPreorder(tree->Root, pf);
    
    printf(">><<\n");
}


//==========================================================================================================
void BST_NumberOfMatches(BSTNode *root, BSTValue values[], int *counter, char *prefix)
{
    
    if(root == NULL)
    {
        return;
    }
    else 
    {
        BST_NumberOfMatches(root->Left, values, counter,prefix);          //prints the address of the pointer
        if (strncmp(root->Key, prefix, strlen(prefix)) == 0)
        {
        values[*counter].Cites = root->Value.Cites;
        values[*counter].Weight = root->Value.Weight;
        (*counter)+=1;
        }
        BST_NumberOfMatches(root->Right, values, counter,prefix);          //prints the address of the pointer
    }
}


//==========================================================================================================
// *BSTNumberOfMatches: traverses the tree inorder so as to yield the (key, value)
// pairs in sorted order by key.  Returns a dynamically-allocated array
// of size N, containing a copy of the value in each tree node.  The
// value N can be obtained by calling BSTCount().
//
// NOTE: the caller is responsible for freeing the returning array.
BSTValue *BSTNumberOfMatches(BST *tree, int *counter, char *prefix, int subCount)
{
    BSTValue *values = (BSTValue *)malloc(tree->Count * sizeof(BSTValue));

    // fill values by traversing trees
    BST_NumberOfMatches(tree->Root, values, counter, prefix);
    
    return values;
}


//==========================================================================================================
BSTValue *sorting(BSTValue arr[], int size)
{
    int i = 0;
    int j = 0;
    BSTValue temporary;
    
    for (i = 0; i < size; ++i)
    {
        for (j = i + 1; j < size; j++)
        {
            if (arr[i].Weight < arr[j].Weight)
            {
                temporary = arr[i];
                arr[i] = arr[j];
                arr[j] = temporary;
            }
            else if (arr[i].Weight == arr[j].Weight && (strcmp(arr[i].Cites, arr[j].Cites) > 0))
            {
                temporary = arr[i];
                arr[i] = arr[j];
                arr[j] = temporary;
            }
        }
    }
    return arr;
}

//==========================================================================================================
// skipRestOfInput:
//
// Inputs and discards the remainder of the current line for the
// given input stream, including the EOL character(s).
void skipRestOfInput(FILE *stream)
{
    char restOfLine[256];
    int rolLength = sizeof(restOfLine) / sizeof(restOfLine[0]);
    
    fgets(restOfLine, rolLength, stream);
}


//==========================================================================================================
// getFileName: inputs a filename from the keyboard, make sure the file can be
// opened, and returns the filename if so.  If the file cannot be opened, an
// error message is output and the program is exited.
char *getFileName()
{
    char filename[512];
    int  fnsize = sizeof(filename) / sizeof(filename[0]);
    
    // input filename from the keyboard:
    fgets(filename, fnsize, stdin);
    filename[strcspn(filename, "\r\n")] = '\0';  // strip EOL char(s):
    
    // make sure filename exists and can be opened:
    FILE *infile = fopen(filename, "r");
    if (infile == NULL)
    {
        printf("**Error: unable to open '%s'\n\n", filename);
        exit(-1);
    }
    
    fclose(infile);
    
    // duplicate and return filename:
    char *s = (char *)malloc((strlen(filename) + 1) * sizeof(char));
    strcpy(s, filename);
    
    return s;
}


//==========================================================================================================
// BuildTree:
//
// Inputs data from keyboard, stores in a BST, and returns tree.
BST *BuildTree()
{
    int  population;
    char cites[64];
    BSTValue  value;
    
    BST *tree = BSTCreate();
    
    // input:
    //   UIN Midterm
    //   UIN Midterm
    //   ...
    //   -1
    scanf("%s", cites);  // first input the netid:
    
    while (cites[0] != '#')
    {
        scanf("%d", &population);  // now input midterm & final scores:
        skipRestOfInput(stdin);
        
        // store Netid, Midterm, Final into a BST value struct to insert:
        value.Cites = (char *)malloc((strlen(cites) + 1) * sizeof(char));
        strcpy(value.Cites, cites);
        value.Weight = population;
        
        // now that we have (key, value) pair, call BSTInsert to
        // copy this (key, value) pair into a new node of the tree:
        BSTInsert(tree, value.Cites, value);
        
        scanf("%s", cites);  // get next netid or #:
    }
    
    // done, return tree:
    return tree;
}


//==========================================================================================================
// PrintOneNode:
//
// Prints the info in one node of the tree.  This function is passed
// to BST inorder/preorder functions to control printing when a node
// is "visited".
void PrintOneNode(BSTNode *cur)
{
    printf("%s: %lld\n", cur->Value.Cites, cur->Value.Weight);
}


//==========================================================================================================
void addCommand(BSTValue *addNode, char text[512], BST *tree)
{
    long long weight;
    char      part2[512];
    int       part2size = sizeof(part2) / sizeof(part2[0]);
    
    //
    // add weight text
    //
    scanf("%lld %s", &weight, text);
    fgets(part2, part2size, stdin);
    part2[strcspn(part2, "\r\n")] = '\0';  // strip EOL char(s):
    strcat(text, part2);
    
    addNode->Cites=(char*)malloc(strlen(text)+1 *sizeof(BSTValue));
    strcpy(addNode->Cites, text);
    
    addNode->Weight = weight;
    
    int i = BSTInsert(tree, addNode->Cites, *addNode);
    if (i == 1)
    {
        printf("**Added\n");
    }
    else
    {
        printf("**Not added...\n");
    }
}


//==========================================================================================================
void findCommand(BSTNode *findNode, char text[512], BST *tree)
{
    char part2[512];
    int  part2size = sizeof(part2) / sizeof(part2[0]);
    
    //
    // find text
    //
    scanf("%s", text);
    fgets(part2, part2size, stdin);
    part2[strcspn(part2, "\r\n")] = '\0';  // strip EOL char(s):
    strcat(text, part2);
    
    findNode = BSTSearch(tree, text);
    
    if (findNode == NULL)
    {
        printf("**Not found...\n");
    }
    else
    {
        printf("%s: %lld\n", text, findNode->Value.Weight);         //finds value and points to weight
    }
}


//==========================================================================================================
void suggestCommand(BSTNode *prefixNode, char text[512], BST *tree)
{
    int  k;
    char part2[512];
    int  part2size = sizeof(part2) / sizeof(part2[0]);
    
    //
    // suggest k text
    //
    scanf("%d %s", &k, text);
    fgets(part2, part2size, stdin);
    part2[strcspn(part2, "\r\n")] = '\0';  // strip EOL char(s):
    strcat(text, part2);
    
    prefixNode = BSTSearchPrefix(tree->Root, text);
    
    if (prefixNode == NULL)
    {
        printf("**No suggestions...\n");
        
    }
    
    else
    {
        printf("** [Sub-tree root:  (%s,%lld)]\n",prefixNode->Value.Cites,prefixNode->Value.Weight);
        
        int c = LookForSubTree(tree->Root, text);
        printf("** [Sub-tree count: %d]\n",c);
        
        BSTValue *temp;
        int hh = 0;
        temp = BSTNumberOfMatches(tree, &hh, text, c);
        
        printf("** [Num matches:    %d]\n",hh);
        
        // hh--;
        int t;
        
        BSTValue *S;
        S = sorting(temp,hh);
        
        for (t=0; t<hh; t++)
        {
            if (t < k)
            {
                printf("%s: %lld\n", S[t].Cites, S[t].Weight);
            }
        }
    }    
}


//==========================================================================================================
void readFILE(long long weight, FILE *FILENAME, char text[512], int textsize, char *cp, BST *tree, BSTValue *value)
{
    fscanf(FILENAME, "%lld",&weight);
    while (!feof(FILENAME))
    {
        fgets(text, textsize, FILENAME);
        // get rid of end of line characters
        text[strcspn(text, "\r\n")] = '\0';
        // advance white space
        while  (*cp == ' ' || *cp == '\t')
        {
            cp++;
        }
        value->Cites = (char*)malloc(strlen(cp)+1 *sizeof(char));
        strcpy(value->Cites, cp);
        value->Weight =  weight;
        BSTInsert(tree, value->Cites, *value);
        fscanf(FILENAME, "%lld",&weight);
    }
}
//==========================================================================================================
