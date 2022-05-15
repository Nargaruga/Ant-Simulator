#include "main_window.h"
#include "ui_main_window.h"
#include <QGraphicsRectItem>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_gui(new Ui::MainWindow) {
  m_gui->setupUi(this);

  connect(m_gui->generateCaveBtn, &QPushButton::clicked, this,
          &MainWindow::onNewCaveRequested);

  m_gen.resize(nCols, nRows);
  m_gen.generateCave();

  m_scene = new QGraphicsScene();
  m_scene->setSceneRect(QRect(0, 0, nCols * 5, nRows * 5));
  populateScene();
  // m_gui->graphicsView->resize(QSize(nCols * 5, nRows * 5));
  m_gui->graphicsView->setViewportUpdateMode(QGraphicsView::NoViewportUpdate);
  m_gui->graphicsView->setScene(m_scene);
}

MainWindow::~MainWindow() {
  delete m_gui;
  delete m_scene;
}

void MainWindow::populateScene() {
  Grid grid = m_gen.getGrid();
  for (size_t x = 0; x < grid.getCols(); x++) {
    for (size_t y = 0; y < grid.getRows(); y++) {
      Cell cell = grid.getCell(x, y);
      QGraphicsRectItem *item =
          new QGraphicsRectItem(QRect(cell.getY() * 5, cell.getX() * 5, 5, 5));
      item->setBrush(QBrush(cell.getColor()));
      m_scene->addItem(item);
    }
  }
}

void MainWindow::onNewCaveRequested() {
  m_gen.generateCave();
  populateScene();
  m_scene->update();
}
