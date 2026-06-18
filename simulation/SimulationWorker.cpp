#include "SimulationWorker.h"

const double PI = 3.14159265358979323846;

SimulationWorker::SimulationWorker(double initVel, double mass, double angle, QObject* parent) : QObject(parent), m_dt(0.01), m_elapsedTime(0.0){
	m_rocket = std::make_unique<RigidBody>(Vector(0,0,0),
											Vector(initVel * std::cos(angle * PI/180),0, initVel * std::sin(angle * PI / 180)),
											mass,angle);
	m_target = std::make_unique<Target>(Vector(250.0, 0, 800.0), Vector(70.0, 0, 50.0));
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
    Vector tVel = m_target->getVelocity();

    double distanceToTarget = (tPos - rPos).findMagnitude();
    if (distanceToTarget >= 50.0){
        Vector relPos = tPos - rPos;
        Vector relVel = tVel - rVel;

        double relPosMag = relPos.findMagnitude();
        double closingSpeed = -relVel.dotProduct(relPos) / relPosMag;

        double timeToCollision = 0.0;
        if (closingSpeed > 0.1)
            timeToCollision = relPosMag / closingSpeed;
        else {
            timeToCollision = relPosMag / std::max(10.0, relVel.findMagnitude());
        }

        if (m_elapsedTime < 1.5) {
            double lookAhead = 2.0;
            if (timeToCollision > lookAhead)
                timeToCollision = lookAhead;
        }

        // time delay due to air drag and gravity drop
        timeToCollision *= 1.15;
        double gravityDrop = 9.81 * timeToCollision * timeToCollision / 2;

        Vector estimatedPos = tPos + tVel * timeToCollision + Vector(0, 0, gravityDrop);
        Vector toEstPos = estimatedPos - rPos;

        double targetAngle = std::atan2(toEstPos.getZ(), toEstPos.getX());

        double currentAngle = m_rocket->getPitchAngleRad();
        double angleDiff = targetAngle - currentAngle;

        while (angleDiff > PI) angleDiff -= 2 * PI;
        while (angleDiff < -PI) angleDiff += 2 * PI;

        double maxTurnRate = 0.7;
        double maxTurnStep = maxTurnRate * m_dt;

        double turnAmount = angleDiff;
        if (turnAmount > maxTurnStep) turnAmount = maxTurnStep;
        if (turnAmount < -maxTurnStep) turnAmount = -maxTurnStep;

        m_rocket->setPitchAngleRad(currentAngle + turnAmount);
    }

	double burnTime = 10.0;
    if (m_elapsedTime < burnTime) {
		m_rocket->applyThrust(10000.0);
    }

    m_rocket->updatePhysics(m_dt);
    m_target->updatePosition(m_dt);
    m_elapsedTime += m_dt;
    double currentSpeed = rVel.findMagnitude();

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
