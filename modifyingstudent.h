#ifndef MODIFYINGSTUDENT_H
#define MODIFYINGSTUDENT_H

#include <QWidget>
#include"studentsql.h"
#include "driver.h"


namespace Ui {
class modifyingStudent;
}

class modifyingStudent : public QWidget
{
    Q_OBJECT

public:
    explicit modifyingStudent(QWidget *parent = nullptr);
    ~modifyingStudent();

    void setStudentInfo(const StudentInfo &student);

    void setData(QList<QTableWidgetItem*> &selectedItems);



private slots:
    void on_btn_WanCheng_clicked();

    void on_btn_XiuGai_clicked();

    void updateLineEdit();



signals:
    void modifyingStudent_Close();//窗口关闭信号

    void addStudent_modifying();//修改完毕信号

private:
    Ui::modifyingStudent *ui;
    studentSQL dbHelper;
    QSqlDatabase db;
};

#endif // MODIFYINGSTUDENT_H
