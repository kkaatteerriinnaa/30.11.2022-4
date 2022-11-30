#pragma once
#include <windows.h>
#include <tchar.h>
#include <time.h>
#include <ctime>
#include "resource.h"
#include <windowsX.h>
#include <commctrl.h>

#pragma comment(lib,"comctl32")

HWND hbutton, hbutton1, hedit, hprogress;
HWND hradio1, hradio2, hradio3;

int score = 0;

DWORD WINAPI Time(LPVOID lp)
{
	DWORD pr = GetPriorityClass(GetCurrentProcess());
	SetPriorityClass(GetCurrentProcess(), ABOVE_NORMAL_PRIORITY_CLASS);
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);
	HWND hWnd = HWND(lp);
	static time_t t;
	static TCHAR str[100];
	t = time(NULL);
	lstrcpy(str, _tctime(&t));
	str[lstrlen(str) - 1] = '\0';
	SetWindowText(hedit, str);
	SetPriorityClass(GetCurrentProcess(), pr);
	return 0;
}
void Close(HWND hwnd)
{
	EndDialog(hwnd, 0);
}

BOOL InitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	hbutton = GetDlgItem(hwnd, IDC_BUTTON1);
	hbutton1 = GetDlgItem(hwnd, IDC_BUTTON2);
	hedit = GetDlgItem(hwnd, IDC_EDIT1);

	hradio1 = GetDlgItem(hwnd, IDC_RADIO1);
	hradio1 = GetDlgItem(hwnd, IDC_RADIO3);
	hradio1 = GetDlgItem(hwnd, IDC_RADIO5);

	hprogress = GetDlgItem(hwnd, IDC_PROGRESS1);
	SendMessage(hprogress, PBM_SETRANGE, 0, MAKELPARAM(0, 3));
	SendMessage(hprogress, PBM_SETSTEP, 1, 0);
	SendMessage(hprogress, PBM_SETPOS, 0, 0);
	SendMessage(hprogress, PBM_SETBKCOLOR, 0, LPARAM(RGB(0, 0, 0)));
	SendMessage(hprogress, PBM_SETBARCOLOR, 0, LPARAM(RGB(255, 0, 0)));

	SetTimer(hwnd, 1, 1000, NULL);

	return TRUE;
}
void Command(HWND hwnd, UINT message, HWND hwndCtl, UINT codeNotify)
{
	switch (message)
	{
	case IDC_BUTTON1:
	{
		TCHAR buf[10];
		LRESULT re1 = SendDlgItemMessage(hwnd, IDC_RADIO1, BM_GETCHECK, 0, 0);
		LRESULT re2 = SendDlgItemMessage(hwnd, IDC_RADIO3, BM_GETCHECK, 0, 0);
		LRESULT re3 = SendDlgItemMessage(hwnd, IDC_RADIO5, BM_GETCHECK, 0, 0);

		if (re1 == BST_CHECKED)
		{
			score++;
			SendMessage(hprogress, PBM_SETPOS, WPARAM(score), 0);
		}
		if (re2 == BST_CHECKED)
		{
			score++;
			SendMessage(hprogress, PBM_SETPOS, WPARAM(score), 0);
		}
		if (re3 == BST_CHECKED)
		{
			score++;
			SendMessage(hprogress, PBM_SETPOS, WPARAM(score), 0);
		}

		wsprintf(buf, TEXT("%d"), score);
		int rez = MessageBox(hwnd, buf, TEXT("Результат"), MB_OK | MB_ICONINFORMATION);
		if (rez == IDOK)
		{
			KillTimer(hwnd, 1);
			DestroyWindow(hwnd);
		}
	}
	case IDC_BUTTON2:
	{
		HANDLE hThread = CreateThread(NULL, 0, Time, hedit, 0, NULL);
		CloseHandle(hThread);
	}
	}
}
BOOL CALLBACK DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, Close);
		HANDLE_MSG(hwnd, WM_INITDIALOG, InitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, Command);
	}
	return FALSE;
}