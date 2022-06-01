#ifndef COLORS_H
#define COLORS_H

#include <QColor>

const QColor floorColor = QColor(96, 96, 96);
const QColor rockColor = QColor(64, 64, 64);
const QColor antColor = QColor(21, 182, 43);
const QColor foodColor = QColor(1831, 111, 51);
const QColor homePheromoneColor = QColor(37, 244, 244);
const QColor foodPheromoneColor = QColor();

/*
 * Blends the colors `a` and `b` with ratio `r`
 */
inline QColor blend(QColor a, QColor b, float r) {
  if (r <= 0)
    return a;
  else if (r >= 1)
    return b;

  int aR, aG, aB;
  int bR, bG, bB;

  a.getRgb(&aR, &aG, &aB);
  b.getRgb(&bR, &bG, &bB);

  QColor blended = QColor::fromRgb(floor(aR * (1 - r) + bR * r),
                                   floor(aG * (1 - r) + bG * r),
                                   floor(aB * (1 - r) + bB * r));

  return blended;
}

#endif // COLORS_H
