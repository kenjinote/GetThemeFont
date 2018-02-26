#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#pragma comment(lib, "uxtheme")

#include <windows.h>
#include <uxtheme.h>
#include <vssym32.h>

TCHAR szClassName[] = TEXT("Window");

HFONT GetThemeFont(TEXTSTYLEPARTS tsp)
{
	HFONT hFont = 0;
	HTHEME hTheme = OpenThemeData(0, VSCLASS_TEXTSTYLE);
	if (hTheme)
	{
		LOGFONTW lf = { 0 };
		if (SUCCEEDED(GetThemeFont(hTheme, 0, tsp, 0, TMT_FONT, &lf)))
		{
			hFont = CreateFontIndirect(&lf);
		}
		CloseThemeData(hTheme);
	}
	return hFont;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HFONT hFont;
	switch (msg)
	{
	case WM_CREATE:
		hFont = GetThemeFont(TEXT_BODYTEXT);
		break;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
			LOGFONTW lf = { 0 };
			GetObject(hFont, sizeof(lf), &lf);
			TextOutW(hdc, 10, 10, lf.lfFaceName, lstrlenW(lf.lfFaceName));
			SelectObject(hdc, hOldFont);
			EndPaint(hWnd, &ps);
		}
		break;
	case WM_DESTROY:
		DeleteObject(hFont);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInst, LPSTR pCmdLine, int nCmdShow)
{
	MSG msg;
	WNDCLASS wndclass = {
		CS_HREDRAW | CS_VREDRAW,
		WndProc,
		0,
		0,
		hInstance,
		0,
		LoadCursor(0,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		0,
		szClassName
	};
	RegisterClass(&wndclass);
	HWND hWnd = CreateWindow(
		szClassName,
		TEXT("Theme のフォントを取得する"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		0,
		CW_USEDEFAULT,
		0,
		0,
		0,
		hInstance,
		0
	);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}
