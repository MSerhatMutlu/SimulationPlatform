#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsPolygonItem>
#include <QGraphicsLineItem>
#include <QVector>
#include <QPointF>
#include <memory>
#include "simulation/SimulationWorker.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onStartButtonClicked();
    void handleTelemetry(double rocketX, double rocketZ, double targetX, double targetZ, double speed, double time);
    void handleSimulationFinished(bool success);

private:
    Ui::MainWindow* ui;
    std::unique_ptr<SimulationWorker> m_worker;
    QGraphicsScene* m_scene = nullptr;
    QGraphicsPolygonItem* m_rocketMarker = nullptr;
    QGraphicsEllipseItem* m_targetMarker = nullptr;
    QVector<QPointF> m_trajectoryPoints;
    QVector<QGraphicsLineItem*> m_drawnLines;
};

#endif