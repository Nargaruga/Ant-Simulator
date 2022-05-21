#include "grid.h"
#include <algorithm>
#include <cmath>

float lerp(int a, int b, float r) { return a * r + b * (1 - r); }

Grid::Grid(int rows, int cols) : m_rows(rows), m_cols(cols) {
  if (rows < 0 || cols < 0)
    throw std::invalid_argument(
        "The number of rows and columns cannot be negative.");

  for (int y = 0; y < rows; y++) {
    for (int x = 0; x < cols; x++) {
      m_cells.push_back(Cell(x, y));
    }
  }
}

void Grid::resize(int rows, int cols) {
  if (rows < 0 || cols < 0)
    throw std::invalid_argument(
        "The number of rows and columns cannot be negative.");

  m_cells.clear();

  m_rows = rows;
  m_cols = cols;

  for (int y = 0; y < rows; y++) {
    for (int x = 0; x < cols; x++) {
      m_cells.push_back(Cell(x, y));
    }
  }
}

void Grid::setCellState(int x, int y, Cell::State state) {
  if (x < 0 || y < 0 || x >= m_cols || y >= m_rows)
    throw std::invalid_argument("Coordinates are out of bounds.");

  m_cells[y * m_cols + x].setState(state);
}

void Grid::setCellLightLevel(int x, int y, float light) {
  if (x < 0 || y < 0 || x >= m_cols || y >= m_rows)
    throw std::invalid_argument("Coordinates are out of bounds.");

  if (light > 1.0)
    light = 1.0;
  else if (light < 0.0)
    light = 0.0;

  m_cells[y * m_cols + x].setLightLevel(light);
}

void Grid::spawnOrganism(int x, int y) {
  m_cells[y * m_cols + x].setState(Cell::State::ORGANISM);
}

void Grid::clearCell(int x, int y) {
  m_cells[y * m_cols + x].setState(Cell::State::FLOOR);
}

bool Grid::isVisible(int x0, int y0, int x1, int y1) const {
  int dist = std::max(abs(x0 - x1), abs(y0 - y1));

  for (int step = 0; step <= dist; step++) {
    float t = dist == 0 ? 0 : (float)step / dist;

    int x = round(lerp(x0, x1, t));
    int y = round(lerp(y0, y1, t));

    if (m_cells[y * m_cols + x].isRock())
      return false;
  }

  return true;
}

bool Grid::isBorder(int x, int y) const {
  if (x == 0 || x == m_cols - 1 || y == 0 || y == m_rows - 1)
    return true;
  else
    return false;
}

std::vector<Cell> Grid::getMooreNeighbourhood(int x, int y, int radius) const {
  if (x < 0 || y < 0 || x >= m_cols || y >= m_rows)
    throw std::invalid_argument("Out of bounds coordinates.");

  std::vector<Cell> neighbourhood;

  for (int i = x - radius; i <= x + radius; i++) {
    for (int j = y - radius; j <= y + radius; j++) {
      if ((i != x || j != y) && i >= 0 && i < m_cols && j >= 0 && j < m_rows)
        neighbourhood.push_back(getCell(i, j));
    }
  }

  return neighbourhood;
}

// TODO use radius
std::vector<Cell> Grid::getNeumannNeighbourhood(int x, int y,
                                                int radius) const {
  if (x < 0 || y < 0 || x >= m_cols || y >= m_rows)
    throw std::invalid_argument("Out of bounds coordinates.");

  std::vector<Cell> neighbourhood;

  for (int i = x - 1; i <= x + 1; i++) {
    if (i != x && i >= 0 && i < m_cols)
      neighbourhood.push_back(getCell(i, y));
  }

  for (int j = y - 1; j <= y + 1; j++) {
    if (j != y && j >= 0 && j < m_rows)
      neighbourhood.push_back(getCell(x, j));
  }

  return neighbourhood;
}
