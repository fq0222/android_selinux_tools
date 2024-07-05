#include "widget.h"
#include "./ui_widget.h"
#include "allow.h"
#include <QFileDialog>
#include <QString>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , mLastUsrPath("/home")
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_btnSelect_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, QString("选择日志文件"), mLastUsrPath, "All Files (*)");

    if (!filePath.isEmpty()) {
        // 用户选择了文件，filePath就是文件的路径
        // 在这里可以处理文件，比如打开文件等
        mLastUsrPath = filePath;
        ui->sllowShow->clear();
        exec = std::make_shared<Allow>(filePath);
    } else {
        // 用户取消了选择
    }
}


void Widget::on_btnGen_clicked()
{
    if (exec != nullptr) {
        vector<string> result;
        exec->filterFileByLine();
        exec->printAllow(result);
        for (auto &item : result) {
            ui->sllowShow->append(QString(item.c_str()));
        }
        exec = nullptr;
    }
}

