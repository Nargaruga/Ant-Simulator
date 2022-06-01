#ifndef ANT_H
#define ANT_H

class Ant {
public:
  Ant(int x, int y) : m_x(x), m_y(y){};

  int getX() { return m_x; }

  int getY() { return m_y; }

  bool hasFood() { return m_hasFood; }

  void setCoords(int x, int y) {
    m_x = x;
    m_y = y;
  }

  void pickUpFood() { m_hasFood = true; }

  void dropFood() { m_hasFood = false; }

private:
  int m_x;
  int m_y;
  bool m_hasFood = false;
};

#endif // ANT_H
