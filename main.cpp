#include "mainwindow.h"
#include <QApplication>
#include "qdebug.h"
#include "qstylefactory.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qDebug() << QStyleFactory::keys();
    a.setStyle(QStyleFactory::create("Fusion"));
    MainWindow w;
    w.show();

    return a.exec();
}
