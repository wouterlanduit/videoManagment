#ifndef _DIALOG_H
#define _DIALOG_H
#include <QDialog>
#include <QLabel>
#include "options.h"
#include <string>

class RateDialog : public QDialog{

    Q_OBJECT

private:
    int rating;
    void setupDialog();

public:
    RateDialog() : rating(5) {
        this->setupDialog();        //nog alles verwijderen na gebruik !!!!!
    }

    int getRating(){
        return rating;
    }


private slots:
    void setRating(int);

};

class OptionsDialog : public QDialog{

    Q_OBJECT

private:
    QString stdFolder;
    QLabel* labelFolder;
    void setupDialog();

public:
    OptionsDialog(): stdFolder("C:/"){ this->setupDialog(); }
    OptionsDialog(QString stdFolder): stdFolder(stdFolder){ this->setupDialog(); }
    QString getStdFolder(){ return this->stdFolder; }

private slots:
    void chooseStdFolder();
    void setLabel();
};


#endif // DIALOG

