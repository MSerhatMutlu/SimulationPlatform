#pragma once

class Vector {

public:

	Vector(double x, double y, double z);

	Vector(const Vector& other);

	~Vector();

	Vector operator+(const Vector& other) const;

	Vector operator-(const Vector& other) const;

	Vector operator*(const double scalar) const;

	Vector operator/(const double scalar) const;

	double findMagnitude() const;

	Vector normalize() const;

	double getX() const;

	double getY() const;

	double getZ() const;

private:
	double x;
	double y;
	double z;

};