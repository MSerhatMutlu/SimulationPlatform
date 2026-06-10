#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    m_scene = new QGraphicsScene(this);
    ui->plotWidget->setScene(m_scene);
    ui->plotWidget->setRenderHint(QPainter::Antialiasing);

    m_scene->setSceneRect(0, 0, ui->plotWidget->width(), ui->plotWidget->height());

    ui->plotWidget->setBackgroundBrush(QBrush(QColor(30, 30, 30)));

    m_rocketMarker = m_scene->addEllipse(-6, -6, 12, 12, QPen(Qt::red), QBrush(Qt::red));
    m_rocketMarker->setVisible(false);

    m_targetMarker = m_scene->addEllipse(-8, -8, 16, 16, QPen(Qt::cyan), QBrush(Qt::transparent));
    m_targetMarker->setVisible(false);

    connect(ui->btnStart, &QPushButton::clicked, this, &MainWindow::onStartButtonClicked);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onStartButtonClicked() {
    for (auto line : m_drawnLines) {
        m_scene->removeItem(line);
        delete line;
    }
    m_drawnLines.clear();
    m_trajectoryPoints.clear();

    m_worker.reset();
    m_worker = std::make_unique<SimulationWorker>(100.0, 50.0, 45.0, this);

    connect(m_worker.get(), &SimulationWorker::telemetryUpdated,
        this, &MainWindow::handleTelemetry);

    connect(m_worker.get(), &SimulationWorker::simulationFinished,
        this, &MainWindow::handleSimulationFinished);

    m_worker->startSimulation();
    ui->btnStart->setEnabled(false);
}

void MainWindow::handleTelemetry(double x, double z, double speed, double time) {
    double totalDistance = std::sqrt((x * x) + (z * z));
    ui->lblDistance->setText(QString::number(totalDistance, 'f', 2) + " m");
    ui->lblSpeed->setText(QString::number(speed, 'f', 2) + " m/s");
    ui->lblTime->setText(QString::number(time, 'f', 2) + " s");

    const double scale = 5.0;
    const double paddingFromBottom = 40.0; 

    double screenX = x / scale;
    double screenY = ui->plotWidget->height() - (z / scale) - paddingFromBottom;

    double targetRealX = 2000.0;
    double targetScreenX = targetRealX / scale;
    double targetScreenY = ui->plotWidget->height() - paddingFromBottom;

    m_rocketMarker->setPos(screenX, screenY);
    m_targetMarker->setPos(targetScreenX, targetScreenY);

    m_rocketMarker->setVisible(true);
    m_targetMarker->setVisible(true);

    if (!m_trajectoryPoints.empty()) {
        QPointF lastPoint = m_trajectoryPoints.back();
        QGraphicsLineItem* line = m_scene->addLine(lastPoint.x(), lastPoint.y(),
            screenX, screenY,
            QPen(Qt::yellow, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        m_drawnLines.push_back(line);
    }
    m_trajectoryPoints.push_back(QPointF(screenX, screenY));
}

void MainWindow::handleSimulationFinished() {
    ui->btnStart->setEnabled(true);

    m_worker.reset();
}