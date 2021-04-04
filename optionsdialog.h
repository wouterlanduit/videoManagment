#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H
#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include <QSlider>
#include <QLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QCheckBox>

#include <string>

#include "settings.h"

class OptionsDialog : public QDialog{

    Q_OBJECT

private:
    Settings tmpSettings;
    QLabel* stdSearch;
    QLabel* video;
    QLabel* stdOpen;
    QCheckBox* openFolder;
    QCheckBox* rate;
    void setupDialog();

public:
    //CONSTRUCTORS
    OptionsDialog(){
        this->setupDialog();
    }

    OptionsDialog(Settings& settings){
        this->tmpSettings = Settings(settings);
        this->setupDialog();
    }

    //DESTRUCTORS
    ~OptionsDialog(){
        delete stdSearch;
        delete video;
        delete stdOpen;
        //delete rate;
        //delete play;
    }


    //METHODS
    QString getOption(int index){ return tmpSettings.getSetting(index); }
    void setOption(int index, const QString& option){ tmpSettings.updateSetting(index, option); }

private slots:
    void chooseStdFolder();
    void chooseVideoProgram();
    void chooseOpeningFolder();
    void chooseDefaultClick(int state);
    void checkedOpeningFolder(int state);
};

#endif // OPTIONSDIALOG

