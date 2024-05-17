#include "hmc_screen_util.h"

std::vector<RECT> hmc_screen_util::GetDeviceCapsAll()
{
    std::vector<RECT> CrectList;
    try
    {
        DISPLAY_DEVICE displayDevice;
        ::ZeroMemory(&displayDevice, sizeof(displayDevice));
        displayDevice.cb = sizeof(displayDevice);
        DEVMODE devMode;
        ::ZeroMemory(&devMode, sizeof(devMode));
        devMode.dmSize = sizeof(devMode);

        for (int i = 0; ::EnumDisplayDevices(NULL, i, &displayDevice, 0); ++i)
        {
            if (::EnumDisplaySettings(displayDevice.DeviceName, ENUM_CURRENT_SETTINGS, &devMode))
            {
                int left = devMode.dmPosition.x;
                int top = devMode.dmPosition.y;
                int right = devMode.dmPosition.x + devMode.dmPelsWidth;
                int bottom = devMode.dmPosition.y + devMode.dmPelsHeight;
                RECT rect;
                rect.bottom = bottom;
                rect.left = left;
                rect.top = top;
                rect.right = right;
                CrectList.push_back(rect);
            }
        }
    }
    catch (...)
    {
    }

    return CrectList;
}

bool hmc_screen_util::captureBmpToFile(std::variant<std::string, std::wstring> output, int upperLeftX, int upperLeftY, int nScopeWidth, int nScopeHeight)
{
    try
    {

        // 获取屏幕DC
        HDC hScreenDC = ::CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
        // 获取屏幕分辨率
        if (nScopeWidth == 0 || nScopeWidth == NULL)
        {
            nScopeWidth = ::GetDeviceCaps(hScreenDC, HORZRES);
        }
        if (nScopeHeight == 0 || nScopeHeight == NULL)
        {
            nScopeHeight = ::GetDeviceCaps(hScreenDC, VERTRES);
        }
        // 创建与屏幕DC兼容的内存DC
        HDC hMemoryDC = ::CreateCompatibleDC(hScreenDC);
        // 创建位图对象
        HBITMAP hBitmap = ::CreateCompatibleBitmap(hScreenDC, nScopeWidth, nScopeHeight);
        // 将位图选入内存DC中
        HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hMemoryDC, hBitmap);
        // 将屏幕内容拷贝到内存DC中
        ::BitBlt(hMemoryDC, 0, 0, nScopeWidth, nScopeHeight, hScreenDC, upperLeftX, upperLeftY, SRCCOPY);
        // 将位图保存为文件
        PBITMAPINFO pBitmapInfo = (PBITMAPINFO) new char[sizeof(BITMAPINFOHEADER)];
        ::memset(pBitmapInfo, 0, sizeof(BITMAPINFOHEADER));
        pBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        pBitmapInfo->bmiHeader.biWidth = nScopeWidth;
        pBitmapInfo->bmiHeader.biHeight = nScopeHeight;
        pBitmapInfo->bmiHeader.biPlanes = 1;
        pBitmapInfo->bmiHeader.biBitCount = 24;
        pBitmapInfo->bmiHeader.biCompression = BI_RGB;
        pBitmapInfo->bmiHeader.biSizeImage = nScopeWidth * nScopeHeight * 3;
        char *pData = NULL;

        std::shared_ptr<void> _shared_close_Library_hMemoryDC(nullptr, [&](void *)
                                                              {
            try
        {
                ::SelectObject(hMemoryDC, hOldBitmap);
                ::DeleteDC(hScreenDC);
                ::DeleteDC(hMemoryDC);
                ::DeleteObject(hBitmap);
        }
            catch (...) {} });

        HGLOBAL hGlobal = ::GlobalAlloc(GMEM_MOVEABLE, pBitmapInfo->bmiHeader.biSizeImage);

        if (hGlobal == NULL)
        {
            return false;
        }

        std::shared_ptr<void> _shared_close_Library_hGlobal(nullptr, [&](void *)
                                                            {
            try
            {
                ::GlobalUnlock(hGlobal);
                ::GlobalFree(hGlobal);
            }
            catch (...) {} });

        pData = (char *)::GlobalLock(hGlobal);

        if (pData == NULL)
        {

            return false;
        }

        ::GetDIBits(hMemoryDC, hBitmap, 0, nScopeHeight, pData, pBitmapInfo, DIB_RGB_COLORS);
        DWORD dwBytesWritten = 0;
        BITMAPFILEHEADER bmfHeader;
        bmfHeader.bfType = 0x4d42;
        bmfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + pBitmapInfo->bmiHeader.biSize;
        bmfHeader.bfSize = bmfHeader.bfOffBits + pBitmapInfo->bmiHeader.biSizeImage;

        if (std::holds_alternative<std::wstring>(output))
        {
            // 创建文件句柄
            HANDLE hFile = ::CreateFileW(std::get<std::wstring>(output).c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
            // 写入魔术头
            ::WriteFile(hFile, &bmfHeader, sizeof(bmfHeader), &dwBytesWritten, NULL);
            // 写入bmp文件头
            ::WriteFile(hFile, pBitmapInfo, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
            // 写入bmp内容
            ::WriteFile(hFile, pData, pBitmapInfo->bmiHeader.biSizeImage, &dwBytesWritten, NULL);
            ::CloseHandle(hFile);
        }
        else if (std::holds_alternative<std::string>(output))
        {
            HANDLE hFile = ::CreateFileA(std::get<std::string>(output).c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
            ::WriteFile(hFile, &bmfHeader, sizeof(bmfHeader), &dwBytesWritten, NULL);
            ::WriteFile(hFile, pBitmapInfo, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
            ::WriteFile(hFile, pData, pBitmapInfo->bmiHeader.biSizeImage, &dwBytesWritten, NULL);
            ::CloseHandle(hFile);
        }

    }
    catch (...)
    {
      
        return false;
    }
    return true;
}

bool hmc_screen_util::captureBmpToBuffer(std::vector<std::uint8_t> &output, int upperLeftX, int upperLeftY, int nScopeWidth, int nScopeHeight)
{
    try
    {

        // 获取屏幕DC
        HDC hScreenDC = ::CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
        // 获取屏幕分辨率
        if (nScopeWidth == 0 || nScopeWidth == NULL)
        {
            nScopeWidth = ::GetDeviceCaps(hScreenDC, HORZRES);
        }
        if (nScopeHeight == 0 || nScopeHeight == NULL)
        {
            nScopeHeight = ::GetDeviceCaps(hScreenDC, VERTRES);
        }
        // 创建与屏幕DC兼容的内存DC
        HDC hMemoryDC = ::CreateCompatibleDC(hScreenDC);
        // 创建位图对象
        HBITMAP hBitmap = ::CreateCompatibleBitmap(hScreenDC, nScopeWidth, nScopeHeight);
        // 将位图选入内存DC中
        HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hMemoryDC, hBitmap);
        // 将屏幕内容拷贝到内存DC中
        ::BitBlt(hMemoryDC, 0, 0, nScopeWidth, nScopeHeight, hScreenDC, upperLeftX, upperLeftY, SRCCOPY);
        // 将位图保存为文件
        PBITMAPINFO pBitmapInfo = (PBITMAPINFO) new char[sizeof(BITMAPINFOHEADER)];
        ::memset(pBitmapInfo, 0, sizeof(BITMAPINFOHEADER));
        pBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        pBitmapInfo->bmiHeader.biWidth = nScopeWidth;
        pBitmapInfo->bmiHeader.biHeight = nScopeHeight;
        pBitmapInfo->bmiHeader.biPlanes = 1;
        pBitmapInfo->bmiHeader.biBitCount = 24;
        pBitmapInfo->bmiHeader.biCompression = BI_RGB;
        pBitmapInfo->bmiHeader.biSizeImage = nScopeWidth * nScopeHeight * 3;
        char *pData = NULL;

        std::shared_ptr<void> _shared_close_Library_hMemoryDC(nullptr, [&](void *)
                                                              {
            try
        {
                ::SelectObject(hMemoryDC, hOldBitmap);
                ::DeleteDC(hScreenDC);
                ::DeleteDC(hMemoryDC);
                ::DeleteObject(hBitmap);
        }
            catch (...) {} });

        HGLOBAL hGlobal = ::GlobalAlloc(GMEM_MOVEABLE, pBitmapInfo->bmiHeader.biSizeImage);

        if (hGlobal == NULL)
        {
            return false;
        }

        std::shared_ptr<void> _shared_close_Library_hGlobal(nullptr, [&](void *)
                                                            {
            try
            {
                ::GlobalUnlock(hGlobal);
                ::GlobalFree(hGlobal);
            }
            catch (...) {} });

        pData = (char *)::GlobalLock(hGlobal);

        if (pData == NULL)
        {

            return false;
        }

        ::GetDIBits(hMemoryDC, hBitmap, 0, nScopeHeight, pData, pBitmapInfo, DIB_RGB_COLORS);
        DWORD dwBytesWritten = 0;
        BITMAPFILEHEADER bmfHeader;
        bmfHeader.bfType = 0x4d42;
        bmfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + pBitmapInfo->bmiHeader.biSize;
        bmfHeader.bfSize = bmfHeader.bfOffBits + pBitmapInfo->bmiHeader.biSizeImage;

        output.clear();
        output.reserve(bmfHeader.bfSize);

        // 写入魔术头
        char *bmfHeader_Temp = (char *)&bmfHeader;

        if (bmfHeader_Temp != NULL)
        {
            for (size_t i = 0; i < sizeof(bmfHeader); i++)
            {
                output.push_back(bmfHeader_Temp[i]);
            }
        }

        // 写入魔术头
        char *pBitmapInfo_Temp = (char *)pBitmapInfo;

        if (pBitmapInfo_Temp != NULL)
        {
            for (size_t i = 0; i < sizeof(BITMAPINFOHEADER); i++)
            {
                output.push_back(pBitmapInfo_Temp[i]);
            }
        }

        if (pData != NULL)
        {
            for (size_t i = 0; i < pBitmapInfo->bmiHeader.biSizeImage; i++)
            {
                output.push_back(pData[i]);
            }
        }
    }
    catch (...)
    {
        output.clear();
        output.shrink_to_fit();
        output.resize(0);
        return false;
    }
    return true;
}

bool hmc_screen_util::isInside(int x1, int y1, int x2, int y2, int x, int y)
{
    if (x > x1 && x < x2 && y > y1 && y < y2)
        return true;
    return false;
}
