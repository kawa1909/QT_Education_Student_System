#include "mainwindow.h"//学生信息管理文件
#include <QApplication>
#include "driver.h"//数据库驱动文件
#include "login.h"//登录文件
#include "studentsql.h"//数据库操作文件

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    driver::init_db();//连接数据库
    login loginWindow;  // 先创建登录窗口

    if (loginWindow.exec() == QDialog::Accepted)  // 等待用户登录
    {
        QString username = loginWindow.getUserName();
        MainWindow w;
        w.setUserName(username);  // **先传递用户名**
        w.show();  // **然后再显示主界面**
        //studentSQL stuSql;//登录成功后，再显示数据库信息
        return a.exec();
    }

    return 0;  //如果登录失败，程序退出
}
