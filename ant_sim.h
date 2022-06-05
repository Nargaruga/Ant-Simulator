#ifndef ANT_SIM_H
#define ANT_SIM_H

#include "ant.h"
#include "grid.h"
#include "sim_cell_data.h"
#include <QObject>

class AntSimulator : public QObject {
  Q_OBJECT

public:
  AntSimulator(int seed = 0);

  int getTotalFood() const { return m_totalFood; }

  int getDeliveredFood() const { return m_deliveredFood; }

public slots:

  void setup(Grid<SimCellData> grid) {
    reset();
    m_grid = grid;
  }

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

signals:
  void gridReady(Grid<SimCellData> grid);

  void updateFoodCount(int delivered, int total);

private:
  Grid<SimCellData> m_grid;
  int m_maxAnts = 50;
  int m_deliveredFood = 0;
  int m_totalFood = 0;
  int m_nestX;
  int m_nestY;
  int m_seed; // Seed for the rng
  std::default_random_engine m_rng;
  std::vector<Ant> m_ants;
};

#endif // ANT_SIM_H
