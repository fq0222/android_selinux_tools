#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Widget w;
    w.setWindowIcon(QIcon(":/resources/icon.ico"));
    w.show();
    return a.exec();
}
