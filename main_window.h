#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "cave_gen.h"
#include <QGraphicsScene>
#include <QMainWindow>

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

  void onSeedValueChanged(int newValue);

  void onThresholdValueChanged(int newValue);

  void onRockRatioValueChanged(int newValue);

  void onStepsValueChanged(int newValue);

private:
  Ui::MainWindow *m_gui;   // Class responsible for the GUI
  QGraphicsScene *m_scene; // Scene depicted in the canvas
  CaveGenerator m_gen;     // Generator for the displayed caves
  int nCols = 128;         // Number of grid columns
  int nRows = 64;          // Number of grid rows
  int cellWidth = 5;       // Width of grid cells, in pixels

  /*
   * Connect the GUI items' signals to the relative slots
   */
  void connectSlots();

  /*
   * Set up the cave generator
   */
  void prepareGenerator();

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
