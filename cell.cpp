#include "cell.h"

Cell::Cell() {}
Cell::Cell(int x, int y) : m_x(x), m_y(y) {}
Cell::Cell(int x, int y, State state) : m_x(x), m_y(y), m_state(state) {}

void Cell::setState(State state) { m_state = state; }

int Cell::getX() { return m_x; }

int Cell::getY() { return m_y; }

bool Cell::isRock() {
  return m_state == State::INNER_ROCK || m_state == State::OUTER_ROCK;
}
