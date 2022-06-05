#include "main_window.h"
#include "ui_main_window.h"
#include <QGraphicsRectItem>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_gui(new Ui::MainWindow) {
  m_gui->setupUi(this);
  m_timer = new QTimer(this);

  m_gen.moveToThread(&m_genWorker);
  m_sim.moveToThread(&m_simWorker);

  prepareGUI();
  connectSlots();

  m_genWorker.start();
  m_simWorker.start();
}

MainWindow::~MainWindow() {
  m_simWorker.quit();
  m_genWorker.quit();
  m_simWorker.wait();
  m_genWorker.wait();

  delete m_gui;
  delete m_scene;
  delete m_timer;
}

void MainWindow::connectSlots() {

  // Cave generation
  connect(m_gui->generateCaveBtn, &QPushButton::clicked, this,
          &MainWindow::onNewCaveRequested);

  connect(this, &MainWindow::startCaveGeneration, &m_gen,
          &CaveGenerator::generateCave);

  connect(&m_gen, &CaveGenerator::gridReady, this, &MainWindow::onGridReady);

  connect(m_gui->seedSB, &QSpinBox::valueChanged, &m_gen,
          &CaveGenerator::setSeed);

  connect(m_gui->thresholdSB, &QSpinBox::valueChanged, &m_gen,
          &CaveGenerator::setThreshold);

  connect(m_gui->rockRatioSB, &QSpinBox::valueChanged, &m_gen,
          &CaveGenerator::setRockRatio);

  connect(m_gui->stepsSB, &QSpinBox::valueChanged, &m_gen,
          &CaveGenerator::setSteps);

  connect(m_gui->radiusSB, &QSpinBox::valueChanged, &m_gen,
          &CaveGenerator::setRadius);

  connect(m_gui->mooreRad, &QRadioButton::toggled, &m_gen,
          &CaveGenerator::setMooreMode);

  connect(m_gui->neumannRad, &QRadioButton::toggled, &m_gen,
          &CaveGenerator::setNeumannMode);

  // Population simulation
  connect(m_gui->startSimBtn, &QPushButton::clicked, this,
          &MainWindow::onSimStartRequested);

  connect(m_gui->stopSimBtn, &QPushButton::clicked, this,
          &MainWindow::onSimStopRequested);

  connect(m_gui->initSimBtn, &QPushButton::clicked, this,
          &MainWindow::onSimInitRequested);

  connect(this, &MainWindow::initializeSim, &m_sim, &AntSimulator::initialize);

  connect(m_timer, &QTimer::timeout, this, &MainWindow::onTimeout);

  connect(this, &MainWindow::performSimStep, &m_sim, &AntSimulator::step);

  connect(&m_sim, &AntSimulator::gridReady, this, &MainWindow::onGridReady);

  connect(&m_gen, &CaveGenerator::gridReady, &m_sim, &AntSimulator::setup);

  connect(&m_sim, &AntSimulator::updateFoodCount, this,
          &MainWindow::onFoodUpdated);

  // Canvas
  connect(m_scene, &CustomGraphicsScene::mouseReleased, this,
          &MainWindow::onCanvasClick);

  connect(this, &MainWindow::cellClicked, &m_sim, &AntSimulator::onCellClicked);

  connect(m_gui->zoomSlider, &QAbstractSlider::valueChanged, this,
          &MainWindow::updateZoom);
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

  m_gui->radiusSB->setRange(m_gen.getRadiusRange().first,
                            m_gen.getRadiusRange().second);
  m_gui->radiusSB->setValue(m_gen.getRadius());

  // Setup the scene
  m_scene = new CustomGraphicsScene();
  m_scene->setSceneRect(QRect(0, 0, m_cols * m_cellSide, m_rows * m_cellSide));

  // Setup the QtGraphicsView which will host the scene
  m_gui->graphicsView->setMinimumSize(
      QSize(m_cols * m_cellSide, m_rows * m_cellSide));
  m_gui->graphicsView->setHorizontalScrollBarPolicy(
      Qt::ScrollBarAsNeeded); // TODO useless
  m_gui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  m_gui->graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
  m_gui->graphicsView->setViewportUpdateMode(QGraphicsView::NoViewportUpdate);
  m_gui->graphicsView->setScene(m_scene);
}

void MainWindow::drawGrid(Grid<SimCellData> grid) {
  for (int x = 0; x < grid.getCols(); x++) {
    for (int y = 0; y < grid.getRows(); y++) {
      Cell<SimCellData> cell = grid.getCell(x, y);

      // Set the shape of the cell
      QGraphicsRectItem *item = new QGraphicsRectItem(
          QRect(x * m_cellSide, y * m_cellSide, m_cellSide, m_cellSide));

      item->setBrush(QBrush(cell.getData().getColor()));
      item->setPen(Qt::NoPen); // TODO add checkbox for outline

      m_scene->addItem(item);
    }
  }

  updateZoom();
}

void MainWindow::onNewCaveRequested() {
  emit startCaveGeneration(m_rows, m_cols);
}

void MainWindow::onSimInitRequested() { emit initializeSim(); }

void MainWindow::onSimStartRequested() {
  m_timer->start(100); // milliseconds
}

void MainWindow::onSimStopRequested() { m_timer->stop(); }

void MainWindow::onTimeout() { emit performSimStep(); }

void MainWindow::onGridReady(Grid<SimCellData> grid) {
  m_scene->clear();
  drawGrid(grid);
  m_scene->update();
}

void MainWindow::onCanvasClick(QPointF coords) {
  int x = floor(coords.x() / m_cellSide);
  int y = floor(coords.y() / m_cellSide);

  emit cellClicked(x, y);
}

void MainWindow::onFoodUpdated(int delivered, int total) {
  m_gui->foodCountLbl->setText(
      QString("Food delivered: %1 out of %2").arg(delivered).arg(total));
}

void MainWindow::updateZoom() {
  qreal scale = qPow(qreal(2), (m_gui->zoomSlider->value() - 250) / qreal(50));

  QTransform matrix;
  matrix.scale(scale, scale);

  m_gui->graphicsView->setTransform(matrix);
}
