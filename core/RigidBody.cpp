#include <iostream>
#include <cmath>
#include "RigidBody.h"
#include "Environment.h"
#include "Vector.h"

const double PI = 3.14159265358979323846;

RigidBody::RigidBody(const Vector& position, const Vector& initVel, double mass, double angle) : position(position), velocity(initVel), mass(mass),
acceleration(0,0,0), netForce(0,0,0), pitchAngle(angle* PI / 180.0) {}

RigidBody::RigidBody(const RigidBody& other) : position(other.position), velocity(other.velocity), mass(other.mass),
acceleration(other.acceleration), netForce(other.netForce), pitchAngle(other.pitchAngle) {}

RigidBody::~RigidBody() {}

void RigidBody::applyForce(const Vector& force) {
	netForce = netForce + force;
}

void RigidBody::clearForces() {
	netForce = Vector(0, 0, 0);
}

void RigidBody::updatePhysics(double dt) {
	Environment env;

	Vector thrustForce = netForce;

	Vector gravityForce = Vector(0, 0, -9.81) * mass;
	Vector dragForce = env.customDrag(velocity, 0.5, 1.0);
	Vector k1_netForce = gravityForce + thrustForce + dragForce;

	Vector k1_pos_derivative = velocity;
	Vector k1_vel_derivative = k1_netForce / mass;

	Vector temp_vel_2 = velocity + k1_vel_derivative * (dt / 2.0);

	Vector dragForce2 = env.customDrag(temp_vel_2, 0.5, 1.0);
	Vector k2_netForce = gravityForce + thrustForce + dragForce2;

	Vector k2_pos_derivative = temp_vel_2;
	Vector k2_vel_derivative = k2_netForce / mass;

	Vector temp_vel_3 = velocity + k2_vel_derivative * (dt / 2.0);

	Vector dragForce3 = env.customDrag(temp_vel_3, 0.5, 1.0);
	Vector k3_netForce = gravityForce + thrustForce + dragForce3;

	Vector k3_pos_derivative = temp_vel_3;
	Vector k3_vel_derivative = k3_netForce / mass;

	Vector temp_vel_4 = velocity + k3_vel_derivative * dt;

	Vector dragForce4 = env.customDrag(temp_vel_4, 0.5, 1.0);
	Vector k4_netForce = gravityForce + thrustForce + dragForce4;

	Vector k4_pos_derivative = temp_vel_4;
	Vector k4_vel_derivative = k4_netForce / mass;

	Vector finalAcceleration = (k1_vel_derivative + k2_vel_derivative * 2 + k3_vel_derivative * 2 + k4_vel_derivative) / 6.0;
	Vector finalVelocity = (k1_pos_derivative + k2_pos_derivative * 2 + k3_pos_derivative * 2 + k4_pos_derivative) / 6.0;

	velocity = velocity + finalAcceleration * dt;
	position = position + finalVelocity * dt;

	clearForces();
}

void RigidBody::applyThrust(double magnitude) {
	Vector thrustForce(std::cos(pitchAngle), 0, std::sin(pitchAngle));

	applyForce(thrustForce * magnitude);
}

double RigidBody::getMass() const {
	return mass;
}

double RigidBody::getPitchAngleDeg() const {
	return pitchAngle * 180.0 / PI;
}

double RigidBody::getPitchAngleRad() const {
	return pitchAngle;
}

void RigidBody::setPitchAngleDeg(double angle) {
	pitchAngle = angle * PI / 180.0;
}

void RigidBody::setPitchAngleRad(double rad) {
	pitchAngle = rad;
}

Vector RigidBody::getPosition() const {
	return position;
}

Vector RigidBody::getVelocity() const {
	return velocity;
}

Vector RigidBody::getAcceleration() const {
	return acceleration;
}

Vector RigidBody::getNetForce() const {
	return netForce;
}
