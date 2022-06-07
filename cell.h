#ifndef CELL_H
#define CELL_H

/*
 * A cell with discrete coordinates and some data of type T.
 */
template <typename T> class Cell {
public:
  /*
   * Constructs a cell with the specified coordinates.
   */
  Cell(int x, int y) : m_x(x), m_y(y){};

  /*
   * Returns the x coordinate of the cell.
   */
  int getX() const { return m_x; }

  /*
   * Returns the y coordinate of the cell.
   */
  int getY() const { return m_y; }

  /*
   * Returns the data associated with the cell.
   */
  T getData() const { return m_data; }

  /*
   * Sets the cell's data to `data`.
   */
  void setData(T data) { m_data = data; }

private:
  int m_x;  // x coordinate
  int m_y;  // y coordinate
  T m_data; // Data associated with the cell
};

#endif // CELL_H
