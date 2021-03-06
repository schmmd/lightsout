#if !defined CANVAS_H
#define CANVAS_H
//-------------------------------------
// Copyright Reliable Software (c) 1999
//-------------------------------------

#include <windows.h>

class Canvas
{
public:
    operator HDC () 
    { 
        return _hdc; 
    }

	void WhiteWash (RECT & rect)
	{
		::PatBlt (_hdc, rect.left, rect.top, rect.right, rect.bottom, WHITENESS);
	}
	
    void SetPixel (int x, int y, COLORREF color)
    {
        ::SetPixel (_hdc, x, y, color);
    }

    void MoveTo (int x, int y)
    {
        ::MoveToEx (_hdc, x, y, 0);
    }

    void LineTo (int x, int y)
    {
        ::LineTo (_hdc, x, y);
    }

    void Line (int x1, int y1, int x2, int y2)
    {
        ::MoveToEx (_hdc, x1, y1, 0);
        ::LineTo (_hdc, x2, y2);
    }

	void FillRect(RECT* lprc, HBRUSH hbr)
	{
		::FillRect(_hdc, lprc, hbr);
	}

	// Text
    void Text (int x, int y, char const * buf, int cBuf)
    {
        ::TextOut (_hdc, x, y, buf, cBuf);
    }

    int TabbedText (int x, int y, char const * buf, int cBuf, int tabSize)
    {
        DWORD dim = ::TabbedTextOut (_hdc, x, y, buf, cBuf, 1, &tabSize, 0);
        return LOWORD (dim); // lenght in pixels
    }

    void GetTextSize (int& cxChar, int& cyChar)
    {
        TEXTMETRIC tm;
        ::GetTextMetrics (_hdc, &tm);
        cxChar = tm.tmAveCharWidth;
        cyChar = tm.tmHeight + tm.tmExternalLeading;
    }

	int GetTextWidth (char* szText)
	{
		SIZE size;
		GetTextExtentPoint32(_hdc, szText, strlen(szText), &size);
		return size.cx;
	}

	// Object
    void SelectObject (void* pObj)
    {
        ::SelectObject (_hdc, pObj);
    }

protected:
    Canvas(HDC hdc) :_hdc(hdc) {}

    HDC  _hdc;
};

// Use for painting after WM_PAINT

class PaintCanvas: public Canvas
{
public:
    PaintCanvas (HWND hwnd)
		:   Canvas (::BeginPaint (hwnd, &_paint)),
        _hwnd (hwnd)
    {}

    ~PaintCanvas ()
    {
        ::EndPaint(_hwnd, &_paint);
    }

    int Top () const    { return _paint.rcPaint.top; }
    int Bottom () const { return _paint.rcPaint.bottom; }
    int Left () const   { return _paint.rcPaint.left; }
    int Right () const  { return _paint.rcPaint.right; }

protected:
                 
    PAINTSTRUCT _paint;
    HWND        _hwnd;
};

// Device Context
// Use for painting other than WM_PAINT

class UpdateCanvas: public Canvas
{
public:
    UpdateCanvas (HWND hwnd)
		:   Canvas (::GetDC(hwnd)),
        _hwnd(hwnd)
    {}
    
    ~UpdateCanvas ()
    {
        ::ReleaseDC (_hwnd, _hdc);
    }

protected:

    HWND _hwnd;
};

// For painting in memory
class MemCanvas: public Canvas
{
public:
    MemCanvas (HDC hdc) 
        : Canvas (::CreateCompatibleDC (hdc))
    {}

    ~MemCanvas ()
    {
        ::DeleteDC(_hdc); 
    }
};

// For painting over a bitmap
class BitmapCanvas: public MemCanvas
{
public:
	BitmapCanvas (HDC hdc, HBITMAP hBitmap)
		: MemCanvas (hdc)
	{
		// convert bitmap to format compatible with canvas
		_hOldBitmap = reinterpret_cast<HBITMAP> (::SelectObject (_hdc, hBitmap));
	}
	~BitmapCanvas ()
	{
		::SelectObject (_hdc, _hOldBitmap);
	}
private:
	HBITMAP _hOldBitmap;
};

// For use in owner draw controls
class DrawItemCanvas: public Canvas
{
public:
    DrawItemCanvas (DRAWITEMSTRUCT *pDraw)
        : Canvas (pDraw->hDC), _pDraw (pDraw)
    {}
    int Left () const   { return _pDraw->rcItem.left; }
    int Top () const    { return _pDraw->rcItem.top; }
    int Right () const  { return _pDraw->rcItem.right; }
    int Bottom () const { return _pDraw->rcItem.bottom; }
    int Width () const 
    { 
        return _pDraw->rcItem.right - _pDraw->rcItem.left; 
    }
    int Height () const 
    { 
        return _pDraw->rcItem.bottom - _pDraw->rcItem.top; 
    }
private:
    DRAWITEMSTRUCT * _pDraw;
};

// For drawing on the desktop
class DesktopCanvas : public Canvas
{
public:
    DesktopCanvas ()
        : Canvas (::CreateDC ("DISPLAY", NULL, NULL, NULL))
    {}
    ~DesktopCanvas ()
    {
        ::DeleteDC (_hdc);
    }
};

#endif
