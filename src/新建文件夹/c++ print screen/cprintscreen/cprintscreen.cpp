// cprintscreen.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Capture.h"

/** these are optional if you want to display th image, or process it
	#include "CImg.h"
	using namespace cimg_library;
*/

int _tmain(int argc, _TCHAR* argv[])
{
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	//so we don' print the console
	Sleep(200);
	Capture c;
	c.PrintScreen("D:\\a.bmp");
	return 0;
}

