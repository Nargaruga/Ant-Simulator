#ifndef ANT_H
#define ANT_H

#include "direction.h"

class Ant {
public:
  Ant(int x, int y, Direction d = Direction::NORTH) : m_d(d), m_x(x), m_y(y){};

  int getX() const { return m_x; }

  int getY() const { return m_y; }

  Direction getDirection() const { return m_d; }

  bool hasFood() { return m_hasFood; }

  void setCoords(int x, int y) {
    if (m_d == Direction::NORTH || m_d == Direction::SOUTH) {
      if (x == m_prevX + 1)
        m_d = Direction::EAST;
      else if (x == m_prevX - 1)
        m_d = Direction::WEST;
    } else {
      if (y == m_prevY + 1)
        m_d = Direction::SOUTH;
      else if (y == m_prevX - 1)
        m_d = Direction::NORTH;
    }

    m_prevX = m_x;
    m_prevY = m_y;
    m_x = x;
    m_y = y;
  }

  void setDirection(Direction d) { m_d = d; }

  void invert() {
    switch (m_d) {
    case NORTH: {
      m_d = SOUTH;
      break;
    }
    case SOUTH: {
      m_d = NORTH;
      break;
    }
    case EAST: {
      m_d = WEST;
      break;
    }
    case WEST: {
      m_d = EAST;
    }
    }
  }

  void pickUpFood() { m_hasFood = true; }

  void dropFood() { m_hasFood = false; }

private:
  Direction m_d = Direction::NORTH;
  int m_prevX;
  int m_prevY;
  int m_x;
  int m_y;
  bool m_hasFood = false;
};

#endif // ANT_H
