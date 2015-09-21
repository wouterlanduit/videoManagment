#ifndef _FILES_H
#define _FILES_H

#include <vector>
#include <string>

#define DATA_FILE "videomanagment.dat"
#define FOLDER "c:\\users\\wouter\\documents\\varia\\youtube\\trailers"

class File{
public:
	std::string name;
	std::string path;
	int rating;			//rating op 10
	//last viewed

	/*constructors*/
	File() : name(""), path(""), rating(-1){};
	File(std::string name, std::string path) : name(name), path(path), rating(-1){}
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
	int loadFiles();		//laad alle files in de folder
	int printFolder(char*);
	int playRandom();
	int rateRandom();

	int saveDataFile();
	int loadDataFile();
};

#endif