#include "mainwindow.h"
#include "auth.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w(1);
    //Auth w;
    w.show();
    return a.exec();
}
