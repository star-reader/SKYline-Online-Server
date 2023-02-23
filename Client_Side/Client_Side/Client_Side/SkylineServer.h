#pragma once
#include <iostream>
using namespace std;

//function 
void renderAIAircraft(HRESULT hr);

static HANDLE hSimConnect = NULL;
#define WM_USER_SIMCONNECT		WM_USER + 1

// Data definitions
enum DATA_DEFINE_ID {
    ClientFlightLiveData,
    ClientAIPosition
};

enum DATA_REQUEST_ID {
    RequestClientFlightLiveData,
    RequestUpdateAIPosition,
};

class CLIENT_FLIGHT_LIVE_DATA {
public:
    double altitude;
    double heading;
};

class AI_PLANE_DATA {
public:
    string title;
    string atc_id;
    string atc_airline;
    string atc_flight_number;
    double latitude;
    double longitude;
    double altitude;
    double pitch;
    double bank;
    double heading;
    DWORD airspeed;

    AI_PLANE_DATA(
        string title,
        string atc_id,
        string atc_airline,
        string atc_flight_number,
        double latitude,
        double longitude,
        double altitude,
        double pitch,
        double bank,
        double heading,
        DWORD airspeed) {
        if (!this) {
            return;
        }
        this->title = title;
        this->atc_id = atc_id;
        this->atc_airline = atc_airline;
        this->atc_flight_number = atc_flight_number;
        this->latitude = latitude;
        this->longitude = longitude;
        this->altitude = altitude;
        this->pitch = pitch;
        this->bank = bank;
        this->heading = heading;
        this->airspeed = airspeed;
    }
};