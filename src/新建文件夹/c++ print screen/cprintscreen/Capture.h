#pragma once
#include <windows.h>

#define LOG(x) printf(x);

class Capture
{
public:
	Capture(HWND window = 0);
	~Capture(void);
	void WriteBMPFile(HBITMAP bitmap, LPTSTR filename, HDC hDC);
	void PrintScreen(char* filename);
	int x;
	int y;
private:
	HWND m_window;
	HDC m_HDC;
	HDC m_memHDC;
	HBITMAP m_bitmap;
};
