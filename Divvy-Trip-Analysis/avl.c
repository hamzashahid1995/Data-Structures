/*avl.c*/

//
// AVL Tree ADT implementation file.
//
// <<Hamza Shahid>>
// U. of Illinois, Chicago
// CS251, Spring 2017
//

// ignore stdlib warnings if working in Visual Studio:
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <math.h>

#define TRUE 1
#define FALSE 0

#include "avl.h"


//==============================================================================================
// AVLCreate:
//
// Dynamically creates and returns an empty AVL tree.
AVL *AVLCreate()
{
    AVL *tree;
    
    tree = (AVL *)malloc(sizeof(AVL));
    tree->Root = NULL;
    tree->Count = 0;
    
    return tree;
}


//==============================================================================================
void _AVLFree(AVLNode *root, void(*fp)(AVLKey key, AVLValue value))
{
    if (root == NULL)
        return;
    else
    {
        fp(root->Key, root->Value);
        free(root);
    }
}

//==============================================================================================
// AVLFree:
//
// Frees the memory associated with the tree: the handle and the nodes.
// The provided function pointer is called to free the memory that
// might have been allocated as part of the key or value.
void AVLFree(AVL *tree, void(*fp)(AVLKey key, AVLValue value))
{

    //printf(">>>>> AVLFree needs to be implemented <<<<<\n");
    _AVLFree(tree->Root,fp);
    free(tree);
}


//==========================================================================================================
// BSTSearch: searches the binary search tree for a node containing the
// same key.  If a match is found, a pointer to the node is returned,
// otherwise NULL is returned.
AVLNode *AVLSearch(AVL *tree, AVLKey key)
{
    AVLNode *current;
    
    current = tree->Root;
    
    while (current != NULL)
    {
        if (key == current->Key)
            return current;
        else if (key < current->Key)
            current = current->Left;
        else
            current = current->Right;
    }
    return NULL;

}


//==============================================================================================
// AVLCompareKeys:
//
// Compares key1 and key2, returning
//   value < 0 if key1 <  key2
//   0         if key1 == key2
//   value > 0 if key1 >  key2
int AVLCompareKeys(AVLKey key1, AVLKey key2)
{
    if (key1 < key2)
        return -1;
    else if (key1 == key2)
        return 0;
    else
        return 1;
}


//==============================================================================================
// AVLCount:
//
// Returns # of nodes in the tree.
int AVLCount(AVL *tree)
{
    return tree->Count;
}


//==============================================================================================
// AVLHeight:
//
// Returns the overall height of the AVL tree.
int AVLHeight(AVL *tree)
{
    if (tree->Root == NULL)
        return -1;
    else
        return tree->Root->Height;
}



//==============================================================================================
int _height(AVLNode *cur)
{
    if (cur == NULL)
        return -1;
    else
        return cur->Height;
}


//==============================================================================================
int _max2(int x, int y)
{
    return (x > y) ? x : y;
}


//============================================================================================
// Rotate right the sub-tree rooted at node k2, return pointer
// to root of newly-rotated sub-tree --- i.e. return pointer
// to node k1 that was rotated up to top of sub-tree.  Heights
// of the rotated sub-tree are also updated by this function.
AVLNode *RightRotate(AVLNode *k2)
{
    // grab pointers to various components
    AVLNode *k1 = k2->Left;
    AVLNode *Y = k1->Right;
    
    // rotate k1 up and k2 down
    k1->Right = k2;
    k2->Left = Y;
    k2 = k1->Right;
    
    // adjust heights (be careful the order you do this)
    k2->Height = 1 + _max2(_height(k2->Left), _height(k2->Right));
    k1->Height = 1 + _max2(_height(k1->Left), _height(k1->Right));
    
    // return the root of the new subtree so
    // caller can link back in main tree
    return k1;
}


