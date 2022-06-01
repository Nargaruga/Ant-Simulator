#include "cave_gen.h"
#include <iostream>
#include <random>

/*
 * Returns the number of rocks adjacent to this cell
 */
int countNearbyRocks(std::vector<Cell<bool>> neighbours) {
  int count = 0;

  for (size_t i = 0; i < neighbours.size(); i++) {
    if (neighbours[i].getData())
      count++;
  }

  // For border cells, missing neighbours count as walls
  return count + (8 - neighbours.size());
}

CaveGenerator::CaveGenerator(int seed, int rockRatio, int threshold, int steps)
    : m_seed(seed), m_rockRatio(rockRatio), m_threshold(threshold),
      m_steps(steps) {

  if (rockRatio < 0 || rockRatio > 100 || threshold < 0 || threshold > 8 ||
      steps < 0)
    std::invalid_argument("Arguments do not fall in the required ranges.");
}

void CaveGenerator::generateCave(int rows, int cols) {
  Grid<bool> grid(rows, cols);

  initialize(grid);
  simulate(grid);

  emit gridReady(grid);
}

void CaveGenerator::initialize(Grid<bool> &grid) {
  std::default_random_engine rng(m_seed);

  for (int x = 0; x < grid.getCols(); x++) {
    for (int y = 0; y < grid.getRows(); y++) {
      if (grid.isBorder(x, y) || (static_cast<int>(rng() % 100) < m_rockRatio))
        grid.setCell(x, y, true);
      else
        grid.setCell(x, y, false);
    }
  }
}

void CaveGenerator::step(Grid<bool> &grid) {
  Grid updatedGrid(grid);

  for (int x = 0; x < grid.getCols(); x++) {
    for (int y = 0; y < grid.getRows(); y++) {
      std::vector<Cell<bool>> neighbourhood = grid.getMooreNeighbourhood(x, y);

      int neighbours = countNearbyRocks(neighbourhood);

      if (neighbours >= m_threshold) {
        updatedGrid.setCell(x, y, true);
      } else {
        updatedGrid.setCell(x, y, false);
      }
    }
  }

  grid = updatedGrid;
}

void CaveGenerator::simulate(Grid<bool> &grid) {
  for (int i = 0; i < m_steps; i++) {
    step(grid);
  }
}
