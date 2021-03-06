#include "ant_sim.h"
#include "sim_cell_data.h"

void AntSimulator::setup(Grid<SimCellData> grid) {
  reset();
  m_grid = grid;
}

void AntSimulator::initialize() {
  reset();

  // Find a place to spawn the nest
  do {
    m_nestX = m_rng() % m_grid.getCols();
    m_nestY = m_rng() % m_grid.getRows();
  } while (m_grid.getCell(m_nestX, m_nestY).getData().getType() ==
           SimCellData::Type::ROCK);

  SimCellData tmp = m_grid.getCell(m_nestX, m_nestY).getData();
  tmp.setType(SimCellData::Type::NEST);
  m_grid.setCell(m_nestX, m_nestY, tmp);

  emit gridReady(m_grid);
  emit initialized();
}

void AntSimulator::step() {
  // Spawn ants
  if (m_ants.size() < m_maxAnts) {
    m_ants.push_back(Ant(m_nestX, m_nestY,
                         std::pair<int, int>(m_rng() % 3 - 1, m_rng() % 3 - 1),
                         m_maxAntSteps));
  }

  // Kill excess ants
  while (m_ants.size() > m_maxAnts) {
    int victimIndex = m_rng() % m_ants.size();
    const Ant &toRemove = m_ants[victimIndex];
    SimCellData data =
        m_grid.getCell(toRemove.getX(), toRemove.getY()).getData();
    data.setType(SimCellData::Type::FLOOR);
    m_grid.setCell(toRemove.getX(), toRemove.getY(), data);
    m_ants.erase(m_ants.begin() + victimIndex);
  }

  // Update nest pheromone
  std::vector<Cell<SimCellData>> nestArea =
      m_grid.getNeumannNeighbourhood(m_nestX, m_nestY, 2);
  for (Cell<SimCellData> cell : nestArea) {
    SimCellData data = cell.getData();
    data.incrementHomePheromone(1.0f, 0, 0);
    m_grid.setCell(cell.getX(), cell.getY(), data);
  }

  // Simulate pheromone evaporation
  for (int x = 0; x < m_grid.getCols(); x++) {
    for (int y = 0; y < m_grid.getRows(); y++) {
      SimCellData tmp = m_grid.getCell(x, y).getData();
      tmp.decrementPheromones(m_phDecay);
      m_grid.setCell(x, y, tmp);
    }
  }

  // Move ants
  for (Ant &ant : m_ants) {
    std::vector<Cell<SimCellData>> neighbourhood =
        m_grid.getDirectionalNeighbourhood(ant.getX(), ant.getY(),
                                           ant.getDirection());

    // We don't consider occupied cells
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

    Cell<SimCellData> destination = ant.pickDestination(neighbourhood, m_rng);

    // Restore the previous cell
    SimCellData tmp = m_grid.getCell(ant.getX(), ant.getY()).getData();
    if (ant.getX() == m_nestX && ant.getY() == m_nestY)
      tmp.setType(SimCellData::Type::NEST);
    else {
      tmp.setType(SimCellData::Type::FLOOR);
    }
    m_grid.setCell(ant.getX(), ant.getY(), tmp);
    spreadPheromone(ant);

    // Update the ant
    ant.move(destination.getX(), destination.getY());
    if (!ant.hasFood() &&
        destination.getData().getType() == SimCellData::Type::FOOD) {
      ant.pickUpFood();
      ant.invert();
      ant.returnHome();
    } else if (destination.getData().getType() == SimCellData::Type::NEST) {
      if (ant.hasFood()) {
        m_deliveredFood++;
        emit updateFoodCount(m_deliveredFood, m_totalFood);
        ant.dropFood();
      }

      ant.invert();
      ant.seekFood();
    }

    // Update the grid
    tmp = m_grid.getCell(destination.getX(), destination.getY()).getData();
    tmp.setType(SimCellData::Type::ANT);
    m_grid.setCell(destination.getX(), destination.getY(), tmp);
  }

  emit gridReady(m_grid);
}

void AntSimulator::spreadPheromone(Ant ant) {
  std::vector<Cell<SimCellData>> neighbourhood =
      m_grid.getNeumannNeighbourhood(ant.getX(), ant.getY(), m_phSpread);
  neighbourhood.push_back(m_grid.getCell(ant.getX(), ant.getY()));

  for (Cell<SimCellData> cell : neighbourhood) {
    SimCellData data = cell.getData();
    // Pheromone strength decreases with distance from the source
    int distFromSource =
        m_grid.manhattanDist(cell.getX(), cell.getY(), ant.getX(), ant.getY());
    if (ant.getMode() == Ant::RETURN && ant.hasFood())
      data.incrementFoodPheromone(m_phStrength, distFromSource,
                                  ant.getTraveledDistance());
    else if (ant.getMode() == Ant::SEEK)
      data.incrementHomePheromone(m_phStrength, distFromSource,
                                  ant.getTraveledDistance());
    m_grid.setCell(cell.getX(), cell.getY(), data);
  }
}

void AntSimulator::onCellClicked(int x, int y) {
  if (x < 0 || x >= m_grid.getCols() || y < 0 || y >= m_grid.getRows())
    return;

  std::vector<Cell<SimCellData>> neighbourhood =
      m_grid.getNeumannNeighbourhood(x, y, 2);
  neighbourhood.push_back(m_grid.getCell(x, y));

  // Place food
  for (Cell<SimCellData> cell : neighbourhood) {
    SimCellData data = cell.getData();
    if (data.getType() == SimCellData::FLOOR) {
      data.setType(SimCellData::Type::FOOD);
      m_grid.setCell(cell.getX(), cell.getY(), data);

      m_totalFood++;
      emit updateFoodCount(m_deliveredFood, m_totalFood);
    }
  }

  emit gridReady(m_grid);
}

void AntSimulator::setMaxAntSteps(int m) {
  m_maxAntSteps = m;
  for (Ant &ant : m_ants) {
    ant.setMaxSteps(m);
  }
}

void AntSimulator::reset() {
  m_nestX = -1;
  m_nestY = -1;
  m_ants.clear();

  // Clears pheromones
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

void AntSimulator::resetParams() {
  m_maxAnts = 20;
  m_phStrength = 1.0f;
  m_phSpread = 2;
  m_phDecay = 0.01f;
}
