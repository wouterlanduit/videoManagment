//#define USE_SYSTEM
#ifndef USE_SYSTEM
	//#define USE_API
#endif

#include "playback.h"
#include <iostream>
#include <stdio.h>
#ifdef USE_QT_PROCESS
    #include <QProcess>
    #include <QString>
    #include <QStringList>
#else
#ifdef USE_SYSTEM
	#include <cstdlib> //system()
#else
    #include <Windows.h> //ShellExecute() && createProcess()
	#include <shellapi.h>
#endif
#endif



using namespace std;

#ifdef USE_QT_PROCESS

int Video::playVideo(const char* folder, const char* file){
    int retVal = 1;
    QString vlc = "C:\\Program Files\\videoLAN\\VLC\\vlc.exe";
    QString video = folder;
    video.append("\\");
    video.append(file);
    video.replace('/', '\\');
    QStringList options;
    options << video << "--play-and-exit";

    QProcess *process = new QProcess();
    try {
        //process->start(vlc, options);
        if(process->startDetached(vlc, options)){
            retVal = 0;
        }else{
            retVal = -1;
        }
    } catch (const std::exception& e) {
        retVal = -1;
        printf("%s", e.what());
    }

    delete process;
    return retVal;
}

#else

int Video::playVideo(const char* folder, const char* file){
	int returnValue = 1;
    const char* vlc = "\"C:\\Program Files (x86)\\videoLAN\\VLC\\vlc.exe\"";
    const char* options = "--play-and-exit";
#if defined USE_SYSTEM
	int length = 1 + strlen(vlc) + 2 + strlen(folder) + 1 + strlen(file) + 2 + strlen(options) + 2;
#elif defined USE_API
	int length = 1 + strlen(folder) + 1 + strlen(file) + 2 + strlen(options) + 2;
#else
	int length = strlen(vlc) + 2 + strlen(folder) + 1 + strlen(file) + 2 + strlen(options) + 1;
#endif
	char* command = new char[length];
	command[0] = '\0';		//so strcat can be used as first command in all cases

#if defined USE_SYSTEM || defined USE_API
	strcat(command, "\"");
#endif
#ifndef USE_API
	strcat(command, vlc);
	strcat(command, " \"");
#endif
	strcat(command, folder);
	strcat(command, "\\");
	strcat(command, file);
	strcat(command, "\" ");
	strcat(command, options);
#if defined USE_SYSTEM || defined USE_API
	strcat(command, "\"");
#endif

	cout << command << endl << endl;

#ifdef USE_SYSTEM
	returnValue = system(command);
	if (returnValue){
		cout << "Could not open the program" << endl;
		returnValue = -1;
	}
#elif defined USE_API
	ShellExecute(NULL, "open", vlc, command, NULL, SW_SHOWNORMAL);
	//fout opvangen
#else
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	//allocate memory
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	if (!CreateProcess(NULL, //use commandline
        command,	//commandline					//! niet te veel quotes
		NULL,	//don't inherit process handle
		NULL,	//don't inherit thread handle
		FALSE,	//disable handle inheritance
		0,		//no creation flags
		NULL,	//use parent's environment block
		NULL,	//use parent's directory
		&si,
		&pi)){
		cout << "Failed to open VLC" << endl;
		returnValue = -1;
	}
	// parent will wait for the child to complete
	//WaitForSingleObject(pi.hProcess, INFINITE);
	//MessageBox(NULL, "Child Complete", "BS - Process creation", MB_OK);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	returnValue = 0; //can't return yet, because delete[] command hasn't been done
#endif

	delete[] command;

	return returnValue;
}

#endif
