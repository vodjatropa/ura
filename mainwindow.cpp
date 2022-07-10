#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <SunSet.h>
#include <QtCore>
#include <QDebug>
#include <ctime>
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>
#include <QDateTimeEdit>
#include <QDate>
#include <QListView>
#include <QVBoxLayout>
#include <QLayoutItem>
#include <QCalendarWidget>
#include <QRect>

#include <QLabel>
#include <QPushButton>
#include <QPixmap>
#include <string>
#include <iostream>
#include <vector>
using namespace std;
#define ONE_HOUR	(60 * 60)
#define TIMEZONE	-2
#define LATITUDE	46.0569465
#define LONGITUDE	14.50575149
#define PI 3.14159265


vector<int> arr_ura;
vector<int> arr_minuta;
vector<string> arr_slika;
vector<string> arr_zmenek;




int radij = 185; // pixels
int center_x = 225;
int center_y = 225;
int stevilka_ure_font_size = 10;
double plac;
QPoint p1, p2, p3;
double sunrise, sunset, sunriseUTC, sunsetUTC;
int moonphase, x, y, x_mis, y_mis;
int resajz();
QString ikona = "";
//double ura_ikone;
double iskana_ura_v_minutah = 1500.01;



QPen cyanpen(Qt::cyan);
QPen greenpen(Qt::green);
QPen blackpen(Qt::black);
QPen redpen(Qt::red);
QPen whitepen(Qt::white);
QPen yellowpen(Qt::yellow);
QPen orangepen;
QPen darkgreenpen;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // for transparent background
    setStyleSheet("background:transparent;");
    //setStyleSheet("background-color: rgba(0,0,0,200);");
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint);

    // for timer
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(MyTimer()));
    timer->start(2000);
    setMouseTracking(true);


}

int resajz()
{
    // for width and height
    QDesktopWidget dw;
    x=dw.width();
    y=dw.height();
    if (y <= x) {
        //plac=y-20; // -20 for dwm bar
        plac=y-2; // -20 for dwm bar
    } else {
        //plac=x-20;
        plac=x-2;
    }
    return plac;
}

void MainWindow::MyTimer()
{

    this->repaint();
}

double uraVstopinje(int ura_v_minutah) {
    double stopinjice;
    //int vmesna_uvm;
    stopinjice=((ura_v_minutah*360)/1440);

    if (stopinjice >= 360) {stopinjice = stopinjice - 360;}
    if (stopinjice >= 0 && stopinjice < 270) {
        stopinjice = 270 - stopinjice;
    }
    if (stopinjice >= 270 && stopinjice < 360) {
        stopinjice = 360 - (stopinjice - 270);
    }


    return stopinjice;
}

void dobiTockoP1(double cas_v_minutah, double radijus)
{
    cas_v_minutah = cas_v_minutah / 4;
    cas_v_minutah = cas_v_minutah + 180;
    cas_v_minutah = cas_v_minutah * PI / 180;
    p1.setX(center_x + sin ( PI - cas_v_minutah) * radijus);
    p1.setY(center_y + cos ( PI - cas_v_minutah) * radijus);
}

void dobiTockoP2(double cas_v_minutah, double radijus)
{
    cas_v_minutah = cas_v_minutah / 4;
    cas_v_minutah = cas_v_minutah + 180;
    cas_v_minutah = cas_v_minutah * PI / 180;
    p2.setX(center_x + sin ( PI - cas_v_minutah) * radijus);
    p2.setY(center_y + cos ( PI - cas_v_minutah) * radijus);

}