//============================================================================================
// Rotate left the sub-tree rooted at node k1, return pointer
// to root of newly-rotated sub-tree --- i.e. return pointer
// to node k2 that was rotated up to top of sub-tree.  Heights
// of the rotated sub-tree are also updated by this function.
AVLNode *LeftRotate(AVLNode *k1)
{
    // grab pointers to various components
    AVLNode *k2 = k1->Right;
    AVLNode *Y = k2->Left;
    
    // rotate k1 up and k2 down
    k2->Left = k1;
    k1->Right = Y;
    k1 = k2->Left;
    
    // adjust heights (be careful the order you do this)
    k1->Height = 1 + _max2(_height(k1->Left), _height(k1->Right));
    k2->Height = 1 + _max2(_height(k2->Left), _height(k2->Right));
    
    // return the root of the new subtree so
    // caller can link back in main tree
    return k2;
}


//==============================================================================================
// AVLInsert:
//
// This is a partial implementation of insert: search, insert, and
// walk back up the tree.  Read carefully to understand what value
// is returned by the function...
//
// Suppose P is the path traversed during the search for the
// insertion point.  If a node is encountered with the same key,
// then the function
//
//    returns 0
//
// Otherwise, Let P' be the reverse path back up the tree.  As
// the function walks up path P', it does the following:
//
//   a. N = pop()
//   b. compute the height of N
//   c. if N's height has not changed, returns N's key
//   d. if AVL condition is broken @ N, returns -(N's key), i.e.
//      the negative value of N's key (without updating height)
//   e. otherwise updates N's Height and continues walking P'
//
// If the function walks up the entire path P' updating the height
// of every node, then the function returns the length of P' ---
// i.e. the # of nodes that were updated.
//
// NOTE: when the first node is inserted, P' is empty, and so the
// function returns 0 (the length of P').
int AVLInsert(AVL *tree, AVLKey key, AVLValue value)
{
    // create a stack
    AVLNode *stack[64];
    int top = -1;
    
    // create two nodes for
    // current and previous
    AVLNode *current;
    AVLNode *previous;
    
    // set the previous node to null
    previous = NULL;
    
    //set the current node to root
    current = tree->Root;
    
    // declare a boolean
    bool rebalance = FALSE;
    
    //****************************SEARCH****************************//
    // search for the insertion point
    while (current != NULL)
    {
        // we have to push current on the top of
        // the stack as we traverse down the tree
        top++;
        stack[top] = current;
        
        // the node we want to insert
        // is already in the tree
        if (key == current->Key)
            return 0;
        // the node we want to insert does exist
        // and shall go to the left of the tree
        else if (key < current->Key)
        {
            previous = current;
            current = current->Left;
        }
        // the node we want to insert does exist
        // and shall go to the right of the tree
        else
        {
            previous = current;
            current = current->Right;
        }
    }
    
    //****************************INSERT****************************//
    // create a new node becuase the node we
    // want to insert does not already exist
    AVLNode *newNode = (AVLNode*)malloc(sizeof(AVLNode));
    newNode->Key = key;
    newNode->Value = value;
    newNode->Height = 0;
    newNode->Left = NULL;
    newNode->Right = NULL;
    
    // if the tree is empty
    if (previous == NULL)
        tree->Root = newNode;
    
    // if we go to the left
    else if (key < previous->Key)
        previous->Left = newNode;
    // if we go to the right
    else
        previous->Right = newNode;
    
    tree->Count++;
    
    //****************************WALK BACK UP****************************//
    AVLNode *parent = NULL;
    
    // walk back up the tree
    while (top > -1)
    {
        parent = stack[top];
        top--;
        
        // compute the new height of newNode
        int hLeft = _height(parent->Left);
        int hRight = _height(parent->Right);
        int newHeight = 1 + _max2(hLeft, hRight);
        
        // if the nodes height haven't
        // changed, then still an AVL tree
        if (parent->Height == newHeight)
        {
            rebalance = FALSE;
            // return newNode's key
            break;
        }
        // if the nodes break the AVL
        // condition then rebalance
        else if (abs(hLeft - hRight) > 1)
        {
            rebalance = TRUE;
            // the negative value of N's
            // key (without updating height)
            break;
        }
        // update and continue up the tree
        else
            parent->Height = newHeight;
    }
    
    //****************************REBALANCE****************************//
    if (rebalance)
    {
        if (top < 0)
            previous = NULL;
        else
            previous = stack[top];
        
        if (AVLCompareKeys(newNode->Key, parent->Key) < 0)
        {
            if (AVLCompareKeys(newNode->Key, parent->Left->Key) == 0) {
                return TRUE;
            }
            else if (AVLCompareKeys(newNode->Key, parent->Left->Key) < 0)
            {
                parent = RightRotate(parent);
            }
            else
            {
                parent->Left = LeftRotate(parent->Left);
                parent = RightRotate(parent);
            }
        }
        else
        {
            if (AVLCompareKeys(newNode->Key, parent->Right->Key) == 0) {
                return TRUE;
            }
            else if (AVLCompareKeys(newNode->Key, parent->Right->Key) < 0)
            {
                parent->Right = RightRotate(parent->Right);
                parent = LeftRotate(parent);
            }
            else
            {
                parent = LeftRotate(parent);
            }
        }
        
        if (previous == NULL)
            tree->Root = parent;
        // check if brandnewNode is previous's left
        else if (previous->Key < parent->Key)
            previous->Right = parent;
        // check if brandnewNode is previous's right
        else
            previous->Left = parent;
    }
    return TRUE;
}


