#ifndef LOGIN_H
#define LOGIN_H
#include <QSqlQuery>
#include <QWidget>
#include<QDialog>

namespace Ui {
class login;
}
class login : public QDialog
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    ~login();
    QString getUserName();  // **获取登录的用户名**

private slots:
    void on_btn_DengLu_clicked();//登录按钮
    void on_btn_TuiChu_clicked();//退出按钮



signals:
    void sendLoginSuccess();//成功登录信号

private:
    Ui::login *ui;
    QSqlDatabase db;
};

#endif // LOGIN_H
