#include <core/cuda.hpp>

#include "Detect.h"
#include "Global.h"
#include "Conversion.h"
#include "DetectManager.h"
#include "CameraManager.h"
#include "ContourFilterManager.h"

int DetectInit(const CameraInfo& camera, const string& param) {

	int num_devices = cv::cuda::getCudaEnabledDeviceCount();

	if (0 == num_devices) {
		std::cerr << "GPU error." << std::endl;
		return -1;
	}

	if (false == readGlobal("Detect.ini")) {
		std::cerr << "Configure error." << std::endl;
		return -2;
	}

	if (false == Conversion::instance()->init(param)) {
		std::cerr << "Param error." << std::endl;
		return -3;
	}

	ContourFilterManager::instance()->init();

	vector<cv::Rect> rects(1, gWholeRect);
	DetectManager::instance()->setRect(rects);

	CameraManager::instance()->add(camera.ip.c_str(), camera.port, 
		camera.userName.c_str(), camera.password.c_str());

	if (false == CameraManager::instance()->login()) {
		std::cerr << "Camera Login error." << std::endl;
		return -4;
	}

	return 0;
}

bool setDetectCallback(DetectCallback ptr) {
	if (nullptr == ptr) {
		std::cerr << "ptr is null." << std::endl;
		return false;
	} else {
		gCallback = ptr;
		return true;
	}
}

bool DetectStart(void) {
	return CameraManager::instance()->start();
}

void DetectRelease() {
	CameraManager::instance()->release();
}