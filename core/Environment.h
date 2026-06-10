#pragma once
#include "RigidBody.h"
#include "Vector.h"

class Environment {
public:
	Environment(const Vector& gravity, double airDensity);
	Environment(const Environment& other);
	Environment();
	~Environment();

	void applyGravity(RigidBody& body) ;
	void applyDrag(RigidBody& body, double dragCoeff, double area) ;
	void setAirDensity(double density);
	Vector customDrag(const Vector& velocity, double dragCoeff, double area) const;

	Vector getGravity() const;
	double getAirDensity() const;

private:
	Vector gravity;
	double airDensity;
};