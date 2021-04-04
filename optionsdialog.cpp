#include <QDialog>
#include <QSlider>
#include <QLabel>
#include <QPushButton>
#include <QLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QButtonGroup>
#include <QRadioButton>
#include <QCheckBox>

#include <iostream>
#include <filesystem>

#include "optionsdialog.h"
#include "options.h"
#include "settings.h"

using namespace std;


/***************/
/*OPTIONSDIALOG*/
/***************/

//destructor
/*OptionsDialog::~OptionsDialog(){
    for(int i=0; i<NUM_OPTIONS; i++){
        delete labels[i];
        delete buttons[i];
        delete layouts[i];
    }
    delete button_ok;
    delete button_cancel;
    delete HlayoutButtons;
    delete Vlayout;
}*/

//methods
void OptionsDialog::setupDialog(){
    this->setWindowTitle("Options");
    this->setMinimumSize(QSize(400, 200));
    //Settings* settings = Settings::getInstance();

    //assemble layout
    QVBoxLayout* Vlayout = new QVBoxLayout();


    // Standard folder to start searching
    QLabel* labelStdSearch = new QLabel("Choose standard search folder:");
    stdSearch = new QLabel(this->getOption((OPTION_STDSEARCH)));
    stdSearch->setContentsMargins(10,0,0,0);
    QPushButton* buttonStdSearch = new QPushButton("Browse");
    buttonStdSearch->setMaximumSize(50,20);
    QObject::connect(buttonStdSearch, SIGNAL(clicked()), this, SLOT(chooseStdFolder()));
    QVBoxLayout* VLayoutStdSearch = new QVBoxLayout();
    VLayoutStdSearch->addWidget(labelStdSearch);
    VLayoutStdSearch->addWidget(stdSearch);
    QHBoxLayout* HLayoutStdSearch = new QHBoxLayout();
    HLayoutStdSearch->addLayout(VLayoutStdSearch);
    HLayoutStdSearch->addStretch();
    HLayoutStdSearch->addWidget(buttonStdSearch);
    Vlayout->addLayout(HLayoutStdSearch);

    // Vlc directory
    QLabel* labelVideo = new QLabel("VLC program:");
    video = new QLabel(this->getOption(OPTION_VIDEO));
    video->setContentsMargins(10,0,0,0);
    QPushButton* buttonVideo = new QPushButton("Browse");
    buttonVideo->setMaximumSize(50,20);
    QObject::connect(buttonVideo, SIGNAL(clicked()), this, SLOT(chooseVideoProgram()));
    QVBoxLayout* VLayoutVideo = new QVBoxLayout();
    VLayoutVideo->addWidget(labelVideo);
    VLayoutVideo->addWidget(video);
    QHBoxLayout* HLayoutVideo = new QHBoxLayout();
    HLayoutVideo->addLayout(VLayoutVideo);
    HLayoutVideo->addStretch();
    HLayoutVideo->addWidget(buttonVideo);
    Vlayout->addLayout(HLayoutVideo);


    // Standard folder to open upon startup
    if(NUM_OPTIONS>2){
        QVBoxLayout* VLayoutStdOpen = new QVBoxLayout();
        // checkbox
        QCheckBox* checkbox = new QCheckBox("Open folder on startup:");
        if(this->getOption(OPTION_STDOPENCHECK).toInt() == Qt::Checked){
            checkbox->setChecked(true);
        }
        QObject::connect(checkbox, SIGNAL(stateChanged(int)), this, SLOT(checkedOpeningFolder(int)));
        VLayoutStdOpen->addWidget(checkbox);
        // path
        stdOpen = new QLabel(this->getOption(OPTION_STDOPEN));
        stdOpen->setContentsMargins(10,0,0,0);
        VLayoutStdOpen->addWidget(stdOpen);
        // button
        QPushButton* buttonStdOpen = new QPushButton("Browse");
        buttonStdOpen->setMaximumSize(50,20);
        QObject::connect(buttonStdOpen, SIGNAL(clicked()), this, SLOT(chooseOpeningFolder()));
        // group the above
        QHBoxLayout* HLayoutStdOpen = new QHBoxLayout();
        HLayoutStdOpen->addLayout(VLayoutStdOpen);
        HLayoutStdOpen->addStretch();
        HLayoutStdOpen->addWidget(buttonStdOpen);
        Vlayout->addLayout(HLayoutStdOpen);
    }



    // Rate or play by default
    if(NUM_OPTIONS>OPTION_DEFAULTCLICK){
        rate = new QCheckBox("Rate after playing.");
        if(getOption(OPTION_DEFAULTCLICK).toInt() == Qt::Checked){
            rate->setChecked(true);
        }
        QObject::connect(rate, SIGNAL(stateChanged(int)), this, SLOT(chooseDefaultClick(int)));
        Vlayout->addWidget(rate);
    }



    //ok and cancel buttons
    QPushButton* button_ok = new QPushButton("OK");
    button_ok->setMaximumSize(50,20);
    QPushButton* button_cancel = new QPushButton("Cancel");
    button_cancel->setMaximumSize(50,20);
    QObject::connect(button_ok, SIGNAL(clicked()), this, SLOT(accept()));
    QObject::connect(button_cancel, SIGNAL(clicked()), this, SLOT(reject()));
    QHBoxLayout* HlayoutButtons = new QHBoxLayout();
    HlayoutButtons->addStretch();
    HlayoutButtons->addWidget(button_ok);
    HlayoutButtons->addWidget(button_cancel);


    Vlayout->addStretch();
    Vlayout->addLayout(HlayoutButtons);
    this->setLayout(Vlayout);

    return;
}

void OptionsDialog::chooseStdFolder(){
    QString folderName = QFileDialog::getExistingDirectory(this, "Choose working directory",this->getOption(OPTION_STDSEARCH));
    if(!folderName.isEmpty()){
        this->setOption(OPTION_STDSEARCH, folderName);
    }
    this->stdSearch->setText(this->getOption(OPTION_STDSEARCH));
    return;
}

void OptionsDialog::chooseVideoProgram(){
    QString filename = QFileDialog::getOpenFileName(this, "Choose program", this->getOption(OPTION_VIDEO),"executables (*.exe);;All files (*.*)");
    //QString folderName = QFileDialog::getExistingDirectory(this, "Choose working directory",this->folders[1]);
    if(!filename.isEmpty()){
        this->setOption(OPTION_VIDEO, filename);
    }
    this->video->setText(this->getOption(OPTION_VIDEO));
    return;
}

void OptionsDialog::chooseOpeningFolder(){
    QString folderName = QFileDialog::getExistingDirectory(this, "Choose working directory",this->getOption(OPTION_STDOPEN));
    if(!folderName.isEmpty()){
        this->setOption(OPTION_STDOPEN, folderName);
    }
    this->stdOpen->setText(this->getOption(OPTION_STDOPEN));
    return;
}

void OptionsDialog::chooseDefaultClick(int state){
    //qDebug("chooseDefaultClick");
    this->setOption(OPTION_DEFAULTCLICK, QString::number(state));
}

void OptionsDialog::checkedOpeningFolder(int state){
    this->setOption(OPTION_STDOPENCHECK, QString::number(state));
}
