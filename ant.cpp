#include "ant.h"

void Ant::move(int x, int y, std::default_random_engine &rng) {
  int dX = x - m_x;
  int dY = y - m_y;

  m_d = std::pair<int, int>(dX, dY);

  m_traveledDistance++;
  if (m_mode == SEEK) {
    if (m_traveledDistance >= m_maxSteps) {
      m_mode = RETURN;
      m_traveledDistance = 0;
      invert();
    }
  }

  m_x = x;
  m_y = y;
}

Cell<SimCellData>
Ant::pickDestination(const std::vector<Cell<SimCellData>> &candidates,
                     std::default_random_engine &rng) {
  // Choose the most appealing cell to move to
  Cell<SimCellData> destination = candidates[rng() % candidates.size()];
  for (Cell<SimCellData> neighbour : candidates) {

    if (m_mode == RETURN) {
      // When in return mode, the ant looks for the nest
      if (neighbour.getData().getType() == SimCellData::Type::NEST) {
        // We found the nest, stop searching
        destination = neighbour;
        break;
      } else if (neighbour.getData().getHomePheromone() >
                 destination.getData().getHomePheromone())
        destination = neighbour;

    } else if (m_mode == SEEK) {
      // When in seek mode, the ant looks for food
      if (neighbour.getData().getType() == SimCellData::Type::FOOD) {
        // We found food, stop searching
        destination = neighbour;
        break;
      } else if (neighbour.getData().getFoodPheromone() >
                     destination.getData().getFoodPheromone() ||
                 neighbour.getData().getHomePheromone() <
                     destination.getData().getHomePheromone())
        destination = neighbour;
    }
  }

  return destination;
}