void MainWindow::paintEvent(QPaintEvent *) {



    QPainter painter(this);

    plac = resajz();
    this->resize(plac,plac);

    double puoPlaca= plac / 2;
    double radij_double = 0.90 * puoPlaca;
    double ostanek_za_risat = puoPlaca - radij_double;
    radij = (int)radij_double; // pixlov
    center_x = puoPlaca;
    center_y = puoPlaca;
    double stevilka_ure_font_size_double = (0.05 * puoPlaca)/2;
    stevilka_ure_font_size = (int)stevilka_ure_font_size_double + 1;

    // for sunset and sunrise
    SunSet sun;

    auto rightnow = std::time(nullptr);
    struct tm *tad = std::localtime(&rightnow);

    sun.setPosition(LATITUDE, LONGITUDE, tad->tm_gmtoff / ONE_HOUR);
    sun.setCurrentDate(tad->tm_year + 1900, tad->tm_mon + 1, tad->tm_mday);
    sunrise = sun.calcSunrise();
    sunset = sun.calcSunset();
    sunriseUTC = sun.calcSunriseUTC();
    sunsetUTC = sun.calcSunsetUTC();
    moonphase = sun.moonPhase(static_cast<int>(rightnow));

    cyanpen.setWidth(3);
    greenpen.setWidth(1);
    blackpen.setWidth(2);
    redpen.setWidth(3);
    whitepen.setWidth(2);
    yellowpen.setWidth(1);
    orangepen.setColor("orange");
    orangepen.setWidth(1);
    darkgreenpen.setColor("darkgreen");
    darkgreenpen.setWidth(2);

    QColor backgroundColor = "black";
    backgroundColor.setAlpha(140);

    int xi = (int)puoPlaca-radij;
    int yi = plac-2*xi;
    painter.setBrush(backgroundColor);
    painter.drawEllipse(QRect(xi,xi,yi,yi));

    int ura_vzhoda=(sunrise / 60);
    int ura_zahoda=(sunset / 60);
    int minuta_vzhoda=(int)sunrise%60;
    int minuta_zahoda=(int)sunset%60;

    int ura_vzhoda_v_minutah = (ura_vzhoda * 60) + minuta_vzhoda;
    int ura_zahoda_v_minutah = (ura_zahoda * 60) + minuta_zahoda;


    QTime ura = QTime::currentTime();
    int ura_zdaj = ura.hour();
    int minuta_zdaj = ura.minute();
    int cas_zdaj_v_minutah = ((ura_zdaj * 60) + minuta_zdaj);

    dobiTockoP1(ura_vzhoda_v_minutah, radij* 9/10);     // sicer pa je to prva tocka
    dobiTockoP2(ura_vzhoda_v_minutah, radij+ostanek_za_risat);     // in to je druga
    painter.setPen(darkgreenpen); // barva, debelina, css...
    painter.drawLine(p1,p2);  // potegni crto
    painter.setPen(greenpen); // barva, debelina, css...
    painter.drawLine(p1,p2);  // potegni crto


    dobiTockoP1(ura_zahoda_v_minutah, radij* 9/10);
    dobiTockoP2(ura_zahoda_v_minutah, radij+ostanek_za_risat);
    painter.setPen(darkgreenpen);
    painter.drawLine(p1,p2);
    painter.setPen(greenpen);
    painter.drawLine(p1,p2);

    dobiTockoP1(cas_zdaj_v_minutah, radij* 6/8);
    dobiTockoP2(cas_zdaj_v_minutah, radij+ostanek_za_risat);
    painter.setPen(redpen);
    painter.drawLine(p1,p2);
    painter.setPen(yellowpen);
    painter.drawLine(p1,p2);

    double us_pou = stevilka_ure_font_size / 2;
    double us_tretina = stevilka_ure_font_size / 3;
    double us_cetrt = stevilka_ure_font_size / 4;
    double us_sest = stevilka_ure_font_size / 6;


    for( int u = 0; u < 24;  u++ ) {
        int u0 = u*60;
        int u10 = u0+10;
        int u20 = u0+20;
        int u30 = u0+30;
        int u40 = u0+40;
        int u50 = u0+50;
        QString xcv = QString::number(u);
        // stevilke
        dobiTockoP2(u0, radij+stevilka_ure_font_size + us_pou);

        switch (u) {
            case 0:
                p2.setX(p2.x() - us_pou + us_sest);
                p2.setY(p2.y() + stevilka_ure_font_size + us_cetrt);
                break;

            case 1:
                p2.setX(p2.x() - us_pou - us_cetrt);
                p2.setY(p2.y() + stevilka_ure_font_size + us_cetrt);
                break;

            case 2:
                p2.setX(p2.x() - us_pou - us_tretina);
                p2.setY(p2.y() + stevilka_ure_font_size + us_tretina);
                break;

            case 3:
                p2.setX(p2.x() - stevilka_ure_font_size);
                p2.setY(p2.y() + stevilka_ure_font_size);
                break;

            case 4:
                p2.setX(p2.x() - stevilka_ure_font_size - us_cetrt);
                p2.setY(p2.y() + stevilka_ure_font_size);
                break;

            case 5:
                p2.setX(p2.x() - stevilka_ure_font_size - us_tretina);
                p2.setY(p2.y() + stevilka_ure_font_size);
                break;

            case 6:
                p2.setX(p2.x() - stevilka_ure_font_size);
                p2.setY(p2.y() + us_pou);
                break;

            case 7:
                p2.setX(p2.x() - stevilka_ure_font_size - us_tretina);
                p2.setY(p2.y());
                break;

            case 8:
                p2.setX(p2.x() - stevilka_ure_font_size - us_cetrt);
                p2.setY(p2.y());
                break;

            case 9:
                p2.setX(p2.x() - stevilka_ure_font_size);
                p2.setY(p2.y());
                break;

            case 10:
                p2.setX(p2.x() - stevilka_ure_font_size);
                p2.setY(p2.y() - us_tretina);
                break;

            case 11:
                p2.setX(p2.x() - us_pou - us_cetrt);
                p2.setY(p2.y() - us_cetrt);
                break;

            case 12:
                p2.setX(p2.x() - us_pou - us_tretina);
                p2.setY(p2.y() - us_cetrt);
                break;

            case 13:
                p2.setX(p2.x() - us_cetrt);
                p2.setY(p2.y() - us_cetrt);
                break;

            case 14:
                p2.setX(p2.x());
                p2.setY(p2.y() - us_tretina);
                break;

            case 15:
                p2.setX(p2.x() + us_cetrt);
                p2.setY(p2.y());
                break;

            case 16:
                p2.setX(p2.x() + us_cetrt);
                p2.setY(p2.y());
                break;

            case 17:
                p2.setX(p2.x() + us_tretina);
                p2.setY(p2.y());
                break;

            case 18:
                p2.setX(p2.x() + us_cetrt);
                p2.setY(p2.y() + us_pou);
                break;

            case 19:
                p2.setX(p2.x() + us_tretina);
                p2.setY(p2.y() + stevilka_ure_font_size);
                break;

            case 20:
                p2.setX(p2.x() + us_cetrt);
                p2.setY(p2.y() + stevilka_ure_font_size);
                break;

            case 21:
                p2.setX(p2.x() + us_cetrt);
                p2.setY(p2.y() + stevilka_ure_font_size);
                break;

            case 22:
                p2.setX(p2.x());
                p2.setY(p2.y() + stevilka_ure_font_size + us_tretina);
                break;

            case 23:
                p2.setX(p2.x() - us_cetrt);
                p2.setY(p2.y() + stevilka_ure_font_size + us_cetrt);
                break;

            default:
                p2.setX(p2.x()+14);
        }

        painter.setPen(blackpen);
        painter.setFont({"Helvetica", stevilka_ure_font_size+1});
        painter.drawText(p2,xcv);

        painter.setPen(orangepen);
        painter.setFont({"Helvetica", stevilka_ure_font_size});
        painter.drawText(p2,xcv);



        dobiTockoP1(u0, radij+1);
        dobiTockoP2(u0, radij+15);
        blackpen.setWidth(3);
        painter.setPen(blackpen);
        painter.drawLine(p1,p2);
        painter.setPen(orangepen);
        painter.drawLine(p1,p2);
        dobiTockoP1(u10, radij+1);
        dobiTockoP2(u10, radij+5);
        blackpen.setWidth(3);
        painter.setPen(blackpen);
        painter.drawLine(p1,p2);
        painter.setPen(orangepen);
        painter.drawLine(p1,p2);
        dobiTockoP1(u20, radij+1);
        dobiTockoP2(u20, radij+5);
        blackpen.setWidth(3);
        painter.setPen(blackpen);
        painter.drawLine(p1,p2);
        painter.setPen(orangepen);
        painter.drawLine(p1,p2);
        dobiTockoP1(u30, radij+1);
        dobiTockoP2(u30, radij+10);
        blackpen.setWidth(3);
        painter.setPen(blackpen);
        painter.drawLine(p1,p2);
        painter.setPen(orangepen);
        painter.drawLine(p1,p2);
        dobiTockoP1(u40, radij+1);
        dobiTockoP2(u40, radij+5);
        blackpen.setWidth(3);
        painter.setPen(blackpen);
        painter.drawLine(p1,p2);
        painter.setPen(orangepen);
        painter.drawLine(p1,p2);
        dobiTockoP1(u50, radij+1);
        dobiTockoP2(u50, radij+5);
        blackpen.setWidth(3);
        painter.setPen(blackpen);
        painter.drawLine(p1,p2);
        painter.setPen(orangepen);
        painter.drawLine(p1,p2);
    }

    QString homePath = QDir::homePath();

    QFile dnevnik(homePath + "/.cache/dnevnik");
    if(dnevnik.open(QIODevice::ReadOnly)){
        QTextStream in(&dnevnik);
        dobiTockoP1((9.75)*60, radij);


        painter.setPen(greenpen);
        painter.setFont({"Mono", (int)(stevilka_ure_font_size/1.5)});


        int tr = 0;
        while(!in.atEnd()) {
            QString line = in.readLine();
            painter.drawText(p1.x()+20,p1.y()+60+tr,line);
            tr = tr + 16;
        }

        dnevnik.close();
    }


    QFile statusbar(homePath + "/.cache/vreme");
    if(statusbar.open(QIODevice::ReadOnly)) {
        QTextStream inor(&statusbar);
        dobiTockoP1((10.35)*60, radij);

        painter.setPen(greenpen);
        painter.setFont({"Meslo", (int)(stevilka_ure_font_size*2)});

        while(!inor.atEnd()) {
            QString line = inor.readLine();
            painter.drawText(p1.x(),p1.y()+60,line);
        }

        statusbar.close();
    }






    ////////////////
    painter.setPen(whitepen);
    painter.setFont({"Meslo", (int)(stevilka_ure_font_size)});

    QFile ikona_0000(homePath + "/.cache/ikone_za_uro/ikona_0000");
    if(ikona_0000.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0000);
        dobiTockoP1((0)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*0.5),p1.y()-(stevilka_ure_font_size*0.5),lineiko);
        ikona_0000.close();
    }

    QFile ikona_0010(homePath + "/.cache/ikone_za_uro/ikona_0010");
    if(ikona_0010.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0010);
        dobiTockoP1((0.166)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*0.5),p1.y()-(stevilka_ure_font_size*0.5),lineiko);
        ikona_0010.close();
    }

    QFile ikona_0020(homePath + "/.cache/ikone_za_uro/ikona_0020");
    if(ikona_0020.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0020);
        dobiTockoP1((0.333)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*0.5),p1.y()-(stevilka_ure_font_size*0.5),lineiko);
        ikona_0020.close();
    }


    QFile ikona_0030(homePath + "/.cache/ikone_za_uro/ikona_0030");
    if(ikona_0030.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0030);
        dobiTockoP1((0.5)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*0.5),p1.y()-(stevilka_ure_font_size*0.5),lineiko);
        ikona_0030.close();
    }

    QFile ikona_0040(homePath + "/.cache/ikone_za_uro/ikona_0040");
    if(ikona_0040.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0040);
        dobiTockoP1((0.666)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*0.1),p1.y()-(stevilka_ure_font_size*0.5),lineiko);
        ikona_0040.close();
    }


    QFile ikona_0050(homePath + "/.cache/ikone_za_uro/ikona_0050");
    if(ikona_0050.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0050);
        dobiTockoP1((0.833)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*0.1),p1.y()-(stevilka_ure_font_size*0.5),lineiko);
        ikona_0050.close();
    }


    QFile ikona_0100(homePath + "/.cache/ikone_za_uro/ikona_0100");
    if(ikona_0100.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0100);
        dobiTockoP1((1)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*0.1),p1.y()-(stevilka_ure_font_size*0.5),lineiko);
        ikona_0100.close();
    }




    QFile ikona_0110(homePath + "/.cache/ikone_za_uro/ikona_0110");
    if(ikona_0110.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0110);
        dobiTockoP1((1.166)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*0.5),p1.y()-(stevilka_ure_font_size*0.5),lineiko);
        ikona_0110.close();
    }

    QFile ikona_0120(homePath + "/.cache/ikone_za_uro/ikona_0120");
    if(ikona_0120.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0120);
        dobiTockoP1((1.333)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*0.5),p1.y()-(stevilka_ure_font_size*0.5),lineiko);
        ikona_0120.close();
    }


    QFile ikona_0130(homePath + "/.cache/ikone_za_uro/ikona_0130");
    if(ikona_0130.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0130);
        dobiTockoP1((1.5)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*0.5),p1.y()-(stevilka_ure_font_size*0.5),lineiko);
        ikona_0130.close();
    }

    QFile ikona_0140(homePath + "/.cache/ikone_za_uro/ikona_0140");
    if(ikona_0140.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0140);
        dobiTockoP1((1.666)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*0.1),p1.y()-(stevilka_ure_font_size*0.5),lineiko);
        ikona_0140.close();
    }


    QFile ikona_0150(homePath + "/.cache/ikone_za_uro/ikona_0150");
    if(ikona_0150.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0150);
        dobiTockoP1((1.833)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*0.1),p1.y()-(stevilka_ure_font_size*0.5),lineiko);
        ikona_0150.close();
    }




