#ifndef CAVEGEN_H
#define CAVEGEN_H

#include "grid.h"
#include <QObject>
#include <mutex>

class CaveGenerator : public QObject {
  Q_OBJECT

public:
  /*
   *  Creates a cave generator with the following parameters:
   *  - seed: seed for the initial configuration
   *  - rockRatio: ratio of rocks in the initial configuration
   *    (0 <= rockRatio <= 100)
   *  - threshold: number of rock neighbours for a cell to turn into a rock
   *    (0 <= threshold <= 8)
   *  - steps: number of iterations (0 <= steps)
   */
  CaveGenerator(int seed = 0, int rockRatio = 50, int threshold = 5,
                int steps = 4);

  /*
   *  Returns the seed parameter
   */
  int getSeed() const { return m_seed; }

  /*
   * Returns a pair containing the minimum and maximum values for the seed
   * parameter
   */
  std::pair<int, int> getSeedRange() const {
    return std::pair(INT_MIN, INT_MAX);
  }

  /*
   *  Returns the threshold parameter
   */
  int getThreshold() const { return m_threshold; }

  /*
   * Returns a pair containing the minimum and maximum values for the threshold
   * parameter
   */
  std::pair<int, int> getThresholdRange() const { return std::pair(0, 8); }

  /*
   *  Returns the rockRatio parameter
   */
  int getRockRatio() const { return m_rockRatio; }

  /*
   * Returns a pair containing the minimum and maximum values for the rockRatio
   * parameter
   */
  std::pair<int, int> getRockRatioRange() const { return std::pair(0, 100); }

  /*
   *  Returns the steps parameter
   */
  int getSteps() const { return m_steps; }

  /*
   * Returns a pair containing the minimum and maximum values for the steps
   * parameter
   */
  std::pair<int, int> getStepsRange() const { return std::pair(0, INT_MAX); }

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

public slots:
  /*
   *  Generates a new cave based on the generator's parameters.
   *  Once the cave is ready, it is broadcasted through a `caveReady` signal
   */
  void generateCave(int rows, int cols);

  /*
   *  Spawns a light source at column `x` and row `y` in `grid`
   */
  void spawnLightSource(Grid grid, int x, int y);

signals:
  void gridReady(Grid grid);

private:
  int m_seed;      // Seed for the initial configuration
  int m_rockRatio; // Amount of rocks in the initial configuration
  int m_threshold; // Rock threshold for the evolution rule
  int m_steps;     // Number of iteration steps

  /*
   *  Sets up the initial state of `grid`, randomly assigning a state to
   *  each cell based on the results of a random number generator seeded with
   *  `m_seed` and the ratio `m_rockRatio`, which decides the likeliness of a
   *  cell to be initialized as rock
   */
  void initialize(Grid &grid);

  /*
   *  Performs one step of the CA simulation on `grid`
   */
  void step(Grid &grid);

  /*
   *  Performs `m_steps` steps of the CA simulation on `m_grid`
   */
  void simulate(Grid &grid);

  /*
   *  Highlights wall cells, that is rock cells which neighbour at least one
   *  floor cell
   */
  void highlightWalls(Grid &grid);
};

#endif
