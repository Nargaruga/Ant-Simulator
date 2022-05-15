#include "grid.h"

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

void Grid::setCellState(int x, int y, Cell::State state) {
  if (x < 0 || y < 0 || x >= m_cols || y >= m_rows)
    throw std::invalid_argument("Coordinates are out of bounds.");

  m_cells[y * m_cols + x].setState(state);
}

bool Grid::isBorder(int x, int y) {
  if (x == 0 || x == m_cols - 1 || y == 0 || y == m_rows - 1)
    return true;
  else
    return false;
}

Cell Grid::getCell(int i) {
  if (i < 0 || i >= m_cols * m_rows)
    throw std::invalid_argument("Out of bounds coordinates.");

  return m_cells[i];
}

Cell Grid::getCell(int x, int y) {
  if (x < 0 || y < 0 || x >= m_cols || y >= m_rows)
    throw std::invalid_argument("Coordinates are out of bounds.");

  return m_cells[y * m_cols + x];
}

std::vector<Cell> Grid::getMooreNeighbourhood(int x, int y) {
  if (x < 0 || y < 0 || x >= m_cols || y >= m_rows)
    throw std::invalid_argument("Out of bounds coordinates.");

  std::vector<Cell> neighbourhood;

  for (int i = x - 1; i <= x + 1; i++) {
    for (int j = y - 1; j <= y + 1; j++) {
      if ((i != x || j != y) && i >= 0 && i < m_cols && j >= 0 && j < m_rows)
        neighbourhood.push_back(getCell(i, j));
    }
  }

  return neighbourhood;
}

int Grid::getSize() { return m_rows * m_cols; }

int Grid::getCols() { return m_cols; }

int Grid::getRows() { return m_rows; }