////////////////////


    QFile ikona_0200(homePath + "/.cache/ikone_za_uro/ikona_0200");
    if(ikona_0200.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0200);
        dobiTockoP1((2)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()+(stevilka_ure_font_size*0.2),p1.y()-(stevilka_ure_font_size*0.5),lineiko);
        ikona_0200.close();
    }


    QFile ikona_0210(homePath + "/.cache/ikone_za_uro/ikona_0210");
    if(ikona_0210.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0210);
        dobiTockoP1((2.166)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()+(stevilka_ure_font_size*0.2),p1.y()-(stevilka_ure_font_size*0.5),lineiko);
        ikona_0210.close();
    }

    QFile ikona_0220(homePath + "/.cache/ikone_za_uro/ikona_0220");
    if(ikona_0220.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0220);
        dobiTockoP1((2.333)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()+(stevilka_ure_font_size*0.2),p1.y()-(stevilka_ure_font_size*0.5),lineiko);
        ikona_0220.close();
    }


    QFile ikona_0230(homePath + "/.cache/ikone_za_uro/ikona_0230");
    if(ikona_0230.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0230);
        dobiTockoP1((2.5)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()+(stevilka_ure_font_size*0.35),p1.y()-(stevilka_ure_font_size*0.5),lineiko);
        ikona_0230.close();
    }


    QFile ikona_0240(homePath + "/.cache/ikone_za_uro/ikona_0240");
    if(ikona_0240.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0240);
        dobiTockoP1((2.666)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()+(stevilka_ure_font_size*0.5),p1.y()-(stevilka_ure_font_size*0.5),lineiko);
        ikona_0240.close();
    }



    QFile ikona_0250(homePath + "/.cache/ikone_za_uro/ikona_0250");
    if(ikona_0250.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0250);
        dobiTockoP1((2.833)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()+(stevilka_ure_font_size*0.5),p1.y()-(stevilka_ure_font_size*0.5),lineiko);
        ikona_0250.close();
    }





///////
    QFile ikona_0300(homePath + "/.cache/ikone_za_uro/ikona_0300");
    if(ikona_0300.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0300);
        dobiTockoP1((3)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()+(stevilka_ure_font_size*0.5),p1.y()-(stevilka_ure_font_size*0.5),lineiko);
        ikona_0300.close();
    }

    QFile ikona_0310(homePath + "/.cache/ikone_za_uro/ikona_0310");
    if(ikona_0310.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0310);
        dobiTockoP1((3.166)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()+(stevilka_ure_font_size*0.5),p1.y()-(stevilka_ure_font_size*0.5),lineiko);
        ikona_0310.close();
    }

    QFile ikona_0320(homePath + "/.cache/ikone_za_uro/ikona_0320");
    if(ikona_0320.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0320);
        dobiTockoP1((3.333)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()+(stevilka_ure_font_size*0.6),p1.y()-(stevilka_ure_font_size*0.5),lineiko);
        ikona_0320.close();
    }

    QFile ikona_0330(homePath + "/.cache/ikone_za_uro/ikona_0330");
    if(ikona_0330.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0330);
        dobiTockoP1((3.5)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()+(stevilka_ure_font_size*0.75),p1.y()-(stevilka_ure_font_size*0.3),lineiko);
        ikona_0330.close();
    }

    QFile ikona_0340(homePath + "/.cache/ikone_za_uro/ikona_0340");
    if(ikona_0340.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0340);
        dobiTockoP1((3.666)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()+(stevilka_ure_font_size*0.75),p1.y(),lineiko);
        ikona_0340.close();
    }
    QFile ikona_0350(homePath + "/.cache/ikone_za_uro/ikona_0350");
    if(ikona_0350.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0350);
        dobiTockoP1((3.833)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()+(stevilka_ure_font_size*0.75),p1.y(),lineiko);
        ikona_0350.close();
    }



    ///////
    QFile ikona_0400(homePath + "/.cache/ikone_za_uro/ikona_0400");
    if(ikona_0400.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0400);
        dobiTockoP1((4)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()+(stevilka_ure_font_size*0.75),p1.y(),lineiko);
        ikona_0400.close();
    }

    QFile ikona_0410(homePath + "/.cache/ikone_za_uro/ikona_0410");
    if(ikona_0410.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0410);
        dobiTockoP1((4.166)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()+(stevilka_ure_font_size*0.75),p1.y(),lineiko);
        ikona_0410.close();
    }

    QFile ikona_0420(homePath + "/.cache/ikone_za_uro/ikona_0420");
    if(ikona_0420.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0420);
        dobiTockoP1((4.333)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()+(stevilka_ure_font_size*0.75),p1.y(),lineiko);
        ikona_0420.close();
    }


    QFile ikona_0430(homePath + "/.cache/ikone_za_uro/ikona_0430");
    if(ikona_0430.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0430);
        dobiTockoP1((4.5)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()+(stevilka_ure_font_size*0.86),p1.y()+(stevilka_ure_font_size*0.166),lineiko);
        ikona_0430.close();
    }



    QFile ikona_0440(homePath + "/.cache/ikone_za_uro/ikona_0440");
    if(ikona_0440.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0440);
        dobiTockoP1((4.666)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()+stevilka_ure_font_size,p1.y()+(stevilka_ure_font_size*0.33),lineiko);
        ikona_0440.close();
    }

    QFile ikona_0450(homePath + "/.cache/ikone_za_uro/ikona_0450");
    if(ikona_0450.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0450);
        dobiTockoP1((4.833)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()+stevilka_ure_font_size,p1.y()+(stevilka_ure_font_size*0.33),lineiko);
        ikona_0450.close();
    }






