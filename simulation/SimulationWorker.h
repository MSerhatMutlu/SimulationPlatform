#pragma once
#include "../core/RigidBody.h"
#include "../core/Target.h"
#include <QObject>
#include <QTimer>
#include <memory>

class SimulationWorker : public QObject {
	Q_OBJECT
public:
	SimulationWorker(double initVel, double mass, double angle, QObject* parent = nullptr);

signals:
	void telemetryUpdated(double rocketX, double rocketZ, double targetX, double targetZ, double velocity, double time);
	void simulationFinished(bool success);

public slots:
	void startSimulation();
	void nextStep();

private:
	std::unique_ptr<RigidBody> m_rocket;
	std::unique_ptr<Target> m_target;
	QTimer* m_simTimer;
	double m_dt;
	double m_elapsedTime;
};