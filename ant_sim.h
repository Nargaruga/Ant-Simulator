#ifndef ANT_SIM_H
#define ANT_SIM_H

#include "ant.h"
#include "grid.h"
#include "sim_cell_data.h"
#include <QObject>

/*
 * Simulates the behaviour of a colony of ants foraging for food.
 */
class AntSimulator : public QObject {
  Q_OBJECT

public:
  /*
   * Construct the simulator with the specified seed for random number
   * generation.
   */
  AntSimulator(int seed = 0) : m_seed(seed), m_rng(seed){};

  /*
   * Set the simulation grid.
   */
  void setup(Grid<SimCellData> grid);

  /*
   * Returns the total amount of food in the environment.
   */
  int getTotalFood() const { return m_totalFood; }

  /*
   * Returns the amount of food that was brought back to the nest.
   */
  int getDeliveredFood() const { return m_deliveredFood; }

  /*
   * Returns the number of steps each ant can perform before aborting the
   * search.
   */
  int getMaxAntSteps() { return m_maxAntSteps; }

  /*
   * Returns the valid range of values for the number of maximum ant steps.
   */
  std::pair<int, int> getMaxAntStepsRange() { return {0, 300}; }

  /*
   * Returns the number of ants to be simulated.
   */
  int getMaxAnts() { return m_maxAnts; }

  /*
   * Returns the valid range of values for the number of ants.
   */
  std::pair<int, int> getMaxAntsRange() { return {0, 100}; }

  /*
   * Returns the pheromone strength parameter.
   */
  int getPhStrength() { return m_phStrength * 100; }

  /*
   * Returns the valid range of values for the number of ants.
   */
  std::pair<int, int> getPhStrengthRange() { return {0, 100}; }

  /*
   * Returns the pheromone spread parameter.
   */
  int getPhSpread() { return m_phSpread; }

  /*
   * Returns the valid range of values for the pheromone spread parameter.
   */
  std::pair<int, int> getPhSpreadRange() { return {0, 5}; }

  /*
   * Returns the pheromone decay parameter.
   */
  int getPhDecay() { return m_phDecay * 100; }

  /*
   * Returns the valid range of values for the pheromone decay parameter.
   */
  std::pair<int, int> getPhDecayRange() { return {0, 100}; }

  /*
   * Sets the pheromone strength to v/100.
   */
  void setPhStrength(int v) {
    if (v < 0 || v > 100)
      return;

    m_phStrength = (float)v / 100;
  }

  /*
   * Sets the pheromone strength to `v`.
   */
  void setPhSpread(int v) {
    if (v < 0 || v > 5)
      return;

    m_phSpread = v;
  }

  /*
   * Sets the pheromone decay rate to v/100.
   */
  void setPhDecay(int v) {
    if (v < 0 || v > 100)
      return;

    m_phDecay = (float)v / 100;
  }

  /*
   * Sets the number of ants to simulate.
   */
  void setMaxAnts(int n) { m_maxAnts = n; }

  /*
   * Sets each ants' maximum number of search steps.
   */
  void setMaxAntSteps(int n);

  /*
   * Resets all parameters to their default value.
   */
  void resetParams();

public slots:

  /*
   *  Sets up the initial state of the population.
   */
  void initialize();

  /*
   *  Performs one step of the simulation.
   */
  void step();

  /*
   * Spreads pheromone from the specified ant.
   */
  void spreadPheromone(Ant ant);

  /*
   * Places food on and around the clicked cell.
   */
  void onCellClicked(int x, int y);

  /*
   * Resets the simulation-
   */
  void reset();

signals:
  /*
   * Emitted when the simulation step is completed.
   */
  void gridReady(Grid<SimCellData> grid);

  /*
   * Emitted when initialization is completed.
   */
  void initialized();

  /*
   * Emitted when the food counters are updated.
   */
  void updateFoodCount(int delivered, int total);

private:
  Grid<SimCellData> m_grid;  // Grid of the simulation
  size_t m_maxAnts = 20;     // Number of ants to simulate
  int m_maxAntSteps = 150;   // Number of steps before aborting the search
  int m_deliveredFood = 0;   // Amount of food delivered to the nest
  int m_totalFood = 0;       // Total food available in the environment
  int m_nestX;               // x coordinate of the nest
  int m_nestY;               // y coordinate of the nest
  float m_phDecay = 0.01f;   // Pheromone decay rate
  float m_phStrength = 1.0f; // Pheromone starting strength
  int m_phSpread = 2;        // Pheromone spread radius
  int m_seed;                // Seed for the rng
  std::default_random_engine m_rng; // Random number generator
  std::vector<Ant> m_ants;          // Vector to keep track of the ants
};

#endif // ANT_SIM_H