/////


    QFile ikona_0500(homePath + "/.cache/ikone_za_uro/ikona_0500");
    if(ikona_0500.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0500);
        dobiTockoP1((5)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()+stevilka_ure_font_size,p1.y()+(stevilka_ure_font_size*0.33),lineiko);
        ikona_0500.close();
    }



    QFile ikona_0510(homePath + "/.cache/ikone_za_uro/ikona_0510");
    if(ikona_0510.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0510);
        dobiTockoP1((5.166)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()+stevilka_ure_font_size,p1.y()+(stevilka_ure_font_size*0.33),lineiko);
        ikona_0510.close();
    }



    QFile ikona_0520(homePath + "/.cache/ikone_za_uro/ikona_0520");
    if(ikona_0520.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0520);
        dobiTockoP1((5.333)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()+stevilka_ure_font_size,p1.y()+(stevilka_ure_font_size*0.33),lineiko);
        ikona_0520.close();
    }



    QFile ikona_0530(homePath + "/.cache/ikone_za_uro/ikona_0530");
    if(ikona_0530.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0530);
        dobiTockoP1((5.5)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()+stevilka_ure_font_size,p1.y()+(stevilka_ure_font_size*0.415),lineiko);
        ikona_0530.close();
    }



    QFile ikona_0540(homePath + "/.cache/ikone_za_uro/ikona_0540");
    if(ikona_0540.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0540);
        dobiTockoP1((5.666)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()+stevilka_ure_font_size,p1.y()+(stevilka_ure_font_size*0.5),lineiko);
        ikona_0540.close();
    }



    QFile ikona_0550(homePath + "/.cache/ikone_za_uro/ikona_0550");
    if(ikona_0550.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0550);
        dobiTockoP1((5.833)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()+stevilka_ure_font_size,p1.y()+(stevilka_ure_font_size*0.5),lineiko);
        ikona_0550.close();
    }




/////
    QFile ikona_0600(homePath + "/.cache/ikone_za_uro/ikona_0600");
    if(ikona_0600.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0600);
        dobiTockoP1((6)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()+stevilka_ure_font_size,p1.y()+(stevilka_ure_font_size*0.5),lineiko);
        ikona_0600.close();
    }

    QFile ikona_0610(homePath + "/.cache/ikone_za_uro/ikona_0610");
    if(ikona_0610.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0610);
        dobiTockoP1((6.166)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()+stevilka_ure_font_size,p1.y()+(stevilka_ure_font_size*0.5),lineiko);
        ikona_0610.close();
    }

    QFile ikona_0620(homePath + "/.cache/ikone_za_uro/ikona_0620");
    if(ikona_0620.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0620);
        dobiTockoP1((6.333)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()+stevilka_ure_font_size,p1.y()+(stevilka_ure_font_size*0.5),lineiko);
        ikona_0620.close();
    }



    QFile ikona_0630(homePath + "/.cache/ikone_za_uro/ikona_0630");
    if(ikona_0630.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0630);
        dobiTockoP1((6.5)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()+(stevilka_ure_font_size*0.7),p1.y()+(stevilka_ure_font_size*0.5),lineiko);
        ikona_0630.close();
    }

    QFile ikona_0640(homePath + "/.cache/ikone_za_uro/ikona_0640");
    if(ikona_0640.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0640);
        dobiTockoP1((6.666)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()+(stevilka_ure_font_size*0.75),p1.y()+(stevilka_ure_font_size*0.66),lineiko);
        ikona_0640.close();
    }

    QFile ikona_0650(homePath + "/.cache/ikone_za_uro/ikona_0650");
    if(ikona_0650.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0650);
        dobiTockoP1((6.833)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()+(stevilka_ure_font_size*0.75),p1.y()+(stevilka_ure_font_size*0.66),lineiko);
        ikona_0650.close();
    }




/////
    QFile ikona_0700(homePath + "/.cache/ikone_za_uro/ikona_0700");
    if(ikona_0700.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0700);
        dobiTockoP1((7)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()+(stevilka_ure_font_size*0.75),p1.y()+(stevilka_ure_font_size*0.66),lineiko);
        ikona_0700.close();
    }


    QFile ikona_0710(homePath + "/.cache/ikone_za_uro/ikona_0710");
    if(ikona_0710.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0710);
        dobiTockoP1((7.166)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()+(stevilka_ure_font_size*0.75),p1.y()+(stevilka_ure_font_size*0.66),lineiko);
        ikona_0710.close();
    }

    QFile ikona_0720(homePath + "/.cache/ikone_za_uro/ikona_0720");
    if(ikona_0720.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0720);
        dobiTockoP1((7.333)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()+(stevilka_ure_font_size*0.75),p1.y()+(stevilka_ure_font_size*0.66),lineiko);
        ikona_0720.close();
    }


    QFile ikona_0730(homePath + "/.cache/ikone_za_uro/ikona_0730");
    if(ikona_0730.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0730);
        dobiTockoP1((7.5)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()+(stevilka_ure_font_size*0.87),p1.y()+(stevilka_ure_font_size*0.833),lineiko);
        ikona_0730.close();
    }


    QFile ikona_0740(homePath + "/.cache/ikone_za_uro/ikona_0740");
    if(ikona_0740.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0740);
        dobiTockoP1((7.666)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()+(stevilka_ure_font_size),p1.y()+(stevilka_ure_font_size),lineiko);
        ikona_0740.close();
    }

    QFile ikona_0750(homePath + "/.cache/ikone_za_uro/ikona_0750");
    if(ikona_0750.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0750);
        dobiTockoP1((7.833)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()+(stevilka_ure_font_size),p1.y()+(stevilka_ure_font_size),lineiko);
        ikona_0750.close();
    }

