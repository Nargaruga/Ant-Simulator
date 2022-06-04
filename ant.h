#ifndef ANT_H
#define ANT_H

#include "cell.h"
#include "direction.h"
#include "sim_cell_data.h"

class Ant {
public:
  enum Mode { SEEK, RETURN };

  Ant(int x, int y, Direction d = Direction::NORTH) : m_d(d), m_x(x), m_y(y){};

  int getX() const { return m_x; }

  int getY() const { return m_y; }

  void move(int x, int y, std::default_random_engine &rng);

  Direction getDirection() const { return m_d; }

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

  bool hasFood() const { return m_hasFood; }

  void returnHome() {
    m_mode = RETURN;
    m_traveledDistance = 0;
  }

  void seekFood() {
    m_mode = SEEK;
    m_traveledDistance = 0;
  }

  Mode getMode() const { return m_mode; }

  Cell<SimCellData>
  pickDestination(const std::vector<Cell<SimCellData>> &candidates,
                  std::default_random_engine &rng);

  int getTraveledDistance() const { return m_traveledDistance; }

private:
  Direction m_d;
  int m_x;
  int m_y;
  bool m_hasFood = false;
  Mode m_mode = SEEK;
  int m_traveledDistance = 0;
  int m_maxSteps = 200;
};

#endif // ANT_H
