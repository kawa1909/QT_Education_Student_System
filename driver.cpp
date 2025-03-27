#include "driver.h"
#include <qcoreapplication.h>
#include<QSqlError>


QSqlDatabase driver::db;//静态成员变量类外定义

//QString UserName_G = "";
//QString ClassID_G = "";
//QString ClassName_G = "";

//驱动加载数据库
void driver::init_db()
{
    db = QSqlDatabase::addDatabase("QSQLITE");  // 指定数据库类型
#if 0
    auto str = QCoreApplication::applicationDirPath();//找到当前执行文件所在的目录
    QDir dbDir(str);
    // 再回到上一层
    dbDir.cdUp();
    // 获取数据库文件所在目录
    QString dbPath = dbDir.absolutePath();
    db.setDatabaseName(dbPath + "/Data.db");
#endif
    db.setDatabaseName("D:/QtCode/Education_Student_System/Data.db");  // 数据库文件（在项目目录下）
    qDebug() << "Qt 连接的数据库文件路径：" << db.databaseName();
    if (!db.open()) {
        qDebug() << "数据库打开失败：" << db.lastError().text();
    }
    qDebug() << "数据库连接成功！";
}

QSqlDatabase& driver::getdb() {
    return db;
}
