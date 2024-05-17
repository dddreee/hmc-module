#include "StdAfx.h"
#include "Capture.h"

/** http://www.saturnsoftmills.com/Articles/a6.htm */
void Capture::WriteBMPFile(HBITMAP bitmap, LPTSTR filename, HDC hDC) {
	BITMAP bmp; 
	PBITMAPINFO pbmi; 
	WORD cClrBits; 
	HANDLE hf; // file handle 
	BITMAPFILEHEADER hdr; // bitmap file-header 
	PBITMAPINFOHEADER pbih; // bitmap info-header 
	LPBYTE lpBits; // memory pointer 
	DWORD dwTotal; // total count of bytes 
	DWORD cb; // incremental count of bytes 
	BYTE *hp; // byte pointer 
	DWORD dwTmp; 
	if (!GetObject(bitmap, sizeof(BITMAP), (LPSTR)&bmp)) {
		LOG("[ERROR] could not retrieve bitmap info");
		return;
	}
	// convert the color format to a count of bits. 
	cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel); 
	if (cClrBits == 1) 
		cClrBits = 1; 
	else if (cClrBits <= 4) 
		cClrBits = 4; 
	else if (cClrBits <= 8) 
		cClrBits = 8; 
	else if (cClrBits <= 16) 
		cClrBits = 16; 
	else if (cClrBits <= 24) 
		cClrBits = 24; 
	else cClrBits = 32; 
	// Allocate memory for the BITMAPINFO structure.
	if (cClrBits != 24) 
		pbmi = (PBITMAPINFO) LocalAlloc(LPTR, 
						sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * (1<< cClrBits)); 
	else 
		pbmi = (PBITMAPINFO) LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER)); 
	// Initialize the fields in the BITMAPINFO structure. 
	pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER); 
	pbmi->bmiHeader.biWidth = bmp.bmWidth; 
	pbmi->bmiHeader.biHeight = bmp.bmHeight; 
	pbmi->bmiHeader.biPlanes = bmp.bmPlanes; 
	pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel; 
	if (cClrBits < 24) 
		pbmi->bmiHeader.biClrUsed = (1<<cClrBits); 
	// If the bitmap is not compressed, set the BI_RGB flag. 
	pbmi->bmiHeader.biCompression = BI_RGB; 
	// Compute the number of bytes in the array of color 
	// indices and store the result in biSizeImage. 
	pbmi->bmiHeader.biSizeImage = (pbmi->bmiHeader.biWidth + 7) /8 * pbmi->bmiHeader.biHeight * cClrBits; 
	// Set biClrImportant to 0, indicating that all of the 
	// device colors are important. 
	pbmi->bmiHeader.biClrImportant = 0; 
	// now open file and save the data
	pbih = (PBITMAPINFOHEADER) pbmi; 
	lpBits = (LPBYTE) GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);
	if (!lpBits) {
		LOG("[ERROR] could not allocate memory");
		return;
	}
	// Retrieve the color table (RGBQUAD array) and the bits 
	if (!GetDIBits(hDC, HBITMAP(bitmap), 0, (WORD) pbih->biHeight, lpBits, pbmi, 
	DIB_RGB_COLORS)) {
		LOG("GetDIB error");
		return;
	}
	// create the .bmp file. 
	hf = CreateFile(filename, GENERIC_READ | GENERIC_WRITE, (DWORD) 0, 
	NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 
	(HANDLE) NULL); 
	if (hf == INVALID_HANDLE_VALUE){
		LOG("[ERROR]could not create file for writing");
		return;
	}
	hdr.bfType = 0x4d42; // 0x42 = "B" 0x4d = "M" 
	// Compute the size of the entire file. 
	hdr.bfSize = (DWORD) (sizeof(BITMAPFILEHEADER) + 
	pbih->biSize + pbih->biClrUsed 
	* sizeof(RGBQUAD) + pbih->biSizeImage); 
	hdr.bfReserved1 = 0; 
	hdr.bfReserved2 = 0; 
	// Compute the offset to the array of color indices. 
	hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) + 
	pbih->biSize + pbih->biClrUsed 
	* sizeof (RGBQUAD);
	// Copy the BITMAPFILEHEADER into the .BMP file. 
	if (!WriteFile(hf, (LPVOID) &hdr, sizeof(BITMAPFILEHEADER), 
	(LPDWORD) &dwTmp, NULL)) {
		LOG("[ERROR]could not write in to file");
		return;
	}
	// Copy the BITMAPINFOHEADER and RGBQUAD array into the file. 
	if (!WriteFile(hf, (LPVOID) pbih, sizeof(BITMAPINFOHEADER) 
	+ pbih->biClrUsed * sizeof (RGBQUAD), 
	(LPDWORD) &dwTmp, ( NULL))){
		LOG("[ERROR]could not write in to file");
		return;
	}
	// Copy the array of color indices into the .BMP file. 
	dwTotal = cb = pbih->biSizeImage; 
	hp = lpBits; 
	if (!WriteFile(hf, (LPSTR) hp, (int) cb, (LPDWORD) &dwTmp,NULL)){
		LOG("[ERROR]could not write in to file");
		return;
	}
	// Close the .BMP file. 
	if (!CloseHandle(hf)){
		LOG("could not close file");
		return;
	}
	GlobalFree((HGLOBAL)lpBits);
}

/** from http://www.voidnish.com/Articles/ShowArticle.aspx?code=ijwscrcap */
Capture::Capture(HWND window) {
	m_window = window;
}

void Capture::PrintScreen(char* filename) {
	WCHAR w_filename[MAX_PATH];
	RECT r;

	/** convert filename */
	MultiByteToWideChar(CP_ACP, 0, filename, -1, w_filename, MAX_PATH); 

	m_HDC = GetDC(m_window);
	m_memHDC = CreateCompatibleDC(m_HDC);
	GetWindowRect(GetDesktopWindow(),&r); //need this for Form
	x = GetSystemMetrics(SM_CXSCREEN);
    	y = GetSystemMetrics(SM_CYSCREEN);
	m_bitmap = CreateCompatibleBitmap(m_HDC, x, y);
	if (m_bitmap) {
		HBITMAP old = (HBITMAP) SelectObject(m_memHDC, m_bitmap);
		BitBlt(m_memHDC, 0, 0, x, y, m_HDC, 0, 0, SRCCOPY);
		SelectObject(m_memHDC, old);
		DeleteDC(m_memHDC);
		ReleaseDC(NULL, m_HDC);
		WriteBMPFile(m_bitmap, w_filename, GetDC(m_window));
		DeleteObject(m_bitmap);
	} else {
		LOG("[ERROR] in CreateCompatibleBitmap\n");
	}
}

Capture::~Capture(void)
{
}
