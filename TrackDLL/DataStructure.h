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
	char* ip; //摄像头IP地址
	int port; //摄像头端口
	char* userName;  //摄像头用户名
	char* password;  //摄像头密码
}CameraInfo;

//预警结果
typedef struct {
	int ruleID; //违反规则ID
	int ID; //违规目标ID
	CPoint* trajectories; //违规目标轨迹
	int lenth; //轨迹长度
	CRect position; //违规目标当前位置
	//tm cur; //违规时间
	//Mat img; //预警图像
}Result;

//入侵检测规则
typedef struct {
	int ID; //规则ID
	CPoint* vertexes; //入侵检测区域
	int size;
}InvadeRule;

//停机检测规则
typedef struct {
	int ID; //规则ID
	CPoint* vertexes; //停机检测区域
	int size;
}HaltRule;

//错误轨迹规则
typedef struct {
	int ID; //规则ID
	int isValid; //角度是否有效	
	int angle; //角度
	CPoint* entryVertexes; //入口区域
	int entrySize;
	CPoint* exitVertexes; //出口区域
	int exitSize;
}WrongTrajectoryRule;

//轨迹冲突规则
typedef struct {
	int ID; //规则ID
	int isValid; //角度是否有效
	int targetAngle; //目标运动角度
	int* detectAngles; //检测角度
	int angleSize;
	CPoint* targetVertexes; //目标区域
	int targetSize;
	CPoint* detectVertexes; //检测区域
	int detectSize;
}ConflictTrajectoryRule;

//跟踪回调函数
//typedef void(__stdcall* TrackCallback) (Result* warning, int len);
typedef void(__stdcall* TrackCallback) (CRect* warning, int len);

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