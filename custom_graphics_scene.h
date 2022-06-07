#ifndef CUSTOM_GRAPHICS_SCENE_H
#define CUSTOM_GRAPHICS_SCENE_H

#include "qtmetamacros.h"
#include <QGraphicsScene>

/*
 * Custom graphics scene with click handling.
 */
class CustomGraphicsScene : public QGraphicsScene {
  Q_OBJECT
public:
  CustomGraphicsScene();

protected:
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

signals:
  /*
   * Emitted on mouse release to transmit the coordinates of the click.
   */
  void mouseReleased(QPointF coords);
};

#endif // CUSTOM_GRAPHICS_SCENE_H
