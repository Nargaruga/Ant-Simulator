#ifndef ANT_H
#define ANT_H

#include "cell.h"
#include "sim_cell_data.h"

/*
 * An ant capable of foraging behaviour in a grid.
 *
 * The direction the ant is facing is denoted by a pair (a,b)
 * where a and b have integer values in range [-1, 1].
 */

class Ant {
public:
  /*
   * Behaviour modes.
   */
  enum Mode { SEEK, RETURN };

  /*
   * Constructs an ant with position (x,y) and direction `d`.
   */
  Ant(int x, int y, std::pair<int, int> d, int maxSteps)
      : m_d(d), m_x(x), m_y(y), m_maxSteps(maxSteps){};

  /*
   * Returns the x coordinate.
   */
  int getX() const { return m_x; }

  /*
   * Returns the y coordinate.
   */
  int getY() const { return m_y; }

  /*
   * Returns the direction of the ant
   */
  std::pair<int, int> getDirection() const { return m_d; }

  /*
   * Sets the ant's direction to `d`.
   */
  void setDirection(std::pair<int, int> d) { m_d = d; }

  /*
   * Returns the ant's current behaviour mode.
   */
  Mode getMode() const { return m_mode; }

  /*
   * Returns the ant's traveled distance.
   */
  int getTraveledDistance() const { return m_traveledDistance; }

  /*
   * Sets the maximum number of steps to perform before aborting the search.
   */
  void setMaxSteps(int s) { m_maxSteps = s; }

  /*
   * Moves the ant to position (x,y).
   */
  void move(int x, int y);

  /*
   * Inverts the ant's direction.
   */
  void invert() {
    m_d.first *= -1;
    m_d.second *= -1;
  }

  /*
   * Start carrying food.
   */
  void pickUpFood() { m_hasFood = true; }

  /*
   * Stop carrying food.
   */
  void dropFood() { m_hasFood = false; }

  /*
   * Returns true if the ant is carrying food, false otherwise.
   */
  bool hasFood() const { return m_hasFood; }

  /*
   * Sets the ant to RETURN mode.
   */
  void returnHome() {
    m_mode = RETURN;
    m_traveledDistance = 0;
  }

  /*
   * Sets the ant to SEEK mode.
   */
  void seekFood() {
    m_mode = SEEK;
    m_traveledDistance = 0;
  }

  /*
   * Pick the next cell to move to.
   */
  Cell<SimCellData>
  pickDestination(const std::vector<Cell<SimCellData>> &candidates,
                  std::default_random_engine &rng);

private:
  std::pair<int, int> m_d;    // direction
  int m_x;                    // x coordinate
  int m_y;                    // y coordinate
  bool m_hasFood = false;     // is the ant carrying food?
  Mode m_mode = SEEK;         // beaviour mode
  int m_traveledDistance = 0; // distance traveled since mode change
  int m_maxSteps = 200;       // maximum distance before return
};

#endif // ANT_H
