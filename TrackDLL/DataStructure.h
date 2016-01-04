#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

typedef struct {
	int x;
	int y;
}CPoint;

typedef struct {
	int X, Y;
	int Width, Height;
}CRect;

//摄像头数据
typedef struct {
	string ip; //摄像头IP地址
	int port; //摄像头端口
	string userName;  //摄像头用户名
	string password;  //摄像头密码
}CameraInfo;

//预警结果
typedef struct {
	int ruleID; //违反规则ID
	int ID; //违规目标ID
	Point* trajectories; //违规目标轨迹
	int lenth; //轨迹长度
	Rect position; //违规目标当前位置
	tm cur; //违规时间
	Mat img; //预警图像
}Result;

//入侵检测规则
typedef struct {
	int ID; //规则ID
	vector<Point> vertexes; //入侵检测区域
}InvadeRule;

//停机检测规则
typedef struct {
	int ID; //规则ID
	vector<Point> vertexes; //停机检测区域
}HaltRule;

//错误轨迹规则
typedef struct {
	int ID; //规则ID
	bool isValid; //角度是否有效	
	int angle; //角度
	vector<Point> entryVertexes; //入口区域
	vector<Point> exitVertexes; //出口区域
}WrongTrajectoryRule;

//轨迹冲突规则
typedef struct {
	int ID; //规则ID
	bool isValid; //角度是否有效
	int targetAngle; //目标运动角度
	vector<int> detectAngles; //检测角度
	vector<Point> targetVertexes; //目标区域
	vector<Point> detectVertexes; //检测区域
}ConflictTrajectoryRule;

//跟踪回调函数
typedef void(__stdcall* TrackCallback) (Result* warning, int len);

//目标ID
typedef long long int ID;

//规则信息
enum RuleType{
	None,
	InvadeType,
	HaltType,
	WrongTrajectoryType,
	ConflictTrajectoryType
};

typedef int RuleID;

class RuleInfo {
	friend bool operator < (const RuleInfo& lri, const RuleInfo& rri);
public:
	friend bool operator < (const RuleInfo& lri, const RuleInfo& rri) {
		if (lri.type == rri.type)
			return lri.id < rri.id;
		return lri.type < rri.type;
	}

	RuleInfo(const RuleType rt, const RuleID ri) :
		type(rt),
		id(ri) {

	}

	RuleType type;
	RuleID id;
};

#endif // DATA_STRUCTURE_H