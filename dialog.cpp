#include <QDialog>
#include <QSlider>
#include <QLabel>
#include <QPushButton>
#include <QLayout>
#include <QFileDialog>
#include <QMessageBox>

#include <iostream>
#include <filesystem>

#include "dialog.h"
#include "options.h"

using namespace std;

void RateDialog::setupDialog(){ //string meegeven std::String prevRate
    this->setWindowTitle("Rate");
    QLabel* previousScore = new QLabel("previous rating:");
#ifdef USE_COMBOBOX
    QComboBox* score = new QComboBox;
    score->addItem("0", 0);score->addItem("1", 1);score->addItem("2", 2);
    score->addItem("3", 3);score->addItem("4", 4);score->addItem("5", 5);
    score->addItem("6", 6);score->addItem("7", 7);score->addItem("8", 8);
    score->addItem("9", 9);score->addItem("10", 10);
#elif defined USE_SLIDER
    QSlider* score = new QSlider(Qt::Horizontal);
    score->setRange(0,10);
    score->setTickPosition(QSlider::TicksBelow);
#endif
    QPushButton* button_ok = new QPushButton("OK");
    QPushButton* button_cancel = new QPushButton("Cancel");

#ifdef USE_SLIDER
    QLabel* scoreLabel = new QLabel;
    QObject::connect(score, SIGNAL(valueChanged(int)), scoreLabel, SLOT(setNum(int)));
    score->setValue(5);
#endif
    QObject::connect(button_ok, SIGNAL(clicked()), this, SLOT(accept()));
    QObject::connect(button_cancel, SIGNAL(clicked()), this, SLOT(reject()));
    QObject::connect(score, SIGNAL(valueChanged(int)),this, SLOT(setRating(int)));
    QHBoxLayout *HlayoutTop = new QHBoxLayout;
    QHBoxLayout *HlayoutBot = new QHBoxLayout;
    QVBoxLayout *Vlayout = new QVBoxLayout;
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

void RateDialog::setRating(int score){
    this->rating = score;
    return;
}

void OptionsDialog::setupDialog(){
    this->setWindowTitle("Options");
    this->setMinimumSize(QSize(400, 200));

    //set folder option
    labelFolder = new QLabel("folder: " + this->stdFolder);
    QPushButton* button_browse = new QPushButton("Browse");
    button_browse->setMaximumSize(50,20);
    QObject::connect(button_browse, SIGNAL(clicked()), this, SLOT(chooseStdFolder()));
    QObject::connect(button_browse, SIGNAL(clicked()), this, SLOT(setLabel()));
    QHBoxLayout *HlayoutFolder = new QHBoxLayout;
    HlayoutFolder->addWidget(labelFolder);
    HlayoutFolder->addWidget(button_browse);

    //set buttons
    QPushButton* button_ok = new QPushButton("OK");
    button_ok->setMaximumSize(50,20);
    QPushButton* button_cancel = new QPushButton("Cancel");
    button_cancel->setMaximumSize(50,20);
    QObject::connect(button_ok, SIGNAL(clicked()), this, SLOT(accept()));
    QObject::connect(button_cancel, SIGNAL(clicked()), this, SLOT(reject()));
    QHBoxLayout *HlayoutButtons = new QHBoxLayout;
    HlayoutButtons->addStretch();
    HlayoutButtons->addWidget(button_ok);
    HlayoutButtons->addWidget(button_cancel);

    //assemble layout
    QVBoxLayout *Vlayout = new QVBoxLayout;
    Vlayout->addLayout(HlayoutFolder);
    Vlayout->addStretch();
    Vlayout->addLayout(HlayoutButtons);
    this->setLayout(Vlayout);

    return;
}

void OptionsDialog::chooseStdFolder(){
    QString folderName = QFileDialog::getExistingDirectory(this, "Choose working directory",this->stdFolder);
    if(!folderName.isEmpty()){
        this->stdFolder = folderName;
    }
    return;
}

void OptionsDialog::setLabel(){
    this->labelFolder->setText(this->stdFolder);
}
