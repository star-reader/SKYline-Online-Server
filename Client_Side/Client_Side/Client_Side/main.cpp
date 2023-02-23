#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>
#include <string>
#include <windef.h>
#include <fstream>
#include "Simconnect_FSX.h"
#include "SkylineServer.h"
using namespace std;

// SimConnect variables
static bool connected = false;
HRESULT hr;
bool isCreated = false;

// simconnect的回调函数
void CALLBACK MySimconnectDispatchProc(SIMCONNECT_RECV* pData, DWORD cbData, void* pContext)
{
    switch (pData->dwID)
    {
    case SIMCONNECT_RECV_ID_OPEN:
    {
        printf("Connected to Microsoft Flight Simulator.\n");
        connected = true;
        //renderAIAircraft(hr);

        break;
    }
    case SIMCONNECT_RECV_ID_QUIT:
    {
        printf("Microsoft Flight Simulator has exited.\n");
        connected = false;
        break;
    } 

    case SIMCONNECT_RECV_ID_EXCEPTION:
    {
        SIMCONNECT_RECV_EXCEPTION* pEx = (SIMCONNECT_RECV_EXCEPTION*)pData;
        printf("Exception received: %d\n", pEx->dwException);
        break;
    }
        

    case SIMCONNECT_RECV_ID_SIMOBJECT_DATA: 
    {
        SIMCONNECT_RECV_SIMOBJECT_DATA* pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA*)pData;

        if (pObjData->dwRequestID == RequestClientFlightLiveData) {
            CLIENT_FLIGHT_LIVE_DATA* pS = (CLIENT_FLIGHT_LIVE_DATA*)&pObjData->dwData;
            //printf("alt: %.2f ft, heading: %.2f,  \n", pS->altitude, pS->heading);
            string fileName = "C:\\simobject_data.txt";
            ofstream output_file(fileName, ios::out);
            output_file << "alt:" << pS->altitude << " , heading:" << pS->heading  << endl;
            output_file.close();
            
            if (!isCreated) {
                SIMCONNECT_DATA_INITPOSITION InitPos = {};
                InitPos.Latitude = 40 + (4.364 / 60);
                InitPos.Longitude = 116 + (34.432 / 60);
                InitPos.Altitude = 180.0;
                InitPos.Heading = 0.0;
                InitPos.Airspeed = (DWORD)25;
                InitPos.OnGround = (DWORD)0;
                SimConnect_AICreateNonATCAircraft(hSimConnect, "Airbus A321 Paint2","TEST2287", InitPos, RequestClientFlightLiveData);
                isCreated = true;
            }
        }
        break;
    }
    
    default:
        break;
    }
}

int main()
{
    hr = SimConnect_Open(&hSimConnect, "Microsoft Flight Simulator X", NULL, 0, 0, 0);
    if (FAILED(hr)) {
        printf("Failed to connect to Microsoft Flight Simulator.\n");
        return 1;
    }
    hr = SimConnect_AddToDataDefinition(hSimConnect, ClientFlightLiveData, "INDICATED ALTITUDE", "feet");
    hr = SimConnect_AddToDataDefinition(hSimConnect, ClientFlightLiveData, "PLANE HEADING DEGREES MAGNETIC", "degrees");
    //循环执行程序获取数据
    while (true)
    {
        hr = SimConnect_RequestDataOnSimObject(
            hSimConnect, RequestClientFlightLiveData, ClientFlightLiveData, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_ONCE
        );
        SimConnect_CallDispatch(hSimConnect, MySimconnectDispatchProc, NULL);
        Sleep(800);
    }
    hr = SimConnect_Close(hSimConnect);

    return 0;
}
