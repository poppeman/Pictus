#include "StdAfx.h"
#include "../PictThumbs/cthumbprovider.h"

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

HBITMAP hbmp;
const int size = 1024;
const float scale = 0.2f;


_Use_decl_annotations_ int WINAPI wWinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR szCmdLine, int iCmdShow) {
	if(szCmdLine == 0)
		return 1;

	IStream* is = 0;
	if (FAILED(SHCreateStreamOnFile(szCmdLine, STGM_READ | STGM_SHARE_DENY_WRITE, &is))) {
		return 1;
	}

	if (!is) {
		return 1;
	}

    static TCHAR szAppName[] = TEXT ("ThumbDisplay");
    HWND         hwnd;
    MSG          msg;
    WNDCLASSEX   wndclassex = {0};
    wndclassex.cbSize        = sizeof(WNDCLASSEX);
    wndclassex.style         = CS_HREDRAW | CS_VREDRAW;
    wndclassex.lpfnWndProc   = WndProc;
    wndclassex.cbClsExtra    = 0;
    wndclassex.cbWndExtra    = 0;
    wndclassex.hInstance     = hInstance;
    wndclassex.hIcon         = LoadIcon (NULL, IDI_APPLICATION);
    wndclassex.hCursor       = LoadCursor (NULL, IDC_ARROW);
    wndclassex.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH);
    wndclassex.lpszMenuName  = NULL;
    wndclassex.lpszClassName = szAppName;
    wndclassex.hIconSm       = wndclassex.hIcon;

    if (!RegisterClassEx (&wndclassex)) {
        MessageBox (NULL, TEXT ("RegisterClassEx failed!"), szAppName, MB_ICONERROR);
        return 0;
    }
    hwnd = CreateWindowEx (WS_EX_OVERLAPPEDWINDOW, 
		                  szAppName, 
        		          TEXT("ThumbDisplay"),
                		  WS_OVERLAPPEDWINDOW,
		                  CW_USEDEFAULT, 
        		          CW_USEDEFAULT, 
                		  CW_USEDEFAULT, 
		                  CW_USEDEFAULT, 
        		          NULL, 
                		  NULL, 
		                  hInstance,
        		          NULL); 

	CPictusThumbnailProvider cptp;

	cptp.Initialize(is, 0);
	WTS_ALPHATYPE a;
	cptp.GetThumbnail(size, &hbmp, &a);
	
	ShowWindow (hwnd, iCmdShow);
	UpdateWindow (hwnd);

    while (GetMessage (&msg, NULL, 0, 0))
    {
        TranslateMessage (&msg);
        DispatchMessage (&msg);
    }
    return msg.wParam;
}
LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    switch (message)
    {
    case WM_CREATE:
        return (0);
		
    case WM_PAINT:
		{
        hdc = BeginPaint (hwnd, &ps);

		HBRUSH hBrush = CreateSolidBrush(RGB(0xFF, 0xFF, 0));
		SelectObject(hdc, hBrush);
		Ellipse(hdc, 0, 0, size, size);                  // a yellow circle in the background 
		SelectObject(hdc, GetStockObject(WHITE_BRUSH));
		DeleteObject(hBrush);




        //TextOut (hdc, 0, 0, L"A Window!", 27);
		HDC hMemDC = CreateCompatibleDC(hdc);
		HGDIOBJ hobj = SelectObject(hMemDC, hbmp);

		BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };


		BITMAPINFO bi;
		GetObject(hbmp,sizeof( BITMAP ), &bi);

		int sw = bi.bmiHeader.biWidth;
		int sh = bi.bmiHeader.biHeight;
		int dw = bi.bmiHeader.biWidth * scale;
		int dh = bi.bmiHeader.biHeight * scale;

		SetStretchBltMode(hdc, COLORONCOLOR);
		AlphaBlend(hdc,
			0, 0, 
			dw, dh,
			hMemDC, 0, 0,
			sw, sh, bf);
		TextOut(hdc, 0, dh, L"AlphaBlend", 10);
		StretchBlt(hdc,
			0, dh + 20,
			dw, dh,
			hMemDC,
			0, 0,
			sw, sh, SRCCOPY);
		TextOut(hdc, 0, 2 * dh + 20, L"StretchBlt", 10);

		SelectObject(hMemDC, hobj);
		DeleteDC(hMemDC);	
        EndPaint (hwnd, &ps);
        return (0);
		}
		
    case WM_DESTROY:
        PostQuitMessage (0);
        return (0);
    }
    return DefWindowProc (hwnd, message, wParam, lParam);
}