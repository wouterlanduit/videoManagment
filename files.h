#ifndef _FILES_H
#define _FILES_H

#include <vector>
#include <string>
#include <ctime>

#define DATA_FILE "videomanagment.dat"
#define FOLDER "c:\\users\\wouter\\documents\\varia\\youtube\\trailers"

class File{
public:
	std::string name;
	std::string path;
	int rating;			//rating op 10
	time_t lastWatched;

	/*constructors*/
	File() : name(""), path(""), rating(-1){};
	File(std::string name, std::string path) : name(name), path(path), rating(-1){}

	/*methds*/
	int rate();
	int setRating(int);

	int playVideo();

	void printFile();

	int exists();
};

class Folder{
public:
	char* name;
	std::vector<File> files;

	/*constructors*/
	Folder(){
		name = NULL;
	}

	Folder(char* name) : name(name){}

	/*methods*/
	int printFolder(char*);

	int playRandom();
	int rateRandom();//misschien options als argument meegeven en dan vragen via input

	int loadFiles();		//laad alle files in de folder
	int loadDataFile();
	int saveDataFile();
	int updateDataFile();

	int getHighestRated(std::vector<File>&);
	std::vector<File*> Folder::getHighestRated();
	std::vector<File*> getLeastRecentlyViewed();

	int printFiles(std::vector<File>&);
	int printFiles(std::vector<File>&, char);
	int printFiles(std::vector<File*>&, char);
};

#endif