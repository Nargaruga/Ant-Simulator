#ifndef COLORS_H
#define COLORS_H

#include <QColor>

const QColor floorColor = QColor(96, 96, 96);
const QColor innerRockColor = QColor(64, 64, 64);
const QColor outerRockColor = QColor(32, 32, 32);
const QColor organismColor = QColor(0, 204, 0);

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
