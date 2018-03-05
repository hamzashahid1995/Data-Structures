/*avl.h*/

//
// AVL Tree ADT header file.
//
// <<Hamza Shahid>>
// U. of Illinois, Chicago
// CS251, Spring 2017
//

// make sure this header file is #include exactly once:
#pragma once


// AVL type declarations:
typedef int  AVLKey;

typedef struct STATION
{
    int  StationID;
    int  tripCount;
    char name[500];
    double latitude;
    double longitude;
    int dpcapacity;
    char online_date[500];
} STATION;

typedef struct TRIP
{
    int Data;
    int TripID;
    int BikeID;
    int toStationID;
    int  tripCount;
    int tripDuration;
    int secDuration;
    int fromStationID;
    char stopTime[100];
    char startTime[100];
    char toStationName[100];
    char fromStationName[500];
} TRIP;

typedef struct BIKE
{
    int  BikeID;
    int  Data;
    
} BIKE;

enum UNIONTYPE
{
    STATIONTYPE,
    TRIPTYPE,
    BIKETYPE
};

typedef struct AVLValue
{
    enum UNIONTYPE Type;  // Station, Trip, or Bike:
    union
    {
        STATION  Station;   // union => only ONE of these is stored:
        TRIP     Trip;
        BIKE     Bike;
    };
} AVLValue;

typedef struct AVLNode
{
    AVLKey    Key;
    AVLValue  Value;
    struct AVLNode  *Left;
    struct AVLNode  *Right;
    int       Height;
} AVLNode;

typedef struct AVL
{
    AVLNode *Root;
    int      Count;
} AVL;


//==============================================================================================
// AVL API: function prototypes
AVL *AVLCreate();
AVLNode *AVLSearch(AVL *tree, AVLKey key);
AVLNode *AVLSearch(AVL *tree, AVLKey key);
AVLValue *sorting(AVLValue arr[], int size);
AVLValue *AVLNumberOfMatches(AVL *tree, double latitude, double longitude, double distance, int *counter);


void _AVLFree(AVLNode *root, void(*fp)(AVLKey key, AVLValue value));
void AVLFree(AVL *tree, void(*fp)(AVLKey key, AVLValue value));
void PrintOneNode(AVLNode *cur);
void ParseTripsData(char text[], int *tripID, char startTime[], char stopTime[],
                   int *BikeID, int *tripDuration, int *fromStationID,
                   char fromStationName[], int *toStationID, char toStationName[100]);
void ParseStationsData(char text[], int *stationID, char name[], double *latitude,
                      double *longitude, int *dpcapacity, char online_date[]);
void AVL_NumberOfMatches(AVLNode *root, AVLValue values[], double latitude, double longitude, double distance, int *counter);
void AVLRoute(AVLNode *root, int *counter, int toStation, int fromStation);
void statsCommand(AVL *stations, AVL *trips, AVL *bikes);
void stationCommand(AVLNode *stationNode, AVL *stations);
void tripCommand(AVLNode *tripNode, AVL *trips);
void bikeCommand(AVLNode *bikeNode, AVL *bikes);
void findCommand(AVLValue *findNode, AVLValue *temp, AVL *stations);
void routeCommand(AVLNode *routeNode, AVL *trips, AVLNode *nodeStat, AVLNode *nodeDest, AVL *stations);
void skipRestOfInput(FILE *stream);
void freeAVLNodeData(AVLKey key, AVLValue value);
void tripCountStations(AVLNode *FromTripFound, AVLNode *ToTripFound, AVL *stations,
                       int fromStationID, int toStationID, AVL *trips, AVLValue tripValue);
void tripCountBikes(AVLNode *bikeFound, AVL *bikes, int BikeID, AVLValue valueBike);


char *getFileName();


int      AVLCompareKeys(AVLKey key1, AVLKey key2);
int      AVLInsert(AVL *tree, AVLKey key, AVLValue value);
int  AVLCount(AVL *tree);
int  AVLHeight(AVL *tree);

