#include "cave_gen.h"
#include <iostream>
#include <random>

/*
 * Returns the number of rocks adjacent to this cell
 */
int countNearbyRocks(std::vector<Cell> neighbours) {
  int count = 0;

  for (size_t i = 0; i < neighbours.size(); i++) {
    if (neighbours[i].isRock())
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
  Grid grid(rows, cols);

  initialize(grid);
  simulate(grid);

  emit gridReady(grid);
}

void CaveGenerator::initialize(Grid &grid) {
  std::default_random_engine rng(m_seed);

  for (int x = 0; x < grid.getCols(); x++) {
    for (int y = 0; y < grid.getRows(); y++) {
      if (grid.isBorder(x, y) || (static_cast<int>(rng() % 100) < m_rockRatio))
        grid.setCellState(x, y, Cell::State::INNER_ROCK);
      else
        grid.setCellState(x, y, Cell::State::FLOOR);
    }
  }
}

void CaveGenerator::step(Grid &grid) {
  Grid updatedGrid(grid);

  for (int x = 0; x < grid.getCols(); x++) {
    for (int y = 0; y < grid.getRows(); y++) {
      std::vector<Cell> neighbourhood = grid.getMooreNeighbourhood(x, y);

      int neighbours = countNearbyRocks(neighbourhood);

      if (neighbours >= m_threshold) {
        updatedGrid.setCellState(x, y, Cell::State::INNER_ROCK);
      } else {
        updatedGrid.setCellState(x, y, Cell::State::FLOOR);
      }
    }
  }

  grid = updatedGrid;
}

void CaveGenerator::simulate(Grid &grid) {
  for (int i = 0; i < m_steps; i++) {
    step(grid);
  }
}

void CaveGenerator::highlightWalls(Grid &grid) {
  Grid updatedGrid(grid);

  for (int x = 0; x < grid.getCols(); x++) {
    for (int y = 0; y < grid.getRows(); y++) {
      std::vector<Cell> neighbourhood = grid.getMooreNeighbourhood(x, y);

      int neighbours = countNearbyRocks(neighbourhood);

      if (grid.getCell(x, y).isRock() && neighbours < 8) {
        updatedGrid.setCellState(x, y, Cell::State::OUTER_ROCK);
      }
    }
  }

  grid = updatedGrid;
}

void CaveGenerator::spawnLightSource(Grid grid, int lightX, int lightY) {
  if (lightX < 0 || lightY < 0 || lightX >= grid.getCols() ||
      lightY >= grid.getRows())
    throw std::invalid_argument("Coordinates are out of bounds.");

  // The light source must be on a floor tile
  if (grid.getCell(lightX, lightY).isRock())
    return;

  // Set the light source
  grid.setCellLightLevel(lightX, lightY, 1.0);

  // Set the lightning value for each cell
  for (int x = 0; x < grid.getCols(); x++) {
    for (int y = 0; y < grid.getRows(); y++) {
      Cell cell = grid.getCell(x, y);

      if (!cell.isInnerRock()) {
        int distance = ceil(sqrt(pow(lightX - x, 2) + pow(lightY - y, 2)));
        bool visible = grid.isVisible(lightX, lightY, x, y);
        float light = 0.0;

        if (visible) {
          if (distance == 0)
            light = 1.0;
          else
            light = 1.0 / (distance / 2);
        }

        grid.setCellLightLevel(x, y, light);
      }
    }
  }

  emit gridReady(grid);
}
