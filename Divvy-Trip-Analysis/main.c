/*main.cpp*/

//
// Divvy Bike Ride Route Analysis, using AVL trees.
//
// <<Hamza Shahid>>
// U. of Illinois, Chicago
// CS251, Spring 2017
// Project #04
//

// ignore stdlib warnings if working in Visual Studio:
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "avl.h"


//==============================================================================================
int main()
{
    printf("** Welcome to Divvy Route Analysis **\n");
    
    //
    // get filenames from the user/stdin:
    //
    char  cmd[64];
    char text[512];
    
    char *StationsFileName = getFileName();
    char *TripsFileName = getFileName();
    
    FILE *_filenameSTATIONS = fopen(StationsFileName, "r");
    FILE *_filenameTRIPS = fopen(TripsFileName, "r");
  
    //
    // As an example, create some trees and insert some
    // dummy (key, value) pairs:
    //
    AVL *stations = AVLCreate();
    AVL *trips = AVLCreate();
    AVL *bikes = AVLCreate();

//**************************************** STATIONS TREE ****************************************//
    //
    // Insert a new station:
    //
    AVLValue stationValue;
    
    // union holds a station:
    stationValue.Type = STATIONTYPE;
    
    int stationID = 0;
    int dpcapacity = 0;
    int tripCount = 0;
    double latitude = 0.0;
    double longitude = 0.0;
    char name[500];
    char buffer1[100];
    char online_date[10];

    // skip the first line of the file
    fgets(buffer1, 100, _filenameSTATIONS);
    
    // until we run out of data:
    while (fgets(text,500, _filenameSTATIONS) != NULL)
    {
        ParseStationsData(text, &stationID, name, &latitude,
                      &longitude,&dpcapacity, online_date);
        
        stationValue.Station.StationID = stationID;
        strcpy(stationValue.Station.name, name);
        stationValue.Station.latitude = latitude;
        stationValue.Station.longitude = longitude;
        stationValue.Station.dpcapacity = dpcapacity;
        strcpy(stationValue.Station.online_date, online_date);
        stationValue.Station.tripCount = tripCount;
        
        AVLInsert(stations, stationValue.Station.StationID, stationValue);
    }
    
//************************************* TRIPS & BIKES TREE *************************************//
    //
    // Insert a new trip & bike:
    //
    AVLValue tripValue;
    tripValue.Type = TRIPTYPE;              // union holds a trip:

    AVLValue bikeValue;
    bikeValue.Type = BIKETYPE;              // union holds a bike:
    
    int tripID = 0;
    int BikeID = 0;
    int toStationID = 0;
    int tripDuration = 0;
    int minDuration = 0;
    int fromStationID = 0;
    char buffer2[150];
    char stopTime[100];
    char startTime[100];
    char toStationName[100];
    char fromStationName[100];

    // skip the first line of the file
    fgets(buffer2, 150, _filenameTRIPS);
    
    // until we run out of data:
    while (fgets(text,500, _filenameTRIPS) != NULL)
    {
        ParseTripsData(text, &tripID, startTime, stopTime, &BikeID, &tripDuration,
                 &fromStationID, fromStationName, &toStationID, toStationName);
        
        minDuration = tripDuration;
        tripValue.Trip.TripID = tripID;
        strcpy(tripValue.Trip.startTime,startTime);
        strcpy(tripValue.Trip.stopTime, stopTime);
        tripValue.Trip.BikeID = BikeID;
        tripValue.Trip.tripDuration = tripDuration;
        
        tripValue.Trip.tripDuration = tripDuration/60;
        tripValue.Trip.secDuration = tripDuration%60;
        
        tripValue.Trip.fromStationID = fromStationID;
        strcpy(tripValue.Trip.fromStationName, fromStationName);
        tripValue.Trip.toStationID = toStationID;
        strcpy(tripValue.Trip.toStationName, toStationName);
        
        AVLNode *FromTripFound = NULL;
        AVLNode *ToTripFound = NULL;

        tripCountStations(FromTripFound, ToTripFound, stations, fromStationID,
                          toStationID, trips, tripValue);

        AVLNode *bikeFound = NULL;
        tripCountBikes(bikeFound, bikes, BikeID, bikeValue);
    }
    
    //
    // now interact with user:
    //
    printf("** Ready **\n");
    
  do {
        scanf("%s", cmd);
      
        if (strcmp(cmd, "stats") == 0)
            statsCommand(stations, trips, bikes);
      
        else if (strcmp(cmd, "station") == 0)
        {
            AVLNode *stationNode = NULL;
            stationCommand(stationNode, stations);
        }
      
        else if (strcmp(cmd, "trip") == 0)
        {
            AVLNode *tripNode = NULL;
            tripCommand(tripNode, trips);
        }
      
        else if (strcmp(cmd, "bike") == 0)
        {
            AVLNode *bikeNode = NULL;
            bikeCommand(bikeNode, bikes);
        }
      
        else if (strcmp(cmd, "find") == 0)
        {
            AVLValue *findNode = NULL;
            AVLValue *temp = NULL;
            findCommand(findNode, temp, stations);
        }
      
        else if (strcmp(cmd, "route") == 0)
        {
            AVLNode *routeNode = NULL;
            AVLNode *nodeStat = NULL;
            AVLNode *nodeDest = NULL;
            routeCommand(routeNode, trips, nodeStat, nodeDest, stations);
        }
        else if (strcmp(cmd, "exit") != 0)
        {
            printf("**unknown cmd, try again...\n");
        }
      else
          printf("** Freeing memory **\n");

    }
    while (strcmp(cmd, "exit") != 0);
    
    //
    // done, free memory and return:
    //
    AVLFree(stations, freeAVLNodeData);
    AVLFree(trips, freeAVLNodeData);
    AVLFree(bikes, freeAVLNodeData);
    
    printf("** Done **\n");
    
    return 0;
}
