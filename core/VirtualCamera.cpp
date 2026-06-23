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

void VirtualCamera::renderUHD(cv::Mat& Frame, bool isVisible, int PixelX, int PixelY) {
	cv::line(Frame, cv::Point(m_width / 2 - 15, m_height / 2), cv::Point(m_width / 2 + 15, m_height / 2), cv::Scalar(0,255,0), 1);
	cv::line(Frame, cv::Point(m_width / 2, m_height / 2 - 15), cv::Point(m_width / 2, m_height / 2 + 15), cv::Scalar(0, 255, 0), 1);

	if (isVisible) {
		cv::circle(Frame, cv::Point(m_width / 2 + PixelX, m_height / 2 + PixelY), 10, cv::Scalar(0, 0, 255), 1);
		cv::putText(Frame, "LOCK ON", cv::Point(m_width / 2 + PixelX - 30, m_height / 2 + PixelY - 15), cv::FONT_HERSHEY_PLAIN, 1.0, cv::Scalar(0, 255, 0), 1, cv::LINE_AA);
	}
	else {
		cv::putText(Frame, "SEARCHING...", cv::Point(m_width / 2 - 40, m_height / 2 + 5), cv::FONT_HERSHEY_PLAIN, 1.0, cv::Scalar(0, 255, 0), 1, cv::LINE_AA);
	}

	cv::putText(Frame, "SEEKER MODE: ACTIVE", cv::Point(20, 30), cv::FONT_HERSHEY_PLAIN, 1.0, cv::Scalar(0, 255, 0), 1, cv::LINE_AA);
}

QImage VirtualCamera::convertMatToQImage(const cv::Mat& src) {
	cv::Mat tempMat;
	cv::cvtColor(src, tempMat, cv::COLOR_BGR2RGB);

	QImage image(tempMat.data, tempMat.cols, tempMat.rows, tempMat.step, QImage::Format_RGB888);

	return image.copy();
}