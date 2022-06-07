#ifndef SIM_CELL_DATA_H
#define SIM_CELL_DATA_H

#include "colors.h"
#include <QColor>
#include <cmath>
#include <random>

/*
 *  Data contained in a cell of the simulation's grid.
 */
class SimCellData {

public:
  /*
   * Possible types of the cell.
   */
  enum Type { FLOOR, ROCK, ANT, FOOD, NEST };

  /*
   * Constructs a data object with the specified type.
   */
  SimCellData(Type type = Type::FLOOR) : m_type(type) {}

  /*
   * Sets the type of the cell.
   */
  void setType(Type type) { m_type = type; }

  /*
   * Increases the cell's home pheromone signal based on the distance from the
   * source and the distance traveled by the emitter ant.
   */
  void incrementHomePheromone(float strength, float sourceDist,
                              float traveledDistance) {
    if (sourceDist < 0 || traveledDistance < 0)
      return;

    m_homePheromone =
        std::min(m_homePheromone + strength / (powf(sourceDist + 1.0f, 2) *
                                               sqrtf(traveledDistance + 1.0f)),
                 1.0f);
  }

  /*
   * Increases the cell's food pheromone signal based on the distance from the
   * source and the distance traveled by the emitter ant.
   */
  void incrementFoodPheromone(float strength, float sourceDist,
                              float traveledDistance) {
    if (sourceDist < 0 || traveledDistance < 0)
      return;

    m_foodPheromone =
        std::min(m_foodPheromone + strength / (powf(sourceDist + 1.0f, 2) *
                                               sqrtf(traveledDistance + 1.0f)),
                 1.0f);
  }

  /*
   *  Simulates pheromone evaporation.
   */
  void decrementPheromones(float rate) {
    m_homePheromone = std::max(m_homePheromone - rate, 0.0f);
    m_foodPheromone = std::max(m_foodPheromone - rate, 0.0f);
  }

  /*
   * Sets all pheromone signals to 0.
   */
  void clearPheromones() {
    m_homePheromone = 0.0f;
    m_foodPheromone = 0.0f;
  }

  /*
   * Returns the type of the cell.
   */
  Type getType() const { return m_type; };

  /*
   * Returns the cell's home pheromone level.
   */
  float getHomePheromone() const { return m_homePheromone; }

  /*
   * Returns the cell's food pheromone level.
   */
  float getFoodPheromone() const { return m_foodPheromone; }

  // Returns the cell's color.
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
  Type m_type;                  // Cell type
  float m_homePheromone = 0.0f; // Home pheromone level
  float m_foodPheromone = 0.0f; // Food pheromone level
};

#endif
