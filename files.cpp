#include <iostream>
#include <filesystem>
#include <vector>
#include <math.h>
#include <sys/stat.h> //to use stat() for testing file existence
#include <ctime>
#include <map>
#include "files.h"
#include "playback.h"
#include "dialog.h"

#include <QString>
#include <QProcess>

using namespace std;
using namespace std::tr2::sys; //https://msdn.microsoft.com/en-us/library/hh874769.aspx

/********/
/* FILE */
/********/

int File::setRating(int rating, bool log){
	if (rating >= 0 && rating <= 10){
		this->rating = rating;
        if(log){
            ofstream log;
            log.open(LOG_FILE, ios::app);
            if (!log.is_open())
            {
                log.clear();
                log.open(LOG_FILE, ios::out); //Create file.
                log.close();
                log.open(LOG_FILE, ios::app);
            }
            log << this->name << " ; " << rating << endl;
            log.close();
        }
		return 0;
	}
	else{
		return -1;	//too large or too small
	}
}

#ifdef WITH_CONSOLE
int File::rate(){
	int rating;
	while (1){
		cout << "Give your rating (max. 10) : ";
		if (cin >> rating){
            return this->setRating(rating, true);
        }else{
			cin.clear(); // clears the error flags
			// this line discards all the input waiting in the stream
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		cout << "invalid input." << endl;
	}
}
#else
/*return rating of -1 bij cancel*/
int File::rate(){
    int newRating = this->rating;
    //rating widget oproepen
    QString prevRate;
    if(this->rating<0){
        prevRate = "unrated";
    }else{
        prevRate = QChar(this->rating + 48);
    }
    RateDialog* popup = new RateDialog(prevRate);
    if(popup->exec()){
        this->setRating(popup->getRating(),true);
        newRating =  popup->getRating();
    }else{
        newRating = -1;
    }

    delete popup;

    return newRating;
}

#endif

int File::playVideo(){
	int success = Video::playVideo(this->path.c_str(), this->name.c_str());
	if (success == 0){
		time(&(this->lastWatched));
	}
	return success;
}

#ifdef WITH_CONSOLE
void File::printFile(){
	cout << this->name << "\t(";
	if (this->rating > -1)
		cout << this->rating << "/10)" << endl;
	else
		cout << "unrated" << ")" << endl;
}
#endif

int File::exists(){
	std::tr2::sys::path dir_path(this->path);
	std::tr2::sys::path file(this->name);
	std::tr2::sys::path full_path = dir_path / file;
    //volgende regel is niet hoofdlettergevoelig
	file_status stat = status(full_path);	// 1: file_not_found
                                            // 2: regular_file

	return (stat.type() != 1);
}

/**********/
/* FOLDER */
/**********/

#ifdef WITH_CONSOLE
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
#endif

File* Folder::playRandom(){
	int count = this->files.size();
	std::srand((int)std::time(0));
	int random = rand();
    File* file = &(this->files[random%count]);
    file->playVideo();
    unsaved = true;//checken of afspelen gelukt is?
    return file;//checken of afspelen gelukt is?
}

#ifdef WITH_CONSOLE
int Folder::rateRandom(){
	int count = this->files.size();
	std::srand((int)std::time(0));
	int random = rand();
    File* file = &(this->files[random%count]);
	file->playVideo();
	cout << "Previous rating : ";
	if(file->rating > -1)
		cout << file->rating;
	else
		cout << "unrated";
	cout << endl;
	return file->rate();
}
#else
int Folder::rateRandom(){
    File* file = this->playRandom();
    unsaved = true;//checken of afspelen gelukt is?
    return file->rate();
}
#endif

int Folder::loadFiles(){
	path dir_path(this->name);
	if (is_directory(dir_path)){//dit moet eigenlijk enkel in de bovenliggende functie gecontrolleerd worden (als deze functie nooit apart wordt opgeroepen)
		for (directory_iterator itr(dir_path); itr != directory_iterator(); ++itr){
			cout << itr->path().filename() << ' '; // display filename only
			if (is_regular_file(itr->status()) && itr->path().filename()!= DATA_FILE){
				File file(itr->path().filename(), dir_path.directory_string());
                file.lastWatched = NULL;
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
    this->unsaved = false;
	return 0;
}

int Folder::saveDataFile(){
    if(this != NULL){
        char* file_path = new char[this->name.size() + 1 + strlen(DATA_FILE) + 1];

        strcpy(file_path, this->name.c_str());
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
            fwrite(&(this->files[i].lastWatched), sizeof(time_t), 1, file);
            fwrite("\n", sizeof(char), 1, file);
        }

        fclose(file);

        //delete log
        remove(LOG_FILE);
        this->unsaved = false;

        delete[] file_path;
    }

	return 0;
}

/**
 * @brief laad de datafile van de Folder
 * @param void
 * @return  0 , alles succesvol
 **/

int Folder::loadDataFile(){
	path dir_path(this->name);
    // checken dat de opgegeven naam wel degelijk een file is die bestaat
	if (is_directory(dir_path)){
        // creeeren van het pad naar de datafile
        char* file_path = new char[this->name.size() + 1 + strlen(DATA_FILE) + 1];
        strcpy(file_path, this->name.c_str());
		strcat(file_path, "\\");
		strcat(file_path, DATA_FILE);
        //openen datafile
		FILE* dataFile = fopen(file_path, "rb");
		if (dataFile == NULL){//er was nog geen datafile
			this->loadFiles();
		}
		else{
			File videoFile;
			do{
				int length = 0;
                videoFile.path = dir_path.directory_string();
                // lees lengte van naam van videofile
				fread(&length, sizeof(int), 1, dataFile);
				if (length != 0){
					char* name = new char[length + 1]; //andere mogelijkheid: maximale lengte geven, dan hoeft new en delete niet meer
					fread(name, sizeof(char), length, dataFile);/*lukt niet rechtstreeks?*/
					name[length] = '\0';
					videoFile.name = name;
					delete[] name;
					fread(&(videoFile.rating), sizeof(int), 1, dataFile);
					fread(&(videoFile.lastWatched), sizeof(time_t), 1, dataFile);
					//fscanf mogelijk???
					this->files.push_back(videoFile);
				}
				
			} while (getc(dataFile) != EOF);	
		}

		delete[] file_path;


		//log file aanpassingen toepassen (houdt last viewed niet bij)
		ifstream log;
		log.open(LOG_FILE);
        if (log.is_open()){
            unsaved = true;
			string temp;
			string fileName;
			int rating;
			while (log >> temp){
				if (temp != ";"){
					fileName += temp + " ";
				}
				else{
					log >> temp;
					rating = stoi(temp);
					fileName.erase(fileName.size() - 1, 1); //remove " " at the end

					//adjust the corresponding record

					File* file = this->findFile(fileName);
                    file->setRating(rating, false);

					fileName = "";
				}
			}
			log.close();
		}

	} else{
		cout << "Folder not found." << endl;
	}

    this->unsaved = false;
	return 0;
}

/**
 * @brief
 * @param void
 * @return  0 , alles succesvol
 **/

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
						//cout << itr_list->name << endl;
						itr_list++;
						itr_new++;
						element_nr++;
					}
					else{
						/*itr_new is een nieuwe file (moet ingevoegd worden voor itr_list)*/

						//while loop doen tot itr_new == itr_list of alle files overlopen zijn
                        while (itr_new->path().filename() != itr_list->name && itr_new != directory_iterator()){//probleem!! hoofdlettergevoelig (File::exists niet)
                            QString newFile = itr_new->path().filename().c_str();
                            QString oldFile = itr_list->name.c_str();
                            if(oldFile.toLower() == newFile.toLower()){
                                //ze zijn op de hoofdletters na gelijk
                                itr_list->name = itr_new->path().filename();
                            }else{
                                File file(itr_new->path().filename(), dir_path.directory_string());
                                file.lastWatched = NULL;//time(nullptr);
                                this->files.insert(itr_list, file);
                                cout << "++" << file.name << endl;
                                element_nr++;
                                itr_list = this->files.begin();
                                itr_list += element_nr;
                                itr_new++;
                            }
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
    this->unsaved = false;
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

vector<File*> Folder::getHighestRated(){
	std::vector<File*> sorted[11]; //pointers om geheugenruimte te sparen

	std::vector<File>::iterator it = (this->files).begin();

	std::vector<File>::iterator end = this->files.end();

	while (it != end){
		if (it->rating >= 0){
			sorted[it->rating].push_back(&(*it));
		}
		it++;
	}

	std::vector<File*> highestRated;

    unsigned int j = 10;
	while (highestRated.size() < 10 && j >= 0){
        for (unsigned int i = 0; i < sorted[j].size(); i++){//misschien geen lus nodig? bestaat er een geoverloade versie die dit kan?
			highestRated.push_back(sorted[j][i]);
		}
		j--;
	}

	return highestRated;

}

vector<File*> Folder::getLeastRecentlyViewed(){
	//eerst allemaal sorteren?-> in multimap. nadeel: veel geheugengebruik
	std::multimap<time_t, File*> sorted;

	std::vector<File>::iterator it = (this->files).begin();

	std::vector<File>::iterator end = this->files.end();

	while (it != end){
		sorted.insert(std::pair<time_t, File*>((it->lastWatched), &(*it)));
		it++;
	}

	std::vector<File*> result;
	std::multimap<time_t, File*>::iterator mm_it = sorted.begin();
	std::multimap<time_t, File*>::iterator mm_it_end = sorted.end();
	int count = 0;

	while (count < 15 && mm_it != mm_it_end){
		result.push_back(mm_it->second);
		count++;
		mm_it++;
	}
	return result;
}

File* Folder::findFile(string fileName){
    std::vector<File>::iterator itr = this->files.begin();
    std::vector<File>::iterator itr_end = this->files.end();

    while (itr < itr_end){
        if (itr->name == fileName){
            return &(*itr);
        }
        itr++;
    }
    cout << "File not found" << endl;
    return NULL;
}

/**
 * @brief zoekt de file met opgegeven pad en bestandsnaam in de folder
 * @param path, pad naar de file
 * @param name, bestandsnaam van het te zoeken bestand
 * @return File*, pointer naar de file
 * @return NULL, als de file niet gevonden werd
 **/

File* Folder::searchFile(std::string path, std::string name){
    std::vector<File>::iterator it = (this->files).begin();

    std::vector<File>::iterator end = this->files.end();

    while (it != end ){
        if(it->name == name){
            if(it->path == path){
                return &(*it);
            }
        }
    it++;
    }

    //FILE NOT FOUND
    return NULL;
}

#ifdef WITH_CONSOLE
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

int Folder::printFiles(vector<File*>& files, char style){// style: arabic numbering ('a'), 
	std::vector<File*>::iterator it = files.begin();
	int i = 1;
	while (it != files.end()){
		if (style == 'a'){
			cout << " (" << i << ") ";
		}
		(*it)->printFile();
		it++;
		i++;
	}
	cout << endl << endl;
	return 0;
}
#endif
