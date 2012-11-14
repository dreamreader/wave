#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QBitmap>
#include <QDebug>
#include <QTimer>
#include <QList>
#include <QPoint>
#include <QPointF>
#include <qmath.h>

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
  void open();

private slots:
  void on_pushButton_3_clicked();

  void on_pushButton_4_clicked();

  void on_pushButton_5_clicked();

  void updateWave();

private:
    Ui::MainWindow *ui;

    QTimer          _timer;
    QPixmap         _pm;
    QImage          _srcImg;
    QImage          _binImg;
    QImage          _displayImg;

    QPixmap         _exPm;
    QImage          _exImg;

    bool            _firstStep;
    QList<QPoint>   _trail;
    QList<QPoint>   _front;

    int             _dx;
    int             _dy;
    QPointF         _lastCm;
    float           _v;

    QList<QPoint>   _wave;
};

#endif // MAINWINDOW_H
