/*main.cpp*/
//
// Auto-complete using a binary search tree.
//
// << Hamza Shahid >>
// U. of Illinois, Chicago
// CS251, Spring 2017
// Project 03
//
// ignore stdlib warnings if working in Visual Studio:
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "bst.h"


//=====================================================================
int main()
{
    char *filename;
    char  cmd[64];
    char  text[512];
    int   textsize = sizeof(text) / sizeof(text[0]);
    
    printf("** Starting Autocomplete **\n");
    
    filename = getFileName();
    FILE *FILENAME;
    FILENAME = fopen(filename, "r");
    
    BST *tree = BSTCreate();
    BSTValue value;
    
    long long weight = 0;
    char *cp = text;
    
    readFILE(weight, FILENAME, text, textsize, cp, tree, &value);
    
    // now interact with user:
    printf("** Ready **\n");
    scanf("%s", cmd);
    
    while (strcmp(cmd, "exit") != 0)
    {
        if (strcmp(cmd, "stats") == 0)
        {
            printf("**Tree count:  %d\n", BSTCount(tree));
            printf("**Tree Height: %d\n", BSTHeight(tree));
        }
        else if (strcmp(cmd, "add") == 0)
        {
            BSTValue addNode;
            addCommand(&addNode, text, tree);
        }
        else if (strcmp(cmd, "find") == 0)
        {
            BSTNode *findNode = NULL;
            findCommand(findNode, text, tree);
        }
        else if (strcmp(cmd, "suggest") == 0)
        {
            BSTNode *prefixNode = NULL;
            suggestCommand(prefixNode, text, tree);
        }
        else
        {
            printf("**unknown cmd, try again...\n");
        }
        scanf("%s", cmd);
    }
    printf("** Done **\n");
    
    return 0;
}
