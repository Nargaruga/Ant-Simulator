#ifndef CAVEGEN_H
#define CAVEGEN_H

#include "grid.h"

class CaveGenerator {
public:
  /*
   *  Creates a cave generator with the following parameters:
   *  - width: number of columns
   *  - height: number of rows
   *  - side: side of the cells
   *  - seed: seed for the initial configuration
   *  - rockRatio: ratio of rocks in the initial configuration
   *  - threshold: number of rock neighbours for a cell to turn into a rock
   *  - steps: number of iterations
   */
  CaveGenerator(int width = 300, int height = 300, int seed = 0,
                int rockRatio = 50, int threshold = 5, int steps = 4)
      : m_grid(width, height), m_width(width), m_height(height), m_seed(seed),
        m_rockRatio(rockRatio), m_threshold(threshold), m_steps(steps){};

  /*
   *  Generates a new cave based on the generator's parameters and stores it in
   *  `m_grid`
   */
  void generateCave();

  /*
   *  Returns a copy of the cave grid
   */
  Grid getGrid() { return m_grid; }

  /*
   *  Creates a new grid with width `width` and height `height`
   */
  void resize(int width, int height);

private:
  Grid m_grid;     // Grid representing the cave
  int m_width;     // Grid width
  int m_height;    // Grid heigth
  int m_seed;      // Seed for the initial configuration
  int m_rockRatio; // Amount of rocks in the initial configuration
  int m_threshold; // Rock threshold for the evolution rule
  int m_steps;     // Number of iteration steps

  /*
   *  Sets up the initial state of `m_grid`, randomly assigning a state to
   *  each cell based on the results of a random number generator seeded with
   *  `m_seed` and the ratio `m_rockRatio`, which decides the likeliness of a
   *  cell to be initialized as rock
   */
  void initialize();

  /*
   *  Performs one step of the CA simulation on `m_grid`
   */
  void step();

  /*
   *  Performs `m_steps` steps of the CA simulation on `m_grid`
   */
  void simulate();

  /*
   *  Highlights wall cells, that is rock cells which neighbour at least one
   *  floor cell
   */
  void highlightWalls();
};

#endif
