#include <iostream>
#include <stdio.h>
#include <cstring>
#include <string>
#include <crtdbg.h>
#include <vector>

#include "playBack.h"
#include "files.h"

using namespace std;

int main()
{
	// Debug flag
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	File file("Wuthering Heights - Carpe Noctem - Seize The Night.mp4", "C:\\Users\\wouter\\Documents\\varia\\youtube\\muziek");
	int stop = 0;
	int option;
	char answer;
	Folder folder(FOLDER);
	folder.loadDataFile(); //eig load from stored data vs. update stored data nodig

	folder.updateDataFile();

	while (!stop){
		cout << "choose what you want to do : " << endl;
		cout << "\t 1. Rate a random trailer." << endl;
		cout << "\t 2. Show top rated trailers." << endl;
		cout << "\t 3. Stop the program." << endl;
		
		if (cin >> option){}
		else{
			option = 0;
			cin.clear(); // clears the error flags
			// this line discards all the input waiting in the stream
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}

		cout << endl;

		vector<File> highRated;
		switch (option){
		case 1:
			while (!stop){
				folder.rateRandom();
				cout << "Do you want to rate another trailer? (y/n)";
				cin >> answer;
				if (answer != 'y' && answer != 'Y'){
					stop = 1;
				}
			}
			stop = 0;
			break;
		case 2:
			highRated = folder.getHighestRated();
			folder.printFiles(highRated, 'a');
			while (!stop){
				cout << "Which high rated trailer would you like to see? (0 to watch none) : ";
				if (cin >> option){
					if (option >= 0 && option <= highRated.size()){
						if (option){ //option != 0
							highRated[option - 1].playVideo();
						}
						stop = 1;
					}
				}
				else{
					option = 0;
					cin.clear(); // clears the error flags
					// this line discards all the input waiting in the stream
					cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				}
				if (!stop){
					cout << "You didn't choose one of the available options. Choose again." << endl;
				}
			}
			stop = 0; //buitenste lus
			cout << endl << endl;
			break;
		case 3:
			cout << "Are you sure? (y/n) : ";
			cin >> answer;
			if (answer == 'y' || answer == 'Y'){
				stop = 1;
			}
			break;
		default:
			cout << "You didn't choose one of the available options. Choose again." << endl;
			stop = 0;
			break;
		}
	}

	folder.saveDataFile();
	getchar();
}