//==============================================================================================
// AVLValue: function that sorts the stations tree in ascending order first by
// the distance we got from the distanceBetween2Points function. If the distance
// we get is repeated, we then sort the stations tree by the station ID.
AVLValue *sorting(AVLValue arr[], int size)
{
    int i = 0;
    int j = 0;
    AVLValue temporary;
    
    for (i = 0; i < size; ++i)
    {
        for (j = i + 1; j < size; j++)
        {
            if (arr[i].Station.latitude > arr[j].Station.latitude)
            {
                temporary = arr[i];
                arr[i] = arr[j];
                arr[j] = temporary;
            }
            else if (arr[i].Station.latitude == arr[j].Station.latitude && ((arr[i].Station.StationID > arr[j].Station.StationID)))
            {
                temporary = arr[i];
                arr[i] = arr[j];
                arr[j] = temporary;
            }
        }
    }
    return arr;
}

//==============================================================================================
// distBetween2Points:
//
// Returns the distance in miles between 2 points (lat1, long1) and (lat2, long2).
double distBetween2Points(double lat1, double long1, double lat2, double long2)
{
    //
    // Reference: http://www8.nau.edu/cvm/latlon_formula.html
    //
    double PI = 3.14159265;
    double earth_rad = 3963.1;  // statue miles:
    
    double lat1_rad = lat1 * PI / 180.0;
    double long1_rad = long1 * PI / 180.0;
    double lat2_rad = lat2 * PI / 180.0;
    double long2_rad = long2 * PI / 180.0;
    
    double dist = earth_rad * acos(
                                   (cos(lat1_rad)*cos(long1_rad)*cos(lat2_rad)*cos(long2_rad))
                                   +
                                   (cos(lat1_rad)*sin(long1_rad)*cos(lat2_rad)*sin(long2_rad))
                                   +
                                   (sin(lat1_rad)*sin(lat2_rad))
                                   );
    
    return dist;
}


//==========================================================================================================
// *AVLNumberOfMatches: traverses the tree inorder so as to yield the (key, value)
// pairs in sorted order by key.  Returns a dynamically-allocated array
// of size N, containing a copy of the value in each tree node.  The
// value N can be obtained by calling AVLCount().
//
// NOTE: the caller is responsible for freeing the returning array
void AVL_NumberOfMatches(AVLNode *root, AVLValue values[], double latitude,
                         double longitude, double distance, int *counter)
{
    double positionFound;
    if(root == NULL)
        return;
    else
    {
        AVL_NumberOfMatches(root->Left, values, latitude, longitude, distance, counter);
        positionFound = distBetween2Points(latitude, longitude, root->Value.Station.latitude, root->Value.Station.longitude);

        if (positionFound <= distance)
        {
            values[*counter].Station.StationID = root->Value.Station.StationID;
            values[*counter].Station.latitude = positionFound;
            (*counter)++;
        }
        AVL_NumberOfMatches(root->Right, values, latitude, longitude, distance, counter);
    }
}


