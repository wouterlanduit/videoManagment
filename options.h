#ifndef _OPTIONS_H
    #define _OPTIONS_H

    #define DATA_FILE "videomanagment.dat"
    #define LOG_FILE "videomanagment.log"
    #define SETTINGS_FILE "videomanagment.set"
    #define FOLDER "c:\\users\\wouter\\documents\\varia\\youtube\\trailers"
    #define NUM_OPTIONS 5
        //1) standard search folder
        //2) VLC location
        //3) standard folder to open
        //4) checkbox of previous
        //5) play or rate by default

    // Choose one of the following 3
    #define USE_QT_PROCESS
    //#define USE_SYSTEM
    //#define USE_API

    #define WITH_GUI
    #ifndef WITH_GUI
        #define WITH_CONSOLE
    #endif

    //#define USE_COMBOBOX
    #ifndef USE_COMBOBOX
        #define USE_SLIDER
    #endif

#endif
