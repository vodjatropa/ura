#include "mainwindow.h"
#include "ui_mainwindow.h"
int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    //plac = resajz();
    //w.resize(plac,plac);
    // for timer
    MainWindow mTimer;



    return a.exec();
}
