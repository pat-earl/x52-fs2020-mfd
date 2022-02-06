// X52-FS2020.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <SimConnect.h>
#include "DirectOutputFn.h"

// GLobals 

static enum DATA_DEFINE_ID {
	DEFINITION_1,
	DEFINITION_2
};

static enum DATA_REQUEST_ID {
	REQUEST_1,
	REQUEST_2
};


struct Requested_Data
{
	double com1_freq;
	double com2_freq;
	double eng1_rpm;
};

// Used to help handle CTRL-C (will probably have to change)
bool isRunning = true;

// DirectOutput Object
DirectOutputFn fn;

// Prototypes
BOOL controlHandler(DWORD);
void CALLBACK DispatchHandler(SIMCONNECT_RECV*, DWORD, void*);
void checkHR(HRESULT hr);


int main()
{

	SetConsoleCtrlHandler((PHANDLER_ROUTINE)controlHandler, TRUE);

	// For hiding the console window later on: https://stackoverflow.com/questions/18260508/c-how-do-i-hide-a-console-window-on-startup

	// Variables
	// SIMCONNECT OBJECT
	HRESULT hr;
	HANDLE hSimConnect = NULL;


	// Setup Device
	checkHR(fn.Initialize(L"FS2020MFD"));

	// Register the device
	fn.RegisterDevice();

	// Check if connection was successful
	if (!fn.GetDeviceType())
	{
		std::cout << "\nPress ENTER to close this application.";
		std::cin.ignore(1000, '\n');
		return 0;
	}

	// Register soft buttons
	checkHR(fn.registerSoftBtnCallback());

	// Register page change callback
	checkHR(fn.registerPageCallback());


	// Connect to the flight simulator "Server" 
	hr = SimConnect_Open(&hSimConnect, "x52 MFD", NULL, 0, 0, SIMCONNECT_OPEN_CONFIGINDEX_LOCAL);

	hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_2, "COM ACTIVE FREQUENCY:1", "MHz");
	hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_2, "COM ACTIVE FREQUENCY:2", "MHz");
	hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_2, "GENERAL ENG RPM:1", "rpm");

	hr = SimConnect_RequestDataOnSimObject(hSimConnect, REQUEST_2, DEFINITION_2, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_SECOND);

	while(isRunning) {
		hr = SimConnect_CallDispatch(hSimConnect, DispatchHandler, NULL);
	}


	// Close sim object
	hr = SimConnect_Close(hSimConnect);

	// Deregister Controller
	checkHR(fn.unRegisterSoftBtnCallback());
	checkHR(fn.unRegisterPageCallback());

	checkHR(fn.Deinitialize());

	std::cout << "\nPress ENTER to close this application.";
	std::cin.ignore(1000, '\n');

	return 0;
}


void CALLBACK DispatchHandler(SIMCONNECT_RECV* pData, DWORD cbData, void* pContext)
{
	switch (pData->dwID) {
	case SIMCONNECT_RECV_ID_SIMOBJECT_DATA:
	{
		SIMCONNECT_RECV_SIMOBJECT_DATA* pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA*)pData;
		switch (pObjData->dwRequestID)
		{
		case REQUEST_2:
		{
			Requested_Data* rD = (Requested_Data*)&pObjData->dwData;

			std::cout << "COM1: " << rD->com1_freq << std::endl;
			std::cout << "COM2: " << rD->com2_freq << std::endl;
			std::cout << "ENG1 RPM: " << rD->eng1_rpm << std::endl;

			auto data = std::make_unique<mfdData>();
			mdfDataPage mdfPage;

			mdfPage.lines.push_back("COM1: " + std::to_string(rD->com1_freq));
			mdfPage.lines.push_back("COM2: " + std::to_string(rD->com2_freq));
			mdfPage.lines.push_back("RPM1: " + std::to_string(rD->eng1_rpm));

			data->pages.push_back(mdfPage);

			fn.SetOrUpdateDisplayData(std::move(data));

			break;
		}
		}
		break;
	}
	default:
		break;
	}
}

/*
	PARAMETERS: HRESULT hr == some functions from DirectOutput return a HRESULT value, this just checks if it pass/fail and ouputs to the console
	RETURNS: none

	FUNCTION: Checks result of the function if it returns an HRESULT value

	Read more about HRESULT return types at
	Microsoft (MSDN) -> https://msdn.microsoft.com/en-us/library/windows/desktop/aa378137(v=vs.85).aspx
	Wikipedia -> https://en.wikipedia.org/wiki/HRESULT

	Taken from X52MFDDriver Project
	https://github.com/peterbn/X52-pro-MFD-JSON-Driver
*/
void checkHR(HRESULT hr)
{
	if (hr == S_OK)
	{
		std::cout << "DONE.\n";
	}
	else
	{
		std::cout << "FAILED/ hr = " << hr << std::endl;
	}
}


BOOL controlHandler(DWORD fdwCtrlType) {
	switch (fdwCtrlType)
	{
		case CTRL_C_EVENT:
			isRunning = false;
			return(TRUE);
		case CTRL_CLOSE_EVENT:
			isRunning = false;
			return(TRUE);
		default:
			return FALSE;
	}
}