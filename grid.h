#ifndef GRID_H
#define GRID_H

#include "cell.h"
#include <vector>

/*
 * A grid of square cells
 */
class Grid {

public:
  /*
   *  Creates a grid of `rows` rows, `cols` columns
   *  containing square cells
   */
  Grid(int rows = 0, int cols = 0);

  /*
   *  Sets the state of the cell situated at column `x` and row `y` to `state`
   */
  void setCellState(int x, int y, Cell::State state);

  /*
   *  Sets the light level of the cell situated at column `x` and row `y` to
   *  `light`
   */
  void setCellLightLevel(int x, int y, float light);

  /*
   *  Resizes the grid to have `rows` rows and `cols` columns
   */
  void resize(int rows, int cols);

  /*
   *  Spawns an organism at column `x` and row `y`
   */
  void spawnOrganism(int x, int y);

  /*
   *  Clears the cell at `x` and row `y`, turning it into a floor tile
   */
  void clearCell(int x, int y);

  /*
   *  Returns true if a line can be drawn from (x0, y0) to (x1, y1), false
   * otherwise
   */
  bool isVisible(int x0, int y0, int x1, int y1) const;

  /*
   *  Returns true if the cell at column `x` and row `y` is on the grid's
   *  border, false otherwise
   */
  bool isBorder(int x, int y) const;

  /*
   *  Returns a copy of the i-th cell
   */
  Cell getCell(int i) const {
    if (i < 0 || i >= m_cols * m_rows)
      throw std::invalid_argument("Out of bounds coordinates.");

    return m_cells[i];
  }

  /*
   *  Returns a copy of the cell at column `x` and row `y`
   */
  Cell getCell(int x, int y) const {
    if (x < 0 || y < 0 || x >= m_cols || y >= m_rows)
      throw std::invalid_argument("Coordinates are out of bounds.");

    return m_cells[y * m_cols + x];
  }

  /*
   *  Returns a vector containing the Moore neighbourhood of the cell found
   *  at column `x` and row `y`. `radius` denotes the radius of the
   *  neighbourhood
   */
  std::vector<Cell> getMooreNeighbourhood(int x, int y, int radius = 1) const;

  /*
   *  Returns a vector containing the Von Neumann neighbourhood of the cell
   *  found at column `x` and row `y`. `radius` denotes the radius of the
   *  neighbourhood
   */
  std::vector<Cell> getNeumannNeighbourhood(int x, int y, int radius = 1) const;

  /*
   *  Returns the size of the grid
   */
  int getSize() const { return m_rows * m_cols; }

  /*
   *  Returns the number of columns
   */
  int getCols() const { return m_cols; }

  /*
   *  Returns the number of rows
   */
  int getRows() const { return m_rows; }

private:
  int m_rows;                // Number of rows
  int m_cols;                // Number of columns
  std::vector<Cell> m_cells; // Cells of the grid
};

#endif
