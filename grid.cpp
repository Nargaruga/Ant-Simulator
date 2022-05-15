#include "grid.h"

Grid::Grid(int rows, int cols) : m_rows(rows), m_cols(cols) {
  for (size_t y = 0; y < rows; y++) {
    for (size_t x = 0; x < cols; x++) {
      m_cells.push_back(Cell(x, y));
    }
  }
}

void Grid::setCellState(int x, int y, Cell::State state) {
  m_cells[y * m_cols + x].setState(state);
}

bool Grid::isBorder(int x, int y) {
  if (x == 0 || x == m_cols - 1 || y == 0 || y == m_rows - 1)
    return true;
  else
    return false;
}

Cell Grid::getCell(int i) { return m_cells[i]; }

Cell Grid::getCell(int x, int y) {
  // TODO
  // if (0 <= x < m_cols && 0 <= y < m_rows)...

  return m_cells[y * m_cols + x];
}

std::vector<Cell> Grid::getMooreNeighbourhood(int x, int y) {
  std::vector<Cell> neighbourhood;

  // TODO
  // if (0 <= x < m_cols && 0 <= y < m_rows)...

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