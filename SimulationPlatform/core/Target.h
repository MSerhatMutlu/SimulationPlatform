#pragma once
#include "Vector.h"

class Target {
public:
	Target(const Vector& position, const Vector& velocity);
	Target(const Target& other);
	~Target();

	void updatePosition(double dt);
	void setPosition(const Vector& newPosition);
	void setVelocity(const Vector& newVelocity);
	Vector getPosition() const;
	Vector getVelocity() const;

private:
	Vector position;
	Vector velocity;
};