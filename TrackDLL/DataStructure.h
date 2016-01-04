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

//����ͷ����
typedef struct {
	string ip; //����ͷIP��ַ
	int port; //����ͷ�˿�
	string userName;  //����ͷ�û���
	string password;  //����ͷ����
}CameraInfo;

//Ԥ�����
typedef struct {
	int ruleID; //Υ������ID
	int ID; //Υ��Ŀ��ID
	Point* trajectories; //Υ��Ŀ��켣
	int lenth; //�켣����
	Rect position; //Υ��Ŀ�굱ǰλ��
	tm cur; //Υ��ʱ��
	Mat img; //Ԥ��ͼ��
}Result;

//���ּ�����
typedef struct {
	int ID; //����ID
	vector<Point> vertexes; //���ּ������
}InvadeRule;

//ͣ��������
typedef struct {
	int ID; //����ID
	vector<Point> vertexes; //ͣ���������
}HaltRule;

//����켣����
typedef struct {
	int ID; //����ID
	bool isValid; //�Ƕ��Ƿ���Ч	
	int angle; //�Ƕ�
	vector<Point> entryVertexes; //�������
	vector<Point> exitVertexes; //��������
}WrongTrajectoryRule;

//�켣��ͻ����
typedef struct {
	int ID; //����ID
	bool isValid; //�Ƕ��Ƿ���Ч
	int targetAngle; //Ŀ���˶��Ƕ�
	vector<int> detectAngles; //���Ƕ�
	vector<Point> targetVertexes; //Ŀ������
	vector<Point> detectVertexes; //�������
}ConflictTrajectoryRule;

//���ٻص�����
typedef void(__stdcall* TrackCallback) (Result* warning, int len);

//Ŀ��ID
typedef long long int ID;

//������Ϣ
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