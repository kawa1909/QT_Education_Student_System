#include "studentsql.h"
#include "driver.h"
#include <QCoreApplication>
#include <QDir>
#include<QSqlRecord>
#include<QHeaderView>
#include<QMessageBox>

studentSQL::studentSQL(QObject *parent) : QObject(parent)
{
    db = driver::getdb();
    //getStudentCnt();//查询所有学生人数
    //    //添加十条学生数据
    //    StudentInfo s;
    //    s.name = "王柳";
    //    for(int i = 0; i < 10; i++){
    //        addStudent(s);
    //    }

    //    QList<StudentInfo> students = getPageStudent(0, 1);
    //    qDebug() << "查询到的学生数量：" << students.size();
    //    for (const StudentInfo &s : students) {
    //        qDebug() << s.id << s.name << s.age;
    //    }

}

studentSQL::~studentSQL()
{
    //    if (db.isOpen()) {
    //        db.close();
    //        qDebug() << "数据库已关闭";
    //    }
    qDebug() << "studentSQL 对象被销毁，但不关闭数据库";
}

//查询学生数量
quint32 studentSQL::getStudentCnt()
{
    QSqlQuery query(db);
    if (!query.exec("SELECT COUNT(*) FROM student")) {  // ✅ 直接执行
        qDebug() << "查询总学生人数失败：" << query.lastError().text();
        return 0;
    }

    if (query.next()) {
        int count = query.value(0).toInt();
        qDebug() << "当前总学生人数：" << count;
        return count;
    }
    return 0;
}

//查询该学生在第几页
QList<StudentInfo> studentSQL::getPageStudent(quint32 page, quint32 uiCnt)
{
    QList<StudentInfo> l;
    // 先获取总记录数，避免 OFFSET 超出范围
    QSqlQuery countQuery(db);
    // 计算 OFFSET，防止超出范围
    int offset = page * uiCnt;
    if (offset >= count) {
        qDebug() << "查询偏移量超出范围，返回空数据";
        return l;
    }

    // 查询指定页的学生数据
    QString strsql = QString("SELECT * FROM student ORDER BY id LIMIT %1 OFFSET %2")
            .arg(uiCnt).arg(offset);
    QSqlQuery query(db);

    qDebug() << "执行 SQL 语句：" << strsql;  // 输出 SQL 语句，方便调试

    if (!query.exec(strsql)) {
        qDebug() << "SQL 查询失败：" << query.lastError().text();
        return l;
    }

    // 读取查询结果
    while (query.next()) {
        StudentInfo student;
        student.id = query.value(0).toUInt();
        student.name = query.value(1).toString();
        student.age = query.value(2).toUInt();
        student.grade = query.value(3).toUInt();
        student.Myclass = query.value(4).toUInt();
        student.studentID = query.value(5).toUInt();
        student.phone = query.value(6).toString();
        student.wechat = query.value(7).toString();
        l.push_back(student);

        qDebug() << "查询到学生：" << student.id << student.name << student.age;
    }
    qDebug() << "最终返回的学生数量：" << l.size();
    return l;
}