//==========================================================================================================
// *AVLNumberOfMatches: traverses the tree inorder so as to yield the (key, value)
// pairs in sorted order by key.  Returns a dynamically-allocated array
// of size N, containing a copy of the value in each tree node.  The
// value N can be obtained by calling AVLCount().
//
// NOTE: the caller is responsible for freeing the returning array.
AVLValue *AVLNumberOfMatches(AVL *tree, double latitude, double longitude, double distance, int *counter)
{
    AVLValue *values = (AVLValue *)malloc(tree->Count * sizeof(AVLValue));
    
    // fill values by traversing trees
    AVL_NumberOfMatches(tree->Root, values, latitude, longitude, distance, counter);
    
    return values;
}


//==========================================================================================================
void AVLRoute(AVLNode *root, int *counter, int toStation, int fromStation)
{
    if(root == NULL)
        return;
    else
    {
        AVLRoute(root->Left, counter, toStation, fromStation);
    
        if ((root->Value.Trip.toStationID == toStation) && (root->Value.Trip.fromStationID == fromStation))
            (*counter)++;
        AVLRoute(root->Right, counter, toStation, fromStation);
    }
}


//==========================================================================================================
void statsCommand(AVL *stations, AVL *trips, AVL *bikes)
{
    //
    // Output some stats about our data structures:
    //
    printf("** Trees:\n");
    
    printf("   Stations: count = %d, height = %d\n",
           AVLCount(stations), AVLHeight(stations));
    printf("   Trips:    count = %d, height = %d\n",
           AVLCount(trips), AVLHeight(trips));
    printf("   Bikes:    count = %d, height = %d\n",
           AVLCount(bikes), AVLHeight(bikes));
    
}


//==========================================================================================================
void stationCommand(AVLNode *stationNode, AVL *stations)
{
    int IDforStations = 0;
    scanf("%d", &IDforStations);
    
    //    AVLNode *stationNode;
    stationNode = AVLSearch(stations, IDforStations);
    
    if (stationNode == NULL)
    {
        printf("**not found\n");
    }
    else
    {
        printf("**Station %d:\n", stationNode->Value.Station.StationID);
        printf("  Name: '%s'\n", stationNode->Value.Station.name);
        printf("  Location:   (%1f,%1f)\n", stationNode->Value.Station.latitude, stationNode->Value.Station.longitude);
        printf("  Capacity:   %d\n", stationNode->Value.Station.dpcapacity);
        printf("  Trip count: %d\n", stationNode->Value.Station.tripCount);
    }
}


//==========================================================================================================
void tripCommand(AVLNode *tripNode, AVL *trips)
{
    int IDforTrips = 0;
    scanf("%d", &IDforTrips);
    
    tripNode = AVLSearch(trips,IDforTrips);
    
    if (tripNode == NULL)
    {
        printf("**not found\n");
    }
    else
    {
        printf("**Trip %d:\n", tripNode->Value.Trip.TripID);
        printf("  Bike: %d\n", tripNode->Value.Trip.BikeID);
        printf("  From: %d\n", tripNode->Value.Trip.fromStationID);
        printf("  To:   %d\n", tripNode->Value.Trip.toStationID);
        printf("  Duration: %d min, %d secs\n", tripNode->Value.Trip.tripDuration, tripNode->Value.Trip.secDuration);
    }
}


//==========================================================================================================
void bikeCommand(AVLNode *bikeNode, AVL *bikes)
{
    int bikesearch = 0;
    scanf("%d", &bikesearch);
    
    // search the bike tree for the ID
    bikeNode = AVLSearch(bikes,bikesearch);
    
    if (bikeNode == NULL)
    {
        printf("**not found\n");
    }
    else
    {
        printf("**Bike %d:\n", bikeNode->Value.Bike.BikeID);
        printf("  Trip count: %d\n", bikeNode->Value.Bike.Data);
    }
}


