#include "population_sim.h"

PopulationSimulator::PopulationSimulator(int seed) : m_seed(seed) {}

void PopulationSimulator::initialize(Grid grid) {
  std::default_random_engine rng(m_seed);

  for (int x = 0; x < grid.getCols(); x++) {
    for (int y = 0; y < grid.getRows(); y++) {
      Cell cell = grid.getCell(x, y);

      // 0.01% chance of spawning a plant on a floor cell
      if (cell.isFloor() && rng() % 100 == 0) {
        grid.spawnOrganism(x, y);
      }
    }
  }

  emit popReady(grid);
}

void PopulationSimulator::step(Grid grid) {
  std::default_random_engine rng(m_seed);
  Grid updatedGrid(grid);

  for (int x = 0; x < grid.getCols(); x++) {
    for (int y = 0; y < grid.getRows(); y++) {
      Cell cell = grid.getCell(x, y);

      if (cell.isFloor() || cell.isOrganism()) {
        int nearbyOrganisms = 0;
        std::vector<Cell> neighbourhood = grid.getNeumannNeighbourhood(x, y);

        // Check if the cell has a neighbouring organism
        for (Cell neighbour : neighbourhood) {
          if (neighbour.isOrganism())
            nearbyOrganisms++;
        }

        if (cell.isFloor() && nearbyOrganisms > 0) {
          // Spawn an organism with chance based on the light level
          if (rng() % 100 < (cell.getLightLevel() * 100 * nearbyOrganisms))
            updatedGrid.spawnOrganism(cell.getX(), cell.getY());
        } else if (cell.isOrganism() && nearbyOrganisms < 4) {
          // If the light level is too low, the organism has a chance to die
          if (rng() % 100 > (cell.getLightLevel() * 100))
            updatedGrid.clearCell(cell.getX(), cell.getY());
        }
      }
    }
  }

  emit popReady(updatedGrid);
}
