#ifndef CELL_H
#define CELL_H

#include "qcolor.h"
#include <random>

/*
 *  A square cell with a state and a (x, y) position in a grid representing a
 *  cavern
 */
class Cell {

public:
  /*
   *  Possible states of the cell
   */
  enum State { FLOOR, INNER_ROCK, OUTER_ROCK };

  /*
   *  Creates a square cell with default values
   */
  Cell();

  /*
   *  Creates a square cell with position (x, y)
   */
  Cell(int x, int y);

  /*
   *  Creates a square cell with position (x, y)
   *  and initial state `state`
   */
  Cell(int x, int y, State state);

  /*
   *  Set the state of the cell as `state`
   */
  void setState(State state);

  /*
   *  Returns the cell's color
   */
  QColor getColor() {
    switch (m_state) {
    case State::INNER_ROCK: {
      return Qt::black;
    }
    case State::OUTER_ROCK: {
      return Qt::red;
    }
    case State::FLOOR: {
      return Qt::white;
    }
    }
  }

  /*
   *  Returns the x position of the cell
   */
  int getX();

  /*
   *  Returns the y position of the cell
   */
  int getY();

  /*
   *  Returns true if the cell contains rock, false otherwise
   */
  bool isRock();

private:
  int m_x = 0;                  // x position
  int m_y = 0;                  // y position
  State m_state = State::FLOOR; // state of the cell
};

#endif