//==========================================================================================================
void findCommand(AVLValue *findNode, AVLValue *temp, AVL *stations)
{
    int i;
    int counter = 0;
    double userLatitude;
    double userLongitude;
    double dist;
    
    scanf("%lf", &userLatitude);
    scanf("%lf", &userLongitude);
    scanf("%lf", &dist);
    
    findNode = AVLNumberOfMatches(stations, userLatitude, userLongitude, dist, &counter);
    
    if (findNode == NULL)
        printf("**not found\n");
    else
    {
        AVLValue *temp;
        temp = sorting(findNode, counter);
        
        for (i = 0; i < counter; i++)
        {
            printf("Station %d: distance %f miles\n", temp[i].Station.StationID, temp[i].Station.latitude);
        }
    }
}


//==========================================================================================================
void routeCommand(AVLNode *routeNode, AVL *trips, AVLNode *nodeStat, AVLNode *nodeDest, AVL *stations)
{
    int TRIPS_id = 0;
    double distance;
    
    // variables that will store the station IDs
    // of FROM and TO from the trip tree
    int STATIONS = 0;
    int DESTINATIONS = 0;
    
    scanf("%d", &TRIPS_id);
    scanf("%lf", &distance);
    
    // searching the trips tree's ID by calling AVLSearch
    // and storing it into route node (calling the trips tree)
    // since that includes the tripIDs
    
    routeNode = AVLSearch(trips, TRIPS_id);
    
    if (routeNode == NULL)
    {
        printf("**not found\n");
        //        continue;
    }
    
    // access FROM and TO station IDs inside the trip tree
    // with the node we just created and then store it
    STATIONS = (routeNode->Value.Trip.fromStationID);
    DESTINATIONS = routeNode->Value.Trip.toStationID;
    
    
    nodeStat = AVLSearch(stations, STATIONS);
    nodeDest = AVLSearch(stations, DESTINATIONS);
    
    if ( nodeStat == NULL || nodeDest == NULL)
        printf("**not found\n");
    else
    {
        int counterForStations = 0;
        int counterForDestinations = 0;
        
        // node for stations and destinations
        AVLValue *valueForStations;
        AVLValue *valueForDestinations;
        
        valueForStations = AVLNumberOfMatches(stations, nodeStat->Value.Station.latitude,
                                              nodeStat->Value.Station.longitude, distance, &counterForStations);
        
        valueForDestinations = AVLNumberOfMatches(stations, nodeDest->Value.Station.latitude,
                                                  nodeDest->Value.Station.longitude, distance, &counterForDestinations);
        
        int k = 0;
        int j = 0;
        int counterForTRIPCOUNTS = 0;
        
        for (k = 0; k < counterForStations; k++)
        {
            for (j = 0; j < counterForStations; j++)
            {
                AVLRoute(trips->Root, &counterForTRIPCOUNTS, valueForDestinations[j].Station.StationID,
                         valueForStations[k].Station.StationID);
            }
        }
        printf("** Route: from station #%d to station #%d\n", routeNode->Value.Trip.fromStationID, routeNode->Value.Trip.toStationID);
        printf("** Trip count: %d\n", counterForTRIPCOUNTS);
        printf("** Percentage: %1f%%\n", ((double)counterForTRIPCOUNTS / (double)trips->Count)*100);
    }
}

//==============================================================================================
// ParseStationsData: function that parses all the data that is in the stations.csv file.
// Since we have 6 things in the file, we create 6 tokens and parse the data which inlcudes
// stationID, name, latitude, longitude, dpcapacity, online_date
void ParseStationsData(char text[], int *stationID, char name[], double *latitude,
                       double *longitude, int *dpcapacity, char online_date[])
{
    // strip end-of-line (EOL) chars to handle platform differences:
    text[strcspn(text, "\r\n")] = '\0';
    
    char *token1;
    token1 = strtok(text, ",");
    *stationID = atoi(token1);
    
    char *token2;
    token2 = strtok(NULL, ",");
    strcpy(name, token2);
    
    char *token3;
    token3 = strtok(NULL, ",");
    *latitude = atof(token3);
    
    char *token4;
    token4 = strtok(NULL, ",");
    *longitude = atof(token4);
    
    char *token5;
    token5 = strtok(NULL, ",");
    *dpcapacity = atoi(token5);
    
    char *token6;
    token6 = strtok(NULL, ",");
    strcpy(online_date, token6);
}


