#include "main_window.h"
#include "ui_main_window.h"
#include <QGraphicsRectItem>
#include <bits/stdc++.h>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_gui(new Ui::MainWindow) {
  m_gui->setupUi(this);

  prepareGenerator();
  connectSlots();
  prepareGUI();
}

MainWindow::~MainWindow() {
  delete m_gui;
  delete m_scene;
}

void MainWindow::connectSlots() {
  connect(m_gui->generateCaveBtn, &QPushButton::clicked, this,
          &MainWindow::onNewCaveRequested);

  connect(m_gui->seedSB, &QSpinBox::valueChanged, this,
          &MainWindow::onSeedValueChanged);

  connect(m_gui->thresholdSB, &QSpinBox::valueChanged, this,
          &MainWindow::onThresholdValueChanged);

  connect(m_gui->rockRatioSB, &QSpinBox::valueChanged, this,
          &MainWindow::onRockRatioValueChanged);

  connect(m_gui->stepsSB, &QSpinBox::valueChanged, this,
          &MainWindow::onStepsValueChanged);
}

void MainWindow::prepareGenerator() {
  m_gen.resize(nCols, nRows);
  m_gen.generateCave();
}

void MainWindow::prepareGUI() {
  // Set up spin boxes
  m_gui->seedSB->setRange(m_gen.getSeedRange().first,
                          m_gen.getSeedRange().second);
  m_gui->seedSB->setValue(m_gen.getSeed());

  m_gui->thresholdSB->setRange(m_gen.getThresholdRange().first,
                               m_gen.getThresholdRange().second);
  m_gui->thresholdSB->setValue(m_gen.getThreshold());

  m_gui->rockRatioSB->setRange(m_gen.getRockRatioRange().first,
                               m_gen.getRockRatioRange().second);
  m_gui->rockRatioSB->setValue(m_gen.getRockRatio());

  m_gui->stepsSB->setRange(m_gen.getStepsRange().first,
                           m_gen.getStepsRange().second);
  m_gui->stepsSB->setValue(m_gen.getSteps());

  // Setup and populate the scene
  m_scene = new QGraphicsScene();
  m_scene->setSceneRect(QRect(0, 0, nCols * cellWidth, nRows * cellWidth));
  populateScene();

  // Setup the QtGraphicsView which will host the scene
  m_gui->graphicsView->setMinimumSize(
      QSize(nCols * cellWidth, nRows * cellWidth));
  m_gui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  m_gui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  m_gui->graphicsView->setViewportUpdateMode(QGraphicsView::NoViewportUpdate);
  m_gui->graphicsView->setScene(m_scene);
}

void MainWindow::populateScene() {
  Grid grid = m_gen.getGrid();
  for (int x = 0; x < grid.getCols(); x++) {
    for (int y = 0; y < grid.getRows(); y++) {
      Cell cell = grid.getCell(x, y);
      // Draw the cell
      QGraphicsRectItem *item = new QGraphicsRectItem(
          QRect(cell.getY() * cellWidth, cell.getX() * cellWidth, cellWidth,
                cellWidth));
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

void MainWindow::onSeedValueChanged(int newValue) { m_gen.setSeed(newValue); }

void MainWindow::onThresholdValueChanged(int newValue) {
  m_gen.setThreshold(newValue);
}

void MainWindow::onRockRatioValueChanged(int newValue) {
  m_gen.setRockRatio(newValue);
}

void MainWindow::onStepsValueChanged(int newValue) { m_gen.setSteps(newValue); }
