#include <iostream>
#include "Environment.h"

Environment::Environment(const Vector& gravity, double airDensity) : gravity(gravity), airDensity(airDensity) {}
Environment::Environment(const Environment& other) : gravity(other.gravity), airDensity(other.airDensity) {}
Environment::Environment() : gravity(Vector(0, 0, -9.81)), airDensity(1.225) {}
Environment::~Environment() {}

void Environment::applyGravity(RigidBody& body) {
	body.applyForce(gravity * body.getMass());
}

void Environment::applyDrag(RigidBody& body, double dragCoeff, double area) {
	double speedSquared = body.getVelocity().findMagnitude() * body.getVelocity().findMagnitude();
	body.applyForce(body.getVelocity().normalize() * -0.5 * dragCoeff * airDensity * area * speedSquared);
}

Vector Environment::customDrag(const Vector& velocity, double dragCoeff, double area) const {
	double speedSquared = velocity.findMagnitude() * velocity.findMagnitude();
	return velocity.normalize() * -0.5 * dragCoeff * airDensity * area * speedSquared;
}

void Environment::setAirDensity(double density) {
	airDensity = density;
}

Vector Environment::getGravity() const {
	return gravity;
}

double Environment::getAirDensity() const {
	return airDensity;
}