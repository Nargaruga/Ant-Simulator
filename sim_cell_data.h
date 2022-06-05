#ifndef SIM_CELL_DATA_H
#define SIM_CELL_DATA_H

#include "colors.h"
#include <QColor>
#include <cmath>
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
  void incrementHomePheromone(float sourceDist, float traveledDistance) {
    if (sourceDist < 0 || traveledDistance < 0)
      return;

    m_homePheromone =
        std::min(m_homePheromone + 1.0f / ((sourceDist + 1.0f) *
                                           (0.8f * traveledDistance + 1.0f)),
                 1.0f);
  }

  /*
   *  TODO
   */
  void incrementFoodPheromone(float sourceDist, float traveledDistance) {
    if (sourceDist < 0 || traveledDistance < 0)
      return;

    m_foodPheromone =
        std::min(m_foodPheromone + 1.0f / ((sourceDist + 1.0f) *
                                           (0.8f * traveledDistance + 1.0f)),
                 1.0f);
  }

  /*
   *  TODO
   */
  void decrementPheromones() {
    m_homePheromone = std::max(m_homePheromone - 0.005f, 0.0f);
    m_foodPheromone = std::max(m_foodPheromone - 0.005f, 0.0f);
  }

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

  // TODO
  QColor getColor() const {
    switch (m_type) {
    case SimCellData::Type::ANT: {
      return antColor;
    }
    case SimCellData::Type::FLOOR: {
      QColor pheromoneColor =
          blend(homePheromoneColor, foodPheromoneColor,
                0.5f + 0.5f * m_foodPheromone - 0.5f * m_homePheromone);

      return blend(floorColor, pheromoneColor,
                   (m_foodPheromone + m_homePheromone) / 2.0f);
    }
    case SimCellData::Type::FOOD: {
      return foodColor;
    }
    case SimCellData::Type::ROCK: {
      return rockColor;
    }
    case SimCellData::Type::NEST: {
      return nestColor;
    }
    default: {
      return QColor();
    }
    }
  }

private:
  Type m_type;
  float m_homePheromone = 0.0f;
  float m_foodPheromone = 0.0f;
};

#endif
