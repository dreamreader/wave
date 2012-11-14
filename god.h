#ifndef GOD_H
#define GOD_H

#include <QList>
#include <QPoint>
#include <QPointF>
#include <QDebug>

class God
{
public:
  God();

  static QPointF centerOfFront(QList<QPoint> &front);

  static void killMinorFronts(QList<QPoint> &front);

private:
  static void selectFront(int pos, QList<QPoint> &front, QList<int> &mask, int mark = 1);

  static int firstUnmarked(QList<int> &mask);

  static int markCount(QList<int> &mask, int mark);

  static void killFrontWithMark(QList<QPoint> &front, QList<int> &mask, int mark);

  static void killFrontWithoutMark(QList<QPoint> &front, QList<int> &mask, int mark);
};

#endif // GOD_H
