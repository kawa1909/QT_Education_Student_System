#include "addstudent.h"
#include "ui_addstudent.h"
#include<QMessageBox>
#include<QCloseEvent>

addStudent::addStudent(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addStudent)
{
    ui->setupUi(this);
    this->setLayout(ui->horizontalLayout_9);
}


addStudent::~addStudent()
{
    delete ui;
}

//更新数据
void addStudent::on_btn_GengXin_clicked()
{
    //QSqlQuery query(db);
    StudentInfo newStudent;
    newStudent.name = ui->lineEdit_name->text();
    newStudent.age = ui->lineEdit_age->text().toInt();
    newStudent.Myclass = ui->lineEdit_class->text().toInt();
    newStudent.grade = ui->lineEdit_grade->text().toInt();
    newStudent.studentID = ui->lineEdit_studentID->text().toInt();
    newStudent.phone = ui->lineEdit_phone->text();
    newStudent.wechat = ui->lineEdit_wechat->text();
    //学生信息补全
    if(ui->lineEdit_name->text().isEmpty()
            || ui->lineEdit_studentID->text().isEmpty()
            || ui->lineEdit_age->text().isEmpty()
            || ui->lineEdit_class->text().isEmpty()
            || ui->lineEdit_grade->text().isEmpty()
            || ui->lineEdit_phone->text().isEmpty()
            || ui->lineEdit_wechat->text().isEmpty()){
        QMessageBox::information(this, "添加", "请将学生信息补充完整！");
    }else{
        //传递学号查询该学生是否已经存在
        if(dbHelper.checkStudent(ui->tableWidget, ui->lineEdit_studentID->text().toInt())){
            QMessageBox::information(this, "错误", "该学生已存在！");
        }else{
            dbHelper.addStudent(newStudent);//添加学生
            dbHelper.displayData(ui->tableWidget);
            emit addStudent_update();//发送一个信号刷新主界面数据
        }
    }
}

//退出添加学生信息界面
void addStudent::on_btn_TuiChu_clicked()
{
    close();  // 关闭当前子窗体
    emit addStudent_closed();//发出关闭子窗口信号
}



