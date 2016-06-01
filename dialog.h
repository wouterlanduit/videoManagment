#ifndef _DIALOG_H
#define _DIALOG_H
#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include <QSlider>
#include <QLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QCheckBox>
#include "options.h"
#include <string>

#define OPTION_STDSEARCH 0
#define OPTION_VIDEO 1
#define OPTION_STDOPEN 2
#define OPTION_STDOPENCHECK 3
#define OPTION_DEFAULTCLICK 4


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

class OptionsDialog : public QDialog{

    Q_OBJECT

private:
    QString options[NUM_OPTIONS];
    QLabel* stdSearch;
    QLabel* video;
    QLabel* stdOpen;
    QCheckBox* openFolder;
    QCheckBox* rate;
    void setupDialog();

public:
    //CONSTRUCTORS
    OptionsDialog(){
        this->options[0] = "C:/";
        this->setupDialog();
    }
    OptionsDialog(QString* options){
        for(int i=0; i<NUM_OPTIONS; i++){
            this->options[i] = options[i];
        }
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
    QString getOption(int index){ return this->options[index]; }

private slots:
    void chooseStdFolder();
    void chooseVideoProgram();
    void chooseOpeningFolder();
    void chooseDefaultClick(int state);
    void checkedOpeningFolder(int state);
};

#endif // DIALOG

