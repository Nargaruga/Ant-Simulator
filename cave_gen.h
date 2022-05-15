#ifndef CAVEGEN_H
#define CAVEGEN_H

#include "grid.h"

class CaveGenerator {
public:
  /*
   *  Creates a cave generator with the following parameters:
   *  - width: number of columns (0 <= width)
   *  - height: number of rows (0 <= height)
   *  - seed: seed for the initial configuration
   *  - rockRatio: ratio of rocks in the initial configuration
   *    (0 <= rockRatio <= 100)
   *  - threshold: number of rock neighbours for a cell to turn into a rock
   *    (0 <= threshold <= 8)
   *  - steps: number of iterations (0 <= steps)
   */
  CaveGenerator(int width = 300, int height = 300, int seed = 0,
                int rockRatio = 50, int threshold = 5, int steps = 4);

  /*
   *  Generates a new cave based on the generator's parameters and stores it in
   *  `m_grid`
   */
  void generateCave();

  /*
   *  Creates a new grid with width `width` and height `height`
   */
  void resize(int width, int height);

  /*
   *  Returns a copy of the cave grid
   */
  Grid getGrid() { return m_grid; }

  /*
   *  Returns the seed parameter
   */
  int getSeed() { return m_seed; }

  /*
   * Returns a pair containing the minimum and maximum values for the seed
   * parameter
   */
  std::pair<int, int> getSeedRange() { return std::pair(INT_MIN, INT_MAX); }

  /*
   *  Returns the threshold parameter
   */
  int getThreshold() { return m_threshold; }

  /*
   * Returns a pair containing the minimum and maximum values for the threshold
   * parameter
   */
  std::pair<int, int> getThresholdRange() { return std::pair(0, 8); }

  /*
   *  Returns the rockRatio parameter
   */
  int getRockRatio() { return m_rockRatio; }

  /*
   * Returns a pair containing the minimum and maximum values for the rockRatio
   * parameter
   */
  std::pair<int, int> getRockRatioRange() { return std::pair(0, 100); }

  /*
   *  Returns the steps parameter
   */
  int getSteps() { return m_steps; }

  /*
   * Returns a pair containing the minimum and maximum values for the steps
   * parameter
   */
  std::pair<int, int> getStepsRange() { return std::pair(0, INT_MAX); }

  /*
   *  Sets the seed to `seed`
   */
  void setSeed(int seed) { m_seed = seed; }

  /*
   *  Sets the threshold parameter to `threshold`
   */
  void setThreshold(int threshold) { m_threshold = threshold; }

  /*
   *  Sets the rock ratio parameter to `rockRatio`
   */
  void setRockRatio(int rockRatio) { m_rockRatio = rockRatio; }

  /*
   *  Sets the steps parameter to `steps`
   */
  void setSteps(int steps) { m_steps = steps; }

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
