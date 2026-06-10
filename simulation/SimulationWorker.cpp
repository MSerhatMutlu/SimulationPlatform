#include "SimulationWorker.h"

const double PI = 3.14159265358979323846;

SimulationWorker::SimulationWorker(double initVel, double mass, double angle, QObject* parent) : QObject(parent), m_dt(0.01), m_elapsedTime(0.0){
	m_rocket = std::make_unique<RigidBody>(Vector(0,0,0),
											Vector(initVel * std::cos(angle * PI/180),0, initVel * std::sin(angle * PI / 180)),
											mass,angle);
	m_simTimer = new QTimer(this);
	connect(m_simTimer, &QTimer::timeout, this, &SimulationWorker::nextStep);
}

SimulationWorker::~SimulationWorker() {}

void SimulationWorker::startSimulation() {
	m_simTimer->start(static_cast<int>(m_dt * 1000));
}

void SimulationWorker::nextStep() {
	m_rocket->updatePhysics(m_dt);
	m_elapsedTime += m_dt;
	emit telemetryUpdated(m_rocket->getPosition().getX(), m_rocket->getPosition().getZ(), m_rocket->getVelocity().findMagnitude(), m_elapsedTime);

	if (m_rocket->getPosition().getZ() <= 0) {
		m_simTimer->stop();
		emit simulationFinished();
	}
}