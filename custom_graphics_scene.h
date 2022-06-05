#ifndef CUSTOM_GRAPHICS_SCENE_H
#define CUSTOM_GRAPHICS_SCENE_H

#include "qtmetamacros.h"
#include <QGraphicsScene>

class CustomGraphicsScene : public QGraphicsScene {
  Q_OBJECT
public:
  CustomGraphicsScene();

protected:
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

signals:
  void mouseReleased(QPointF coords);

private:
  bool m_panning = false;
  int m_panStartX;
  int m_panStartY;
};

#endif // CUSTOM_GRAPHICS_SCENE_H
