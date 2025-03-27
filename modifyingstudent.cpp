#include "modifyingstudent.h"
#include "ui_modifyingstudent.h"
#include<QSqlRecord>

modifyingStudent::modifyingStudent(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::modifyingStudent)
{
    ui->setupUi(this);
    this->setLayout(ui->horizontalLayout_9);

    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  // 选中整行
}

modifyingStudent::~modifyingStudent()
{
    delete ui;
}


void modifyingStudent::on_btn_XiuGai_clicked()
{
    // 获取用户修改后的数据
    StudentInfo updatedStudent;
    updatedStudent.studentID = ui->lineEdit_studentID->text().toInt();
    updatedStudent.name = ui->lineEdit_name->text();
    updatedStudent.age = ui->lineEdit_age->text().toInt();
    updatedStudent.Myclass = ui->lineEdit_class->text().toInt();
    updatedStudent.grade = ui->lineEdit_grade->text().toInt();
    updatedStudent.phone = ui->lineEdit_phone->text();
    updatedStudent.wechat = ui->lineEdit_wechat->text();



    // 检查输入是否合法
    if (ui->lineEdit_name->text().isEmpty()
            || ui->lineEdit_studentID->text().isEmpty()
            || ui->lineEdit_age->text().isEmpty()
            || ui->lineEdit_class->text().isEmpty()
            || ui->lineEdit_grade->text().isEmpty()
            || ui->lineEdit_phone->text().isEmpty()
            || ui->lineEdit_wechat->text().isEmpty()) {
        QMessageBox::warning(this, "错误", "请填写完整的学生信息！");
        return;
    }

    // 执行数据库修改操作
    if (dbHelper.changeStudentInfo(updatedStudent)) {
        QMessageBox::information(this, "成功", "学生信息修改成功！");
        close();
        emit addStudent_modifying();  // 触发信号，通知主界面刷新数据
    } else {
        QMessageBox::warning(this, "失败", "学生信息修改失败，请检查输入！");
    }
}


void modifyingStudent::on_btn_WanCheng_clicked()
{
    close();
    emit modifyingStudent_Close();
}

//读取需要修改的信息到LineEdit上
void modifyingStudent::setStudentInfo(const StudentInfo &student)
{
    ui->lineEdit_studentID->setText(QString::number(student.studentID));
    ui->lineEdit_name->setText(student.name);
    ui->lineEdit_age->setText(QString::number(student.age));
    ui->lineEdit_class->setText(QString::number(student.Myclass));
    ui->lineEdit_grade->setText(QString::number(student.grade));
    ui->lineEdit_phone->setText(student.phone);
    ui->lineEdit_wechat->setText(student.wechat);
}

//将选中的数据导入到QTableWidget组件中
//void modifyingStudent::setData(QList<QTableWidgetItem*> &selectedItems)
//{
//    // 清空表格
//    ui->tableWidget->clearContents();
//    ui->tableWidget->setRowCount(0);

//    int columnCount = 7;  // 你的表格有 7 列数据
//    ui->tableWidget->setColumnCount(columnCount);
//    ui->tableWidget->setHorizontalHeaderLabels({"ID", "姓名", "年龄", "年级", "班级", "电话", "微信"}); // 设置表头

//    // 遍历选中的数据，并插入到 tableWidget
//    int row = 0;
//    QSet<int> selectedRows;  // 记录已选中的行，防止重复
//    for (QTableWidgetItem *item : selectedItems) {
//        int currentRow = item->row();
//        if (selectedRows.contains(currentRow)) continue;  // 避免重复插入
//        selectedRows.insert(currentRow);

//        // 添加新行
//        ui->tableWidget->insertRow(row);
//        for (int col = 0; col < columnCount; col++) {
//            ui->tableWidget->setItem(row, col, new QTableWidgetItem(
//                item->tableWidget()->item(currentRow, col)->text()));  // 复制选中的数据
//        }
//        row++;
//    }

//    ui->tableWidget->resizeColumnsToContents();  // 调整列宽
//}
void modifyingStudent::setData(QList<QTableWidgetItem*> &selectedItems)
{
    // 清空表格
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    int columnCount = 8;  // 你的表格有 7 列数据
    ui->tableWidget->setColumnCount(columnCount);
    ui->tableWidget->setHorizontalHeaderLabels({"ID", "姓名", "年龄", "年级", "班级", "学号", "电话", "微信"}); // 设置表头

    int row = 0;
    QSet<int> selectedRows;
    StudentInfo firstStudent;  // 用于存储第一个选中的学生信息

    for (QTableWidgetItem *item : selectedItems) {
        int currentRow = item->row();
        if (selectedRows.contains(currentRow)) continue;
        selectedRows.insert(currentRow);

        ui->tableWidget->insertRow(row);
        for (int col = 0; col < columnCount; col++) {
            QString cellText = item->tableWidget()->item(currentRow, col)->text();
            ui->tableWidget->setItem(row, col, new QTableWidgetItem(cellText));

            // **保存第一个学生信息到 firstStudent**
            if (row == 0) {
                switch (col) {
                    case 0: firstStudent.id = cellText.toInt(); break;
                    case 1: firstStudent.name = cellText; break;
                    case 2: firstStudent.age = cellText.toInt(); break;
                    case 3: firstStudent.grade = cellText.toInt(); break;
                    case 4: firstStudent.Myclass = cellText.toInt(); break;
                    case 5: firstStudent.studentID = cellText.toInt(); break;
                    case 6: firstStudent.phone = cellText; break;
                    case 7: firstStudent.wechat = cellText; break;
                }
            }
        }
        row++;
    }

    ui->tableWidget->resizeColumnsToContents();

    // **同步更新 LineEdit，显示第一个选中的学生信息**
    if (!selectedRows.isEmpty()) {
        setStudentInfo(firstStudent);
    }

    // **连接 QTableWidget 的 itemSelectionChanged 事件，实现动态更新**
    connect(ui->tableWidget, &QTableWidget::itemSelectionChanged, this, &modifyingStudent::updateLineEdit);
}




void modifyingStudent::updateLineEdit()
{
    QList<QTableWidgetItem*> selectedItems = ui->tableWidget->selectedItems();
    if (selectedItems.isEmpty()) return;

    int row = selectedItems.first()->row();
    StudentInfo selectedStudent;

    selectedStudent.studentID = ui->tableWidget->item(row, 0)->text().toInt();
    selectedStudent.name = ui->tableWidget->item(row, 1)->text();
    selectedStudent.age = ui->tableWidget->item(row, 2)->text().toInt();
    selectedStudent.grade = ui->tableWidget->item(row, 3)->text().toInt();
    selectedStudent.Myclass = ui->tableWidget->item(row, 4)->text().toInt();
    selectedStudent.phone = ui->tableWidget->item(row, 5)->text();
    selectedStudent.wechat = ui->tableWidget->item(row, 6)->text();

    // **填充右侧 LineEdit**
    setStudentInfo(selectedStudent);
}
