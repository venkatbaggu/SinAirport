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
	int Width;
	int Height;
}CRect;

//����ͷ����
typedef struct {
	char* ip; //����ͷIP��ַ
	int port; //����ͷ�˿�
	char* userName;  //����ͷ�û���
	char* password;  //����ͷ����
}CameraInfo;

//���ص�����
typedef void(_stdcall* DetectCallback) (CRect* data, int len);

//Ŀ��ID
typedef long long int ID;

#endif // DATA_STRUCTURE_H