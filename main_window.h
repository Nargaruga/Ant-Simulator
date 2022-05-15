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

private:
  Ui::MainWindow *m_gui;
  QGraphicsScene *m_scene;
  CaveGenerator m_gen;
  int nCols = 128;
  int nRows = 64;

  void populateScene();
};
#endif // MAINWINDOW_H
