#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.initializeVisualization( 800, 800, 67, 67 );

    w.show();

    return a.exec();
}
