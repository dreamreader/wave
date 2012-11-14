#include "god.h"

God::God()
{
}

QPointF God::centerOfFront(QList<QPoint> &front)
{
  float x;
  float y;
  int i,j;
  for (int f=0; f<front.size(); f++)
  {
    i = front[f].x();
    j = front[f].y();
    x += (float)i / (float)front.size();
    y += (float)j / (float)front.size();
  }

  return QPointF(x,y);
}

void God::killMinorFronts(QList<QPoint> &fronts)
{
  if (!fronts.size()) return;
  //qDebug() << "front size " + QString::number(fronts.size());

  QList<int> mask;
  for (int i=0; i<fronts.size(); i++)
    mask.push_back(0);

  int unmarked = 0;
  int marker = 1;
  while (unmarked != -1)
  {
    selectFront(unmarked, fronts, mask, marker++);
    unmarked = firstUnmarked(mask);
    //qDebug() << "mark " + QString::number(marker);
    //qDebug() << "unmarked " + QString::number(unmarked);
  }

  int markOfMax(0);
  int countMax(0);
  int count(0);
  marker = 1;
  do
  {
    count = markCount(mask, marker);
    //qDebug() << "mark " + QString::number(marker);
    //qDebug() << "count " + QString::number(count);
    if (count > countMax)
    {
      markOfMax = marker;
      countMax = count;
    }

    marker++;
  }
  while(count);

  //qDebug() << "max front power " + QString::number(countMax);
  //qDebug() << "max front mark " + QString::number(markOfMax);

  //qDebug() << fronts.size();
  killFrontWithoutMark(fronts, mask, markOfMax);
  //qDebug() << fronts.size();
  //qDebug() << "___________";
}

void God::selectFront(int pos, QList<QPoint> &fronts, QList<int> &mask, int mark)
{
  if (*(mask.begin() + pos) == mark) return;
  *(mask.begin() + pos) = mark;

  QPoint p = fronts[pos];
  p.setY(p.y() - 1); int p1 = fronts.indexOf(p);
  p.setX(p.x() + 1); int p2 = fronts.indexOf(p);
  p.setY(p.y() + 1); int p3 = fronts.indexOf(p);
  p.setY(p.y() + 1); int p4 = fronts.indexOf(p);
  p.setX(p.x() - 1); int p5 = fronts.indexOf(p);
  p.setX(p.x() - 1); int p6 = fronts.indexOf(p);
  p.setY(p.y() - 1); int p7 = fronts.indexOf(p);
  p.setY(p.y() - 1); int p8 = fronts.indexOf(p);

  QString str;
  str = "neightbors ";
  str += (QString::number(p1) + " ");
  str += (QString::number(p2) + " ");
  str += (QString::number(p3) + " ");
  str += (QString::number(p4) + " ");
  str += (QString::number(p5) + " ");
  str += (QString::number(p6) + " ");
  str += (QString::number(p7) + " ");
  str += (QString::number(p8) + " ");
  //qDebug() << str;

  if (p1 != -1) selectFront(p1, fronts, mask, mark);
  if (p2 != -1) selectFront(p2, fronts, mask, mark);
  if (p3 != -1) selectFront(p3, fronts, mask, mark);
  if (p4 != -1) selectFront(p4, fronts, mask, mark);
  if (p5 != -1) selectFront(p5, fronts, mask, mark);
  if (p6 != -1) selectFront(p6, fronts, mask, mark);
  if (p7 != -1) selectFront(p7, fronts, mask, mark);
  if (p8 != -1) selectFront(p8, fronts, mask, mark);
}

int God::firstUnmarked(QList<int> &mask)
{
  for (int i=0; i<mask.size(); i++)
  {
      if (!mask[i]) return i;
  }
  return -1;
}

int God::markCount(QList<int> &mask, int mark)
{
  int c(0);
  for (int i=0; i<mask.size(); i++)
  {
      if (mask[i] == mark)
        c++;
  }
  return c;
}

void God::killFrontWithMark(QList<QPoint> &fronts, QList<int> &mask, int mark)
{
  for (int i=0; i<mask.size(); i++)
  {
    if (mask[i] == mark)
    {
      fronts.erase(fronts.begin()+i);
      mask.erase(mask.begin()+i);
      i--;
    }
  }
}

void God::killFrontWithoutMark(QList<QPoint> &fronts, QList<int> &mask, int mark)
{
  for (int i=0; i<mask.size(); i++)
  {
    if (mask[i] != mark)
    {
      fronts.erase(fronts.begin()+i);
      mask.erase(mask.begin()+i);
      i--;
    }
  }
}

