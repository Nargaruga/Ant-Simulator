#ifndef ANT_SIM_H
#define ANT_SIM_H

#include "ant.h"
#include "grid.h"
#include "sim_cell_data.h"
#include <QObject>

class AntSimulator : public QObject {
  Q_OBJECT

public:
  AntSimulator(int seed = 0) : m_seed(seed), m_rng(seed){};

  void setup(Grid<SimCellData> grid);

  int getTotalFood() const { return m_totalFood; }

  int getDeliveredFood() const { return m_deliveredFood; }

  int getMaxAnts() { return m_maxAnts; }

  std::pair<int, int> getMaxAntsRange() { return {0, 100}; }

  int getPhStrength() { return m_phStrength * 100; }

  std::pair<int, int> getPhStrengthRange() { return {0, 100}; }

  int getPhSpread() { return m_phSpread; }

  std::pair<int, int> getPhSpreadRange() { return {0, 5}; }

  int getPhDecay() { return m_phDecay * 100; }

  std::pair<int, int> getPhDecayRange() { return {0, 100}; }

  void setPhStrength(int v) {
    if (v < 0 || v > 100)
      return;

    m_phStrength = (float)v / 100;
  }

  void setPhSpread(int v) {
    if (v < 0 || v > 5)
      return;

    m_phSpread = v;
  }

  void setPhDecay(int v) {
    if (v < 0 || v > 100)
      return;

    m_phDecay = (float)v / 100;
  }

public slots:

  /*
   *  Sets up the initial state of the population
   */
  void initialize();

  /*
   *  Performs one step of the simulation
   */
  void step();

  void spreadPheromone(Ant ant);

  void onCellClicked(int x, int y);

  void reset();

  void setMaxAnts(int n) { m_maxAnts = n; }

signals:
  void gridReady(Grid<SimCellData> grid);

  void initialized();

  void updateFoodCount(int delivered, int total);

private:
  Grid<SimCellData> m_grid;
  int m_maxAnts = 20;
  int m_deliveredFood = 0;
  int m_totalFood = 0;
  int m_nestX;
  int m_nestY;
  float m_phDecay = 0.01f;
  float m_phStrength = 1.0f;
  int m_phSpread = 2;
  int m_seed; // Seed for the rng
  std::default_random_engine m_rng;
  std::vector<Ant> m_ants;
};

#endif // ANT_SIM_H
