#include <iostream>
#include <string>
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>
#include "Simconnect_FSX.h"
#include "SkylineServer.h"
using namespace std;

void renderAIAircraft(HRESULT hr) {
	AI_PLANE_DATA AIPlane(
		"test AI",
		"2287",
		"CES2287",
		"CES2287",
		39.9042,
		116.4074,
		1240,
		0,
		0,
		280,
		190
	);

	//simconnect位置数据格式
	SIMCONNECT_DATA_INITPOSITION InitPos = { 0 };
	InitPos.Latitude = AIPlane.latitude;
	InitPos.Longitude = AIPlane.longitude;
	InitPos.Altitude = AIPlane.altitude;
	InitPos.Heading = AIPlane.heading; 
	InitPos.Airspeed = AIPlane.airspeed;
	InitPos.OnGround = 0;

	hr = SimConnect_AICreateNonATCAircraft(hSimConnect, "My AI Plane", "Boeing 737-800", InitPos, RequestClientFlightLiveData);
	printf("ok");
}