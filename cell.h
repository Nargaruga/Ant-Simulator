#ifndef CELL_H
#define CELL_H

template <typename T> class Cell {
public:
  Cell(int x, int y) : m_x(x), m_y(y){};

  int getX() const { return m_x; }

  int getY() const { return m_y; }

  T getData() const { return m_data; }

  void setData(T data) { m_data = data; }

private:
  int m_x;
  int m_y;
  T m_data;
};

#endif // CELL_H
