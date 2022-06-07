#ifndef CAVEGEN_H
#define CAVEGEN_H

#include "grid.h"
#include "sim_cell_data.h"
#include <QObject>
#include <mutex>

/*
 * Generates 2D caves trough the use of cellular automata.
 */
class CaveGenerator : public QObject {
  Q_OBJECT

public:
  /*
   * Neighbourhood types.
   */
  enum Mode { MOORE, NEUMANN };

  /*
   *  Creates a cave generator with the following parameters:
   *  - seed: seed for the initial configuration;
   *  - rockRatio: ratio of rocks in the initial configuration;
   *  - threshold: number of rock neighbours for a cell to turn into a rock;
   *  - steps: number of iterations;
   *  - radius: radius of the neighbourhood to consider.
   */
  CaveGenerator(int seed = 0, int rockRatio = 60, int threshold = 5,
                int steps = 8, int radius = 1);

  /*
   *  Returns the seed parameter.
   */
  int getSeed() const { return m_seed; }

  /*
   * Returns a pair containing the minimum and maximum values for the seed
   * parameter.
   */
  std::pair<int, int> getSeedRange() const { return {INT_MIN, INT_MAX}; }

  /*
   *  Returns the threshold parameter.
   */
  int getThreshold() const { return m_threshold; }

  /*
   * Returns a pair containing the minimum and maximum values for the threshold
   * parameter.
   */
  std::pair<int, int> getThresholdRange() const { return {0, INT_MAX}; }

  /*
   *  Returns the rockRatio parameter.
   */
  int getRockRatio() const { return m_rockRatio; }

  /*
   * Returns a pair containing the minimum and maximum values for the rockRatio
   * parameter.
   */
  std::pair<int, int> getRockRatioRange() const { return {0, 100}; }

  /*
   *  Returns the steps parameter.
   */
  int getSteps() const { return m_steps; }

  /*
   * Returns a pair containing the minimum and maximum values for the steps
   * parameter.
   */
  std::pair<int, int> getStepsRange() const { return {0, INT_MAX}; }

  /*
   * Returns the radius parameter.
   */
  int getRadius() const { return m_radius; }

  /*
   * Returns a pair containing the minimum and maximum values for the radius
   * parameter.
   */
  std::pair<int, int> getRadiusRange() const { return std::pair{0, INT_MAX}; }

  /*
   *  Sets the seed to `seed`.
   */
  void setSeed(int seed) { m_seed = seed; }

  /*
   *  Sets the threshold parameter to `threshold`.
   */
  void setThreshold(int threshold) { m_threshold = threshold; }

  /*
   *  Sets the rock ratio parameter to `rockRatio`.
   */
  void setRockRatio(int rockRatio) { m_rockRatio = rockRatio; }

  /*
   *  Sets the steps parameter to `steps`.
   */
  void setSteps(int steps) { m_steps = steps; }

  /*
   * Sets the radius parameter to `radius`.
   */
  void setRadius(int radius) { m_radius = radius; }

  /*
   * Resets all parameters to their default values.
   */
  void resetParams();

public slots:
  /*
   *  Generates a new cave based on the generator's parameters.
   *  Once the cave is ready, it is broadcasted through a `caveReady` signal
   */
  void generateCave(int rows, int cols);

  /*
   * Sets the neighbourhood type to MOORE.
   */
  void setMooreMode(bool checked) {
    if (checked)
      m_mode = MOORE;
  }

  /*
   * Sets the neighbourhood type to NEUMANN.
   */
  void setNeumannMode(bool checked) {
    if (checked)
      m_mode = NEUMANN;
  }

signals:
  void gridReady(Grid<SimCellData> grid);

private:
  int m_seed;          // Seed for the initial configuration
  int m_rockRatio;     // Amount of rocks in the initial configuration
  int m_threshold;     // Rock threshold for the evolution rule
  int m_steps;         // Number of iteration steps
  Mode m_mode = MOORE; // Neighbourhood mode
  int m_radius;        // Neighbourhood radius

  /*
   *  Sets up the initial state of `grid`, randomly assigning a state to
   *  each cell based on the results of a random number generator seeded with
   *  `m_seed` and the ratio `m_rockRatio`, which decides the likeliness of a
   *  cell to be initialized as rock
   */
  void initialize(Grid<SimCellData> &grid);

  /*
   *  Performs one step of the CA simulation on `grid`
   */
  void step(Grid<SimCellData> &grid);

  /*
   *  Performs `m_steps` steps of the CA simulation on `m_grid`
   */
  void simulate(Grid<SimCellData> &grid);
};

#endif
