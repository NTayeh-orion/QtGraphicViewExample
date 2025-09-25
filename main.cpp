#include "mainwindow.h"
#include "crashhandler.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    installCrashHandler();
    w.show();

    return a.exec();
}