// 增加学生
bool studentSQL::addStudent(const StudentInfo &student)
{
    QSqlQuery query(db);
    query.prepare("INSERT INTO student (name, age, grade, class, studentid, phone, wechat) "
                  "VALUES (?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(student.name);
    query.addBindValue(student.age);
    query.addBindValue(student.grade);
    query.addBindValue(student.Myclass);
    query.addBindValue(student.studentID);
    query.addBindValue(student.phone);
    query.addBindValue(student.wechat);

    if (!query.exec()) {
        qDebug() << "插入数据失败：" << query.lastError().text();
        return false;
    }
    qDebug() << "学生信息插入成功";
    return true;
}

//删除学生
bool studentSQL::delStudent(int id)
{
    QSqlQuery query(db);
    query.prepare("DELETE FROM student WHERE id = :id");
    query.bindValue(":id", id);

    if(!query.exec()){
        qDebug() << "删除失败原因：" << query.lastError().text();
        return false;
    }else{
        qDebug() << "删除成功！";
        return true;
    }
    return 0;
}

//搜索某个学生贴到数据里
bool studentSQL::searchStudent(QTableWidget *table, const QString &name)
{
    QSqlQuery query(db);
    query.prepare("SELECT * FROM student WHERE name LIKE ?");
    query.addBindValue("%" + name + "%");  //模糊查询，匹配包含 strUser 的所有结果

    if(!query.exec()){
        qDebug() << "搜索失败原因：" << query.lastError().text();
        return false;
    }

    table->clearContents();  // **清空原表格**
    table->setRowCount(0);//清空表格

    int row = 0;
    while(query.next()){
        table->insertRow(row);//一直插入一行一行
        for(int col = 0; col < query.record().count(); col++){
            table->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
        }
        row++;
    }
    emit searchCompleted(row);  // **发送查询完成信号**
    return true;
}


//查询完后恢复数据
void studentSQL::restoreData(QTableWidget *table)
{
    QSqlQuery query(db);

    // 使用事务确保操作的原子性
    query.exec("BEGIN");

    // 更新学生 id
    QSqlQuery getStudentsQuery("SELECT id, name FROM student ORDER BY id");
    if (getStudentsQuery.exec()) {
        int row = 1;  // 从 id = 1 开始
        while (getStudentsQuery.next()) {
            int currentId = getStudentsQuery.value("id").toInt();
            QString studentName = getStudentsQuery.value("name").toString();

            // 更新每个学生的 id
            query.prepare("UPDATE student SET id = :newId WHERE id = :currentId");
            query.bindValue(":newId", row);
            query.bindValue(":currentId", currentId);
            query.exec();

            row++;
        }
        query.exec("COMMIT");
    } else {
        query.exec("ROLLBACK");  // 出现错误时回滚事务
    }
    // 刷新表格视图
    displayData(table);  // **恢复所有数据**
}

//修改学生信息
bool studentSQL::changeStudentInfo(const StudentInfo &student_info)
{
    QSqlQuery query(db);

    // 先检查 studentID 是否存在
    QSqlQuery checkQuery(db);
    checkQuery.prepare("SELECT COUNT(*) FROM student WHERE studentID = ?");
    checkQuery.addBindValue(student_info.studentID);
    if (checkQuery.exec() && checkQuery.next() && checkQuery.value(0).toInt() == 0) {
        qDebug() << "学号" << student_info.studentID << "不存在，无法修改";
        return false;
    }

    db.transaction();  // 开启事务
    query.prepare("UPDATE student SET name = ?, age = ?, grade = ?, class = ?, phone = ?, wechat = ? WHERE studentID = ?");
    query.addBindValue(student_info.name);
    query.addBindValue(student_info.age);
    query.addBindValue(student_info.grade);
    query.addBindValue(student_info.Myclass);
    query.addBindValue(student_info.phone);
    query.addBindValue(student_info.wechat);
    query.addBindValue(student_info.studentID);

    if (!query.exec()) {
        qDebug() << "更新学生信息失败：" << query.lastError().text();
        db.rollback();
        return false;
    }

    db.commit();
    qDebug() << "学生信息更新成功";

    // 验证数据是否真的更新
    QSqlQuery verifyQuery(db);
    verifyQuery.prepare("SELECT * FROM student WHERE studentID = ?");
    verifyQuery.addBindValue(student_info.studentID);
    if (verifyQuery.exec() && verifyQuery.next()) {
        qDebug() << "修改后的数据：" << verifyQuery.value(1).toString()  // name
                 << verifyQuery.value(2).toInt();  // age
    }

    return true;
}

//检查新添加的学生是否已经存在
bool studentSQL::checkStudent(QTableWidget *table, const int &stuID)
{
    QSqlQuery query(db);
    query.prepare("SELECT COUNT(*) FROM student WHERE studentID = ?");
    query.addBindValue(stuID);  //匹配 stuID 的所有结果

    //qDebug() << query.next() << "添加的已存在学生学号为：" << query.value(0).toString();

    if(!query.exec()){
        qDebug() << "查重失败原因：" << query.lastError().text();
        return true;
    }else{
        if(query.next()){
            int cnt = query.value(0).toInt();
            qDebug() << "学号" << stuID << "查询几个相同学号结果：" << cnt;
            return cnt > 0;
        }
    }
    return false;
}


#if 0
//管理员操作
//清空数据库
void studentSQL::clearStudentTable()
{

}
#endif

//将数据库中的数据导入到QTableWidget组件中
void studentSQL::displayData(QTableWidget *table)
{
    QSqlQuery query(db);  // 获取数据库连接
    query.exec("SELECT * FROM student");  // 查询所有学生数据

    //清空表格
    table->clearContents();
    table->setRowCount(0);

    int row = 0;
    // 获取列数（动态适配）
    int columnCount = query.record().count();
    table->setColumnCount(columnCount);
    table->setHorizontalHeaderLabels({"ID", "姓名", "年龄", "年级", "班级", "学号", "电话", "微信"}); // 设置表头

    while (query.next()) {
        table->insertRow(row);  // 添加一行
        for (int col = 0; col < columnCount; col++) {
            table->setItem(row, col, new QTableWidgetItem(query.value(col).toString())); // 插入数据
        }
        row++;
    }
    table->resizeColumnsToContents();  // 调整列宽适应内容
    //  设置固定行高（30 像素）
    for (int i = 0; i < table->rowCount(); i++) {
        table->setRowHeight(i, 20);
    }
    //  设置固定列宽（默认 100 像素）
    for (int i = 0; i < table->columnCount(); i++) {
        table->setColumnWidth(i, 70);
    }
    //  允许列自动扩展（但最小宽度固定）
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
}


