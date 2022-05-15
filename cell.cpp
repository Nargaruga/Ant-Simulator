#include "cell.h"

Cell::Cell() {}
Cell::Cell(int x, int y) : m_x(x), m_y(y) {}
Cell::Cell(int x, int y, State state) : m_x(x), m_y(y), m_state(state) {}
