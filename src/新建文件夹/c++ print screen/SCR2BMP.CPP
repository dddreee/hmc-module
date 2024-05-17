// scr2bmp.cpp - save the Windows desktop image to a .BMP file

// this is based on a Microsoft C++ sample - no copyright was claimed and none is here.

// it compiles in Borland C++ 5.01 as a Windows Console app

#include <windows.h>
#include <stdio.h>
#include <dir.h>

#define STRICT

int nExitStatus = 0;

void CreateBMPFile(LPTSTR pszFile, PBITMAPINFO pbi, HBITMAP hBMP, HDC hDC);

PBITMAPINFO CreateBitmapInfoStruct(HBITMAP hBmp);

void main(int argc, char* argv[])
{
    char cOutputFile[MAXPATH] = ".";

    // Not enough information to run
    if (argc < 2)
    {
        printf("usage: SCR2BMP <.BMP image file>\n");
        exit(1);
    }

    // Usage
    if ( strcmp(strupr(argv[1]), "/?") == 0 )
    {
        printf("usage: SCR2BMP <.BMP image file>\n");
        exit(0);
    }

    // Get output file name
    strcpy(cOutputFile, argv[1]);

    // Create a normal DC and a memory DC for the entire screen. The
    // normal DC provides a "snapshot" of the screen contents. The
    // memory DC keeps a copy of this "snapshot" in the associated
    // bitmap.

    HDC hdcScreen = CreateDC("DISPLAY", NULL, NULL, NULL);

    HDC hdcCompatible = CreateCompatibleDC(hdcScreen);

    // Create a compatible bitmap for hdcScreen.
    HBITMAP hbmScreen = CreateCompatibleBitmap(hdcScreen,
        GetDeviceCaps(hdcScreen, HORZRES),
        GetDeviceCaps(hdcScreen, VERTRES));

    if (hbmScreen == 0)
    {
        printf("SCR2BMP: failed to create bitmap\n");
        exit(1);
    }

    // Select the bitmaps into the compatible DC.

    if (! SelectObject(hdcCompatible, hbmScreen))
    {
        printf("SCR2BMP: failed to select bitmatp into DC\n");
        exit(1);
    }

    // Copy color data for the entire display into a bitmap that is selected into a compatible DC.
    if (!BitBlt(hdcCompatible,
        0,0,
        GetDeviceCaps(hdcScreen, HORZRES), GetDeviceCaps(hdcScreen, VERTRES),
        hdcScreen,
        0,0,
        SRCCOPY))
    {
        printf("SCR2BMP: failed to BitBlt\n");
        exit(1);
    }

    PBITMAPINFO pBitmapInfo = CreateBitmapInfoStruct(hbmScreen);

    CreateBMPFile(cOutputFile, pBitmapInfo, hbmScreen, hdcScreen);

    exit(nExitStatus);
}

PBITMAPINFO CreateBitmapInfoStruct(HBITMAP hBmp)
{
    BITMAP bmp;
    PBITMAPINFO pbmi;
    WORD    cClrBits;

    // Retrieve the bitmap color format, width, and height.
    if (! GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp))
    {
        printf("SCR2BMP: failed to retrieve Bitmap\n");
        exit(1);
    }

    // Convert the color format to a count of bits.
    cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);

    if (cClrBits == 1)
    {
        cClrBits = 1;
    }
    else if (cClrBits <= 4)
    {
        cClrBits = 4;
    }
    else if (cClrBits <= 8)
    {
        cClrBits = 8;
    }
    else if (cClrBits <= 16)
    {
        cClrBits = 16;
    }
    else if (cClrBits <= 24)
    {
        cClrBits = 24;
    }
    else
    {
        cClrBits = 32;
    }

    // Allocate memory for the BITMAPINFO structure. (This structure
    // contains a BITMAPINFOHEADER structure and an array of RGBQUAD
    // data structures.)

    if (cClrBits != 24)
    {
        pbmi = (PBITMAPINFO) LocalAlloc(LPTR,
            sizeof(BITMAPINFOHEADER) +
            sizeof(RGBQUAD) * (1<< cClrBits));
    }

    // There is no RGBQUAD array for the 24-bit-per-pixel format.
    else
    {
        pbmi = (PBITMAPINFO) LocalAlloc(LPTR,
            sizeof(BITMAPINFOHEADER));
    }

    // Initialize the fields in the BITMAPINFO structure.
    pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    pbmi->bmiHeader.biWidth = bmp.bmWidth;
    pbmi->bmiHeader.biHeight = bmp.bmHeight;
    pbmi->bmiHeader.biPlanes = bmp.bmPlanes;
    pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;

    if (cClrBits < 24)
    {
        pbmi->bmiHeader.biClrUsed = (1<<cClrBits);
    }

    // If the bitmap is not compressed, set the BI_RGB flag.
    pbmi->bmiHeader.biCompression = BI_RGB;

    // Compute the number of bytes in the array of color
    // indices and store the result in biSizeImage.
    // For Windows NT, the width must be DWORD aligned unless
    // the bitmap is RLE compressed. This example shows this.
    // For Windows 95/98/Me, the width must be WORD aligned unless the
    // bitmap is RLE compressed.
    pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * cClrBits +31) & ~31) /8
        * pbmi->bmiHeader.biHeight;

    // Set biClrImportant to 0, indicating that all of the device colors are important.
    pbmi->bmiHeader.biClrImportant = 0;

    return pbmi;
}

