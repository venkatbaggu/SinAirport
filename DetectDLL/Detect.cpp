#include <core/cuda.hpp>

#include "Detect.h"
#include "Global.h"
#include "Conversion.h"
#include "DetectManager.h"
#include "CameraManager.h"
#include "ContourFilterManager.h"

int DetectInit(CameraInfo camera, char* param) {

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

	CameraManager::instance()->add(camera.ip, camera.port, 
		camera.userName, camera.password);

	if (false == CameraManager::instance()->login()) {
		std::cerr << "Camera Login error." << std::endl;
		return -4;
	}

	return 0;
}

int setDetectCallback(DetectCallback ptr) {
	if (nullptr == ptr) {
		std::cerr << "ptr is null." << std::endl;
		return -1;
	} else {
		gCallback = ptr;
		return 0;
	}
}

int DetectStart(void) {
	if (false == CameraManager::instance()->start()) {
		std::cerr << "Start Camera error." << std::endl;
		return -1;
	} 
		
	return 0;	
}

void DetectRelease() {
	CameraManager::instance()->release();
}