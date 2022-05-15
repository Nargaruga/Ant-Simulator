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

CaveGenerator::CaveGenerator(int width, int height, int seed, int rockRatio,
                             int threshold, int steps)
    : m_grid(width, height), m_width(width), m_height(height), m_seed(seed),
      m_rockRatio(rockRatio), m_threshold(threshold), m_steps(steps) {

  if (width < 0 || height < 0 || rockRatio < 0 || rockRatio > 100 ||
      threshold < 0 || threshold > 8 || steps < 0)
    std::invalid_argument("Arguments do not fall in the required ranges.");
}

void CaveGenerator::generateCave() {
  initialize();
  simulate();
}

void CaveGenerator::resize(int width, int height) {
  if (width < 0 || height < 0)
    throw std::invalid_argument("Width and height must be greater than 0.");

  m_width = width;
  m_height = height;
  m_grid = Grid(width, height);
}

void CaveGenerator::initialize() {
  std::default_random_engine rng(m_seed);

  for (int x = 0; x < m_grid.getCols(); x++) {
    for (int y = 0; y < m_grid.getRows(); y++) {
      if (m_grid.isBorder(x, y) ||
          (static_cast<int>(rng() % 100) < m_rockRatio))
        m_grid.setCellState(x, y, Cell::State::INNER_ROCK);
      else
        m_grid.setCellState(x, y, Cell::State::FLOOR);
    }
  }
}

void CaveGenerator::step() {
  Grid updatedGrid(m_grid);

  for (int x = 0; x < m_grid.getCols(); x++) {
    for (int y = 0; y < m_grid.getRows(); y++) {
      std::vector<Cell> neighbourhood = m_grid.getMooreNeighbourhood(x, y);

      int neighbours = countNearbyRocks(neighbourhood);

      if (neighbours >= m_threshold) {
        updatedGrid.setCellState(x, y, Cell::State::INNER_ROCK);
      } else {
        updatedGrid.setCellState(x, y, Cell::State::FLOOR);
      }
    }
  }

  m_grid = updatedGrid;
}

void CaveGenerator::simulate() {
  for (int i = 0; i < m_steps; i++) {
    step();
  }
}

void CaveGenerator::highlightWalls() {
  Grid updatedGrid(m_grid);

  for (int x = 0; x < m_grid.getCols(); x++) {
    for (int y = 0; y < m_grid.getRows(); y++) {
      std::vector<Cell> neighbourhood = m_grid.getMooreNeighbourhood(x, y);

      int neighbours = countNearbyRocks(neighbourhood);

      if (m_grid.getCell(x, y).isRock() && neighbours < 8) {
        updatedGrid.setCellState(x, y, Cell::State::OUTER_ROCK);
      }
    }
  }

  m_grid = updatedGrid;
}
