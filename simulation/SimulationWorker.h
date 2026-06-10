#pragma once
#include <iostream>
#include "Rigidbody.h"
#include <QObject>
#include <QTimer>
#include <memory>

class SimulationWorker : public QObject {
	Q_OBJECT
public:
	SimulationWorker(double initVel, double mass, double angle, QObject* parent = nullptr);
	~SimulationWorker();

signals:
	void telemetryUpdated(double x, double z, double velocity, double time);
	void simulationFinished();

public slots:
	void startSimulation();
	void nextStep();

private:
	std::unique_ptr<RigidBody> m_rocket;
	QTimer* m_simTimer;
	double m_dt;
	double m_elapsedTime;
};