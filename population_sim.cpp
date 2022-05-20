#include "population_sim.h"

PopulationSimulator::PopulationSimulator(Grid &grid, int seed)
    : m_grid(grid), m_seed(seed) {}

void PopulationSimulator::initialize() {
  std::default_random_engine rng(m_seed);
  Cell cell;
  do {
    cell = m_grid.getCell(rng() % m_grid.getSize());
  } while (cell.isRock());

  buildCluster(cell.getX(), cell.getY());
}

void PopulationSimulator::buildCluster(int x, int y) {
  if (x < 0 || y < 0 || x >= m_grid.getCols() || y >= m_grid.getRows())
    std::invalid_argument("Coordinates are out of bounds.");

  m_grid.setCellState(x, y, Cell::State::ORGANISM);

  std::vector<Cell> neighbourhood = m_grid.getNeumannNeighbourhood(x, y);
  for (Cell cell : neighbourhood) {
    if (!cell.isFloor()) {
      m_grid.setCellState(cell.getX(), cell.getY(), Cell::State::ORGANISM);
    }
  }
}

void PopulationSimulator::step() {
  std::default_random_engine rng(m_seed);
  Grid updatedGrid(m_grid);

  for (int x = 0; x < m_grid.getCols(); x++) {
    for (int y = 0; y < m_grid.getRows(); y++) {
      Cell cell = m_grid.getCell(x, y);

      if (cell.isFloor()) {
        bool hasOrganismNeighbour = false;
        std::vector<Cell> neighbourhood = m_grid.getNeumannNeighbourhood(x, y);

        // Check if the floor has a neighbouring organism
        for (Cell neighbour : neighbourhood) {
          if (neighbour.isOrganism()) {
            hasOrganismNeighbour = true;
            break;
          }
        }

        if (hasOrganismNeighbour) {
          // Spawn an organism with chance based on the light level
          if (rng() % 100 < (cell.getLightLevel() * 500))
            updatedGrid.spawnOrganism(cell.getX(), cell.getY());
        }

      } else if (cell.isOrganism()) {
        // If the light level is too low, the organism has a chance to die
        if (rng() % 100 > (cell.getLightLevel() * 100))
          updatedGrid.clearCell(cell.getX(), cell.getY());
      }
    }
  }

  m_grid = updatedGrid;
}
