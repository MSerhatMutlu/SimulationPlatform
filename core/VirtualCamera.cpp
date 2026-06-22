#pragma once
#include "VirtualCamera.h"

const double PI = 3.14159265358979323846;

VirtualCamera::VirtualCamera(double height, double width, double fieldOfView) : 
							m_height(height), m_width(width), m_fieldOfView(fieldOfView) {}

VirtualCamera::~VirtualCamera() {}

cv::Mat VirtualCamera::createEmptyFrame() {
	return cv::Mat::zeros(static_cast<int>(m_height), static_cast<int>(m_width), CV_8UC3);
}

bool VirtualCamera::getPixelOffset(const Vector& rocketPos, const Vector& targetPos, double rocketAngleRad, int& pixelX, int& pixelY) {
	Vector toTarget = targetPos - rocketPos;

	double targetAngle = std::atan2(toTarget.getZ(), toTarget.getX());
	double angleDiff = targetAngle - rocketAngleRad;

	while (angleDiff > PI) angleDiff -= 2 * PI;
	while (angleDiff < -PI) angleDiff += 2 * PI;

	if (std::abs(angleDiff) > (m_fieldOfView * PI / 180.0) / 2) {
		return false;
	}

	pixelY = -(angleDiff / m_fieldOfView) * m_height;
	pixelX = 0;
	return true;
}