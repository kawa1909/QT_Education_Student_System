#ifndef ADDSTUDENT_H
#define ADDSTUDENT_H
#include <QWidget>
#include "studentsql.h"
#include <QApplication>

namespace Ui {
class addStudent;
}

class addStudent : public QWidget
{
    Q_OBJECT

public:
    explicit addStudent(QWidget *parent = nullptr);
    ~addStudent();


private slots:
    void on_btn_GengXin_clicked();

    void on_btn_TuiChu_clicked();

signals:
    void addStudent_closed();  // 自定义关闭信号
    void addStudent_update();//刷新主界面数据

private:
    Ui::addStudent *ui;
    QSqlDatabase db;
    studentSQL dbHelper;

};

#endif // ADDSTUDENT_H
