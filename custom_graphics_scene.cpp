#include "custom_graphics_scene.h"
#include <QGraphicsSceneMouseEvent>

CustomGraphicsScene::CustomGraphicsScene() {}

void CustomGraphicsScene::mouseReleaseEvent(
    QGraphicsSceneMouseEvent *mouseEvent) {

  if (mouseEvent->button() == Qt::LeftButton) {
    QPointF coords = mouseEvent->scenePos();
    emit mouseReleased(coords);
  }
}
