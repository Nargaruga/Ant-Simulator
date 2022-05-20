#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "cave_gen.h"
#include "custom_graphics_scene.h"
#include "population_sim.h"
#include <QGraphicsScene>
#include <QMainWindow>
#include <QThread>
#include <QTimer>
#include <mutex>

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
  void onNewCaveRequested();

  void onCaveReady(Grid grid);

  void onSimulationInitialized();

  void onSimulationStarted();

  void onSimulationStopped();

  void onSimulationStep();

  void onSeedValueChanged(int newValue);

  void onThresholdValueChanged(int newValue);

  void onRockRatioValueChanged(int newValue);

  void onStepsValueChanged(int newValue);

  void onCanvasClicked(QPointF coords);

signals:
  void startCaveGeneration(int rows, int cols);

private:
  Ui::MainWindow *m_gui;        // Class responsible for the GUI
  CustomGraphicsScene *m_scene; // Scene depicted in the canvas
  CaveGenerator m_gen;          // Generator for the displayed caves
  PopulationSimulator m_sim;    // Population behaviour simulator
  QTimer *m_timer;              // Simulation timer
  bool m_showOutlines =
      false;            // Should the grid cells have an outline? TODO checkbox
  Grid m_grid;          // Grid to draw on canvas
  std::mutex m_gridMtx; // Mutex for mutual exclusion on grid accesses
  int m_cols = 128;     // Number of grid columns
  int m_rows = 64;      // Number of grid rows
  int m_cellSide = 5;   // Width of grid cells, in pixels
  QThread m_worker;     // Worker thread

  /*
   * Connect the GUI items' signals to the relative slots
   */
  void connectSlots();

  /*
   * Set up the GUI elements
   */
  void prepareGUI();

  /*
   * Draw the generator's grid on the canvas
   */
  void populateScene();
};
#endif // MAINWINDOW_H
