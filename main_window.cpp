#include "main_window.h"
#include "ui_main_window.h"
#include <QGraphicsRectItem>
#include <bits/stdc++.h>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_gui(new Ui::MainWindow), m_sim(m_grid) {
  m_gui->setupUi(this);
  m_timer = new QTimer(this);

  m_gen.moveToThread(&m_worker);

  m_grid.resize(m_rows, m_cols);
  prepareGUI();
  connectSlots();

  m_worker.start();
}

MainWindow::~MainWindow() {
  m_worker.quit();
  m_worker.wait();

  delete m_gui;
  delete m_scene;
  delete m_timer;
}

void MainWindow::connectSlots() {

  connect(m_gui->generateCaveBtn, &QPushButton::clicked, this,
          &MainWindow::onNewCaveRequested);

  connect(this, &MainWindow::startCaveGeneration, &m_gen,
          &CaveGenerator::generateCave);

  connect(&m_gen, &CaveGenerator::caveReady, this, &MainWindow::onCaveReady);

  connect(m_gui->initSimBtn, &QPushButton::clicked, this,
          &MainWindow::onSimulationInitialized);

  connect(m_gui->startSimBtn, &QPushButton::clicked, this,
          &MainWindow::onSimulationStarted);

  connect(m_gui->stopSimBtn, &QPushButton::clicked, this,
          &MainWindow::onSimulationStopped);

  connect(m_gui->seedSB, &QSpinBox::valueChanged, this,
          &MainWindow::onSeedValueChanged);

  connect(m_gui->thresholdSB, &QSpinBox::valueChanged, this,
          &MainWindow::onThresholdValueChanged);

  connect(m_gui->rockRatioSB, &QSpinBox::valueChanged, this,
          &MainWindow::onRockRatioValueChanged);

  connect(m_gui->stepsSB, &QSpinBox::valueChanged, this,
          &MainWindow::onStepsValueChanged);

  connect(m_scene, &CustomGraphicsScene::mouseReleased, this,
          &MainWindow::onCanvasClicked);

  connect(m_timer, &QTimer::timeout, this, &MainWindow::onSimulationStep);
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
  m_scene = new CustomGraphicsScene();
  m_scene->setSceneRect(QRect(0, 0, m_cols * m_cellSide, m_rows * m_cellSide));
  populateScene();

  // Setup the QtGraphicsView which will host the scene
  m_gui->graphicsView->setMinimumSize(
      QSize(m_cols * m_cellSide, m_rows * m_cellSide));
  m_gui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  m_gui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  m_gui->graphicsView->setViewportUpdateMode(QGraphicsView::NoViewportUpdate);
  m_gui->graphicsView->setScene(m_scene);
}

void MainWindow::populateScene() {
  for (int x = 0; x < m_grid.getCols(); x++) {
    for (int y = 0; y < m_grid.getRows(); y++) {
      const Cell &cell = m_grid.getCell(x, y);

      // Draw the cell
      QGraphicsRectItem *item = new QGraphicsRectItem(
          QRect(cell.getX() * m_cellSide, cell.getY() * m_cellSide, m_cellSide,
                m_cellSide));
      item->setBrush(QBrush(cell.getColor()));

      if (!m_showOutlines)
        item->setPen(Qt::NoPen);

      m_scene->addItem(item);
    }
  }
}

void MainWindow::onNewCaveRequested() {
  // TODO add loading indicator
  emit startCaveGeneration(m_rows, m_cols);
}

void MainWindow::onCaveReady(Grid grid) {
  // TODO remove loading indicator
  m_grid = grid;
  populateScene();
  m_scene->update();
}

void MainWindow::onSimulationInitialized() {
  m_sim.initialize();
  m_grid = m_sim.getGrid();
  populateScene();
  m_scene->update();
}

void MainWindow::onSimulationStarted() {
  m_timer->start(1000); // milliseconds
};

void MainWindow::onSimulationStopped() { m_timer->stop(); };

void MainWindow::onSimulationStep() {
  m_sim.step();
  m_grid = m_sim.getGrid();
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

void MainWindow::onCanvasClicked(QPointF coords) {
  m_grid.setLightSource(round(coords.x() / m_cellSide),
                        round(coords.y() / m_cellSide));
  populateScene();
  m_scene->update();
}
