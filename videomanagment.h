#ifndef VIDEOMANAGMENT_H
#define VIDEOMANAGMENT_H

#include <QMainWindow>
#include <QString>
#include <QWidget>
#include <QCloseEvent>
#include <QTableWidget>
#include <QWidgetItem>
#include <QPoint>

#include "files.h"

namespace Ui {
class VideoManagment;
}

class VideoManagment : public QMainWindow{
    Q_OBJECT

public:
    explicit VideoManagment(QWidget *parent = 0);
    ~VideoManagment();

    QString getStdFolder(){return this->pathOptions[0];}
    void setStdFolder(QString folder){pathOptions[0]=folder;return;}

protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void cellDoubleClicked(QTableWidgetItem*);
    void cellRightClicked(const QPoint&);

    void on_actionExit_triggered();
    void on_actionOpen_triggered();
    void on_actionRate_random_triggered();
    void on_actionWatch_random_triggered();
    void on_actionSave_triggered();
    void on_actionOptions_triggered();

private:
    Ui::VideoManagment *ui;
    Folder* folder;  //lijst van folder? (zodat er meerdere kunnen openstaan) en dan int om bij te houden welke je momenteel gebruikt
    QString* pathOptions;
        //1) standard search folder
        //2) VLC folder
        //3) standard opening folder

    void setupTable();
    void fillTable(Folder*);
    void clearTable();
    void loadSettings();
    void openFolder(QString name);
};

#endif // VIDEOMANAGMENT_H