/////
    QFile ikona_0800(homePath + "/.cache/ikone_za_uro/ikona_0800");
    if(ikona_0800.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0800);
        dobiTockoP1((8)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()+(stevilka_ure_font_size),p1.y()+(stevilka_ure_font_size),lineiko);
        ikona_0800.close();
    }


    QFile ikona_0810(homePath + "/.cache/ikone_za_uro/ikona_0810");
    if(ikona_0810.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0810);
        dobiTockoP1((8.166)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()+(stevilka_ure_font_size),p1.y()+(stevilka_ure_font_size),lineiko);
        ikona_0810.close();
    }

    QFile ikona_0820(homePath + "/.cache/ikone_za_uro/ikona_0820");
    if(ikona_0820.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0820);
        dobiTockoP1((8.333)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()+(stevilka_ure_font_size),p1.y()+(stevilka_ure_font_size),lineiko);
        ikona_0820.close();
    }


    QFile ikona_0830(homePath + "/.cache/ikone_za_uro/ikona_0830");
    if(ikona_0830.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0830);
        dobiTockoP1((8.5)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()+(stevilka_ure_font_size*0.5),p1.y()+(stevilka_ure_font_size*1.1),lineiko);
        ikona_0830.close();
    }


    QFile ikona_0840(homePath + "/.cache/ikone_za_uro/ikona_0840");
    if(ikona_0840.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0840);
        dobiTockoP1((8.666)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()+(stevilka_ure_font_size*0.75),p1.y()+(stevilka_ure_font_size*1.25),lineiko);
        ikona_0840.close();
    }


    QFile ikona_0850(homePath + "/.cache/ikone_za_uro/ikona_0850");
    if(ikona_0850.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0850);
        dobiTockoP1((8.833)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()+(stevilka_ure_font_size*0.75),p1.y()+(stevilka_ure_font_size*1.25),lineiko);
        ikona_0850.close();
    }



//////////
    QFile ikona_0900(homePath + "/.cache/ikone_za_uro/ikona_0900");
    if(ikona_0900.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0900);
        dobiTockoP1((9)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()+(stevilka_ure_font_size*0.75),p1.y()+(stevilka_ure_font_size*1.25),lineiko);
        ikona_0900.close();
    }

    QFile ikona_0910(homePath + "/.cache/ikone_za_uro/ikona_0910");
    if(ikona_0910.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0910);
        dobiTockoP1((9.166)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()+(stevilka_ure_font_size*0.75),p1.y()+(stevilka_ure_font_size*1.25),lineiko);
        ikona_0910.close();
    }

    QFile ikona_0920(homePath + "/.cache/ikone_za_uro/ikona_0920");
    if(ikona_0920.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0920);
        dobiTockoP1((9.33)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()+(stevilka_ure_font_size*0.75),p1.y()+(stevilka_ure_font_size*1.25),lineiko);
        ikona_0920.close();
    }


    QFile ikona_0930(homePath + "/.cache/ikone_za_uro/ikona_0930");
    if(ikona_0930.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0930);
        dobiTockoP1((9.5)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()+(stevilka_ure_font_size*0.75),p1.y()+(stevilka_ure_font_size*1.45),lineiko);
        ikona_0930.close();
    }


    QFile ikona_0940(homePath + "/.cache/ikone_za_uro/ikona_0940");
    if(ikona_0940.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0940);
        dobiTockoP1((9.666)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()+(stevilka_ure_font_size*0.5),p1.y()+(stevilka_ure_font_size*1.5),lineiko);
        ikona_0940.close();
    }

    QFile ikona_0950(homePath + "/.cache/ikone_za_uro/ikona_0950");
    if(ikona_0950.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_0950);
        dobiTockoP1((9.833)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()+(stevilka_ure_font_size*0.5),p1.y()+(stevilka_ure_font_size*1.5),lineiko);
        ikona_0950.close();
    }


//////////
    QFile ikona_1000(homePath + "/.cache/ikone_za_uro/ikona_1000");
    if(ikona_1000.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1000);
        dobiTockoP1((10)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()+(stevilka_ure_font_size*0.5),p1.y()+(stevilka_ure_font_size*1.5),lineiko);
        ikona_1000.close();
    }

    QFile ikona_1010(homePath + "/.cache/ikone_za_uro/ikona_1010");
    if(ikona_1010.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1010);
        dobiTockoP1((10.166)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()+(stevilka_ure_font_size*0.5),p1.y()+(stevilka_ure_font_size*1.5),lineiko);
        ikona_1010.close();
    }

    QFile ikona_1020(homePath + "/.cache/ikone_za_uro/ikona_1020");
    if(ikona_1020.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1020);
        dobiTockoP1((10.333)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()+(stevilka_ure_font_size*0.5),p1.y()+(stevilka_ure_font_size*1.5),lineiko);
        ikona_1020.close();
    }


    QFile ikona_1030(homePath + "/.cache/ikone_za_uro/ikona_1030");
    if(ikona_1030.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1030);
        dobiTockoP1((10.5)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()+(stevilka_ure_font_size*0.25),p1.y()+(stevilka_ure_font_size*1.5),lineiko);
        ikona_1030.close();
    }

    QFile ikona_1040(homePath + "/.cache/ikone_za_uro/ikona_1040");
    if(ikona_1040.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1040);
        dobiTockoP1((10.666)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*0.1),p1.y()+(stevilka_ure_font_size*1.6),lineiko);
        ikona_1040.close();
    }

    QFile ikona_1050(homePath + "/.cache/ikone_za_uro/ikona_1050");
    if(ikona_1050.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1050);
        dobiTockoP1((10.833)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*0.1),p1.y()+(stevilka_ure_font_size*1.7),lineiko);
        ikona_1050.close();
    }







//////////

    QFile ikona_1100(homePath + "/.cache/ikone_za_uro/ikona_1100");
    if(ikona_1100.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1100);
        dobiTockoP1((11)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*0.1),p1.y()+(stevilka_ure_font_size*1.7),lineiko);
        ikona_1100.close();
    }

    QFile ikona_1110(homePath + "/.cache/ikone_za_uro/ikona_1110");
    if(ikona_1110.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1110);
        dobiTockoP1((11.166)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*0.1),p1.y()+(stevilka_ure_font_size*1.8),lineiko);
        ikona_1110.close();
    }

    QFile ikona_1120(homePath + "/.cache/ikone_za_uro/ikona_1120");
    if(ikona_1120.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1120);
        dobiTockoP1((11.333)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*0.1),p1.y()+(stevilka_ure_font_size*1.8),lineiko);
        ikona_1120.close();
    }


    QFile ikona_1130(homePath + "/.cache/ikone_za_uro/ikona_1130");
    if(ikona_1130.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1130);
        dobiTockoP1((11.5)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*0.1),p1.y()+(stevilka_ure_font_size*1.8),lineiko);
        ikona_1130.close();
    }

    QFile ikona_1140(homePath + "/.cache/ikone_za_uro/ikona_1140");
    if(ikona_1140.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1140);
        dobiTockoP1((11.666)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()+(stevilka_ure_font_size*0.1)-(stevilka_ure_font_size*0.5),p1.y()+(stevilka_ure_font_size*1.7),lineiko);
        ikona_1140.close();
    }

    QFile ikona_1150(homePath + "/.cache/ikone_za_uro/ikona_1150");
    if(ikona_1150.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1150);
        dobiTockoP1((11.833)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()+(stevilka_ure_font_size*0.1)-(stevilka_ure_font_size*0.5),p1.y()+(stevilka_ure_font_size*1.7),lineiko);
        ikona_1150.close();
    }