void CreateBMPFile(LPTSTR pszFile, PBITMAPINFO pbi, HBITMAP hBMP, HDC hDC)
{
    HANDLE hf;                  // file handle
    BITMAPFILEHEADER hdr;       // bitmap file-header
    PBITMAPINFOHEADER pbih;     // bitmap info-header
    LPBYTE lpBits;              // memory pointer
    DWORD cb;                   // incremental count of bytes
    BYTE *hp;                   // byte pointer
    DWORD dwTmp;

    pbih = (PBITMAPINFOHEADER) pbi;

    lpBits = (LPBYTE) GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);

    if (lpBits)
    {
        // Retrieve the color table (RGBQUAD array) and the bits
        // (array of palette indices) from the DIB.
        if ( GetDIBits(hDC, hBMP, 0, (WORD) pbih->biHeight, lpBits, pbi, DIB_RGB_COLORS))
        {
            // Create the .BMP file.
            hf = CreateFile(pszFile, GENERIC_READ | GENERIC_WRITE, (DWORD) 0, NULL, CREATE_ALWAYS,
                FILE_ATTRIBUTE_NORMAL, (HANDLE) NULL);

            if (hf != INVALID_HANDLE_VALUE)
            {

                hdr.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M"

                // Compute the size of the entire file.
                hdr.bfSize = (DWORD) (sizeof(BITMAPFILEHEADER) + pbih->biSize +
                    ( pbih->biClrUsed * sizeof(RGBQUAD) ) +
                    pbih->biSizeImage);

                hdr.bfReserved1 = 0;
                hdr.bfReserved2 = 0;

                // Compute the offset to the array of color indices.
                hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) +
                    pbih->biSize + ( pbih->biClrUsed * sizeof (RGBQUAD) );

                // Copy the BITMAPFILEHEADER into the .BMP file.
                if ( WriteFile(hf, (LPVOID) &hdr, sizeof(BITMAPFILEHEADER), (LPDWORD) &dwTmp,  NULL))
                {

                    // Copy the BITMAPINFOHEADER and RGBQUAD array into the file.
                    if ( WriteFile(hf, (LPVOID) pbih, sizeof(BITMAPINFOHEADER) +
                        ( pbih->biClrUsed * sizeof (RGBQUAD) ), (LPDWORD) &dwTmp, ( NULL)))
                    {

                        // Copy the array of color indices into the .BMP file.
                        cb = pbih->biSizeImage;

                        hp = lpBits;

                        if (! WriteFile(hf, (LPSTR) hp, (int) cb, (LPDWORD) &dwTmp,NULL))
                        {
                            printf("SCR2BMP: failed to write color array\n");
                            nExitStatus = 1;
                        }

                    }

                    // WriteFile INFOHEADER failed
                    else
                    {
                        printf("SCR2BMP: failed to write info header\n");
                        nExitStatus = 1;
                    }

                }

                // WriteFile HEADRER failed
                else
                {
                    printf("SCR2BMP: failed to write header\n");
                    nExitStatus = 1;
                }

                // Close the .BMP file.
                if (!CloseHandle(hf))
                {
                    printf("SCR2BMP: failed to close file\n");
                    nExitStatus = 1;
                }

            }

            // Failed CreateFile
            else
            {
                printf("SCR2BMP: failed to create file\n");
                nExitStatus = 1;
            }

        }

        // Failed GetDIBits
        else
        {
            printf("SCR2BMP: failed to get golor table\n");
            nExitStatus = 1;
        }


        // Free memory.
        GlobalFree((HGLOBAL)lpBits);
    }

    // Failed GlobalAlloc
    else
    {
        printf("SCR2BMP: failed to allocate storage\n");
        nExitStatus = 1;
    }

}  // CreateBMPFile
