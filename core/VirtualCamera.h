#include <opencv2/opencv.hpp>
#include "Vector.h"

class VirtualCamera {
public:
	VirtualCamera(double height, double width, double fieldOfView);
	~VirtualCamera();

	cv::Mat createEmptyFrame();
	bool getPixelOffset(const Vector& rocketPos, const Vector& targetPos, double rocketAngleRad, int& pixelX, int& pixelY);


private:
	double m_height;
	double m_width;
	double m_fieldOfView;
};