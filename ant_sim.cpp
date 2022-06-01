#include "ant_sim.h"
#include "sim_cell_data.h"

AntSimulator::AntSimulator(int seed) : m_seed(seed), m_rng(seed) {}

void AntSimulator::setupEnvironment(Grid<bool> cave) {
  m_grid.resize(cave.getRows(), cave.getCols());

  for (int x = 0; x < cave.getCols(); x++) {
    for (int y = 0; y < cave.getRows(); y++) {
      if (cave.getCell(x, y).getData())
        m_grid.setCell(x, y, SimCellData(SimCellData::Type::ROCK));
      else
        m_grid.setCell(x, y, SimCellData(SimCellData::Type::FLOOR));
    }
  }
}

void AntSimulator::initialize() {
  int x = -1, y = -1;

  // TODO: limit iterations or change initialization method
  do {
    x = m_rng() % m_grid.getCols();
    y = m_rng() % m_grid.getRows();
  } while (m_grid.getCell(x, y).getData().getType() == SimCellData::Type::ROCK);

  if (x > 0 && y > 0)
    m_ants.push_back(Ant(x, y));

  SimCellData tmp = m_grid.getCell(x, y).getData();
  tmp.setType(SimCellData::Type::ANT);
  m_grid.setCell(x, y, tmp);

  emit gridReady(m_grid);
}

void AntSimulator::step() {

  // TODO: ants are always processed in the same order, maybe this should change
  for (Ant &ant : m_ants) {
    std::vector<Cell<SimCellData>> neighbourhood =
        m_grid.getNeumannNeighbourhood(ant.getX(), ant.getY());

    // We don't consider occupied cells
    std::erase_if(neighbourhood, [&](Cell<SimCellData> cell) {
      return cell.getData().getType() == SimCellData::Type::ROCK ||
             cell.getData().getType() == SimCellData::Type::ANT;
    });

    // No suitable neighbouring cells to move to
    if (neighbourhood.empty())
      continue;

    // Choose the most appealing cell to move to
    Cell<SimCellData> destination =
        neighbourhood[m_rng() % neighbourhood.size()];
    for (Cell<SimCellData> neighbour : neighbourhood) {

      // If the ant is carrying food, it will want to head home, otherwise it
      // will look for food
      if (ant.hasFood()) {
        // TODO: should check if neighbour is nest
        if (neighbour.getData().getHomePheromone() >
            destination.getData().getHomePheromone())
          destination = neighbour;
      } else {
        if (neighbour.getData().getType() == SimCellData::Type::FOOD ||
            neighbour.getData().getFoodPheromone() >
                destination.getData().getFoodPheromone())
          destination = neighbour;
      }
    }

    if (destination.getData().getType() == SimCellData::Type::FOOD)
      ant.pickUpFood();

    // TODO should devise a better way to change a cell's type...
    SimCellData tmp = m_grid.getCell(ant.getX(), ant.getY()).getData();
    tmp.setType(SimCellData::Type::FLOOR);
    m_grid.setCell(ant.getX(), ant.getY(), tmp);

    tmp = m_grid.getCell(destination.getX(), destination.getY()).getData();
    tmp.setType(SimCellData::Type::ANT);
    m_grid.setCell(destination.getX(), destination.getY(), tmp);

    ant.setCoords(destination.getX(), destination.getY());
  }

  emit gridReady(m_grid);
}
