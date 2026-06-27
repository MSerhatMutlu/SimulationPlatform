#include "SimulationWorker.h"

const double PI = 3.14159265358979323846;
const double navConst = 4.0;

SimulationWorker::SimulationWorker(double initVel, double mass, double angle, QObject* parent) : QObject(parent), m_dt(0.01), m_elapsedTime(0.0){
	m_rocket = std::make_unique<RigidBody>(Vector(0,0,0),
											Vector(initVel * std::cos(angle * PI/180),0, initVel * std::sin(angle * PI / 180)),
											mass,angle);
	m_target = std::make_unique<Target>(Vector(5500.0, 0, 2500.0), Vector(-300.0, 0, 0.0));
    m_camera = std::make_unique<VirtualCamera>(200,200, 40.0);
	m_simTimer = new QTimer(this);
	connect(m_simTimer, &QTimer::timeout, this, &SimulationWorker::nextStep);
    m_prevAngleDiff = 0.0;
    m_lockLostCounter = 0;
}

void SimulationWorker::startSimulation() {
	m_simTimer->start(static_cast<int>(m_dt * 1000));
}

void SimulationWorker::nextStep() {
    Vector rPos = m_rocket->getPosition();
    Vector tPos = m_target->getPosition();
    Vector rVel = m_rocket->getVelocity();
    Vector tVel = m_target->getVelocity();

    cv::Mat Frame = m_camera->createEmptyFrame();
    int PixelX = 0;
    int PixelY = 0;
    bool isVisible = m_camera->getPixelOffset(rPos, tPos, m_rocket->getPitchAngleRad(), PixelX, PixelY);

    if (isVisible) {
        double angleDiff;
        if (std::cos(m_rocket->getPitchAngleRad()) >= 0) {
            angleDiff = PixelY * m_camera->getFieldOfView() / m_camera->getHeight();
        }
        else {
            angleDiff = -PixelY * m_camera->getFieldOfView() / m_camera->getHeight();
        }

        double turnAmount = navConst * (angleDiff - m_prevAngleDiff);
        m_prevAngleDiff = angleDiff;

        while (turnAmount > PI) turnAmount -= 2 * PI;
        while (turnAmount <= -PI) turnAmount += 2 * PI;

        double maxTurnRate = m_rocket->getVelocity().findMagnitude() * 0.006;
        double maxTurnAmount = maxTurnRate * m_dt;

        if (turnAmount > maxTurnAmount) turnAmount = maxTurnAmount;
        if (turnAmount < -maxTurnAmount) turnAmount = -maxTurnAmount;

        m_rocket->setPitchAngleRad(m_rocket->getPitchAngleRad() + turnAmount);
    }
    else {
        Vector relPos = tPos - rPos;
        double targetAngle = std::atan2(relPos.getZ(), relPos.getX());
        double angleDiff = targetAngle - m_rocket->getPitchAngleRad();

        while (angleDiff > PI) angleDiff -= 2 * PI;
        while (angleDiff <= -PI) angleDiff += 2 * PI;
        double turnAmount = angleDiff;

        double maxTurnRate = m_rocket->getVelocity().findMagnitude() * 0.006;
        double maxTurnAmount = maxTurnRate * m_dt;

        if (turnAmount > maxTurnAmount) turnAmount = maxTurnAmount;
        if (turnAmount < -maxTurnAmount) turnAmount = -maxTurnAmount;

        m_prevAngleDiff = 0.0;

        m_rocket->setPitchAngleRad(m_rocket->getPitchAngleRad() + turnAmount);
    }

    double massLoss = 5.0;
    double burnTime = 8.0;
    if (m_elapsedTime < burnTime) {
        double newMass = m_rocket->getMass() - massLoss * m_dt;
        m_rocket->setMass(newMass);

        double thrust = 22000.0;
        if (isVisible) {
            double VelAngleRad = std::atan2(m_rocket->getVelocity().getZ(), m_rocket->getVelocity().getX());
            double weightConst = 0.20;
            double weightedAngle = weightConst * m_rocket->getPitchAngleRad() + (1 - weightConst) * VelAngleRad;
            m_rocket->applyThrustRad(thrust, weightedAngle);
        }
        else {
            m_rocket->applyThrust(thrust);
        }
    }

    bool showLockOn = isVisible;
    if (isVisible) {
        m_lockLostCounter = 0;
    }
    else {
        m_lockLostCounter++;
        if (m_lockLostCounter < 15 && m_elapsedTime > 0.5) {
            showLockOn = true;
        }
    }

    m_camera->renderUHD(Frame, showLockOn, PixelX, PixelY);
    QImage qimage = m_camera->convertMatToQImage(Frame);

    m_rocket->updatePhysics(m_dt);
    m_target->updatePosition(m_dt);
    m_elapsedTime += m_dt;

    double currentSpeed = rVel.findMagnitude();
    double distanceToTarget = (m_target->getPosition() - m_rocket->getPosition()).findMagnitude();

    emit imageUpdated(qimage);
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
