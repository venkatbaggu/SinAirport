#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

#include <ctime>
#include <vector>
#include <string>
using std::tm;
using std::vector;
using std::string;

#include <opencv.hpp>
using cv::Mat;
using cv::Rect; 
using cv::Point;

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
	vector<cv::Point> vertexes; //���ּ������
}InvadeRule;

//ͣ��������
typedef struct {
	int ID; //����ID
	vector<cv::Point> vertexes; //ͣ���������
}HaltRule;

//����켣����
typedef struct {
	int ID; //����ID
	bool isValid; //�Ƕ��Ƿ���Ч	
	int angle; //�Ƕ�
	vector<cv::Point> entryVertexes; //�������
	vector<cv::Point> exitVertexes; //��������
}WrongTrajectoryRule;

//�켣��ͻ����
typedef struct {
	int ID; //����ID
	bool isValid; //�Ƕ��Ƿ���Ч
	int targetAngle; //Ŀ���˶��Ƕ�
	vector<int> detectAngles; //���Ƕ�
	vector<cv::Point> targetVertexes; //Ŀ������
	vector<cv::Point> detectVertexes; //�������
}ConflictTrajectoryRule;

//���ص�����
typedef void(__stdcall* DetectCallback) (const vector<Rect>& objs);

//���ٻص�����
typedef void(__stdcall* TrackCallback) (vector<Result> warnigns);

//Ŀ��ID
typedef long long int ID;

#endif // DATA_STRUCTURE_H