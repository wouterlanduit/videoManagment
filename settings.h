#ifndef SETTINGS_H
#define SETTINGS_H

#include "options.h"

#include <string>

#include <QWidget>
#include <Qt>

class Settings
{
    private:
        QString settingsArray[NUM_OPTIONS];

    public:
        Settings(){}
        Settings(Settings&);

        void loadSettings(std::string const& filePath);
        int saveSettings(std::string const& filePath);
        /*std::string optionStdSearch;
        std::string optionVideo;
        std::string optionStdOpen;
        Qt::CheckState optionStdOpenCheck;
        Qt::CheckState optionDefaultClick;*/

        QString getSetting(int location);

        void updateSetting(int location, QString updatedSettings);
        void updateSettings(Settings& settings);


        /*QString* getSettingsArray(){return &settingsArray[0];}
        QString getOptionStdSearch(){return settingsArray[OPTION_STDSEARCH];}
        QString getOptionVideo(){return settingsArray[OPTION_VIDEO];}
        QString getOptionStdOpen(){return settingsArray[OPTION_STDOPEN];}
        Qt::CheckState getOptionStdOpenCheck(){return static_cast<Qt::CheckState>(settingsArray[OPTION_STDOPENCHECK].toInt());}
        Qt::CheckState getOptionDefaultClick(){return static_cast<Qt::CheckState>(settingsArray[OPTION_DEFAULTCLICK].toInt());}*/
};

#endif // SETTINGS_H
