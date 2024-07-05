#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QString>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Allow;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_btnSelect_clicked();

    void on_btnGen_clicked();

private:
    Ui::Widget *ui;
    std::shared_ptr<Allow> exec;
    QString mLastUsrPath;
};
#endif // WIDGET_H
