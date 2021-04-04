#include "ratedialog.h"
#include "options.h"

#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QObject>
#include <QComboBox>
#include <QSlider>

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
