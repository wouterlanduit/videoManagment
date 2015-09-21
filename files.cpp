#include <iostream>
#include <filesystem>
#include <vector>
#include <math.h>
#include "files.h"
#include "playback.h"

using namespace std;
using namespace std::tr2::sys;

int Folder::printFolder(char* folder){
	path dir_path(folder);
	if (is_directory(dir_path)){
		for (directory_iterator itr(dir_path); itr != directory_iterator(); ++itr){
			cout << itr->path().filename() << ' '; // display filename only
			if (is_regular_file(itr->status())) cout << " [" << file_size(itr->path()) << ']';
			cout << '\n';
		}
	}
	else{
		cout << "Folder not found." << endl;
	}
	return 0;
}

int Folder::loadFiles(){
	path dir_path(this->name);
	if (is_directory(dir_path)){
		for (directory_iterator itr(dir_path); itr != directory_iterator(); ++itr){
			cout << itr->path().filename() << ' '; // display filename only
			if (is_regular_file(itr->status()) && itr->path().filename()!= DATA_FILE){
				File file(itr->path().filename(), dir_path.directory_string());
				this->files.push_back(file);
				cout << " [" << file_size(itr->path()) << ']';
			}
			else{
				//in de map de files zoeken
			}
			cout << '\n';
		}
	}
	else{
		cout << "Folder not found." << endl;
	}
	return 0;
}

int Folder::playRandom(){
	int count = this->files.size();
	std::srand((int)std::time(0));
	int random = rand();
	File file = this->files[random%count];
	playVideo(file.path.c_str(), file.name.c_str());
	return 0;
}

int Folder::rateRandom(){
	int count = this->files.size();
	std::srand((int)std::time(0));
	int random = rand();
	File* file = &(this->files[random%count]);
	playVideo(file->path.c_str(), file->name.c_str());
	cout << "Give your rating for this trailer (max. 10) : " << endl;
	int rating;
	if(cin >> rating);
	else{//wat als input niet juist is??
		cin.clear(); // clears the error flags
		// this line discards all the input waiting in the stream
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	if (rating >= 0 && rating <= 10){
		file->rating = rating;
	}
	
	return 0;
}

int Folder::saveDataFile(){
	char* file_path = new char[strlen(this->name) + 1 + strlen(DATA_FILE) + 1];

	strcpy(file_path, this->name);
	strcat(file_path, "\\");
	strcat(file_path, DATA_FILE);

	FILE* file = fopen(file_path, "wb+");

	//alles wegschrijven
	for (unsigned int i = 0; i < this->files.size(); i++){
		int length = this->files[i].name.size()*sizeof(char);
		fwrite(&length, sizeof(int), 1, file);
		//fwrite(" &|& ", sizeof(char), 5, file);
		fwrite(this->files[i].name.c_str(), sizeof(char), this->files[i].name.length(), file);
		//fwrite(" &|& ", sizeof(char), 5, file);
		fwrite(&(this->files[i].rating), sizeof(int), 1, file);
		fwrite("\n", sizeof(char), 1, file);
	}

	fclose(file);

	delete[] file_path;

	return 0;
}

int Folder::loadDataFile(){
	path dir_path(this->name);
	if (is_directory(dir_path)){
		char* file_path = new char[strlen(this->name) + 1 + strlen(DATA_FILE) + 1];
		strcpy(file_path, this->name);
		strcat(file_path, "\\");
		strcat(file_path, DATA_FILE);
		FILE* dataFile = fopen(file_path, "rb");
		if (dataFile == NULL){//er was nog geen datafile
			this->loadFiles();
		}
		else{
			File videoFile;
			do{
				int length = 0;
				videoFile.path = this->name;
				fread(&length, sizeof(int), 1, dataFile);
				if (length != 0){
					char* name = new char[length + 1]; //andere mogelijkheid: maximale lengte geven, dan hoeft new en delete niet meer
					fread(name, sizeof(char), length, dataFile);/*lukt niet rechtstreeks?*/
					name[length] = '\0';
					videoFile.name = name;
					delete[] name;
					fread(&(videoFile.rating), sizeof(int), 1, dataFile);
					//fscanf mogelijk???
					this->files.push_back(videoFile);
				}
				
			} while (getc(dataFile) != EOF);	
		}

		delete[] file_path;
	} else{
		cout << "Folder not found." << endl;
	}

	return 0;
}