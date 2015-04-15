#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QtPlugin>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator trans;
    trans.load(QString(":/prefix1/prefix1/qt_zh_CN"));
    a.installTranslator(&trans);

    QCoreApplication::addLibraryPath("./");

    MainWindow w;
    w.show();

    return a.exec();
}
