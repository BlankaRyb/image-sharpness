//Ostrost.h

//#if !defined(AFX_LOADPIC_H__5EB6AF99_E13D_11D2_90C6_00C04F9403C2__INCLUDED_)
//#define AFX_LOADPIC_H__5EB6AF99_E13D_11D2_90C6_00C04F9403C2__INCLUDED_

//#if _MSC_VER > 1000
//#pragma once
//#endif // _MSC_VER > 1000

//#include "resource.h"
#include "dib.h"

double ZjistiOstrost(HDC hdc, HBITMAP hBitmap);
double ZjistiOstrost(HDC hdc, HBITMAP hBitmap);
double ZjistiOstrost(HDC hdc, HDIB hBIB);
double ZjistiOstrost(HDC hdc, HDIB hBIB, RECT rec);
double ZjistiOstrostAUloz(HDC hdc, HBITMAP hBitmap);
double ZjistiOstrostAUloz(HDC hdc, HBITMAP hBitmap, RECT rec);
double ZjistiOstrostAUloz(HDC hdc, HDIB hBIB);
double ZjistiOstrostAUloz(HDC hdc, HDIB hBIB, RECT rec);
void SetDPI(__int16 iDPI); //pro nastaveni DPI
void SetNaz (LPCTSTR naz);// pro nasteveni noveho nazvu

//#endif // !defined(AFX_LOADPIC_H__5EB6AF99_E13D_11D2_90C6_00C04F9403C2__INCLUDED_)
