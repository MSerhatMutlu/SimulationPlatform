#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QMessageBox>
#include <QPainter>
#include <QPushButton>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    m_scene = new QGraphicsScene(this);
    ui->plotWidget->setScene(m_scene);
    ui->plotWidget->setRenderHint(QPainter::Antialiasing);

    m_scene->setSceneRect(0, 0, ui->plotWidget->width(), ui->plotWidget->height());

    ui->plotWidget->setBackgroundBrush(QBrush(QColor(30, 30, 30)));

    QPolygonF rocketShape;
    rocketShape << QPointF(10, 0) << QPointF(-5, 5) << QPointF(-5, -5);
    m_rocketMarker = m_scene->addPolygon(rocketShape, QPen(Qt::red), QBrush(Qt::red));    m_rocketMarker->setVisible(false);

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
    m_worker = std::make_unique<SimulationWorker>(0.0, 85.0, 45.0, this);

    connect(m_worker.get(), &SimulationWorker::telemetryUpdated,
        this, &MainWindow::handleTelemetry);

    connect(m_worker.get(), &SimulationWorker::simulationFinished,
        this, &MainWindow::handleSimulationFinished);

    m_worker->startSimulation();
    ui->btnStart->setEnabled(false);
}

void MainWindow::handleTelemetry(double rocketX, double rocketZ, double targetX, double targetZ, double speed, double time) {
    double dx = targetX - rocketX;
    double dz = targetZ - rocketZ;
    double currentDistance = std::sqrt(dx * dx + dz * dz);

    ui->lblDistance->setText("Distance: " + QString::number(currentDistance, 'f', 1) + " m");
    ui->lblSpeed->setText("Speed: " + QString::number(speed, 'f', 1) + " m/s");
    ui->lblTime->setText("Time Elapsed: " + QString::number(time, 'f', 2) + " s");

    const double maxArea = 1000.0;

    double scale = ui->plotWidget->width() / maxArea;
    const double paddingFromBottom = 40.0;

    double rScreenX = rocketX * scale;
    double rScreenY = ui->plotWidget->height() - (rocketZ * scale) - paddingFromBottom;

    double tScreenX = targetX * scale;
    double tScreenY = ui->plotWidget->height() - (targetZ * scale) - paddingFromBottom;

    m_rocketMarker->setPos(rScreenX, rScreenY);
    m_targetMarker->setPos(tScreenX, tScreenY);

    m_rocketMarker->setVisible(true);
    m_targetMarker->setVisible(true);

    if (!m_trajectoryPoints.empty()) {
        QPointF lastPoint = m_trajectoryPoints.back();
        double screenDx = rScreenX - lastPoint.x();
        double screenDy = rScreenY - lastPoint.y();

        double visualAngle = std::atan2(screenDy, screenDx) * 180.0 / 3.14159265;
        m_rocketMarker->setRotation(visualAngle);
    }

    if (!m_trajectoryPoints.empty()) {
        QPointF lastPoint = m_trajectoryPoints.back();
        QGraphicsLineItem* line = m_scene->addLine(lastPoint.x(), lastPoint.y(), rScreenX, rScreenY,
            QPen(Qt::yellow, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        m_drawnLines.push_back(line);
    }
    m_trajectoryPoints.push_back(QPointF(rScreenX, rScreenY));
}

void MainWindow::handleSimulationFinished(bool success) {
    ui->btnStart->setEnabled(true);

    if (success) {
        QMessageBox::information(this, "Simulation Result", "MISSION COMPLETED!\nTarget successfully intercepted.");
    }
    else {
        QMessageBox::critical(this, "Simulation Result", "MISSION FAILED!\nMissile hit the ground.");
    }

    m_worker.reset();
}