#include "videomanagment.h"
#include <QApplication>
#include <crtdbg.h>

int main(int argc, char *argv[])
{
    QApplication prog(argc, argv);
    VideoManagment w;
    w.show();

    return prog.exec();
}

/*******/
/*TO DO*/
/*******/

//0. Datum opslaan is naar de ballen -> fix zo rap mogelijk

//1. Log anders organiseren: nu onmogelijk om meerdere filesmanagmentens te loggen

//2. Probleem met niet-hoofdlettergevoeligheid van File::exists()-> mooier proberen oplossen

//3. About en Help schrijven

//4. Bij openen video die tijdens uitvoering verwijderd werd -> foutboodschap weergeven en niet crashen

//5. Als log file bestaat -> vorige keer abnormaal afgesloten: zeg dit en vraag om de aanpassingen te herstellen

//6. random trailers bekijken met opgegeven rating (lijst van ratings/min rating)

//7. meerdere folders tegelijk open kunnen hebben staan: lijst van pointers naar Folder + ref naar huidige folder -> extra menu met openstaande folder in

//9. wat als er subfolders zijn??

//10. Wat als log-entry niet gevonden wordt (ondertussen hernoemd of verwijderd)? -> geen probleem denk ik: je laadt de logs in op datafile voordat je ze update met nieuwe of gedeletete file
