#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "driver.h"
#include<QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    w_login.setFixedSize(522, 280);//固定登录界面窗口大小
    //w_login.show();

    //登录成功后显示主页面
    connect(&w_login, &login::sendLoginSuccess, this, [&](){
        w_login.close();
        this->show();
    });

    //将默认表头“1”隐藏起来
    ui->treeWidget->setHeaderHidden(true);
    // 创建父节点
    QTreeWidgetItem *parentItem = new QTreeWidgetItem(ui->treeWidget);
    parentItem->setText(0, "学生信息管理列表");  // 正确设置文本
    ui->treeWidget->addTopLevelItem(parentItem); // 加入根节点
    // 创建子节点
    QTreeWidgetItem *childItem1 = new QTreeWidgetItem(parentItem);
    childItem1->setText(0, "学生管理"); // 设置文本
    QTreeWidgetItem *childItem2 = new QTreeWidgetItem(parentItem);
    childItem2->setText(0, "管理员管理"); // 设置文本
    // 添加子节点
    parentItem->addChild(childItem1);
    parentItem->addChild(childItem2);
    // 展开所有节点
    //ui->treeWidget->expandAll();

    //调用数据库数据
    studentSQL dbHelper;
    dbHelper.displayData(ui->tableWidget);  // **填充 `QTableWidget`**
    // **连接 studentSQL 的信号和槽**
    connect(&dbHelper, &studentSQL::searchCompleted, this, &MainWindow::updateSearchStatus);

    //label_RenShu调用studentSQL::getStudentCnt显示学生人数
    ui->label_RenShu->setText("学生总人数:" + QString::number(dbHelper.getStudentCnt()));

    //搜索文本为空恢复数据
    connect(ui->lineEdit, &QLineEdit::textChanged, this, &MainWindow::on_SearchTextChanged);

    ui->tableWidget->setSelectionMode(QAbstractItemView::MultiSelection);  // 允许多选
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  // 选中整行

}

MainWindow::~MainWindow()
{
    delete ui;
    //delete addStudent_Window;  // 删除 addStudent 窗口对象
}

//主页面退出按钮
void MainWindow::on_btn_TuiChu_clicked()
{
    close();
}

void MainWindow::setUserName(const QString &username)
{
    ui->label_YongHuMing->setText("用户名：" + username);
}

//删除学生
void MainWindow::on_btn_ShanChu_clicked()
{
    //"delete from student where id = 1;"
    QList<QTableWidgetItem*> selectedItems = ui->tableWidget->selectedItems();

    //删除学生幂函数
    auto delList = [&](QList<QTableWidgetItem*> selectedItems){
        for (QTableWidgetItem *item : selectedItems) {
            int row = item->row();
            // 删除选中的学生的数据库操作
            qDebug() << "删除学生行号: " << row;
            // 执行数据库删除操作
            int id = ui->tableWidget->item(row, 0)->text().toInt();//读取第一列id
            dbHelper.delStudent(id);//传入参数
        }
    };

    if (isQuanXuan) {  // 判断是否是全选
        // 执行删除所有学生操作
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "确认删除", "是否删除所有学生？", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            // 删除所有学生的数据库操作
            qDebug() << "删除所有学生";
            delList(selectedItems);
        }
    } else if (!selectedItems.isEmpty()) {  // 如果没有全选，则删除选中的数据
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "确认删除", "是否删除选中的学生？",
                                      QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            delList(selectedItems);
        }
    } else {
        QMessageBox::information(this, "删除", "没有选中任何学生！");
    }
    // 刷新表格id
    dbHelper.restoreData(ui->tableWidget);
}

//搜索
void MainWindow::on_btn_SouSuo_clicked()
{
    QString searchText = ui->lineEdit->text().trimmed();
    if (!searchText.isEmpty()) {
        bool found = dbHelper.searchStudent(ui->tableWidget, searchText);
        if (!found) {
            QMessageBox::information(this, "搜索结果", "未找到匹配的学生！");
        }
    }
}

void MainWindow::updateSearchStatus(int count)
{
    QMessageBox::information(this, "查询完成", QString("找到 %1 条匹配记录").arg(count));
}

//搜索文本为空时自动恢复数据
void MainWindow::on_SearchTextChanged()
{
    if(ui->lineEdit->text().isEmpty()){
        dbHelper.displayData(ui->tableWidget);
    }
}

//全选按钮
void MainWindow::on_checkBox_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked) {  // 选中时，全选
        isQuanXuan = true;
        for (int row = 0; row < ui->tableWidget->rowCount(); row++) {
            ui->tableWidget->selectRow(row);  // 选中整行
        }
    } else {  //  取消选中时，清空选择
        isQuanXuan = false;
        ui->tableWidget->clearSelection();
    }
}

//增加学生
void MainWindow::on_btn_ZengJia_clicked()
{
    this->hide();
    addStudent_Window = new addStudent(nullptr);

    addStudent_Window->show();
    connect(addStudent_Window, &addStudent::addStudent_closed, this, &MainWindow::showMainWindow);
    connect(addStudent_Window, &addStudent::addStudent_update, this, [&](){
        dbHelper.restoreData(ui->tableWidget);
    });
}

// 重新显示主窗口
void MainWindow::showMainWindow()
{
    show();  // 显示主窗口
}

//修改学生信息
void MainWindow::on_btn_XiuGai_clicked()
{
    //选择需要修改的学生信息，跳转修改界面
    // 选中的学生信息
    QList<QTableWidgetItem*> selectedItems = ui->tableWidget->selectedItems();

    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "修改", "请先选择需要修改的学生！");
        return;
    }

    // 只选取选中的行，防止重复
    QSet<int> selectedRows;
    for (QTableWidgetItem *item : selectedItems) {
        selectedRows.insert(item->row());
    }


    // 取第一个选中的学生信息，传递到修改窗口
    int row = *selectedRows.begin();  // 保证按选中的顺序传递
    StudentInfo student;
    student.id = ui->tableWidget->item(row, 0)->text().toInt();
    student.name = ui->tableWidget->item(row, 1)->text();
    student.age = ui->tableWidget->item(row, 2)->text().toInt();
    student.grade = ui->tableWidget->item(row, 3)->text().toInt();
    student.Myclass = ui->tableWidget->item(row, 4)->text().toInt();
    student.studentID = ui->tableWidget->item(row, 5)->text().toInt();
    student.phone = ui->tableWidget->item(row, 6)->text();
    student.wechat = ui->tableWidget->item(row, 7)->text();

    qDebug() << "选中学生数据："
             << student.studentID << student.name
             << student.age << student.grade
             << student.Myclass << student.phone
             << student.wechat;

    // 打开修改窗口，并传递学生信息
    modify_Window = new modifyingStudent(nullptr);
    modify_Window->setStudentInfo(student);
    modify_Window->setData(selectedItems);   // 这里调用 setData
    modify_Window->show();
    connect(modify_Window, &modifyingStudent::modifyingStudent_Close, this, &MainWindow::showMainWindow);
    // 监听修改窗口关闭，刷新主界面数据
    connect(modify_Window, &modifyingStudent::addStudent_modifying, this, &MainWindow::refreshStudentData);
}


void MainWindow::refreshStudentData()
{
    dbHelper.restoreData(ui->tableWidget);  // 重新加载数据库数据
}





















