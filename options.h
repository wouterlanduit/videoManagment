#ifndef OPTIONS_H
#define OPTIONS_H

#define DATA_FILE "videomanagment.dat"
#define LOG_FILE "videomanagment.log"
#define SETTINGS_FILE "videomanagment.set"
#define NUM_OPTIONS 5

#define OPTION_STDSEARCH 0          //1) standard search folder
#define OPTION_VIDEO 1              //2) VLC location
#define OPTION_STDOPEN 2            //3) standard folder to open
#define OPTION_STDOPENCHECK 3       //4) checkbox of previous
#define OPTION_DEFAULTCLICK 4       //5) play or rate by default

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
