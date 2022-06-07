#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ant_sim.h"
#include "cave_gen.h"
#include "custom_graphics_scene.h"
#include <QGraphicsScene>
#include <QMainWindow>
#include <QThread>
#include <QTimer>
#include <mutex>

/*
 * Main GUI class.
 */

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  /*
   * Ask the cave generator for a new cave.
   */
  void onNewCaveRequested();

  /*
   * Ask the simulator to perform initialization.
   */
  void onSimInitRequested();

  /*
   * Ask the simulator to start the simulation.
   */
  void onSimStartRequested();

  /*
   * Ask the simulator to stop the simulation.
   */
  void onSimStopRequested();

  /*
   * Allow the user to initialize the simulation.
   */
  void allowSimInit();

  /*
   * Allow the user to start and stop the simulation.
   */
  void allowSimControl();

  /*
   * Prevent the user from starting or stopping the simulation.
   */
  void revokeSimControl();

  /*
   * Set the simulation speed based on `speed`.
   */
  void setSimSpeed(int speed);

  /*
   * Draw the cave.
   */
  void onCaveReady(Grid<SimCellData> grid);

  /*
   * Draw the simulation grid.
   */
  void onSimReady(Grid<SimCellData> grid);

  /*
   * Pass the clicked cell coordinates to the simulator.
   */
  void onCanvasClick(QPointF coords);

  /*
   * Update food counters.
   */
  void onFoodUpdated(int delivered, int total);

  /*
   * Update zoom level.
   */
  void updateZoom();

  /*
   * Reset the generator's parameters to their defaults.
   */
  void resetGenParams();

  /*
   * Reset the simulator's parameters to their defaults.
   */
  void resetSimParams();

signals:
  /*
   * Emitted when a new cave is requested.
   */
  void startCaveGeneration(int rows, int cols);

  /*
   * Emitted when the initialization of the simulator is requested.
   */
  void initializeSim();

  /*
   * Emitted on each simulation update.
   */
  void performSimStep();

  /*
   * Emitted on canvas click.
   */
  void cellClicked(int x, int y);

private:
  Ui::MainWindow *m_gui;        // Class responsible for the GUI
  CustomGraphicsScene *m_scene; // Scene depicted in the canvas
  CaveGenerator m_gen;          // Cave generator object
  AntSimulator m_sim;           // Ant simulator object
  int m_simSpeed = 5;           // Simulation speed
  QTimer *m_timer;              // Simulation timer
  int m_cols = 128;             // Number of grid columns
  int m_rows = 64;              // Number of grid rows
  int m_cellSide = 1;           // Width of grid cells, in pixels
  QThread m_genWorker;          // Cave generation thread
  QThread m_simWorker;          // Population simulation thread

  /*
   * Connect the GUI items' signals to the relative slots.
   */
  void connectSlots();

  /*
   * Set up the GUI elements.
   */
  void prepareGUI();

  /*
   * Set the GUI parameters for cave generation to their defaults.
   */
  void setGenGUIParams();

  /*
   * Set the GUI parameters for ant simulation to their defaults.
   */
  void setSimGUIParams();

  /*
   * Draw the grid on the canvas.
   */
  void drawGrid(Grid<SimCellData> grid);
};
#endif // MAINWINDOW_H
