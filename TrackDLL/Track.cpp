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

int __declspec(dllexport) TrackInit(void* pDC,
					const vector<CameraInfo>& cameras) {

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

	for (auto camera : cameras) {
		CameraManager::instance()->add(camera.ip.c_str(), camera.port,
			camera.userName.c_str(), camera.password.c_str());
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

void TrackRun(vector<vector<cv::Rect>> objs) {
	vector<cv::Rect> obj = mergeAll(objs);

	TrackingManager::instance()->update(obj);

	map<RuleInfo, std::set<ID>> alerts;
	WarningManager::instance()->warning(alerts);

	if (false == alerts.empty()) {
		vector<Result> results;

		(*gCallback)(results);
	}

	return ;
}

void clearRules(void) {
	WarningManager::instance()->release();
	return;
}

bool updateInvadeRule(const vector<InvadeRule>& rules) {
	list<unique_ptr<IWarning>> dts;

	const auto end = rules.end();
	for (auto iter = rules.begin();
		end != iter; ++iter) {
		dts.emplace_back(new Invade((*iter).ID, (*iter).vertexes));
	}

	WarningManager::instance()->reset(dts);
	return true;
}

bool updateHaltRule(vector<HaltRule> rules) {
	list<unique_ptr<IWarning>> dts;

	const auto end = rules.end();
	for (auto iter = rules.begin();
		end != iter; ++iter) {
		dts.emplace_back(new Halt((*iter).ID, (*iter).vertexes));
	}

	WarningManager::instance()->reset(dts);
	return true;
}

bool updateWrongTrajectoryRule(
			vector<WrongTrajectoryRule> rules) {
	list<unique_ptr<IWarning>> dts;

	const auto end = rules.end();
	for (auto iter = rules.begin();
		end != iter; ++iter) {
		dts.emplace_back(
			new WrongTrajectory((*iter).ID
			, (*iter).entryVertexes
			, (*iter).exitVertexes
			, (*iter).isValid
			, (*iter).angle));
	}

	WarningManager::instance()->reset(dts);
	return true;
}

bool updateConflictTrajectoryRule(
			vector<ConflictTrajectoryRule> rules) {
	
	list<unique_ptr<IWarning>> dts;

	const auto end = rules.end();
	for (auto iter = rules.begin();
		end != iter; ++iter) {
		dts.emplace_back(
			new ConflictTrajectory((*iter).ID
			, (*iter).targetVertexes
			, (*iter).detectVertexes
			, (*iter).isValid
			, (*iter).targetAngle
			, (*iter).detectAngles));
	}

	WarningManager::instance()->reset(dts);
	return true;
}

void TrackRelease(void) {
	CameraManager::instance()->release();
}

bool TrackStart(void) {

	if (false == CameraManager::instance()->start()) {
		std::cerr << "Start Camera error." << std::endl;
		return false;
	}

	return true;
}