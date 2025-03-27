#include "login.h"
#include "ui_login.h"
#include "studentsql.h"
#include "driver.h"
#include <QMessageBox>
#include<QDebug>
#include <QSqlError>
#include <QPropertyAnimation>

login::login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
    //this->setLayout(ui->gridLayout);//设置Layout跟随窗口拉伸
    this->setFixedSize(522, 280);  // 设置窗口大小为固定400x300，不可调整大小
    //db = driver::getdb();
}

login::~login()
{
    delete ui;
}


//获取登录用户名
QString login::getUserName()
{
    //qDebug() << "用户名为：" << ui->le_YongHuMingText->text();
    return ui->le_YongHuMingText->text();
}

//登录按钮
void login::on_btn_DengLu_clicked()
{
    QSqlQuery query(db);
    QMessageBox msgBox;
    msgBox.setWindowTitle("注意");
    msgBox.setIcon(QMessageBox::Warning);

    //用户名和密码不能为空
    if(ui->le_MiMaText->text().isEmpty() || ui->le_YongHuMingText->text().isEmpty()){
        msgBox.setText("用户名和密码不能为空!");
        msgBox.exec();
        return;
    }else{
        //从数据库查找是否有这个用户
        if(!query.exec(QString("SELECT * FROM student WHERE name = '%1'").arg(ui->le_YongHuMingText->text()))){
            qDebug() << "登录界面查询失败原因：" << query.lastError().text();
            msgBox.exec();
            return;
        }
    }

    // **必须检查是否查询到数据**
    if (query.next()) {
        emit sendLoginSuccess();  // **登录成功，发送信号**
        accept(); // **关闭对话框，返回 Accepted**
    } else {
        msgBox.setText("用户名或密码错误！");
        msgBox.exec();
    }
}

//退出按钮
void login::on_btn_TuiChu_clicked()
{
    close();
}

