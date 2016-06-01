#ifndef _MYTABLEWIDGET_H
#define _MYTABLEWIDGET_H

#include <QTableWidgetItem>

class DateTableWidgetItem : public QObject, public QTableWidgetItem{
    Q_OBJECT
    public:
        bool operator< ( const QTableWidgetItem &other ) const;
};

#endif // _MYTABLEWIDGET_H

