#include <stdio.h>

#include "dlgtilesize.h"
#include "resource.h"

int g_iRatio1;
int g_iRatio2;

bool TileSizeDlg::Run(HINSTANCE hInstance, HWND hwnd, int iRatio1, int iRatio2)
{
	g_iRatio1 = iRatio1;
	g_iRatio2 = iRatio2;
	int result = DialogBoxParam (hInstance, MAKEINTRESOURCE(IDD_TILESIZE), hwnd, &DialogProc, (LPARAM)hwnd);

	if (result == 1)
		return true;
	else
		return false;
}

BOOL CALLBACK TileSizeDlg::DialogProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	char szBuffer[16];
	RECT rect, rect2;
	int i;

	switch (message)
	{
		case WM_INITDIALOG:
			// Center dialog
			GetWindowRect((HWND)lParam, &rect);
			GetWindowRect(hwnd , &rect2);
			SetWindowPos (hwnd, HWND_TOP, ((rect.left + rect.right) / 2) - ((rect2.right - rect2.left) / 2), ((rect.top + rect.bottom) / 2) - ((rect2.bottom - rect2.top) / 2), 0, 0, SWP_NOSIZE);

			for (i = 1; i <= 5; i += 1)
			{
				sprintf(szBuffer, "%i", i);
				SendDlgItemMessage(hwnd, IDC_RATIO1, CB_ADDSTRING, 0, (LPARAM)szBuffer);
				SendDlgItemMessage(hwnd, IDC_RATIO2, CB_ADDSTRING, 0, (LPARAM)szBuffer);
			}

			SendDlgItemMessage(hwnd, IDC_RATIO1, CB_SETCURSEL, g_iRatio1 - 1, 0);
			SendDlgItemMessage(hwnd, IDC_RATIO2, CB_SETCURSEL, g_iRatio2 - 1, 0);
			return 1;
		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				case IDOK:
					GetDlgItemText(hwnd, IDC_RATIO1, szBuffer, 16);
					g_iRatio1 = atoi(szBuffer);
					GetDlgItemText(hwnd, IDC_RATIO2, szBuffer, 16);
					g_iRatio2 = atoi(szBuffer);

					EndDialog(hwnd, 1);

					return 1;

				case IDCANCEL:
					EndDialog(hwnd, 0);
					return 1;
			}
		break;

		case WM_DESTROY:
			// EndDialog(hwnd, 0);
			return 1;
	}

	return 0;
}