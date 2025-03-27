#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QApplication>
#include <QMainWindow>
#include<QDebug>
#include<QTreeWidgetItem>

#include"login.h"
#include "studentsql.h"
#include "ui_mainwindow.h"
#include "ui_login.h"
#include "driver.h"
#include "addstudent.h"
#include "modifyingstudent.h"



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setUserName(const QString &username);//读取用户名

private slots:
    void on_btn_TuiChu_clicked();

    void on_btn_ShanChu_clicked();

    void on_btn_SouSuo_clicked();//搜索名字

    void updateSearchStatus(int count);  // 查询完成槽

    void on_SearchTextChanged();//但搜索文本为空时自动恢复数据

    void on_checkBox_stateChanged(int arg1);//全选按钮

    void on_btn_ZengJia_clicked();//增加功能

    void showMainWindow();          // 重新显示主窗口

    void on_btn_XiuGai_clicked();

    void refreshStudentData();
signals:

private:
    Ui::MainWindow *ui;
    login w_login;//登录界面
    studentSQL dbHelper;  // 创建 studentSQL 对象
    bool isQuanXuan = false;  // 用来标记是否为全选状态
    addStudent *addStudent_Window;//添加学生子窗口
    modifyingStudent *modify_Window;//修改学生子窗口
};
#endif // MAINWINDOW_H




