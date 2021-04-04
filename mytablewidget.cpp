#include "mytablewidget.h"
#include <QTableWidgetItem>

bool DateTableWidgetItem::operator< ( const QTableWidgetItem &other ) const{
    return data(Qt::UserRole+1) < other.data(Qt::UserRole+1);
}
