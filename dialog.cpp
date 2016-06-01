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

#include "dialog.h"
#include "options.h"

using namespace std;

/************/
/*RATEDIALOG*/
/************/

void RateDialog::setupDialog(QString prevRate){
    this->setWindowTitle("Rate");
    previousScore = new QLabel("previous rating: " + prevRate);
#ifdef USE_COMBOBOX
    score = new QComboBox;
    score->addItem("0", 0);score->addItem("1", 1);score->addItem("2", 2);
    score->addItem("3", 3);score->addItem("4", 4);score->addItem("5", 5);
    score->addItem("6", 6);score->addItem("7", 7);score->addItem("8", 8);
    score->addItem("9", 9);score->addItem("10", 10);
#elif defined USE_SLIDER
    score = new QSlider(Qt::Horizontal);
    score->setRange(0,10);
    score->setTickPosition(QSlider::TicksBelow);
#endif
    button_ok = new QPushButton("OK");
    button_cancel = new QPushButton("Cancel");

#ifdef USE_SLIDER
    scoreLabel = new QLabel;
    QObject::connect(score, SIGNAL(valueChanged(int)), scoreLabel, SLOT(setNum(int)));
    score->setValue(5);
#endif
    QObject::connect(button_ok, SIGNAL(clicked()), this, SLOT(accept()));
    QObject::connect(button_cancel, SIGNAL(clicked()), this, SLOT(reject()));
    QObject::connect(score, SIGNAL(valueChanged(int)),this, SLOT(setRating(int)));
    HlayoutTop = new QHBoxLayout;
    HlayoutBot = new QHBoxLayout;
    Vlayout = new QVBoxLayout;
    HlayoutTop->addWidget(score);
    HlayoutTop->addWidget(scoreLabel);
    HlayoutBot->addWidget(button_ok);
    HlayoutBot->addWidget(button_cancel);
    Vlayout->addWidget(previousScore);
    Vlayout->addLayout(HlayoutTop);
    Vlayout->addLayout(HlayoutBot);
    this->setLayout(Vlayout);

    return;
}

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

    //assemble layout
    QVBoxLayout* Vlayout = new QVBoxLayout();


    // Standard folder to start searching
    QLabel* labelStdSearch = new QLabel("Choose standard search folder:");
    stdSearch = new QLabel(this->options[OPTION_STDSEARCH]);
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
    video = new QLabel(this->options[OPTION_VIDEO]);
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
        if(options[OPTION_STDOPENCHECK].toInt()==Qt::Checked){
            checkbox->setChecked(true);
        }
        QObject::connect(checkbox, SIGNAL(stateChanged(int)), this, SLOT(checkedOpeningFolder(int)));
        VLayoutStdOpen->addWidget(checkbox);
        // path
        stdOpen = new QLabel(this->options[OPTION_STDOPEN]);
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
        if(options[OPTION_DEFAULTCLICK].toInt() == Qt::Checked){
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
    QString folderName = QFileDialog::getExistingDirectory(this, "Choose working directory",this->options[OPTION_STDSEARCH]);
    if(!folderName.isEmpty()){
        this->options[OPTION_STDSEARCH] = folderName;
    }
    this->stdSearch->setText(this->options[OPTION_STDSEARCH]);
    return;
}

void OptionsDialog::chooseVideoProgram(){
    QString filename = QFileDialog::getOpenFileName(this, "Choose program", this->options[OPTION_VIDEO],"executables (*.exe);;All files (*.*)");
    //QString folderName = QFileDialog::getExistingDirectory(this, "Choose working directory",this->folders[1]);
    if(!filename.isEmpty()){
        this->options[OPTION_VIDEO] = filename;
    }
    this->video->setText(this->options[OPTION_VIDEO]);
    return;
}

void OptionsDialog::chooseOpeningFolder(){
    QString folderName = QFileDialog::getExistingDirectory(this, "Choose working directory",this->options[OPTION_STDOPEN]);
    if(!folderName.isEmpty()){
        this->options[OPTION_STDOPEN] = folderName;
    }
    this->stdOpen->setText(this->options[OPTION_STDOPEN]);
    return;
}

void OptionsDialog::chooseDefaultClick(int state){
    //qDebug("chooseDefaultClick");
    this->options[OPTION_DEFAULTCLICK] = QString::number(state);
}

void OptionsDialog::checkedOpeningFolder(int state){
    this->options[OPTION_STDOPENCHECK] = QString::number(state);
}
