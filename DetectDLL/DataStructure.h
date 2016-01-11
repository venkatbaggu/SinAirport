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

//摄像头数据
typedef struct {
	char* ip; //摄像头IP地址
	int port; //摄像头端口
	char* userName;  //摄像头用户名
	char* password;  //摄像头密码
}CameraInfo;

//检测回调函数
typedef void(_stdcall* DetectCallback) (CRect* data, int len);

//目标ID
typedef long long int ID;

#endif // DATA_STRUCTURE_H