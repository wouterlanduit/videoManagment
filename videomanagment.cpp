#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>
#include <QLayout>
#include <QComboBox>
#include <QSlider>
#include <QPushButton>
#include <QLabel>
#include <QInputDialog>
#include <QEventLoop>
#include <QTableWidget>
#include <QWidgetItem>
#include <QWidget>
#include <QCloseEvent>
#include <QtGui>
#include <QPoint>

#include <vector>
#include <iostream>
#include <filesystem>
#include <ctime>

#include "videomanagment.h"
#include "ui_videomanagment.h"
#include "playback.h"
#include "files.h"
#include <string>
#include "dialog.h"
#include "options.h"
#include "mytablewidget.h"

using namespace std;

VideoManagment::VideoManagment(QWidget *parent): QMainWindow(parent), ui(new Ui::VideoManagment){
    folder = NULL;
    pathOptions = new QString[NUM_OPTIONS];
    ui->setupUi(this);
    setupTable();
    this->loadSettings();
    ui->actionSave->setEnabled(false);
    if(pathOptions[OPTION_STDOPENCHECK].toInt()==Qt::Checked){
        if(!pathOptions[OPTION_STDOPEN].isEmpty()){
            openFolder(pathOptions[OPTION_STDOPEN]);
        }
    }
}

VideoManagment::~VideoManagment(){
    if(folder != NULL){
        delete folder;
    }
    this->clearTable();
    delete[] pathOptions;
    delete ui;
}

void VideoManagment::setupTable(){
    ui->videoTable->setColumnCount(0);
    ui->videoTable->setRowCount(0);
    ui->videoTable->setContextMenuPolicy(Qt::CustomContextMenu );
    QObject::connect(ui->videoTable, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(cellDoubleClicked(QTableWidgetItem*)));
    QObject::connect(ui->videoTable, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(cellRightClicked(QPoint)));     // voor context menu
}

void VideoManagment::fillTable(Folder* folder){
    int num_videos = folder->files.size();

    ui->videoTable->clear();

    ui->videoTable->setColumnCount(3);
    ui->videoTable->setRowCount(num_videos);
    QStringList column_titles;
    column_titles << "Video" << "Rating" << "Last viewed";
    ui->videoTable->setHorizontalHeaderLabels(column_titles);
    for(int i=0; i<num_videos;i++){
        File* file = &(folder->files[i]);
        QTableWidgetItem *video = new QTableWidgetItem(file->name.c_str());
        //video->setData(Qt::UserRole, &file); //dit lukt niet
        QTableWidgetItem *rating;
        DateTableWidgetItem *last_viewed = new DateTableWidgetItem();
        rating = new QTableWidgetItem();
        if(file->rating>=0){
           rating->setData(Qt::DisplayRole, file->rating);
        }/*else{
            rating->setData(Qt::StatusTipRole, file->rating);
        }*/

        if(file->lastWatched > 0){
            char date[15];
            std::strftime(date, sizeof(date), "%d/%m/%Y", std::localtime(&(file->lastWatched)));
            last_viewed->setText(date);
        }else{
            last_viewed->setText("");
        }

        video->setFlags(video->flags() & ~Qt::ItemIsEditable);
        rating->setFlags(rating->flags() & ~Qt::ItemIsEditable);
        last_viewed->setFlags(last_viewed->flags() & ~Qt::ItemIsEditable);

        //try to use Qt::UserRole+1 to sort
        video->setData(Qt::UserRole+1, file->name.c_str());
        rating->setData(Qt::UserRole+1, file->rating);
        last_viewed->setData(Qt::UserRole+1, file->lastWatched);

        ui->videoTable->setItem(i, 0, video);
        ui->videoTable->setItem(i, 1, rating);
        ui->videoTable->setItem(i, 2, last_viewed);
    }

    //sorting als columnheader geklikt wordt
    //ui->videoTable->setSortRole(Qt::UserRole+1);
    ui->videoTable->setSortingEnabled(true);



    return;
}

void VideoManagment::clearTable(){
    int rows = ui->videoTable->rowCount();
    int columns = ui->videoTable->columnCount();

    for(int i=0; i<rows; i++){
        for(int j=0; j<columns; j++){
            delete ui->videoTable->item(i, j);
        }
    }
    return;
}


