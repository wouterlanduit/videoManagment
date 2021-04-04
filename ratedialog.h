#ifndef RATEDIALOG_H
#define RATEDIALOG_H

#include <QLabel>
#include <QComboBox>
#include <QSlider>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDialog>

#include "options.h"


class RateDialog : public QDialog{
    Q_OBJECT

private:
    int rating;
    QLabel *previousScore, *scoreLabel;
#ifdef USE_COMBOBOX
    QComboBox* score;
#else
    QSlider* score;
#endif
    QPushButton *button_ok, *button_cancel;
    QHBoxLayout *HlayoutTop, *HlayoutBot;
    QVBoxLayout *Vlayout;

    void setupDialog(QString);

public:
    RateDialog(QString prevRate) : rating(5) {
        this->setupDialog(prevRate);        //nog alles verwijderen na gebruik !!!!!
    }

    ~RateDialog(){  // is naar het schijnt niet nodig, widgets worden hierarchisch gedeletet
        delete previousScore;
        delete scoreLabel;
        delete score;
        delete button_ok;
        delete button_cancel;
        delete HlayoutTop;
        delete HlayoutBot;
        delete Vlayout;
    }

    int getRating(){
        return rating;
    }


private slots:
    void setRating(int score){
        this->rating = score;
        return;
    }


};

#endif // RATEDIALOG_H
