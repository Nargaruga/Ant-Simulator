#ifndef POPULATION_SIM_H
#define POPULATION_SIM_H

#include "grid.h"
#include <QObject>

class PopulationSimulator : public QObject {
  Q_OBJECT

public:
  PopulationSimulator(int seed = 0);

public slots:

  /*
   *  Sets up the initial state of the population, randomly creating a random
   *  cluster of individuals
   */
  void initialize(Grid grid);

  /*
   *  Performs one step of the CA simulation on `m_grid`
   */
  void step(Grid grid);

signals:
  void popReady(Grid grid);

private:
  int m_seed; // Seed for the rng
};

#endif // POPULATION_SIM_H
