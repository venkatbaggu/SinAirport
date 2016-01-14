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
	char* ip; //����ͷIP��ַ
	int port; //����ͷ�˿�
	char* userName;  //����ͷ�û���
	char* password;  //����ͷ����
}CameraInfo;

//Ԥ�����
typedef struct {
	int ruleID; //Υ������ID
	int ID; //Υ��Ŀ��ID
	CPoint* trajectories; //Υ��Ŀ��켣
	int lenth; //�켣����
	CRect position; //Υ��Ŀ�굱ǰλ��
	//tm cur; //Υ��ʱ��
	//Mat img; //Ԥ��ͼ��
}Result;

//���ּ�����
typedef struct {
	int ID; //����ID
	CPoint* vertexes; //���ּ������
	int size;
}InvadeRule;

//ͣ��������
typedef struct {
	int ID; //����ID
	CPoint* vertexes; //ͣ���������
	int size;
}HaltRule;

//����켣����
typedef struct {
	int ID; //����ID
	int isValid; //�Ƕ��Ƿ���Ч	
	int angle; //�Ƕ�
	CPoint* entryVertexes; //�������
	int entrySize;
	CPoint* exitVertexes; //��������
	int exitSize;
}WrongTrajectoryRule;

//�켣��ͻ����
typedef struct {
	int ID; //����ID
	int isValid; //�Ƕ��Ƿ���Ч
	int targetAngle; //Ŀ���˶��Ƕ�
	int* detectAngles; //���Ƕ�
	int angleSize;
	CPoint* targetVertexes; //Ŀ������
	int targetSize;
	CPoint* detectVertexes; //�������
	int detectSize;
}ConflictTrajectoryRule;

//���ٻص�����
//typedef void(__stdcall* TrackCallback) (Result* warning, int len);
typedef void(__stdcall* TrackCallback) (CRect* warning, int len);

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