void VideoManagment::loadSettings(){    //ik doe hier precies wel weinig veiligheidschecks
    ifstream file;
    file.open(SETTINGS_FILE, ios::in);
    int i;
    if(file.is_open()){
        string temp;
        i=0;
        while(file>>temp && i<NUM_OPTIONS){//reads untill '/n' or ' ' (second conditie is for safety)
            if(i<NUM_OPTIONS){
                if(temp[temp.size()-1] == ';'){ //einde van de optie is bereikt
                    //het eindteken eraf halen
                    std::string::iterator it = temp.end();
                    it--;
                    temp.erase(it);
                    //toevoegen aan de optie
                    this->pathOptions[i] += temp.c_str();
                    //klaar voor de volgende optie
                    i++;
                }else{
                    this->pathOptions[i] += temp.c_str();
                    this->pathOptions[i] += ' ';
                }
            }
        }
        file.close();
    }else{
        // settings file bestaat misschien nog niet -> een maken met defaults geladen
    }

    return;
}

void VideoManagment::openFolder(QString folderName){
    std::string str = std::tr2::sys::path(folderName.toStdString()).directory_string();
    char* fold = new char[str.size()+1];
    for(unsigned int i=0; i<=str.size();i++){
        fold[i] = str[i];
    }
    fold[str.size()] = '\0';
    folder = new Folder(fold);
    folder->loadDataFile();
    folder->updateDataFile();
    this->fillTable(folder);
    ui->actionSave->setEnabled(true);
    return;
}

/***********/
/*OWN SLOTS*/
/***********/

void VideoManagment::cellDoubleClicked(QTableWidgetItem* item){
    if(item->column()!=0){
        item = this->ui->videoTable->item(item->row(), 0);
    }
    QString tekst = item->text();
    std::string str = tekst.toStdString();
    char* fileName = new char[str.size()+1];
    for(unsigned int i=0; i<=str.size();i++){
        fileName[i] = str[i];
    }
    File* file = this->folder->searchFile(this->folder->name.c_str(), fileName);
    //File* file = item->data(Qt::UserRole);    //dit lukt niet
    delete[] fileName;
    if(file != NULL){
        file->playVideo();
        this->folder->unsaved = true;
        //give rating
        if(NUM_OPTIONS>OPTION_DEFAULTCLICK && (pathOptions[OPTION_DEFAULTCLICK].toInt() == Qt::Checked)){
            switch(QMessageBox::question(this,"Rate","Would you like to rate this trailer?", QMessageBox::Yes, QMessageBox::No)){
            case QMessageBox::Yes:{
                int rating = file->rate();
                if(rating >= 0){
                    QTableWidgetItem* rateItem =  this->ui->videoTable->item(item->row(), 1);
                    rateItem->setData(Qt::DisplayRole, rating);
                    rateItem->setData(Qt::UserRole+1, rating);
                }
                break;}
            case QMessageBox::No:
                break;
            };
        }
        QTableWidgetItem* lastViewedItem = this->ui->videoTable->item(item->row(), 2);
        char date[15];
        time_t watchTime = time(nullptr);
        std::strftime(date, sizeof(date), "%d/%m/%Y", std::localtime(&(watchTime)));
        lastViewedItem->setText(date);
        lastViewedItem->setData(Qt::UserRole+1, watchTime);
    }else{
        QMessageBox::critical(this, "FILE NOT FOUND", "Error: Could not find the specified file.", QMessageBox::Ok);
    }
}

void VideoManagment::cellRightClicked(const QPoint &point){
    qDebug("item clicked");
    QMenu menu(this);
    QAction *play = menu.addAction("Play trailer");
    QAction *rate = menu.addAction("Change rating");
    QAction *action = menu.exec(ui->videoTable->viewport()->mapToGlobal(point));
    QTableWidgetItem* item = ui->videoTable->currentItem();
    // moet hier niet nagegaan worden of item != NULL  ???
    if(action == play){
        this->cellDoubleClicked(item);
    }else if(action == rate){
        int row = item->row();
        std::string str = ui->videoTable->item(row,0)->text().toStdString();       //column 0 = filename
        char* fileName = new char[str.size()+1];
        for(unsigned int i=0; i<=str.size();i++){
            fileName[i] = str[i];
        }
        File* file = this->folder->searchFile(this->folder->name.c_str(), fileName);
        delete[] fileName;
        if(file != NULL){
            int rating = file->rate();
            if(rating >= 0){
                this->folder->unsaved = true;
                QTableWidgetItem* rateItem =  this->ui->videoTable->item(row, 1); // column 1 = rating
                rateItem->setData(Qt::DisplayRole, rating);
                rateItem->setData(Qt::UserRole+1, rating);
            }
        }else{
            QMessageBox::critical(this, "FILE NOT FOUND", "Error: Could not find the specified file.", QMessageBox::Ok);
        }
    }
}





