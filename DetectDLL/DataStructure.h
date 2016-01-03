#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

#include <string>
using std::string;

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
	int len; //�켣����
	CRect position; //Υ��Ŀ�굱ǰλ��
	//tm cur; //Υ��ʱ��
	//Mat img; //Ԥ��ͼ��
}Result;

//���ּ�����
typedef struct {
	int ID; //����ID
	CPoint* vertexes; //���ּ������
	int len;
}InvadeRule;

//ͣ��������
typedef struct {
	int ID; //����ID
	CPoint* vertexes; //ͣ���������
	int len;
}HaltRule;

//����켣����
typedef struct {
	int ID; //����ID
	bool isValid; //�Ƕ��Ƿ���Ч	
	int angle; //�Ƕ�

	CPoint* entryVertexes; //�������
	int entryLen;

	CPoint* exitVertexes; //��������
	int exitLen;
}WrongTrajectoryRule;

//�켣��ͻ����
typedef struct {
	int ID; //����ID
	bool isValid; //�Ƕ��Ƿ���Ч
	int targetAngle; //Ŀ���˶��Ƕ�

	int* detectAngles; //���Ƕ�
	int angleLen;

	CPoint* targetVertexes; //Ŀ������
	int targetLen;

	CPoint detectVertexes; //�������
	int detectLen;
}ConflictTrajectoryRule;

//���ص�����
typedef void(__stdcall* DetectCallback) (CRect* data, int len);

//���ٻص�����
typedef void(__stdcall* TrackCallback) (Result* warning, int len);

//Ŀ��ID
typedef long long int ID;

#endif // DATA_STRUCTURE_H