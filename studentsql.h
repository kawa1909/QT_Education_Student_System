#ifndef STUDENTSQL_H
#define STUDENTSQL_H
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <qtablewidget.h>
#include "driver.h"

//#include <mainwindow.h>

//学生类
struct StudentInfo{
    int id;//唯一标识
    QString name;//姓名
    int age;//年龄
    int grade;//年级
    int Myclass;//班级
    int studentID;//学生id
    QString phone;//手机
    QString wechat;//微信
};

//用户类
struct UserInfo{
    //用户名字、密码、权限
    QString UserName, password, aud;
};

class studentSQL : public QObject
{
    Q_OBJECT
public:
    explicit studentSQL(QObject *parent = nullptr);
    ~studentSQL();

    bool connectDB();                      // 连接数据库√
    quint32 getStudentCnt();                // 查询学生数量
    QList<StudentInfo> getPageStudent(quint32 page, quint32 uiCnt);  // 查询分页学生√
    bool addStudent(const StudentInfo &student);  // 增加学生√
    bool delStudent(int id);                // 删除学生√
    void clearStudentTable();               // 清除学生表√
    bool searchStudent(QTableWidget *table, const QString &name);    // 查询学生是否存在√
    int count = 0;//学生总人数
    void displayData(QTableWidget *table);//将数据库的数据放到组件中√
    void restoreData(QTableWidget *table);  // 恢复数据的id，如删除和增加功能需要√
    bool changeStudentInfo(const StudentInfo &student_info); // 修改学生信息√
    bool checkStudent(QTableWidget *table, const int &stuID);//检查学生是否已经在数据库里面了√

#if 0
    //用户后面再做
    //QList<UserInfo> getAllUser();           // 查询所有用户
    void changeUserAut(const QString &strAut); // 修改用户权限
    void addUser(const UserInfo &user_info); // 添加用户
    void delUserName(const QString &strUserName); // 删除用户
#endif




private:
    QSqlDatabase db;

signals:
    void searchCompleted(int count);  //查询完成信号
};

#endif // STUDENTSQL_H
