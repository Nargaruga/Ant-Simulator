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

  void onCaveReady(Grid<bool> cave);

  void onSimInitRequested();

  void onSimStartRequested();

  void onSimStopRequested();

  void onSimReady(Grid<SimCellData> cave);

  void onTimeout();

  void onCanvasClick(QPointF coords);

  void onFoodUpdated(int delivered, int total);

signals:
  void startCaveGeneration(int rows, int cols);

  void initializeSim();

  void performSimStep();

  void cellClicked(int x, int y);

private:
  Ui::MainWindow *m_gui;        // Class responsible for the GUI
  CustomGraphicsScene *m_scene; // Scene depicted in the canvas
  CaveGenerator m_gen;          // Generator for the displayed caves
  AntSimulator m_sim;           // Population behaviour simulator
  QTimer *m_timer;              // Simulation timer
  bool m_showOutlines =
      false; // Should the grid cells have an outline? TODO checkbox
  Grid<SimCellData> grid;
  int m_cols = 128;    // Number of grid columns
  int m_rows = 64;     // Number of grid rows
  int m_cellSide = 10; // Width of grid cells, in pixels
  QThread m_genWorker; // Cave generation thread
  QThread m_simWorker; // Population simulation thread

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
  void drawCave(Grid<bool> grid);

  /*
   * Draw the simulator's grid on the canvas
   */
  void drawAnts(Grid<SimCellData> grid);
};
#endif // MAINWINDOW_H
