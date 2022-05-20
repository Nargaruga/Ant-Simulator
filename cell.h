#ifndef CELL_H
#define CELL_H

#include "colors.h"
#include <QColor>
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
  enum State { FLOOR, INNER_ROCK, OUTER_ROCK, ORGANISM };

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
  void setState(State state) { m_state = state; };

  /*
   *  Sets the light level to `light`
   */
  void setLightLevel(float light) { m_lightLevel = light; };

  /*
   *  Returns the cell's color
   */
  QColor getColor() const {
    switch (m_state) {
    case State::INNER_ROCK: {
      return innerRockColor;
    }
    case State::OUTER_ROCK: {
      return outerRockColor;
    }
    case State::ORGANISM: {
      return organismColor;
    }
    case State::FLOOR: {
      QColor color = blend(floorColor, Qt::yellow, m_lightLevel);
      return color;
    }

    default:
      return State::FLOOR;
    }
  }

  /*
   *  Returns the x position of the cell
   */
  int getX() const { return m_x; }

  /*
   *  Returns the y position of the cell
   */
  int getY() const { return m_y; }

  /*
   *  Returns the cell's light level
   */
  float getLightLevel() const { return m_lightLevel; }

  /*
   *  Returns true if the cell contains rock, false otherwise
   */
  bool isRock() const {
    return m_state == State::INNER_ROCK || m_state == State::OUTER_ROCK;
  }

  /*
   *  Returns true if the cell contains an organism, false otherwise
   */
  bool isOrganism() const { return m_state == State::ORGANISM; }

  /*
   *  Returns true if the cell is empty (just floor), false otherwise
   */
  bool isFloor() const { return m_state == State::FLOOR; }

private:
  int m_x = 0;                  // x position
  int m_y = 0;                  // y position
  State m_state = State::FLOOR; // state of the cell
  float m_lightLevel = 0.0;     // Light level of the cell
};

#endif
