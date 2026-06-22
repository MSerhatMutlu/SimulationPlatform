#include <iostream>
#include <cmath>
#include "Vector.h"

Vector::Vector(double x, double y, double z): x(x), y(y), z(z) {}

Vector::Vector(const Vector& other): x(other.x), y(other.y), z(other.z) {}

Vector::~Vector() {}

Vector Vector::operator+(const Vector& other) const {
	return Vector(x + other.x, y + other.y, z + other.z);
}

Vector Vector::operator-(const Vector& other) const {
	return Vector(x - other.x, y - other.y, z - other.z);
}

Vector Vector::operator*(const double scalar) const {
	return Vector(x*scalar, y*scalar, z*scalar);
}

Vector Vector::operator/(const double scalar) const {
	if (scalar == 0) {
		return Vector(0,0,0);
	}
	return Vector(x / scalar, y / scalar, z / scalar);
}

double Vector::findMagnitude() const {
	return sqrt(x * x + y * y + z * z);
}

Vector Vector::normalize() const {
	double magnitude = findMagnitude();
	if (magnitude == 0) {
		return Vector(0, 0, 0);
	}
	return Vector(x / magnitude, y / magnitude, z / magnitude);
}

double Vector::dotProduct(Vector& other) const {
	return (x * other.getX()) + (y * other.getY()) + (z * other.getZ());
}

double Vector::getX() const {
	return x;
}

double Vector::getY() const {
	return y;
}

double Vector::getZ() const {
	return z;
}
