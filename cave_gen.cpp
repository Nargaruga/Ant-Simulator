#include "cave_gen.h"
#include <iostream>
#include <random>

/*
 * Returns the number of rocks adjacent to this cell
 */
int countNearbyRocks(std::vector<Cell<SimCellData>> neighbours) {
  int count = 0;

  for (size_t i = 0; i < neighbours.size(); i++) {
    if (neighbours[i].getData().getType() == SimCellData::Type::ROCK)
      count++;
  }

  return count;
}

CaveGenerator::CaveGenerator(int seed, int rockRatio, int threshold, int steps,
                             int radius)
    : m_seed(seed), m_rockRatio(rockRatio), m_threshold(threshold),
      m_steps(steps), m_radius(radius) {

  if (rockRatio < 0 || rockRatio > 100 || threshold < 0 || threshold > 8 ||
      steps < 0 || radius < 0)
    std::invalid_argument("Arguments do not fall in the required ranges.");
}

void CaveGenerator::generateCave(int rows, int cols) {
  Grid<SimCellData> grid(rows, cols);

  initialize(grid);
  simulate(grid);

  emit gridReady(grid);
}

void CaveGenerator::initialize(Grid<SimCellData> &grid) {
  std::default_random_engine rng(m_seed);

  for (int x = 0; x < grid.getCols(); x++) {
    for (int y = 0; y < grid.getRows(); y++) {
      if (grid.isBorder(x, y) || (static_cast<int>(rng() % 100) < m_rockRatio))
        grid.setCell(x, y, SimCellData::ROCK);
      else
        grid.setCell(x, y, SimCellData::FLOOR);
    }
  }
}

void CaveGenerator::step(Grid<SimCellData> &grid) {
  Grid updatedGrid(grid);

  for (int x = 0; x < grid.getCols(); x++) {
    for (int y = 0; y < grid.getRows(); y++) {
      std::vector<Cell<SimCellData>> neighbourhood;

      if (m_mode == MOORE) {
        neighbourhood = grid.getMooreNeighbourhood(x, y, m_radius);
      } else {
        neighbourhood = grid.getNeumannNeighbourhood(x, y, m_radius);
      }

      if (grid.isBorder(x, y)) {
        // Border cells are always rock
        updatedGrid.setCell(x, y, SimCellData::ROCK);
      } else {
        int neighbours = countNearbyRocks(neighbourhood);

        if (neighbours >= m_threshold) {
          updatedGrid.setCell(x, y, SimCellData::Type::ROCK);
        } else {
          updatedGrid.setCell(x, y, SimCellData::Type::FLOOR);
        }
      }
    }
  }

  grid = updatedGrid;
}

void CaveGenerator::simulate(Grid<SimCellData> &grid) {
  for (int i = 0; i < m_steps; i++) {
    step(grid);
  }
}

void CaveGenerator::resetParams() {
  m_seed = 0;
  m_rockRatio = 60;
  m_threshold = 5;
  m_steps = 8;
  m_radius = 1;
}
