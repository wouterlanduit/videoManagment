#ifndef _FILES_H
#define _FILES_H

#include <vector>
#include <string>
#include <ctime>
#include "options.h"

#include <QProcess>

#define DATA_FILE "videomanagment.dat"
#define LOG_FILE "videomanagment.log"
#define FOLDER "c:\\users\\wouter\\documents\\varia\\youtube\\trailers"


class File{
public:
	std::string name;
	std::string path;
	int rating;			//rating op 10
	time_t lastWatched;

	/*constructors*/
    File() : name(""), path(""), rating(-1){}
	File(std::string name, std::string path) : name(name), path(path), rating(-1){}

    /*methods*/
	int rate();
    int setRating(int, bool);

	int playVideo();
#ifdef WITH_CONSOLE
	void printFile();
#endif

	int exists();
};

class Folder{   /*extends File????*/
public:
    //char* name;
    std::string name;
    std::vector<File> files;
    bool unsaved;

	/*constructors*/
	Folder(){
        name = "";
        unsaved = false;
	}

    Folder(char* name) : name(name){
        unsaved = false;
    }

	/*methods*/
#ifdef WITH_CONSOLE
	int printFolder(char*);
#endif

    File *playRandom();
    int rateRandom();//misschien options als argument meegeven en dan vragen via input

	int loadDataFile();
	int saveDataFile();		//is er een manier om enkel de log-entries aant te passen (wat dan bij toevoegen en verwijderen?)
	int updateDataFile();

	int getHighestRated(std::vector<File>&);
    std::vector<File*> getHighestRated();
    std::vector<File*> getLeastRecentlyViewed();
    File* findFile(std::string);	//aanpassen voor als er subfolders toegelaten worden (dubbels mogelijk met ander pad)
    File* searchFile(std::string, std::string); /*pad, naam*/

#ifdef WITH_CONSOLE
	int printFiles(std::vector<File>&);
	int printFiles(std::vector<File>&, char);
	int printFiles(std::vector<File*>&, char);
#endif

private:
    int loadFiles();		//laad alle files in de folder
};

#endif
