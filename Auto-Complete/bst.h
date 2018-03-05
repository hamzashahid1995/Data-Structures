//  bst.h
//  Project03 - Auto-Complete
//
//  Created by Hamza Shahid on 2/6/17.
//  Copyright © 2017 Hamza Shahid. All rights reserved.
//b

// make sure this header file is #include exactly once:
#pragma once

// BST type declarations:
typedef char*  BSTKey;
typedef struct BSTValue
{
    char *Cites;
    long long Weight;
} BSTValue;

typedef struct BSTNode
{
    BSTKey    Key;
    BSTValue  Value;
    struct BSTNode  *Left;
    struct BSTNode  *Right;
} BSTNode;

typedef struct BST
{
    BSTNode *Root;
    int      Count;
} BST;


// BST API: function prototypes
BST *BSTCreate();
BST *BuildTree();

BSTNode *BSTSearch(BST *tree, BSTKey key);
BSTNode *BSTSearchPrefix(BSTNode *tree, BSTKey key);

BSTValue *BSTNumberOfMatches(BST *tree, int *counter, char *prefix, int subCount);
BSTValue *sorting (BSTValue arr[], int size);
BSTValue *BSTSorting(BST *tree);

int  BSTCompareKeys(BSTKey key1, BSTKey key2);
int LookForSubTree(BSTNode *Root, BSTKey key);
int  BSTSearchDepth(BST *tree, BSTKey key);
int  BSTInsert(BST *tree, BSTKey key, BSTValue value);

int BSTHeight(BST *tree);
int _BSTHeight(BSTNode *root);
int _max2(int x, int y);

int       BSTCount(BST *tree);
int _BSTCount(BSTNode *root);
int subtreecount(BSTNode *root);
int LookSubTree(BSTNode *Root, BSTKey key);

char *getFileName();

void BSTPrintInorder(BST *tree, void(*pf)(BSTNode*));
void BSTPrintPreorder(BST *tree, void(*pf)(BSTNode*));
void skipRestOfInput(FILE *stream);
void BST_NumberOfMatches(BSTNode *root, BSTValue values[], int *counter, char *prefix);
void PrintOneNode(BSTNode *cur);
void addCommand(BSTValue *addNode, char text[512], BST *tree);
void findCommand(BSTNode *findNode, char text[512], BST *tree);
void suggestCommand(BSTNode *prefixNode, char text[512], BST *tree);
void readFILE(long long weight, FILE *FILENAME, char text[512], int textsize, char *cp, BST *tree, BSTValue *value);




