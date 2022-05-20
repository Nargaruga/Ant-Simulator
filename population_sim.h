#ifndef POPULATION_SIM_H
#define POPULATION_SIM_H

#include "grid.h"

class PopulationSimulator {
public:
  PopulationSimulator(Grid &grid, int seed = 0);

  /*
   *  Returns a copy of the grid TODO
   */
  Grid getGrid() { return m_grid; }

  /*
   *  Sets up the initial state of the population, randomly creating a random
   *  cluster of individuals
   */
  void initialize();

  /*
   *  Performs one step of the CA simulation on `m_grid`
   */
  void step();

private:
  Grid &m_grid; // Grid representing the environment
  int m_seed;   // Seed for the rng

  /*
   *  Builds a cluster centered at column `x` and row `y`
   */
  void buildCluster(int x, int y);
};

#endif // POPULATION_SIM_H