/////////////
    QFile ikona_1200(homePath + "/.cache/ikone_za_uro/ikona_1200");
    if(ikona_1200.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1200);
        dobiTockoP1((12)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()+(stevilka_ure_font_size*0.1)-(stevilka_ure_font_size*0.5),p1.y()+(stevilka_ure_font_size*1.7),lineiko);
        ikona_1200.close();
    }

    QFile ikona_1210(homePath + "/.cache/ikone_za_uro/ikona_1210");
    if(ikona_1210.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1210);
        dobiTockoP1((12.166)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()+(stevilka_ure_font_size*0.1)-(stevilka_ure_font_size*0.5),p1.y()+(stevilka_ure_font_size*1.7),lineiko);
        ikona_1210.close();
    }

    QFile ikona_1220(homePath + "/.cache/ikone_za_uro/ikona_1220");
    if(ikona_1220.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1220);
        dobiTockoP1((12.333)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()+(stevilka_ure_font_size*0.1)-(stevilka_ure_font_size*0.5),p1.y()+(stevilka_ure_font_size*1.7),lineiko);
        ikona_1220.close();
    }


    QFile ikona_1230(homePath + "/.cache/ikone_za_uro/ikona_1230");
    if(ikona_1230.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1230);
        dobiTockoP1((12.5)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()+(stevilka_ure_font_size*0.1)-(stevilka_ure_font_size*0.5),p1.y()+(stevilka_ure_font_size*1.7),lineiko);
        ikona_1230.close();
    }


    QFile ikona_1240(homePath + "/.cache/ikone_za_uro/ikona_1240");
    if(ikona_1240.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1240);
        dobiTockoP1((12.666)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*0.75),p1.y()+(stevilka_ure_font_size*1.7),lineiko);
        ikona_1240.close();
    }
    QFile ikona_1250(homePath + "/.cache/ikone_za_uro/ikona_1250");
    if(ikona_1250.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1250);
        dobiTockoP1((12.833)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*0.75),p1.y()+(stevilka_ure_font_size*1.7),lineiko);
        ikona_1250.close();
    }




/////////////
    QFile ikona_1300(homePath + "/.cache/ikone_za_uro/ikona_1300");
    if(ikona_1300.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1300);
        dobiTockoP1((13)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*0.75),p1.y()+(stevilka_ure_font_size*1.7),lineiko);
        ikona_1300.close();
    }


    QFile ikona_1310(homePath + "/.cache/ikone_za_uro/ikona_1310");
    if(ikona_1310.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1310);
        dobiTockoP1((13.166)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*0.75),p1.y()+(stevilka_ure_font_size*1.7),lineiko);
        ikona_1310.close();
    }

    QFile ikona_1320(homePath + "/.cache/ikone_za_uro/ikona_1320");
    if(ikona_1320.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1320);
        dobiTockoP1((13.333)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*0.75),p1.y()+(stevilka_ure_font_size*1.7),lineiko);
        ikona_1320.close();
    }

    QFile ikona_1330(homePath + "/.cache/ikone_za_uro/ikona_1330");
    if(ikona_1330.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1330);
        dobiTockoP1((13.5)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*0.75),p1.y()+(stevilka_ure_font_size*1.6),lineiko);
        ikona_1330.close();
    }


    QFile ikona_1340(homePath + "/.cache/ikone_za_uro/ikona_1340");
    if(ikona_1340.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1340);
        dobiTockoP1((13.666)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size),p1.y()+(stevilka_ure_font_size*1.5),lineiko);
        ikona_1340.close();
    }
    QFile ikona_1350(homePath + "/.cache/ikone_za_uro/ikona_1350");
    if(ikona_1350.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1350);
        dobiTockoP1((13.833)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size),p1.y()+(stevilka_ure_font_size*1.5),lineiko);
        ikona_1350.close();
    }


/////////////
    QFile ikona_1400(homePath + "/.cache/ikone_za_uro/ikona_1400");
    if(ikona_1400.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1400);
        dobiTockoP1((14)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size),p1.y()+(stevilka_ure_font_size*1.5),lineiko);
        ikona_1400.close();
    }


    QFile ikona_1410(homePath + "/.cache/ikone_za_uro/ikona_1410");
    if(ikona_1410.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1410);
        dobiTockoP1((14.166)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size),p1.y()+(stevilka_ure_font_size*1.5),lineiko);
        ikona_1410.close();
    }

    QFile ikona_1420(homePath + "/.cache/ikone_za_uro/ikona_1420");
    if(ikona_1420.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1420);
        dobiTockoP1((14.333)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size),p1.y()+(stevilka_ure_font_size*1.5),lineiko);
        ikona_1420.close();
    }

    QFile ikona_1430(homePath + "/.cache/ikone_za_uro/ikona_1430");
    if(ikona_1430.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1430);
        dobiTockoP1((14.5)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size),p1.y()+(stevilka_ure_font_size*1.5),lineiko);
        ikona_1430.close();
    }

    QFile ikona_1440(homePath + "/.cache/ikone_za_uro/ikona_1440");
    if(ikona_1440.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1440);
        dobiTockoP1((14.666)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*1.25),p1.y()+(stevilka_ure_font_size*1.25),lineiko);
        ikona_1440.close();
    }

    QFile ikona_1450(homePath + "/.cache/ikone_za_uro/ikona_1450");
    if(ikona_1450.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1450);
        dobiTockoP1((14.833)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*1.25),p1.y()+(stevilka_ure_font_size*1.25),lineiko);
        ikona_1450.close();
    }



/////////////
    QFile ikona_1500(homePath + "/.cache/ikone_za_uro/ikona_1500");
    if(ikona_1500.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1500);
        dobiTockoP1((15)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*1.25),p1.y()+(stevilka_ure_font_size*1.25),lineiko);
        ikona_1500.close();
    }

    QFile ikona_1510(homePath + "/.cache/ikone_za_uro/ikona_1510");
    if(ikona_1510.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1510);
        dobiTockoP1((15.166)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*1.25),p1.y()+(stevilka_ure_font_size*1.25),lineiko);
        ikona_1510.close();
    }

    QFile ikona_1520(homePath + "/.cache/ikone_za_uro/ikona_1520");
    if(ikona_1520.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1520);
        dobiTockoP1((15.333)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*1.25),p1.y()+(stevilka_ure_font_size*1.25),lineiko);
        ikona_1520.close();
    }

    QFile ikona_1530(homePath + "/.cache/ikone_za_uro/ikona_1530");
    if(ikona_1530.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1530);
        dobiTockoP1((15.5)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*1.25),p1.y()+(stevilka_ure_font_size*1.25),lineiko);
        ikona_1530.close();
    }

    QFile ikona_1540(homePath + "/.cache/ikone_za_uro/ikona_1540");
    if(ikona_1540.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1540);
        dobiTockoP1((15.666)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*1.5),p1.y()+(stevilka_ure_font_size),lineiko);
        ikona_1540.close();
    }

    QFile ikona_1550(homePath + "/.cache/ikone_za_uro/ikona_1550");
    if(ikona_1550.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1550);
        dobiTockoP1((15.833)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*1.5),p1.y()+(stevilka_ure_font_size),lineiko);
        ikona_1550.close();
    }


