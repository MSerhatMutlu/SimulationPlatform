#include "SimulationWorker.h"

const double PI = 3.14159265358979323846;

SimulationWorker::SimulationWorker(double initVel, double mass, double angle, QObject* parent) : QObject(parent), m_dt(0.01), m_elapsedTime(0.0){
	m_rocket = std::make_unique<RigidBody>(Vector(0,0,0),
											Vector(initVel * std::cos(angle * PI/180),0, initVel * std::sin(angle * PI / 180)),
											mass,angle);
	m_target = std::make_unique<Target>(Vector(400.0, 0, 700.0), Vector(20.0, 0, 0));
	m_simTimer = new QTimer(this);
	connect(m_simTimer, &QTimer::timeout, this, &SimulationWorker::nextStep);
}

SimulationWorker::~SimulationWorker() {}

void SimulationWorker::startSimulation() {
	m_simTimer->start(static_cast<int>(m_dt * 1000));
}

void SimulationWorker::nextStep() {
    Vector rPos = m_rocket->getPosition();
    Vector tPos = m_target->getPosition();
    Vector rVel = m_rocket->getVelocity();

    double dx = tPos.getX() - rPos.getX();
    double dz = tPos.getZ() - rPos.getZ();
    double distanceToTarget = std::sqrt(dx * dx + dz * dz);

    double currentSpeed = rVel.findMagnitude();
    if (currentSpeed < 1.0) currentSpeed = 1.0;

    double timeToTarget = distanceToTarget / currentSpeed;
    double gravityDrop = 0.15 * 9.81 * timeToTarget * timeToTarget;

    double aimZ = tPos.getZ() + gravityDrop;
    double dz_aim = aimZ - rPos.getZ();

    double targetAngle = std::atan2(dz_aim, dx);

    double currentAngle = m_rocket->getPitchAngleRad();
    double angleDiff = targetAngle - currentAngle;

    while (angleDiff > PI) angleDiff -= 2 * PI;
    while (angleDiff < -PI) angleDiff += 2 * PI;

    double maxTurnRate = 0.6;
    double maxTurnStep = maxTurnRate * m_dt;

    double turnAmount = angleDiff;
    if (turnAmount > maxTurnStep) turnAmount = maxTurnStep;
    if (turnAmount < -maxTurnStep) turnAmount = -maxTurnStep;

    m_rocket->setPitchAngleRad(currentAngle + turnAmount);

	double burnTime = 5.0;
    if (m_elapsedTime < burnTime) {
		m_rocket->applyThrust(12000.0);
    }

    m_rocket->updatePhysics(m_dt);
    m_target->updatePosition(m_dt);
    m_elapsedTime += m_dt;

    emit telemetryUpdated(rPos.getX(), rPos.getZ(), tPos.getX(), tPos.getZ(), currentSpeed, m_elapsedTime);

    if (m_rocket->getPosition().getZ() <= 0) {
        m_simTimer->stop();
        emit simulationFinished(false);
    }
    else if (distanceToTarget < 20.0) {
        m_simTimer->stop();
        emit simulationFinished(true);
    }
}
