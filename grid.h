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
   *  Resizes the grid to have `rows` rows and `cols` columns
   */
  void resize(int rows, int cols);

  /*
   *  Sets the state of the cell situated at column `x` and row `y` to `state`
   */
  void setCellState(int x, int y, Cell::State state);

  /*
   *  Sets the cell situated at column `x` and row `y` as light source
   */
  void setLightSource(int x, int y);

  /*
   *  Spawns an organism at column `x` and row `y` as light source
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
   *  Returns a const reference to the i-th cell
   */
  const Cell &getCell(int i) const;

  /*
   *  Returns a copy of the cell at column `x` and row `y`
   */
  const Cell &getCell(int x, int y) const;

  /*
   *  Returns a vector containing the Moore neighbourhood of the cell found
   *  at column `x` and row `y`
   */
  std::vector<Cell> getMooreNeighbourhood(int x, int y) const;

  /*
   *  Returns a vector containing the Von Neumann neighbourhood of the cell
   *  found at column `x` and row `y`
   */
  std::vector<Cell> getNeumannNeighbourhood(int x, int y) const;

  /*
   *  Returns the size of the grid
   */
  int getSize() const;

  /*
   *  Returns the number of columns
   */
  int getCols() const;

  /*
   *  Returns the number of rows
   */
  int getRows() const;

private:
  int m_rows;                // Number of rows
  int m_cols;                // Number of columns
  std::vector<Cell> m_cells; // Cells of the grid
  Cell m_lightSource;        // Light source
};

#endif