/////////////
    QFile ikona_1600(homePath + "/.cache/ikone_za_uro/ikona_1600");
    if(ikona_1600.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1600);
        dobiTockoP1((16)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*1.5),p1.y()+(stevilka_ure_font_size),lineiko);
        ikona_1600.close();
    }

    QFile ikona_1610(homePath + "/.cache/ikone_za_uro/ikona_1610");
    if(ikona_1610.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1610);
        dobiTockoP1((16.166)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*1.5),p1.y()+(stevilka_ure_font_size),lineiko);
        ikona_1610.close();
    }

    QFile ikona_1620(homePath + "/.cache/ikone_za_uro/ikona_1620");
    if(ikona_1620.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1620);
        dobiTockoP1((16.333)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*1.5),p1.y()+(stevilka_ure_font_size),lineiko);
        ikona_1620.close();
    }


    QFile ikona_1630(homePath + "/.cache/ikone_za_uro/ikona_1630");
    if(ikona_1630.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1630);
        dobiTockoP1((16.5)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*1.4),p1.y()+(stevilka_ure_font_size*0.75),lineiko);
        ikona_1630.close();
    }


    QFile ikona_1640(homePath + "/.cache/ikone_za_uro/ikona_1640");
    if(ikona_1640.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1640);
        dobiTockoP1((16.666)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*1.75),p1.y()+(stevilka_ure_font_size*0.66),lineiko);
        ikona_1640.close();
    }

    QFile ikona_1650(homePath + "/.cache/ikone_za_uro/ikona_1650");
    if(ikona_1650.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1650);
        dobiTockoP1((16.833)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*1.75),p1.y()+(stevilka_ure_font_size*0.66),lineiko);
        ikona_1650.close();
    }






/////////////
    QFile ikona_1700(homePath + "/.cache/ikone_za_uro/ikona_1700");
    if(ikona_1700.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1700);
        dobiTockoP1((17)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*1.75),p1.y()+(stevilka_ure_font_size*0.66),lineiko);
        ikona_1700.close();
    }


    QFile ikona_1710(homePath + "/.cache/ikone_za_uro/ikona_1710");
    if(ikona_1710.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1710);
        dobiTockoP1((17.166)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*1.75),p1.y()+(stevilka_ure_font_size*0.66),lineiko);
        ikona_1710.close();
    }

    QFile ikona_1720(homePath + "/.cache/ikone_za_uro/ikona_1720");
    if(ikona_1720.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1720);
        dobiTockoP1((17.333)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*1.75),p1.y()+(stevilka_ure_font_size*0.66),lineiko);
        ikona_1720.close();
    }



    QFile ikona_1730(homePath + "/.cache/ikone_za_uro/ikona_1730");
    if(ikona_1730.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1730);
        dobiTockoP1((17.5)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*1.75),p1.y()+(stevilka_ure_font_size*0.66),lineiko);
        ikona_1730.close();
    }



    QFile ikona_1740(homePath + "/.cache/ikone_za_uro/ikona_1740");
    if(ikona_1740.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1740);
        dobiTockoP1((17.666)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*2),p1.y()+(stevilka_ure_font_size*0.5),lineiko);
        ikona_1740.close();
    }


    QFile ikona_1750(homePath + "/.cache/ikone_za_uro/ikona_1750");
    if(ikona_1750.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1750);
        dobiTockoP1((17.833)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*2),p1.y()+(stevilka_ure_font_size*0.5),lineiko);
        ikona_1750.close();
    }



/////////////
    QFile ikona_1800(homePath + "/.cache/ikone_za_uro/ikona_1800");
    if(ikona_1800.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1800);
        dobiTockoP1((18)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*2),p1.y()+(stevilka_ure_font_size*0.5),lineiko);
        ikona_1800.close();
    }

    QFile ikona_1810(homePath + "/.cache/ikone_za_uro/ikona_1810");
    if(ikona_1810.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1810);
        dobiTockoP1((18.166)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*2),p1.y()+(stevilka_ure_font_size*0.5),lineiko);
        ikona_1810.close();
    }
    QFile ikona_1820(homePath + "/.cache/ikone_za_uro/ikona_1820");
    if(ikona_1820.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1820);
        dobiTockoP1((18.333)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*2),p1.y()+(stevilka_ure_font_size*0.5),lineiko);
        ikona_1820.close();
    }

    QFile ikona_1830(homePath + "/.cache/ikone_za_uro/ikona_1830");
    if(ikona_1830.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1830);
        dobiTockoP1((18.5)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*2),p1.y()+(stevilka_ure_font_size*0.5),lineiko);
        ikona_1830.close();
    }

    QFile ikona_1840(homePath + "/.cache/ikone_za_uro/ikona_1840");
    if(ikona_1840.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1840);
        dobiTockoP1((18.666)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*1.75),p1.y()+(stevilka_ure_font_size*0.33),lineiko);
        ikona_1840.close();
    }

    QFile ikona_1850(homePath + "/.cache/ikone_za_uro/ikona_1850");
    if(ikona_1850.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1850);
        dobiTockoP1((18.833)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*1.75),p1.y()+(stevilka_ure_font_size*0.33),lineiko);
        ikona_1850.close();
    }


/////////////
    QFile ikona_1900(homePath + "/.cache/ikone_za_uro/ikona_1900");
    if(ikona_1900.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1900);
        dobiTockoP1((19)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*1.75),p1.y()+(stevilka_ure_font_size*0.33),lineiko);
        ikona_1900.close();
    }

    QFile ikona_1910(homePath + "/.cache/ikone_za_uro/ikona_1910");
    if(ikona_1910.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1910);
        dobiTockoP1((19.166)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*1.75),p1.y()+(stevilka_ure_font_size*0.33),lineiko);
        ikona_1910.close();
    }

    QFile ikona_1920(homePath + "/.cache/ikone_za_uro/ikona_1920");
    if(ikona_1920.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1920);
        dobiTockoP1((19.333)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*1.75),p1.y()+(stevilka_ure_font_size*0.33),lineiko);
        ikona_1920.close();
    }

    QFile ikona_1930(homePath + "/.cache/ikone_za_uro/ikona_1930");
    if(ikona_1930.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1930);
        dobiTockoP1((19.5)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*1.75),p1.y()+(stevilka_ure_font_size*0.166),lineiko);
        ikona_1930.close();
    }



    QFile ikona_1940(homePath + "/.cache/ikone_za_uro/ikona_1940");
    if(ikona_1940.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1940);
        dobiTockoP1((19.666)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*1.5),p1.y(),lineiko);
        ikona_1940.close();
    }


    QFile ikona_1950(homePath + "/.cache/ikone_za_uro/ikona_1950");
    if(ikona_1950.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_1950);
        dobiTockoP1((19.833)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*1.5),p1.y(),lineiko);
        ikona_1950.close();
    }








