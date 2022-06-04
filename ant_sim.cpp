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
  reset();

  do {
    m_nestX = m_rng() % m_grid.getCols();
    m_nestY = m_rng() % m_grid.getRows();
  } while (m_grid.getCell(m_nestX, m_nestY).getData().getType() ==
           SimCellData::Type::ROCK);

  SimCellData tmp = m_grid.getCell(m_nestX, m_nestY).getData();
  tmp.setType(SimCellData::Type::NEST);
  m_grid.setCell(m_nestX, m_nestY, tmp);

  emit gridReady(m_grid);
}

void AntSimulator::step() {
  // Spawn an ant if necessary
  if (m_ants.size() < m_maxAnts) {
    m_ants.push_back(
        Ant(m_nestX, m_nestY, static_cast<Direction>(m_rng() % 4)));
  }

  // Simulate pheromone evaporation
  for (int x = 0; x < m_grid.getCols(); x++) {
    for (int y = 0; y < m_grid.getRows(); y++) {
      SimCellData tmp = m_grid.getCell(x, y).getData();
      tmp.decrementFoodPheromone();
      tmp.decrementHomePheromone();
      m_grid.setCell(x, y, tmp);
    }
  }

  // Move ants
  for (Ant &ant : m_ants) {
    std::vector<Cell<SimCellData>> neighbourhood =
        m_grid.getCardinalNeighbourhood(ant.getX(), ant.getY(),
                                        ant.getDirection());

    // We don't consider occupied cells, the cell we just visited and, if the
    // ant is carrying food, food cells
    std::erase_if(neighbourhood, [&](Cell<SimCellData> cell) {
      return cell.getData().getType() == SimCellData::Type::ROCK ||
             cell.getData().getType() == SimCellData::Type::ANT ||
             (ant.hasFood() &&
              cell.getData().getType() == SimCellData::Type::FOOD);
    });

    // No suitable neighbouring cells to move to
    if (neighbourhood.empty()) {
      ant.invert();
      continue;
    }

    // Choose the most appealing cell to move to
    Cell<SimCellData> destination =
        neighbourhood[m_rng() % neighbourhood.size()];
    for (Cell<SimCellData> neighbour : neighbourhood) {

      // If the ant is carrying food, it will want to head home, otherwise it
      // will look for food
      if (ant.hasFood()) {
        if (neighbour.getData().getType() == SimCellData::Type::NEST ||
            neighbour.getData().getHomePheromone() >
                destination.getData().getHomePheromone())
          destination = neighbour;
      } else {
        if (neighbour.getData().getType() == SimCellData::Type::FOOD ||
            neighbour.getData().getFoodPheromone() >
                destination.getData().getFoodPheromone())
          destination = neighbour;
      }
    }

    // Drop/pick up food
    if (destination.getData().getType() == SimCellData::Type::FOOD) {
      ant.pickUpFood();
      ant.invert();
    } else if (destination.getData().getType() == SimCellData::Type::NEST) {
      ant.dropFood();
      ant.invert();
    }

    // Restore the previous cell
    SimCellData tmp = m_grid.getCell(ant.getX(), ant.getY()).getData();
    if (ant.getX() == m_nestX && ant.getY() == m_nestY)
      tmp.setType(SimCellData::Type::NEST);
    else {
      tmp.setType(SimCellData::Type::FLOOR);
      // Leave pheromone
      if (ant.hasFood()) {
        tmp.incrementFoodPheromone();
      } else {
        tmp.incrementHomePheromone();
      }
    }
    m_grid.setCell(ant.getX(), ant.getY(), tmp);

    tmp = m_grid.getCell(destination.getX(), destination.getY()).getData();
    tmp.setType(SimCellData::Type::ANT);
    m_grid.setCell(destination.getX(), destination.getY(), tmp);

    ant.setCoords(destination.getX(), destination.getY());
  }

  emit gridReady(m_grid);
}

void AntSimulator::onCellClicked(int x, int y) {
  if (x < 0 || x >= m_grid.getCols() || y < 0 || y >= m_grid.getRows())
    return;

  std::vector<Cell<SimCellData>> neighbourhood =
      m_grid.getNeumannNeighbourhood(x, y, 2);
  neighbourhood.push_back(m_grid.getCell(x, y));

  for (Cell<SimCellData> cell : neighbourhood) {
    SimCellData data = cell.getData();
    if (data.getType() == SimCellData::FLOOR) {
      data.setType(SimCellData::Type::FOOD);
      m_grid.setCell(cell.getX(), cell.getY(), data);
    }
  }

  emit gridReady(m_grid);
}

void AntSimulator::reset() {
  m_nestX = -1;
  m_nestY = -1;
  m_ants.clear();

  // Simulate pheromone evaporation
  for (int x = 0; x < m_grid.getCols(); x++) {
    for (int y = 0; y < m_grid.getRows(); y++) {
      SimCellData tmp = m_grid.getCell(x, y).getData();
      if (tmp.getType() == SimCellData::Type::ANT ||
          tmp.getType() == SimCellData::Type::NEST ||
          tmp.getType() == SimCellData::Type::FOOD)
        tmp.setType(SimCellData::Type::FLOOR);

      tmp.clearPheromones();
      m_grid.setCell(x, y, tmp);
    }
  }
}