//==============================================================================================
// ParseTripsData: function that parses all the data that is in the trips.csv file.
// Since we have 12 things in the file, we create 12 tokens and parse the data which inlcudes
// tripsID, startTime, stopTime, bikeID, tripDuration, fromStationID, formStationName,
// toStationID, toStationName
void ParseTripsData(char text[], int *tripID, char startTime[], char stopTime[],
                    int *BikeID, int *tripDuration, int *fromStationID,
                    char fromStationName[], int *toStationID, char toStationName[100])
{
    // strip end-of-line (EOL) chars to handle platform differences:
    text[strcspn(text, "\r\n")] = '\0';
    
    char *token1;
    token1 = strtok(text, ",");
    *tripID = atoi(token1);
    
    char *token2;
    token2 = strtok(NULL, ",");
    strcpy(startTime, token2);
    
    char *token3;
    token3 = strtok(NULL, ",");
    strcpy(stopTime, token3);
    
    char *token4;
    token4 = strtok(NULL, ",");
    *BikeID = atoi(token4);
    
    char *token5;
    token5 = strtok(NULL, ",");
    *tripDuration = atoi(token5);
    
    char *token6;
    token6 = strtok(NULL, ",");
    *fromStationID = atoi(token6);
    
    char *token7;
    token7 = strtok(NULL, ",");
    strcpy(fromStationName, token7);
    
    char *token8;
    token8 = strtok(NULL, ",");
    *toStationID = atoi(token8);
    
    char *token9;
    token9 = strtok(NULL, ",");
    strcpy(toStationName, token9);
    
    char *token10;
    token10 = strtok(NULL, ",");
    
    char *token11;
    token11 = strtok(NULL, ",");
    
    char *token12;
    token12 = strtok(NULL, ",");
}


//==============================================================================================
// freeAVLNodeData
//
// Works with AVLFree() to free the data inside (key, value) pairs.
void freeAVLNodeData(AVLKey key, AVLValue value)
{
    //
    // what we free depends on what type of value we have:
    //
    if (value.Type == STATIONTYPE)
    {
        
    }
    else if (value.Type == TRIPTYPE)
    {
        
    }
    else if (value.Type == BIKETYPE)
    {
        
    }
    else
    {
        printf("**ERROR: unexpected value type in freeAVLNodeData!\n\n");
        exit(-1);
    }
}


//==============================================================================================
// getFileName:
//
// Inputs a filename from the keyboard, make sure the file can be
// opened, and returns the filename if so.  If the file cannot be
// opened, an error message is output and the program is exited.
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


//==============================================================================================
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


//==============================================================================================
void tripCountStations(AVLNode *FromTripFound, AVLNode *ToTripFound, AVL *stations,
                       int fromStationID, int toStationID, AVL *trips, AVLValue tripValue)
{
    FromTripFound = AVLSearch(stations, fromStationID);
    
    if (FromTripFound != NULL)
        FromTripFound->Value.Station.tripCount++;
    
    ToTripFound = AVLSearch(stations, toStationID);
    
    if (ToTripFound != NULL)
        ToTripFound->Value.Station.tripCount++;
    
    AVLInsert(trips, tripValue.Trip.TripID, tripValue);
}


//==============================================================================================
void tripCountBikes(AVLNode *bikeFound, AVL *bikes, int BikeID, AVLValue valueBike)
{
    bikeFound = AVLSearch(bikes, BikeID);
    
    if (bikeFound != NULL)
        bikeFound->Value.Bike.Data++;
    else
    {
        AVLValue valueBike;
        valueBike.Type = BIKETYPE;
        valueBike.Bike.BikeID = BikeID;
        valueBike.Bike.Data = 1;
        AVLInsert(bikes, valueBike.Bike.BikeID, valueBike);
    }
}
