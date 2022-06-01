#ifndef SIM_CELL_DATA_H
#define SIM_CELL_DATA_H

#include "colors.h"
#include <QColor>
#include <random>

/*
 *  Data contained in a cell of the simulation's grid
 */
class SimCellData {

public:
  /*
   *  Possible types of the cell
   */
  enum Type { FLOOR, ROCK, ANT, FOOD };

  /*
   *  Constructs a data object with the specified type
   */
  SimCellData(Type type = Type::FLOOR) : m_type(type) {}

  /*
   *  Sets the type of the cell
   */
  void setType(Type type) { m_type = type; }

  /*
   *  TODO
   */
  void startHomePheromone() { m_homePheromone = 1.0; }

  /*
   *  TODO
   */
  void incrementHomePheromone() {
    m_homePheromone = std::min(m_homePheromone + 0.1, 1.0);
  }

  /*
   *  TODO
   */
  void decrementHomePheromone() {
    m_homePheromone = std::max(m_homePheromone - 0.1, 0.0);
  }

  /*
   *  Returns the type of the cell
   */
  Type getType() const { return m_type; };

  /*
   *  TODO
   */
  float getHomePheromone() const { return m_homePheromone; }

  /*
   *  TODO
   */
  float getFoodPheromone() const { return m_foodPheromone; }

private:
  Type m_type = Type::FLOOR;
  float m_homePheromone = 0.0;
  float m_foodPheromone = 0.0;
};

#endif
