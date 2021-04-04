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
#include "settings.h"

namespace Ui {
class VideoManagment;
}

class VideoManagment : public QMainWindow{
    Q_OBJECT

public:
    explicit VideoManagment(QWidget *parent = nullptr);
    ~VideoManagment() override;

    QString getStdFolder(){return settings.getSetting(OPTION_STDSEARCH);}
    void setStdFolder(QString folder){settings.updateSetting(OPTION_STDSEARCH, folder);return;}

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
    Settings settings;

    void setupTable();
    void fillTable(Folder*);
    void clearTable();
    void loadSettings();
    void openFolder(QString name);
};

#endif // VIDEOMANAGMENT_H
