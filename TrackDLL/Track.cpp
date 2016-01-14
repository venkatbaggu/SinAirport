#include "Track.h"
#include "Global.h"
#include "Stitch.h"

#include "TargetManager.h"
#include "CameraManager.h"
#include "WarningManager.h"
#include "TrackingManager.h"

#include "Halt.h"
#include "Invade.h"
#include "WrongTrajectory.h"
#include "ConflictTrajectory.h"

int __declspec(dllexport) 
	TrackInit(void* pDC, CameraInfo* cameras, int size) {

	if (false == readGlobal("Track.ini")) {
		std::cerr << "Read Config error." << std::endl;
		return -1;
	}

	if (nullptr == pDC) {
		std::cerr << "dc error." << std::endl;
		return -2; 
	} else {
		gDC = pDC;
	}

	for (int idx = 0; idx < size; ++idx) {
		CameraManager::instance()->add(cameras[idx].ip, cameras[idx].port,
			cameras[idx].userName, cameras[idx].password);
	}

	if (false == CameraManager::instance()->login()) {
		std::cerr << "Camera Login error." << std::endl;
		return -3;
	}

	if (false == Stitch::instance()->init("stitch.xml")) {
		std::cerr << "Read Param error." << std::endl;
		return -4;
	}

	return 0;
}

int __declspec(dllexport) setTrackCallback(TrackCallback ptr) {

	if (nullptr == ptr) {
		std::cerr << "ptr is null." << std::endl;
		return -1;
	} else {
		gCallback = ptr;
	}

	return 0;
}

void TrackRun(CRect** objs, int row, int col) {
	vector<vector<cv::Rect>> vecObjs(row, vector<cv::Rect>(col));

	for (int r = 0; r < row; ++r) {
		for (int c = 0; c < col; ++c) {
			vecObjs[r][c].x = objs[r][c].X;
			vecObjs[r][c].y = objs[r][c].Y;
			vecObjs[r][c].width = objs[r][c].Width;
			vecObjs[r][c].height = objs[r][c].Height;
		}
	}

	vector<cv::Rect> obj = mergeAll(vecObjs);

	TrackingManager::instance()->update(obj);

	map<RuleInfo, std::set<ID>> alerts;
	WarningManager::instance()->warning(alerts);

	

	//int len = results.size();
	//Result* res = new Result[len];

	//for (int idx = 0; idx < len; ++idx) {
	//	res[idx].ID
	//}

	//(*gCallback)(res, len);

	return ;
}

void clearRules(void) {
	WarningManager::instance()->release();
	return;
}

int updateInvadeRule(InvadeRule* rules, int size) {
	list<unique_ptr<IWarning>> dts;

	for (int idx = 0; idx < size; ++idx) {
		vector<cv::Point> tmp(rules[idx].size);

		for (int i = 0; i < size; ++i) {
			tmp[i].x = rules[idx].vertexes[i].x;
			tmp[i].y = rules[idx].vertexes[i].y;
		}

		dts.emplace_back(new Invade(rules[idx].ID, tmp));
	}

	WarningManager::instance()->reset(dts);
	return 0;
}

int updateHaltRule(HaltRule* rules, int size) {
	list<unique_ptr<IWarning>> dts;

	for (int idx = 0; idx < size; ++idx) {
		vector<cv::Point> tmp(rules[idx].size);

		for (int i = 0; i < size; ++i) {
			tmp[i].x = rules[idx].vertexes[i].x;
			tmp[i].y = rules[idx].vertexes[i].y;
		}

		dts.emplace_back(new Halt(rules[idx].ID, tmp));
	}

	WarningManager::instance()->reset(dts);
	return 0;
}

int updateWrongTrajectoryRule(
			WrongTrajectoryRule* rules, int size) {
	list<unique_ptr<IWarning>> dts;

	for (int idx = 0; idx < size; ++idx) {

		vector<cv::Point> tmpEntry(rules[idx].entrySize);
		for (int i = 0; i < rules[idx].entrySize; ++i) {
			tmpEntry[i].x = rules[idx].entryVertexes[i].x;
			tmpEntry[i].y = rules[idx].entryVertexes[i].y;
		}

		vector<cv::Point> tmpExit(rules[idx].exitSize);
		for (int i = 0; i < rules[idx].exitSize; ++i) {
			tmpExit[i].x = rules[idx].exitVertexes[i].x;
			tmpExit[i].y = rules[idx].exitVertexes[i].y;
		}

		dts.emplace_back(
			new WrongTrajectory(rules[idx].ID
			, tmpEntry
			, tmpExit
			, rules[idx].isValid
			, rules[idx].angle));
	}

	WarningManager::instance()->reset(dts);
	return 0;
}

int updateConflictTrajectoryRule(
			ConflictTrajectoryRule* rules, int size) {
	
	list<unique_ptr<IWarning>> dts;

	for (int idx = 0; idx < size; ++idx) {

		vector<cv::Point> tmpTarget(rules[idx].targetSize);
		for (int i = 0; i < rules[idx].targetSize; ++i) {
			tmpTarget[i].x = rules[idx].targetVertexes[i].x;
			tmpTarget[i].y = rules[idx].targetVertexes[i].y;
		}

		vector<cv::Point> tmpDetect(rules[idx].detectSize);
		for (int i = 0; i < rules[idx].detectSize; ++i) {
			tmpDetect[i].x = rules[idx].detectVertexes[i].x;
			tmpDetect[i].y = rules[idx].detectVertexes[i].y;
		}

		dts.emplace_back(
			new ConflictTrajectory(rules[idx].ID
			, tmpTarget
			, tmpDetect
			, rules[idx].isValid
			, rules[idx].targetAngle
			, vector<int>(rules[idx].detectAngles,
			rules[idx].detectAngles + rules[idx].angleSize)));
	}

	WarningManager::instance()->reset(dts);
	return 0;
}

void TrackRelease(void) {
	CameraManager::instance()->release();
}

int TrackStart(void) {

	if (false == CameraManager::instance()->start()) {
		std::cerr << "Start Camera error." << std::endl;
		return -1;
	}

	return 0;
}