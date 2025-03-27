#ifndef DRIVER_H
#define DRIVER_H
#include <QString>
#include <QtDebug>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>

class driver
{
public:
    driver(){

    }
    static void init_db();
    static QSqlDatabase& getdb();  // **返回数据库引用**

private:
    static QSqlDatabase db;
};

#endif // DRIVER_H
