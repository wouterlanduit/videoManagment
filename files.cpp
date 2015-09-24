#include <iostream>
#include <filesystem>
#include <vector>
#include <math.h>
#include <sys/stat.h> //to use stat() for testing filen existence
#include "files.h"
#include "playback.h"

using namespace std;
using namespace std::tr2::sys; //https://msdn.microsoft.com/en-us/library/hh874769.aspx

int File::setRating(int rating){
	if (rating >= 0 && rating <= 10){
		this->rating = rating;
		return 0;
	}
	else{
		cout << "This rating is not possible";
		return -1;	//too large or too small
	}
}

int File::playVideo(){
	return Video::playVideo(this->path.c_str(), this->name.c_str());
}

void File::printFile(){
	cout << this->name << "\t(" << this->rating << "/10)" << endl;
}

int File::exists(){
	std::tr2::sys::path dir_path(this->path);
	std::tr2::sys::path file(this->name);
	std::tr2::sys::path full_path = dir_path / file;
	file_status stat = status(full_path);	//1: file_not_found
											// 2:regular_file

	return (stat.type() != 1);
}

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

int Folder::playRandom(){
	int count = this->files.size();
	std::srand((int)std::time(0));
	int random = rand();
	File file = this->files[random%count];
	file.playVideo();
	return 0;
}

int Folder::rateRandom(){
	int count = this->files.size();
	std::srand((int)std::time(0));
	int random = rand();
	File* file = &(this->files[random%count]);
	file->playVideo();
	cout << "Give your rating for this trailer (max. 10) : " << endl;
	int rating;
	if (cin >> rating);
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

int Folder::loadFiles(){
	path dir_path(this->name);
	if (is_directory(dir_path)){//dit moet eigenlijk enkel in de bovenliggende functie gecontrolleerd worden (als deze functie nooit apart wordt opgeroepen)
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

int Folder::updateDataFile(){//wat doen als er nog folders inzitten (overloaden met functie die verder pad als argument heeft?)
	path dir_path(this->name);
	std::vector<File>::iterator itr_list = this->files.begin();
	std::vector<File>::iterator itr_list_end = this->files.end();
	directory_iterator itr_new(dir_path);
	
	if (is_directory(dir_path)){
		if (this->files.size() == 0){
			this->loadFiles();
		}
		else{
			int element_nr = 0; //om bij toevoegingen nog naar het juiste element te kunnen itereren
			while (itr_list != itr_list_end){
				if (itr_list->exists()){ 
					//file has not been deleted
					if (itr_new->path().filename() == DATA_FILE){
						itr_new++;
					}
					if (itr_list->name == itr_new->path().filename()){
						/*file bestond al*/
						cout << itr_list->name << endl;
						itr_list++;
						itr_new++;
						element_nr++;
					}
					else{
						/*itr_new is een nieuwe file (moet ingevoegd worden voor itr_list)*/

						//while loop doen tot itr_new == itr_list of alle files overlopen zijn
						while (itr_new->path().filename() != itr_list->name && itr_new != directory_iterator()){
							File file(itr_new->path().filename(), dir_path.directory_string());
							this->files.insert(itr_list, file);
							cout << "++" << file.name << endl;
							element_nr++;
							itr_list = this->files.begin();
							itr_list += element_nr;
							itr_new++;
						}
						itr_list_end = this->files.end();
					}
				}
				else{//file was deleted
					cout << "--" << itr_list->name << endl;
					if (itr_list != this->files.begin()){
						std::vector<File>::iterator temp = itr_list - 1;
						this->files.erase(itr_list); //verwijderd het gedelete element uit de lijst
						itr_list = temp + 1;
						//erase ----> Invalidates iterators and references at or after the point of the erase, including the end() iterator.
					}
					else{
						this->files.erase(itr_list);
						itr_list = this->files.begin();
					}
					itr_list_end = this->files.end();		
				}
			}
			//files na laatste toevoegen
			while (itr_new != directory_iterator()){
				File file(itr_new->path().filename(), dir_path.directory_string());
				this->files.push_back(file);
				itr_new++;
			}
		}
	}
	else{
		cout << "Folder not found." << endl;
	}
	return 0;
}

int Folder::getHighestRated(vector<File>& bestVideos){ //enkel deze met de hoogste rating
	int highestRating = -1;

	std::vector<File>::iterator it = (this->files).begin();

	std::vector<File>::iterator end = this->files.end();

	while (it != end){
		if (it->rating >= highestRating){
			if (it->rating == highestRating){
				if (highestRating > -1){
					bestVideos.push_back(*it);
				}
			} else{
				bestVideos.clear();
				highestRating = it->rating;
				bestVideos.push_back(*it);
			}
		}
		it++;
	}

	if (highestRating == -1)
		return -1;
	else
		return 0;
}

vector<File> Folder::getHighestRated(){
	std::vector<File> sorted[11];

	std::vector<File>::iterator it = (this->files).begin();

	std::vector<File>::iterator end = this->files.end();

	while (it != end){
		if (it->rating >= 0){
			sorted[it->rating].push_back(*it);
		}
		it++;
	}

	std::vector<File> highestRated;

	int j = 10;
	while (highestRated.size() < 10 && j >= 0){
		for (int i = 0; i < sorted[j].size(); i++){
			highestRated.push_back(sorted[j][i]);
		}
		j--;
	}

	return highestRated;

}

int Folder::printFiles(vector<File>& files){
	std::vector<File>::iterator it = files.begin();

	while (it != files.end()){
		it->printFile();
		it++;
	}

	cout << endl << endl;

	return 0;
}

int Folder::printFiles(vector<File>& files, char style){// style: arabic numbering ('a'), 
	std::vector<File>::iterator it = files.begin();
	int i = 1;

	while (it != files.end()){
		if (style == 'a'){
			cout << " (" << i << ") ";
		}
		it->printFile();
		it++;
		i++;
	}

	cout << endl << endl;

	return 0;
}