/***********/
/*FILE MENU*/
/***********/

void VideoManagment::on_actionSave_triggered(){
    folder->saveDataFile();
    return;
}

void VideoManagment::on_actionExit_triggered(){
    if(folder){
        if(this->folder->unsaved){
            switch(QMessageBox::question(this,"Exit","You have unsaved changes, would you like to save them?", QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel)){
            case QMessageBox::Yes:
                folder->saveDataFile();
            case QMessageBox::No:
                exit(0);
                break;
            case QMessageBox::Cancel:
                break;
            };
        }else{
            switch(QMessageBox::question(this,"Exit","Are you sure?", QMessageBox::Yes, QMessageBox::No)){
            case QMessageBox::Yes:
                exit(0);
                break;
            case QMessageBox::No:
                break;
            };
        }
    }else{
        exit(0);
    }
    return;
}

void VideoManagment::on_actionOpen_triggered(){
    //QString filename = QFileDialog::getOpenFileName(this, "Open File", this->optStdFolder,"Data files (*.dat);;All files (*.*)");
    QString folderName = QFileDialog::getExistingDirectory(this, "Open folder",this->pathOptions[0]);
    //if( !filename.isEmpty() ){
    if(!folderName.isEmpty()){
        openFolder(folderName);
        /*std::string str = std::tr2::sys::path(folderName.toStdString()).directory_string();
        char* fold = new char[str.size()+1];
        for(unsigned int i=0; i<=str.size();i++){
            fold[i] = str[i];
        }
        fold[str.size()] = '\0';
        folder = new Folder(fold);
        folder->loadDataFile();
        folder->updateDataFile();
        this->fillTable(folder);
        ui->actionSave->setEnabled(true);*/
    }
    return;
}

/************/
/*VIDEO MENU*/
/************/

void VideoManagment::on_actionRate_random_triggered(){
    if(folder == NULL){
        QMessageBox::critical(this, "ERROR: NO FILE", "You haven't opened a file yet.", QMessageBox::Ok);
    }else{
        File* file = folder->playRandom();
        file->rate();
    }

    return;
}

void VideoManagment::on_actionWatch_random_triggered(){
    if(folder == NULL){
        QMessageBox::critical(this, "ERROR: NO FILE", "You haven't opened a file yet.", QMessageBox::Ok);
    }else{

    }

    return;
}

/************/
/*EXTRA MENU*/
/************/

void VideoManagment::on_actionOptions_triggered(){
    OptionsDialog* options = new OptionsDialog(this->pathOptions);
    if(options->exec()){
        ofstream file;
        file.open(SETTINGS_FILE, ios::out);
        if (!file.is_open()){
            QMessageBox::critical(this, "ERROR: NO FILE", "Error saving settings", QMessageBox::Ok);
        }else{

            for(int i=0;i<NUM_OPTIONS; i++){
                //opslaan in data file (voor toekomstige sessies)
                file << options->getOption(i).toStdString() << ';' << endl;
                //aanpassen voor huidige sessie
                this->pathOptions[i] = options->getOption(i);
            }
            file.close();        
        }
    }
    delete options;
    return;
}

/***********/
/*HELP MENU*/
/***********/

/*CLOSE EVENT*/

void VideoManagment::closeEvent(QCloseEvent *event){
    if(folder){
        if(this->folder->unsaved){
            switch(QMessageBox::question(this,"Exit","You have unsaved changes, would you like to save them?", QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel)){
            case QMessageBox::Yes:
                folder->saveDataFile();
                event->accept();
                break;
            case QMessageBox::No:
                event->accept();
                break;
            case QMessageBox::Cancel:
                event->ignore();
                break;
            };
        }else{
            switch(QMessageBox::question(this,"Exit","Are you sure?", QMessageBox::Yes, QMessageBox::No)){
            case QMessageBox::Yes:
                event->accept();
                break;
            case QMessageBox::No:
                event->ignore();
                break;
            };
        }
    }else{
        event->accept();
    }
}
