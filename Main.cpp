#include <iostream>
#include <stdio.h>
#include <cstring>
#include <string>
#include <crtdbg.h>

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

	while (!stop){
		cout << "choose what you want to do : " << endl;
		cout << "\t 1. Rate a random trailer." << endl;
		cout << "\t 2. Stop the program." << endl;
		if (cin >> option){}
		else{
			option = 0;
			cin.clear(); // clears the error flags
			// this line discards all the input waiting in the stream
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}

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
			cout << "Are you sure? (y/n)";
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
	//folder.playRandom();
	
	//playVideo(file.path.c_str(), file.name.c_str());
	getchar();
}