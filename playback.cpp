#define USE_SYSTEM

#include "playback.h"
#include <iostream>
#include <stdio.h>
#ifdef USE_SYSTEM
	#include <cstdlib> //system()
#else
	#include <windows.h> //ShellExecute()
	#include <shellapi.h>
#endif

using namespace std;

int Video::playVideo(const char* folder, const char* file){
	int returnValue = 1;
	char* vlc = "\"c:\\Program Files (x86)\\videoLAN\\VLC\\vlc.exe\"";
	char* options = "--play-and-exit";
	//char* folder = "C:\\Users\\wouter\\Documents\\varia\\youtube\\muziek";
	//char* file = "Wuthering Heights - Carpe Noctem - Seize The Night.mp4";
#ifdef USE_SYSTEM
	int length = 1 + strlen(vlc) + 2 + strlen(folder) + 1 + strlen(file) + 2 + strlen(options) + 2;
#else
	int length = 1 + strlen(folder) + 1 + strlen(file) + 2 + strlen(options) + 2;
#endif
	char* command = new char[length];

	strcpy(command, "\"");
#ifdef USE_SYSTEM
	strcat(command, vlc);
	strcat(command, " \"");
#endif
	strcat(command, folder);
	strcat(command, "\\");
	strcat(command, file);
	strcat(command, "\" ");
	strcat(command, options);
	strcat(command, "\"");

	cout << command << endl << endl;

#ifdef USE_SYSTEM
	returnValue = system(command);
	if (returnValue){
		cout << "Could not open the program" << endl;
	}
#else
	ShellExecute(NULL, "open", vlc, command, NULL, SW_SHOWNORMAL);
#endif

	delete[] command;

	return returnValue;
}
