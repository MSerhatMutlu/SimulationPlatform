#include <iostream>
#include "Target.h"

Target::Target(const Vector& position, const Vector& velocity) : position(position), velocity(velocity) {}
Target::Target(const Target& other) : position(other.position), velocity(other.velocity) {}
Target::~Target() {}

void Target::updatePosition(double dt) {
	position = position + velocity * dt;
}

void Target::setPosition(const Vector& newPosition) {
	position = newPosition;
}

void Target::setVelocity(const Vector& newVelocity) {
	velocity = newVelocity;
}

Vector Target::getPosition() const {
	return position;
}

Vector Target::getVelocity() const {
	return velocity;
}