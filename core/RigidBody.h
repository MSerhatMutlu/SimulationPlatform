#pragma once
#include "Vector.h"

class RigidBody {
public:
	RigidBody(const Vector& position, const Vector& initVel, double mass, double angle);
	RigidBody(const RigidBody& other);
	~RigidBody();

	void applyForce(const Vector& force);
	void clearForces();
	void updatePhysics(double dt);
	void applyThrust(double magnitude);

	double getMass() const;
	double getPitchAngleDeg() const;
	double getPitchAngleRad() const;
	void setPitchAngleDeg(double angle);
	void setPitchAngleRad(double rad);
	Vector getVelocity() const;
	Vector getAcceleration() const;
	Vector getPosition() const;
	Vector getNetForce() const;

private:
	double mass;
	double pitchAngle; 
	Vector position;
	Vector velocity;
	Vector acceleration;
	Vector netForce;
};