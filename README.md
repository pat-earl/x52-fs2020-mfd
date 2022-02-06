# Logitech X52 Pro MFD for Flight Simulator 2020

This is a small project to make use of the Multi-Function Display that comes on the Logitech X52 H.O.T.A.S. which is a bit dated at this point. 
My idea for this came from reading the manual, which has a small section about a plugin that was available for FSX, which would display the COM and AFD values
in the radio stack. This functionality wasn't and probably won't ever be carried over to FS2020. Hence this project. 

None of the code to display information on the MFD is my own, so please see the credits below.

## Features

- [X] Basic Functinality
	- Currently with the game running and while in a "mission", the MFD will display COM1 & COM2 Freq and Eng1 RPMs. 
- [ ] Format the values returned so that there is more floating point percision. 
- [ ] Add configuration file so there isn't a need to hard-code the values pulled.
	- Plan is to extend this so that you can specify what information is displayed on what page (The MFD supports up to 3 pages).
	- Also determine if the polling rate is okay.
- [ ] Console window is annoying to look at, figure out how to place it in the taskbar and hide console window. 
- [ ] Automatic start when FS is launched.

## Building

No "releases" are planned, so you'll have to build it yourself. I recommend using VS2019 since that is what my solution files are from.

WIP

## Other stuff

A lot of reading of the FS SDK was required. Doing C++ programming really isn't my thing, let alone doing it for Windows. This is very much a side project and will be worked 
on while I have. Pull requests are okay, but don't expect quick responses to any issues posted. 

# Thanks

Most of the credit goes to these two:
[Peter Pakkenberg](https://github.com/peterbn/X52-pro-MFD-JSON-Driver) for his X52-Pro-MFD-JSON-Driver &
[Anthony Zaprzalka](https://github.com/AZaps) for the original version. 

This program doesn't use JSON, but directly calls the X52 functions that I got from their software. 

Also thanks for the flight sim team for putting out the [SDK](https://docs.flightsimulator.com/html/index.htm#t=Introduction%2FIntroduction.htm)
