#include "Stitch.h"
#include "Data.h"
#include "Global.h"
#include "windows.h"

Stitch::Stitch(void) {

}

int Stitch::init(const string& parameters) {
	stitcher.initialize(parameters);
	return 1;
}

void FillBitmapInfo(BITMAPINFO* bmi, int width, int height, int bpp, int origin)
{
	assert(bmi && width >= 0 && height >= 0 && 
		(bpp == 8 || bpp == 24 || bpp == 32));

	BITMAPINFOHEADER* bmih = &(bmi->bmiHeader);

	memset(bmih, 0, sizeof(*bmih));
	bmih->biSize = sizeof(BITMAPINFOHEADER);
	bmih->biWidth = width;
	bmih->biHeight = origin ? abs(height) : -abs(height);
	bmih->biPlanes = 1;
	bmih->biBitCount = (unsigned short)bpp;
	bmih->biCompression = BI_RGB;

	if (bpp == 8)
	{
		RGBQUAD* palette = bmi->bmiColors;
		int i;
		for (i = 0; i < 256; i++)
		{
			palette[i].rgbBlue = palette[i].rgbGreen = palette[i].rgbRed = (BYTE)i;
			palette[i].rgbReserved = 0;
		}
	}
}

#include <time.h>  

void Stitch::run(void) {

	//cv::namedWindow("test");
	//HWND hWnd = (HWND)cvGetWindowHandle("test");

	vector<cv::Mat> frames = Data::instance()->read();

	if (frames[0].empty() || frames[1].empty() ||
		frames[2].empty())  {
		return ;
	} else {
		stitcher.process3cam(frames[0], frames[1], frames[2]);
		stitcher.getPanorama(m_cur);

		//show.
		//cvNamedWindow("stitch", 0);
		//imshow("stitch", m_cur);
		//cvWaitKey(1);

		//paint.
		HWND hWnd = (HWND)gDC;

		if (hWnd == NULL || m_cur.empty())
			return ;


		BITMAPINFO bitmapinfo;
		FillBitmapInfo(&bitmapinfo, m_cur.cols, m_cur.rows,
			m_cur.channels() * 8, 0);
		PAINTSTRUCT ptStr;

		int state = 1;
		// 开始绘图 
		HDC hDC = BeginPaint(hWnd, &ptStr);
		//state = StretchDIBits(hDC, 0, 0, m_cur.cols + 14,	\
					m_cur.rows + 35, 0, 0, m_cur.cols, m_cur.rows,	\
					m_cur.data, &bitmapinfo, DIB_RGB_COLORS, SRCCOPY);

		//SetMapMode(hDC, MM_TEXT);          // 设置映射模式 
		HPEN hPen = (HPEN)GetStockObject(WHITE_PEN);
		SelectObject(hDC, hPen);                 // 将画笔选入设备上下文   

		HFONT font = CreateFont(30, 0, 0, 0, FW_BLACK, FALSE, FALSE, FALSE, GB2312_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH | FF_MODERN, L"宋体");
		SelectObject(hDC, font);   //选入字体 
		SetBkMode(hDC, TRANSPARENT);
		TextOut(hDC, 100, 600, L"abc", lstrlen(L"abc"));//输出 

		HBRUSH hBrh = (HBRUSH)(GetStockObject(WHITE_BRUSH));  
		SelectObject(hDC, hBrh);
		RoundRect(hDC, 50, 50, 200, 350, 150, 300);      // 圆角矩形 

		// 结束绘图 
		EndPaint(hWnd, &ptStr);

		RECT rct;
		rct.left = 0;
		rct.right = m_cur.cols;
		rct.top = 0;
		rct.bottom = m_cur.rows;

		/*state = ShowWindow(hWnd, SW_SHOW);
		state = UpdateWindow(hWnd);
		state = InvalidateRect(hWnd, &rct, 1);*/
		
		SendMessage(hWnd, WM_SHOWWINDOW, 0, 0);
		SendMessage(hWnd, WM_SETREDRAW, 0, 0);
		SendMessage(hWnd, WM_PAINT, 0, 0);

		std::cout << "stitch" << std::endl;

		/*cv::String str = cv::format("e:/1/%06d.jpg", cnt++);
		cv::Mat tmp;
		cv::resize(m_cur, tmp, cv::Size(800, 300));
		cv::imwrite(str, tmp);*/

	    //TIMER_BEGIN(1)
		//Mat tmp = m_cur(Rect(0, 0, m_cur.cols / 4 * 4, m_cur.rows)).clone();
		//my_imshow("stitch", tmp);
		//cvWaitKey(1);
		//TIMER_END(1, "clone")
	 
		return ;
	}
}