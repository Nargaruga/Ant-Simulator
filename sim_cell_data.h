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
  enum Type { FLOOR, ROCK, ANT, FOOD, NEST };

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
  void incrementHomePheromone() {
    m_homePheromone = std::min(m_homePheromone + 0.5f, 1.0f);
  }

  /*
   *  TODO
   */
  void decrementHomePheromone() { m_homePheromone *= 0.99f; }

  /*
   *  TODO
   */
  void incrementFoodPheromone() {
    m_foodPheromone = std::min(m_foodPheromone + 0.5f, 1.0f);
  }

  /*
   *  TODO
   */
  void decrementFoodPheromone() { m_foodPheromone *= 0.99f; }

  void clearPheromones() {
    m_homePheromone = 0.0f;
    m_foodPheromone = 0.0f;
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
  float m_homePheromone = 0.0f;
  float m_foodPheromone = 0.0f;
};

#endif