/////////////
    QFile ikona_2000(homePath + "/.cache/ikone_za_uro/ikona_2000");
    if(ikona_2000.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_2000);
        dobiTockoP1((20)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*1.5),p1.y(),lineiko);
        ikona_2000.close();
    }



    QFile ikona_2010(homePath + "/.cache/ikone_za_uro/ikona_2010");
    if(ikona_2010.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_2010);
        dobiTockoP1((20.166)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*1.5),p1.y(),lineiko);
        ikona_2010.close();
    }




    QFile ikona_2020(homePath + "/.cache/ikone_za_uro/ikona_2020");
    if(ikona_2020.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_2020);
        dobiTockoP1((20.333)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*1.5),p1.y(),lineiko);
        ikona_2020.close();
    }



    QFile ikona_2030(homePath + "/.cache/ikone_za_uro/ikona_2030");
    if(ikona_2030.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_2030);
        dobiTockoP1((20.5)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*1.2),p1.y(),lineiko);
        ikona_2030.close();
    }


    QFile ikona_2040(homePath + "/.cache/ikone_za_uro/ikona_2040");
    if(ikona_2040.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_2040);
        dobiTockoP1((20.666)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*1.1),p1.y()-(stevilka_ure_font_size*0.5),lineiko);
        ikona_2040.close();
    }


    QFile ikona_2050(homePath + "/.cache/ikone_za_uro/ikona_2050");
    if(ikona_2050.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_2050);
        dobiTockoP1((20.833)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size),p1.y()-(stevilka_ure_font_size*0.5),lineiko);
        ikona_2050.close();
    }

/////////////

    QFile ikona_2100(homePath + "/.cache/ikone_za_uro/ikona_2100");
    if(ikona_2100.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_2100);
        dobiTockoP1((21)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size),p1.y()-(stevilka_ure_font_size*0.5),lineiko);
        ikona_2100.close();
    }




    QFile ikona_2110(homePath + "/.cache/ikone_za_uro/ikona_2110");
    if(ikona_2110.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_2110);
        dobiTockoP1((21.166)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size),p1.y()-(stevilka_ure_font_size*0.5),lineiko);
        ikona_2110.close();
    }



    QFile ikona_2120(homePath + "/.cache/ikone_za_uro/ikona_2120");
    if(ikona_2120.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_2120);
        dobiTockoP1((21.333)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size),p1.y()-(stevilka_ure_font_size*0.5),lineiko);
        ikona_2120.close();
    }

    QFile ikona_2130(homePath + "/.cache/ikone_za_uro/ikona_2130");
    if(ikona_2130.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_2130);
        dobiTockoP1((21.5)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size),p1.y()-(stevilka_ure_font_size*0.5),lineiko);
        ikona_2130.close();
    }


    QFile ikona_2140(homePath + "/.cache/ikone_za_uro/ikona_2140");
    if(ikona_2140.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_2140);
        dobiTockoP1((21.666)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size),p1.y()-(stevilka_ure_font_size*0.75),lineiko);
        ikona_2140.close();
    }

    QFile ikona_2150(homePath + "/.cache/ikone_za_uro/ikona_2150");
    if(ikona_2150.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_2150);
        dobiTockoP1((21.833)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size),p1.y()-(stevilka_ure_font_size*0.75),lineiko);
        ikona_2150.close();
    }
/////////////

    QFile ikona_2200(homePath + "/.cache/ikone_za_uro/ikona_2200");
    if(ikona_2200.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_2200);
        dobiTockoP1((22)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size),p1.y()-(stevilka_ure_font_size*0.75),lineiko);
        ikona_2200.close();
    }

    QFile ikona_2210(homePath + "/.cache/ikone_za_uro/ikona_2210");
    if(ikona_2210.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_2210);
        dobiTockoP1((22.166)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*0.5),p1.y()-(stevilka_ure_font_size*0.75),lineiko);
        ikona_2210.close();
    }

    QFile ikona_2220(homePath + "/.cache/ikone_za_uro/ikona_2220");
    if(ikona_2220.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_2220);
        dobiTockoP1((22.333)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*0.5),p1.y()-(stevilka_ure_font_size*0.75),lineiko);
        ikona_2220.close();
    }

    QFile ikona_2230(homePath + "/.cache/ikone_za_uro/ikona_2230");
    if(ikona_2230.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_2230);
        dobiTockoP1((22.5)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*0.5),p1.y()-(stevilka_ure_font_size*0.75),lineiko);
        ikona_2230.close();
    }

    QFile ikona_2240(homePath + "/.cache/ikone_za_uro/ikona_2230");
    if(ikona_2230.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_2230);
        dobiTockoP1((22.666)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*0.5),p1.y()-(stevilka_ure_font_size*0.75),lineiko);
        ikona_2230.close();
    }

    QFile ikona_2250(homePath + "/.cache/ikone_za_uro/ikona_2250");
    if(ikona_2250.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_2250);
        dobiTockoP1((22.833)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*0.5),p1.y()-(stevilka_ure_font_size*0.75),lineiko);
        ikona_2250.close();
    }

/////////////
    QFile ikona_2300(homePath + "/.cache/ikone_za_uro/ikona_2300");
    if(ikona_2300.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_2300);
        dobiTockoP1((23)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*0.4),p1.y()-(stevilka_ure_font_size*0.75),lineiko);
        ikona_2300.close();
    }


    QFile ikona_2310(homePath + "/.cache/ikone_za_uro/ikona_2310");
    if(ikona_2310.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_2310);
        dobiTockoP1((23.166)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*0.4),p1.y()-(stevilka_ure_font_size*0.75),lineiko);
        ikona_2310.close();
    }


    QFile ikona_2320(homePath + "/.cache/ikone_za_uro/ikona_2320");
    if(ikona_2320.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_2320);
        dobiTockoP1((23.333)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*0.4),p1.y()-(stevilka_ure_font_size*0.75),lineiko);
        ikona_2320.close();
    }

    QFile ikona_2330(homePath + "/.cache/ikone_za_uro/ikona_2330");
    if(ikona_2330.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_2330);
        dobiTockoP1((23.5)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*0.4),p1.y()-(stevilka_ure_font_size*0.75),lineiko);
        ikona_2330.close();
    }

    QFile ikona_2340(homePath + "/.cache/ikone_za_uro/ikona_2340");
    if(ikona_2340.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_2340);
        dobiTockoP1((23.666)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*0.5),p1.y()-(stevilka_ure_font_size*0.75),lineiko);
        ikona_2340.close();
    }

    QFile ikona_2350(homePath + "/.cache/ikone_za_uro/ikona_2350");
    if(ikona_2350.open(QIODevice::ReadOnly)){
        QTextStream iniko(&ikona_2350);
        dobiTockoP1((23.833)*60, radij);
        QString lineiko = iniko.readLine();
        painter.drawText(p1.x()-(stevilka_ure_font_size*0.5),p1.y()-(stevilka_ure_font_size*0.75),lineiko);
        ikona_2350.close();
    }




/////////////
}


MainWindow::~MainWindow()
{
    delete ui;
}

















































