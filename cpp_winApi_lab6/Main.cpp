#include <windows.h>
#include <tchar.h>
#include <math.h>
#include <string>
#include "resource.h"


static TCHAR szWindowClass[] = _T("win32app");
static TCHAR szTitle[] = _T("Calculator");
HINSTANCE hInst;


const int WIDTH = 400;
const int HEIGHT = 550;
const int BTN_WIDTH = 90;
const int BTN_HEIGHT = 60;
const int EDIT_HEIGHT = 50;
const int MARGIN = 10;
const int KEYBOARD_MARGIN_HEIGHT = 100;


HWND hEdit;
HWND hMEdit;
HWND hBtns[10];
HWND hMc, hMs, hMr, hMplus;
HWND hCe, hC, hBs;
HWND hSqr, hRev, hDiv, hMult, hSub, hAdd, hEqual;

int op1_nom, op1_denom, op2_nom, op2_denom, curOpNom;
char curOperator;
int curOpNom, curOpDenom;

int memNom = 0, memDenom = -1;
bool memIsEmpty = true;

struct Op
{
	int nom;
	int denom;
	int sign;
};

void SetComponents(HWND hWnd) {
	hEdit = CreateWindow(L"edit", L"", WS_CHILD | WS_VISIBLE | ES_RIGHT, MARGIN+30, KEYBOARD_MARGIN_HEIGHT-MARGIN-EDIT_HEIGHT, WIDTH-20-5*MARGIN, EDIT_HEIGHT, hWnd, 0, hInst, NULL);
	HFONT hFont = CreateFont(50, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
	SendMessage(hEdit, WM_SETFONT, WPARAM(hFont), TRUE);

	hMEdit = CreateWindow(L"edit", L"M", WS_CHILD | WS_VISIBLE, MARGIN, KEYBOARD_MARGIN_HEIGHT - MARGIN - EDIT_HEIGHT, 20, 20, hWnd, 0, hInst, NULL);
	
	//memory keys

	hMc = CreateWindow(L"button", L"MC", WS_CHILD | WS_VISIBLE, MARGIN, MARGIN + KEYBOARD_MARGIN_HEIGHT, BTN_WIDTH, BTN_HEIGHT, hWnd, 0, hInst, NULL);
	hMs = CreateWindow(L"button", L"MS", WS_CHILD | WS_VISIBLE, MARGIN+BTN_WIDTH, MARGIN + KEYBOARD_MARGIN_HEIGHT, BTN_WIDTH, BTN_HEIGHT, hWnd, 0, hInst, NULL);
	hMr = CreateWindow(L"button", L"MR", WS_CHILD | WS_VISIBLE, MARGIN + BTN_WIDTH *2, MARGIN + KEYBOARD_MARGIN_HEIGHT, BTN_WIDTH, BTN_HEIGHT, hWnd, 0, hInst, NULL);
	hMplus = CreateWindow(L"button", L"M+", WS_CHILD | WS_VISIBLE, MARGIN + BTN_WIDTH * 3, MARGIN + KEYBOARD_MARGIN_HEIGHT, BTN_WIDTH, BTN_HEIGHT, hWnd, 0, hInst, NULL);

	//operators keys

	hDiv = CreateWindow(L"button", L"/", WS_CHILD | WS_VISIBLE, MARGIN+BTN_WIDTH*3, MARGIN*2 + KEYBOARD_MARGIN_HEIGHT+BTN_HEIGHT, BTN_WIDTH, BTN_HEIGHT, hWnd, 0, hInst, NULL);
	hMult = CreateWindow(L"button", L"*", WS_CHILD | WS_VISIBLE, MARGIN + BTN_WIDTH * 3, MARGIN * 2 + KEYBOARD_MARGIN_HEIGHT + BTN_HEIGHT*2, BTN_WIDTH, BTN_HEIGHT, hWnd, 0, hInst, NULL);
	hSub = CreateWindow(L"button", L"-", WS_CHILD | WS_VISIBLE, MARGIN + BTN_WIDTH * 3, MARGIN * 2 + KEYBOARD_MARGIN_HEIGHT + BTN_HEIGHT * 3, BTN_WIDTH, BTN_HEIGHT, hWnd, 0, hInst, NULL);
	hAdd = CreateWindow(L"button", L"+", WS_CHILD | WS_VISIBLE, MARGIN + BTN_WIDTH * 3, MARGIN * 2 + KEYBOARD_MARGIN_HEIGHT + BTN_HEIGHT * 4, BTN_WIDTH, BTN_HEIGHT, hWnd, 0, hInst, NULL);
	hEqual = CreateWindow(L"button", L"=", WS_CHILD | WS_VISIBLE, MARGIN + BTN_WIDTH * 3, MARGIN * 2 + KEYBOARD_MARGIN_HEIGHT + BTN_HEIGHT * 5, BTN_WIDTH, BTN_HEIGHT, hWnd, 0, hInst, NULL);
	hSqr = CreateWindow(L"button", L"SQR", WS_CHILD | WS_VISIBLE, MARGIN, MARGIN * 2 + KEYBOARD_MARGIN_HEIGHT + BTN_HEIGHT * 5, BTN_WIDTH, BTN_HEIGHT, hWnd, 0, hInst, NULL);
	hRev = CreateWindow(L"button", L"REV", WS_CHILD | WS_VISIBLE, MARGIN+BTN_WIDTH, MARGIN * 2 + KEYBOARD_MARGIN_HEIGHT + BTN_HEIGHT * 5, BTN_WIDTH, BTN_HEIGHT, hWnd, 0, hInst, NULL);
	
	//delete keys

	hCe = CreateWindow(L"button", L"CE", WS_CHILD | WS_VISIBLE, MARGIN, MARGIN * 2 + KEYBOARD_MARGIN_HEIGHT + BTN_HEIGHT, BTN_WIDTH, BTN_HEIGHT, hWnd, 0, hInst, NULL);
	hC = CreateWindow(L"button", L"C", WS_CHILD | WS_VISIBLE, MARGIN+BTN_WIDTH, MARGIN * 2 + KEYBOARD_MARGIN_HEIGHT + BTN_HEIGHT, BTN_WIDTH, BTN_HEIGHT, hWnd, 0, hInst, NULL);
	hBs = CreateWindow(L"button", L"BS", WS_CHILD | WS_VISIBLE, MARGIN + BTN_WIDTH*2, MARGIN * 2 + KEYBOARD_MARGIN_HEIGHT + BTN_HEIGHT, BTN_WIDTH, BTN_HEIGHT, hWnd, 0, hInst, NULL);

	//number keys

	int counter = 1;
	WCHAR buffer[10];
	for (int i = 3; i >0; i--)
		for (int j = 0; j < 3; j++) {
			wsprintf(buffer, TEXT("%d"), counter);
			hBtns[counter] = CreateWindow(L"button", buffer, WS_CHILD | WS_VISIBLE, MARGIN + j*BTN_WIDTH, MARGIN*2+KEYBOARD_MARGIN_HEIGHT + i*BTN_HEIGHT+BTN_HEIGHT, BTN_WIDTH, BTN_HEIGHT, hWnd, 0, hInst, NULL);
			counter++;
		}
	hBtns[0] = CreateWindow(L"button", L"0", WS_CHILD | WS_VISIBLE, MARGIN + 2*BTN_WIDTH, MARGIN*2+KEYBOARD_MARGIN_HEIGHT + 5*BTN_HEIGHT, BTN_WIDTH, BTN_HEIGHT, hWnd, 0, hInst, NULL);

}

void Add(int op1_nom, int op1_denom, int op2_nom, int op2_denom) {

}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Win32"),
			NULL);

		return 1;
	}

	hInst = hInstance;
	HWND hWnd = CreateWindow(
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		WIDTH,HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL
		);

	if (!hWnd)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("Failure!"),
			NULL);
		return 1;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}





LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hDc;
	PAINTSTRUCT ps;
	RECT rect = { 0,0,WIDTH, KEYBOARD_MARGIN_HEIGHT};

	switch (message)
	{
	case WM_GETMINMAXINFO:
	{
		//forbid resize

		MINMAXINFO *pInfo = (MINMAXINFO *)lParam;
		POINT Min = {WIDTH,HEIGHT };
		POINT Max = { WIDTH,HEIGHT };
		pInfo->ptMinTrackSize = Min;
		pInfo->ptMaxTrackSize = Max;
		return 0;
	}
	case WM_CREATE:
		SetComponents(hWnd);
		break;
	case WM_PAINT:
		//HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255));
		hDc = BeginPaint(hWnd, &ps);
		FillRect(hDc, &rect, NULL);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_COMMAND:

		//menu items clicks

		switch (LOWORD(wParam))
		{
		case ID_FILE_EXIT:
			PostMessage(hWnd, WM_CLOSE, 0, 0);
			break;
		}

		//number button clicks

		if (lParam == (LPARAM)hBtns[4]) {
		
			SetWindowText(hEdit, L"4");
			UpdateWindow(hWnd);
	
		}
		if (lParam == (LPARAM)hBtns[3]) {

			SetWindowText(hEdit, L"3");
			UpdateWindow(hWnd);

		}
		
		//mem buttons click

		if (lParam == (LPARAM)hMc) {
			memIsEmpty = true;
			memNom = 0;
			memDenom = 1;
		}
		if (lParam == (LPARAM)hMs) {
			memIsEmpty = false;
			memNom = curOpNom;
			memDenom = curOpDenom;
		}
		if (lParam == (LPARAM)hMr) {
			//
		}
		if (lParam == (LPARAM)hMplus) {
			memIsEmpty = false;
		}



		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	return 0;
}

