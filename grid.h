#ifndef GRID_H
#define GRID_H

#include "cell.h"
#include <memory>
#include <vector>

/*
 * A grid of cells with contents of type T.
 */
template <typename T> class Grid {

public:
  /*
   *  Creates a grid with the specified number of rows and columns.
   */
  Grid(int rows = 0, int cols = 0) : m_rows(rows), m_cols(cols) {
    if (rows < 0 || cols < 0)
      throw std::invalid_argument(
          "The number of rows and columns cannot be negative.");

    for (int y = 0; y < rows; y++) {
      for (int x = 0; x < cols; x++) {
        m_cells.push_back(Cell<T>(x, y));
      }
    }
  }

  /*
   *  Resizes the grid to be `rows` tall and `cols` wide.
   *  All contents are discarded.
   */
  void resize(int rows, int cols) {
    if (rows < 0 || cols < 0)
      throw std::invalid_argument(
          "The number of rows and columns cannot be negative.");

    m_rows = rows;
    m_cols = cols;

    std::vector<Cell<T>> newCells;

    for (int y = 0; y < rows; y++) {
      for (int x = 0; x < cols; x++) {
        newCells.push_back(Cell<T>(x, y));
      }
    }

    m_cells = newCells;
  }

  /*
   *  Computes the Manhattan distance between two points (x1, y1) and (x2, y2).
   */
  int manhattanDist(int x1, int y1, int x2, int y2) const {
    if (!areValid(x1, y1) || !areValid(x2, y2))
      throw std::invalid_argument("Out of bounds coordinates.");

    int dx = abs(x1 - x2);
    int dy = abs(y1 - y2);
    return dx + dy;
  }

  /*
   *  Sets the cell at column `x` and row `y` to `val`.
   */
  void setCell(int x, int y, T val) {
    if (!areValid(x, y))
      throw std::invalid_argument("Out of bounds coordinates.");

    m_cells[y * m_cols + x].setData(val);
  }

  /*
   *  Returns true if the cell at column `x` and row `y` is on the grid's
   *  border, false otherwise.
   */
  bool isBorder(int x, int y) const {
    if (x == 0 || x == m_cols - 1 || y == 0 || y == m_rows - 1)
      return true;
    else
      return false;
  }

  /*
   *  Returns the i-th cell.
   */
  Cell<T> getCell(int i) const {
    if (i < 0 || i >= m_cols * m_rows)
      throw std::invalid_argument("Out of bounds coordinates.");

    return m_cells[i];
  }

  /*
   *  Returns the cell at column `x` and row `y`.
   */
  Cell<T> getCell(int x, int y) const {
    if (!areValid(x, y))
      throw std::invalid_argument("Out of bounds coordinates.");

    return m_cells[y * m_cols + x];
  }

  /*
   * Returns true if (x,y) is a cell in the grid, false otherwise.
   */
  bool areValid(int x, int y) const {
    return x >= 0 && x < m_cols && y >= 0 && y < m_rows;
  }

  /*
   *  Returns a vector containing the Moore neighbourhood of the cell found
   *  at column `x` and row `y`. `radius` denotes the radius of the
   *  neighbourhood.
   */
  std::vector<Cell<T>> getMooreNeighbourhood(int x, int y,
                                             int radius = 1) const {
    if (!areValid(x, y))
      throw std::invalid_argument("Out of bounds coordinates.");

    std::vector<Cell<T>> neighbourhood;

    for (int i = x - radius; i <= x + radius; i++) {
      for (int j = y - radius; j <= y + radius; j++) {
        if ((i != x || j != y) && i >= 0 && i < m_cols && j >= 0 && j < m_rows)
          neighbourhood.push_back(getCell(i, j));
      }
    }

    return neighbourhood;
  }

  /*
   *  Returns a vector containing the Von Neumann neighbourhood of the cell
   *  found at column `x` and row `y`. `radius` denotes the radius of the
   *  neighbourhood
   */
  std::vector<Cell<T>> getNeumannNeighbourhood(int x, int y,
                                               int radius = 1) const {
    if (!areValid(x, y))
      throw std::invalid_argument("Out of bounds coordinates.");

    std::vector<Cell<T>> neighbourhood;

    for (int i = x - radius; i <= x + radius; i++) {
      for (int j = y - radius; j <= y + radius; j++) {
        if ((i != x || j != y) && i >= 0 && i < m_cols && j >= 0 &&
            j < m_rows && manhattanDist(x, y, i, j) <= radius)
          neighbourhood.push_back(getCell(i, j));
      }
    }

    return neighbourhood;
  }

  /*
   * Returns a vector containing three cells computed based on the current
   * position (x,y) and the direction `d`.
   */
  std::vector<Cell<T>> getDirectionalNeighbourhood(int x, int y,
                                                   std::pair<int, int> d) {
    if (!areValid(x, y))
      throw std::invalid_argument("Out of bounds coordinates.");

    std::vector<Cell<T>> neighbourhood = getMooreNeighbourhood(x, y);

    std::erase_if(neighbourhood, [&](Cell<T> cell) {
      return manhattanDist(cell.getX(), cell.getY(), x + d.first,
                           y + d.second) > 1;
    });

    return neighbourhood;
  }

  /*
   *  Returns the size of the grid.
   */
  int getSize() const { return m_rows * m_cols; }

  /*
   *  Returns the number of columns.
   */
  int getCols() const { return m_cols; }

  /*
   *  Returns the number of rows.
   */
  int getRows() const { return m_rows; }

private:
  int m_rows;                   // Number of rows
  int m_cols;                   // Number of columns
  std::vector<Cell<T>> m_cells; // Cells of the grid
};

#endif
