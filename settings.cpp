#include <fstream>

#include <QString>
#include <QMessageBox>

#include "settings.h"
#include "options.h"

using namespace std;

Settings::Settings(Settings& settings){
    this->updateSettings(settings);
}

void Settings::loadSettings(std::string const& filePath){    //ik doe hier precies wel weinig veiligheidschecks
    ifstream file;
    file.open(filePath, ios::in);
    int i;

    if(file.is_open()){
        string temp;
        string option;
        i=0;
        while(file>>temp && i<NUM_OPTIONS){//reads untill '/n' or ' ' (second conditie is for safety)
            option.append(temp);
            option += ' ';
            if(temp[temp.size()-1] == ';'){ //einde van de optie is bereikt
                option.pop_back();  // pop ' '
                option.pop_back();  // pop ';'
                if(i < NUM_OPTIONS)
                {
                    settingsArray[i] = QString(option.c_str());
                }
                i++;
                option.clear();
            }
        }
        file.close();
    }else{
        // settings file bestaat misschien nog niet -> een maken met defaults geladen
    }

    return;
}

void Settings::updateSettings(Settings& settings){
    for(int i=0; i<NUM_OPTIONS; i++){
        this->updateSetting(i, settings.settingsArray[i]);
    }
}

QString Settings::getSetting(int location){
    QString ret;
    if(location < NUM_OPTIONS)
    {
        ret = settingsArray[location];
    }
    return ret;
}

void Settings::updateSetting(int location, QString updatedSetting){
    if(location < NUM_OPTIONS){
        settingsArray[location] = updatedSetting;
    }
}
int Settings::saveSettings(std::string const& filePath){
    int returnValue = 0;
    ofstream file;
    file.open(filePath, ios::out);
    if (!file.is_open()){
        returnValue = -1;
        //QMessageBox::critical(widget, "ERROR: NO FILE", "Error saving settings", QMessageBox::Ok);
    }else{
        for(int i=0; i <  NUM_OPTIONS; i++){
            file << settingsArray[i].toStdString() << ';' << endl;
        }
        file.close();
    }

    return returnValue;
}
