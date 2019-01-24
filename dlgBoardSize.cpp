#include <stdio.h>

#include "dlgboardsize.h"
#include "resource.h"

int g_iWidth;

bool BoardSizeDlg::Run(HINSTANCE hInstance, HWND hwnd, int width, int height)
{
	g_iWidth  = width;
	int result = DialogBoxParam (hInstance, MAKEINTRESOURCE(IDD_BOARDSIZE), hwnd, &DialogProc, (LPARAM)hwnd);

	if (result == 1)
		return true;
	else
		return false;
}

BOOL CALLBACK BoardSizeDlg::DialogProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
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

			for (i = 3; i <= 9; i += 2)
			{
				sprintf(szBuffer, "%i", i);
				SendDlgItemMessage(hwnd, IDC_WIDTH,  CB_ADDSTRING, 0, (LPARAM)szBuffer);
			}

			SendDlgItemMessage(hwnd, IDC_WIDTH,  CB_SETCURSEL, g_iWidth  / 2 - 1, 0);
			return 1;
		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				case IDOK:
					GetDlgItemText(hwnd, IDC_WIDTH, szBuffer, 16);
					g_iWidth = atoi(szBuffer);

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