#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QString>
#include <QCalendarWidget>
// za visino in sirino ekrana
#include <QDesktopWidget>

// za transparentnost
#include <QtGui>
#include <QtCore>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    // to za timer
    QTimer *timer;
    //void start();
    //void mousePressEvent(QMouseEvent *event);


signals:
    void clicked();

// to za timer
public slots:
    void MyTimer();

private:
    Ui::MainWindow *ui;
    //PrviDialog *goFigure;

// to za risanje
protected:
    void paintEvent(QPaintEvent *e);

//private slots:
//void on_pushButton_clicked();


};
#endif // MAINWINDOW_H







































