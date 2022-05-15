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
  Grid(int rows, int cols);

  /*
   *  Sets the state of the cell situated at column `x` and row `y` to `state`
   */
  void setCellState(int x, int y, Cell::State state);

  /*
   *  Returns true if the cell at column `x` and row `y` is on the grid's
   *  border, false otherwise
   */
  bool isBorder(int x, int y);

  /*
   *  Returns a copy of the i-th cell
   */
  Cell getCell(int i);

  /*
   *  Returns a copy of the cell at column `x` and row `y`
   */
  Cell getCell(int x, int y);

  /*
   *  Returns a vector containing the Moore neighbourhood of the cell found
   *  at column `x` and row `y`
   */
  std::vector<Cell> getMooreNeighbourhood(int x, int y);

  /*
   *  Returns the size of the grid
   */
  int getSize();

  /*
   *  Returns the number of columns
   */
  int getCols();

  /*
   *  Returns the number of rows
   */
  int getRows();

private:
  int m_rows;                // number of rows
  int m_cols;                // number of columns
  std::vector<Cell> m_cells; // array of cells
};

#endif
