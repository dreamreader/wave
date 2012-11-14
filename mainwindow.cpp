#include "god.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _dx(0),
    _dy(0),
    _v(0)

{
  //форма волны
  _wave.push_back(QPoint(0, 0 - 3));
  _wave.push_back(QPoint(0 + 1, 0 - 3));
  _wave.push_back(QPoint(0 + 2, 0 - 2));
  _wave.push_back(QPoint(0 + 3, 0 - 1));
  _wave.push_back(QPoint(0 + 3, 0));
  _wave.push_back(QPoint(0 + 3, 0 + 1));
  _wave.push_back(QPoint(0 + 2, 0 + 2));
  _wave.push_back(QPoint(0 + 1, 0 + 3));
  _wave.push_back(QPoint(0, 0 + 3));
  _wave.push_back(QPoint(0 - 1, 0 + 3));
  _wave.push_back(QPoint(0 - 2, 0 + 2));
  _wave.push_back(QPoint(0 - 3, 0 + 1));
  _wave.push_back(QPoint(0 - 3, 0));
  _wave.push_back(QPoint(0 - 3, 0 - 1));
  _wave.push_back(QPoint(0 - 2, 0 - 2));
  _wave.push_back(QPoint(0 - 1, 0 - 3));

  ui->setupUi(this);
  ui->scrollArea->setWidget(ui->label);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::open()
{
  QString fileName = QFileDialog::getOpenFileName(this,
                                      tr("Open File"), QDir::currentPath());
  if (!fileName.isEmpty())
  {
    _srcImg.load(fileName);
    if (_srcImg.isNull())
    {
      QMessageBox::information(this, tr("Image Viewer"),
                                   tr("Cannot load %1.").arg(fileName));
      return;
    }

    _pm = QPixmap::fromImage(_srcImg );
    ui->label->setPixmap(_pm);

    _exImg = _srcImg;
    _exImg.fill(Qt::white);
    _exPm = QPixmap::fromImage(_exImg);
  }
}

void MainWindow::on_pushButton_3_clicked()
{
  open();
  _firstStep = true;
}

void MainWindow::on_pushButton_4_clicked()
{
  _binImg = _srcImg.copy();

  QRgb col;
  int gray;
  int width = _binImg.width();
  int height = _binImg.height();

  int aver(ui->horizontalSlider->value());
  for (int i = 0; i < width; ++i)
  {
      for (int j = 0; j < height; ++j)
      {
          col = _binImg.pixel(i, j);
          gray = qRed(col);
          if (gray > aver)
            _binImg.setPixel(i, j, qRgb(0xff, 0xff, 0xff));
          else
            _binImg.setPixel(i, j, qRgb(0x00, 0x00, 0x00));
      }
  }

  _pm = QPixmap::fromImage(_binImg);
  ui->label->setPixmap(_pm);
  ui->label->update();
  _firstStep = true;
}

void MainWindow::on_pushButton_5_clicked()
{
  ui->pushButton_5->setEnabled(false);

  if (_firstStep)
  {
    _displayImg = _binImg.copy();
    _firstStep = false;
    _lastCm.setX(-1);
    _lastCm.setY(-1);
  }

  int x0;
  int y0;
  do
  {
    x0 = qrand() % _displayImg.width();
    y0 = qrand() % _displayImg.height();
  }
  while(qRed(_displayImg.pixel(x0, y0)) != 0);

  _displayImg.setPixel(x0, y0, qRgb(0xff, 0x00, 0x00));
  //_front.clear();
  _front.push_back(QPoint(x0 ,y0));

  _firstStep = false;

  _timer.connect(&_timer, SIGNAL(timeout()), this, SLOT(updateWave()));
  _timer.start(100);

  ui->pushButton_5->setEnabled(true);
}

void MainWindow::updateWave()
{
  int v0r, v0g, v0b, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16;

  int res = 0;
  int i,j;
  QList<QPoint> newfront;
  while (_front.size())
  {
    QPoint f = _front[0];
    i = f.x();
    j = f.y();
    if (!_trail.contains(f)) _trail.push_back(f);
    _front.pop_front();

    //текущее направление волны
    int wDirect = _wave.indexOf(QPoint(_dx, _dy));
    int startDirect, endDirect;
    QList<QPoint> actualWave;

    qDebug() << "________";
    qDebug() << _v;
    qDebug() << wDirect;
    qDebug() << "________";

    //задать сектор волны
    if ((_v == 0) || (wDirect < 0))
    {
      startDirect = 0;
      endDirect = 16;
    }
    else if ((_v >= 0) && (_v < 1))
    {
      startDirect = wDirect - 4;
      endDirect = wDirect + 4;
    }
    else if ((_v >= 1) && (_v < 2))
    {
      startDirect = wDirect - 2;
      endDirect = wDirect + 2;
    }
    else if ((_v >= 2) && (_v < 3))
    {
      startDirect = wDirect - 1;
      endDirect = wDirect + 1;
    }
    else
    {
      startDirect = wDirect;
      endDirect = wDirect;
    }

    qDebug() << "________";
    qDebug() << startDirect;
    qDebug() << endDirect;
    qDebug() << "________";

    //заполнить вектор направлений распространения волны
    if (startDirect < 0)
    {
      for (int i = 15 + startDirect; i <= 15; i++)
        actualWave.push_back(_wave[i]);
      for (int i = 0; i < endDirect; i++)
        actualWave.push_back(_wave[i]);
    }
    else if (endDirect > 15)
    {
      for (int i = startDirect; i <= 15; i++)
        actualWave.push_back(_wave[i]);
      for (int i = 0; i < endDirect - 15; i++)
        actualWave.push_back(_wave[i]);
    }
    else
    {
      for (int i = startDirect; i <= endDirect; i++)
        actualWave.push_back(_wave[i]);
    }

    //перебрать направления распространения
    for (int d = 0; d < actualWave.size(); d++)
    {
        int di = actualWave[d].x();
        int dj = actualWave[d].y();
        qDebug() << "di " + QString::number(di);
        qDebug() << "dj " + QString::number(dj);
        int v = qRed(_displayImg.pixel(i + di, j + dj));
        if (v == 0)
        {
            QPoint p(i + di, j + dj);
            if (!newfront.contains(p)) newfront.push_back(p);
            res++;
        }
    }
  }

  for (int t = 0; t < _trail.size(); t++)
  {
      i = _trail[t].x();
      j = _trail[t].y();
      /*v0r = qRed(_displayImg.pixel(i, j));
      v0g = qGreen(_displayImg.pixel(i, j));
      v0b = qBlue(_displayImg.pixel(i, j));
      if (v0r - v0g > 0x16)
        _displayImg.setPixel(i, j, qRgb(v0r - 0x08, v0g + 0x08, v0b + 0x08));*/

      QColor color(_displayImg.pixel(i, j));
      if (color.hue() < 270)
      {
        color.setHsv(color.hue() + 10, color.saturation(), color.value());
        _displayImg.setPixel(i, j, qRgb(color.red(), color.green(), color.blue()));
      }
      else
        _trail.erase(_trail.begin() + t--);
  }
  _front = newfront;

  //Убить мелкие фронты
  //God::killMinorFronts(_front);

  //Найти сдвиг центра масс
  /*QPointF cm = God::centerOfFront(_front);
  if (_lastCm != QPointF(-1,-1))
  {
      _dx = (int)(_lastCm.x() - cm.x());
      _dy = (int)(_lastCm.y() - cm.y());
      _v = sqrt( (_lastCm.x() - cm.x())*(_lastCm.x() - cm.x()) + (_lastCm.y() - cm.y())*(_lastCm.y() - cm.y()) );
  }
  _lastCm = cm;
  qDebug() << "dx " + QString::number(_dx);
  qDebug() << "dy " + QString::number(_dy);
  qDebug() << "v " + QString::number(_v);
  qDebug() << "cm.x " + QString::number(_lastCm.x());
  qDebug() << "cm.y " + QString::number(_lastCm.y());
  qDebug() << " ";
  _exImg.setPixel((int)cm.x(), (int)cm.y(), Qt::darkRed);*/

  for (int f = 0; f < _front.size(); f++)
  {
      i = _front[f].x();
      j = _front[f].y();
      _displayImg.setPixel(i, j, qRgb(0xff, 0x00, 0x00));
  }

  //qDebug() << res;

  QPixmap newpm = QPixmap::fromImage(_displayImg);
  _pm = newpm.scaled(ui->label->width(), ui->label->height(), Qt::KeepAspectRatio);
  ui->label->setPixmap(_pm);
  _pm = QPixmap::fromImage(_displayImg);
  ui->label->setPixmap(_pm);
  ui->label->update();

  /*_exPm = QPixmap::fromImage(_exImg);
  ui->label_2->setPixmap(_exPm);
  ui->label_2->update();*/

  if (!_trail.size() && !_front.size()) _timer.stop();
}
