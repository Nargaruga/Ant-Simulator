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

public slots:

  /*
   *  Build the simulation environment from the cave
   */
  void setupEnvironment(Grid<bool> cave);

  /*
   *  Sets up the initial state of the population
   */
  void initialize();

  /*
   *  Performs one step of the simulation
   */
  void step();

signals:
  void gridReady(Grid<SimCellData> grid);

private:
  Grid<SimCellData> m_grid;
  int m_seed; // Seed for the rng
  std::default_random_engine m_rng;
  std::vector<Ant> m_ants;
};

#endif // ANT_SIM_H
