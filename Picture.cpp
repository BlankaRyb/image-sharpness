// Picture.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"
#include "picture.h"
#include "ostrost/ostrost.h"
//#include "dib.h"

#define MAX_LOADSTRING	1000
#define LOADSTRING	100
#define HIMETRIC_INCH	2540
#define MAP_LOGHIM_TO_PIX(x,ppli)   ( ((ppli)*(x) + HIMETRIC_INCH/2) / HIMETRIC_INCH )
#define PRAH     90

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// The title bar text
LPPICTURE gpPicture;
HWND ghWnd;
HDIB hDIB;
HPALETTE palDIB;
__int16 _DPI;            //Parametr do ktereho je mozne zadat DPI
TCHAR szFileNew[MAX_LOADSTRING];  //jmeno noveho souboru

// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    Dial(HWND, UINT, WPARAM, LPARAM);

//
//  FUNCTION: Dial(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for "Dial" dialog box
//       This version allows greater flexibility over the contents of the 'Dial' box,
//       by pulling out values from the 'Version' resource.
//
//  MESSAGES:
//
// WM_INITDIALOG - initialize dialog box
// WM_COMMAND    - Input received
//
//
LRESULT CALLBACK Dial(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	double dPrah;
	char str[LOADSTRING];

	switch (message) {
		case WM_INITDIALOG:
			if (hDIB == NULL)
			{
				_DPI = 300;
			}
			dPrah = (double)PRAH/_DPI*200;
			itoa(_DPI,str,10);
			SetDlgItemText(hDlg, IDC_DPI, str);

			gcvt(dPrah,10,str);
			SetDlgItemText(hDlg, IDC_PRAH, str);
//         ShowWindow (hDlg, SW_HIDE);
			return (TRUE);

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) //1||2
			{
				EndDialog(hDlg, TRUE);
				return (TRUE);
			}
			else
			{
				if((wParam!=0x040003e9)&&(wParam!=0x030003e9)&&(wParam!=0x50332649))
				{
					//zjisti DPI
					GetDlgItemText(hDlg, IDC_DPI, str, LOADSTRING);
					_DPI = atoi(str);
					//spocte prah
					dPrah = (double)PRAH/_DPI*200;
					gcvt(dPrah,10,str);
					//zobrazi prah
					SetDlgItemText(hDlg, IDC_PRAH, str);
				}
			}
		break;
	}

    return FALSE;
}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	MSG msg;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_LOADPIC, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (gpPicture)
		gpPicture->Release();

	return msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_LOADPIC);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_LOADPIC;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable
   gpPicture = NULL;

   ghWnd = hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

// vrati cast textu, ktera je za "."
LPCTSTR FindExtension(LPCTSTR name)
{
	int len = strlen(name);
	int i;
	char sNavrat[MAX_LOADSTRING];
	LPCTSTR str;

	ZeroMemory(sNavrat, MAX_LOADSTRING);
	sNavrat[0] = 0;

	for (i = len-1; i >= 0; i--)
	{
		if (name[i] == '.')
		{

			i++;
			strcat(sNavrat,&name[i]);
			str = sNavrat;
			return str;
		}
	}
	str = sNavrat;
	return str;
}

void InitDIBData()
{
	if (palDIB != NULL)
	{
		//delete palDIB;
		palDIB = NULL;
	}
	if (hDIB == NULL)
	{
		return;
	}
	// Set up document size
	LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
	//INT_MAX = 2147483647
	if (::DIBWidth(lpDIB) > 2147483647 ||::DIBHeight(lpDIB) > 2147483647)
	{
		::GlobalUnlock((HGLOBAL) hDIB);
		::GlobalFree((HGLOBAL) hDIB);
		hDIB = NULL;
/*		CString strMsg;
		strMsg.LoadString(IDS_DIB_TOO_BIG);
		MessageBox(NULL, strMsg, NULL, MB_ICONINFORMATION | MB_OK);*/
		return;
	}
	::GlobalUnlock((HGLOBAL) hDIB);
	// Create copy of palette
	if (::CreateDIBPalette(hDIB, &palDIB) == NULL)
	{
		// DIB may not have a palette
		delete palDIB;
		palDIB = NULL;
		return;
	}
}

// This function loads a file into an IStream.
void LoadPictureFile(LPCTSTR szFile)
{
	// open file
	HANDLE hFile = CreateFile(szFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	_ASSERTE(INVALID_HANDLE_VALUE != hFile);

	// get file size
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	_ASSERTE(-1 != dwFileSize);

	LPCTSTR chTyp;
	chTyp = FindExtension(szFile);
	if ((stricmp (chTyp,"DIB") == 0) || ((stricmp (chTyp,"BMP") == 0)))
	{
		LPSTR pDIB;
		LPVOID pvData = NULL;
		BITMAPFILEHEADER bmfHeader;

		/*
		 * Cteni hlavicky
		 * Go read the DIB file header and check if it's valid.
		 */
//		if (file.Read((LPSTR)&bmfHeader, sizeof(bmfHeader)) != sizeof(bmfHeader))
//			return NULL;
		DWORD dwBytesRead = 0;
		BOOL bRead = ReadFile(hFile, (LPSTR)&bmfHeader, sizeof(bmfHeader), &dwBytesRead, NULL);
		_ASSERTE(FALSE != bRead);

		if (bmfHeader.bfType != DIB_HEADER_MARKER)
			return;

		/*
		 * Allocate memory for DIB
		 */
		hDIB = (HDIB) ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, dwFileSize);
		if (hDIB == 0)
		{
			return;
		}
		pDIB = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);

		/*
		 * Go read the bits.
		 */
//		if (file.ReadHuge(pDIB, dwBitsSize - sizeof(BITMAPFILEHEADER)) !=
//			dwBitsSize - sizeof(BITMAPFILEHEADER) )
		bRead = ReadFile(hFile, pDIB, dwFileSize-sizeof(BITMAPFILEHEADER), &dwBytesRead, NULL);
		_ASSERTE(FALSE != bRead);

		InitDIBData();

////Nepotrebna cast kodu jen pro jednodussi praci s obrazky/////////////////////////////
		//velikost obdelniku
		LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
		long cxDIB = (long) ::DIBWidth(lpDIB);         // Size of DIB - x
		long cyDIB = (long) ::DIBHeight(lpDIB);        // Size of DIB - y
		::GlobalUnlock((HGLOBAL) hDIB);              //POTREBANA CAST
		rcBmp.left   = 0;
		rcBmp.top    = 0;
		rcBmp.right  = cxDIB;
		rcBmp.bottom = cyDIB;
		rcDis = rcBmp;
////////////////////////////////////////////////////////////////////////////////////////
		CloseHandle(hFile);

		gpPicture = NULL;
	}
	else
	{
		LPVOID pvData = NULL;
		// alloc memory based on file size
		HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, dwFileSize);
		_ASSERTE(NULL != hGlobal);

		pvData = GlobalLock(hGlobal);
		_ASSERTE(NULL != pvData);

		DWORD dwBytesRead = 0;
		// read file and store in global memory
		BOOL bRead = ReadFile(hFile, pvData, dwFileSize, &dwBytesRead, NULL);
		_ASSERTE(FALSE != bRead);
		GlobalUnlock(hGlobal);
		CloseHandle(hFile);

		LPSTREAM pstm = NULL;
		// create IStream* from global memory
		HRESULT hr = CreateStreamOnHGlobal(hGlobal, TRUE, &pstm);
		_ASSERTE(SUCCEEDED(hr) && pstm);

		// Create IPicture from image file
		if (gpPicture)
			gpPicture->Release();
		hr = ::OleLoadPicture(pstm, dwFileSize, FALSE, IID_IPicture, (LPVOID *)&gpPicture);
		_ASSERTE(SUCCEEDED(hr) && gpPicture);	
		pstm->Release();

////Nepotrebna cast kodu jen pro jednodussi praci s obrazky/////////////////////////////

		//velikost obdelniku
		long hmWidth;
		long hmHeight;
		gpPicture->get_Width(&hmWidth);
		gpPicture->get_Height(&hmHeight);
	//	m_x = 96; //GetDeviceCaps(hDC, LOGPIXELSX)
	//	m_y = 96; //GetDeviceCaps(hDC, LOGPIXELSY)
		// convert himetric to pixels
	//	int nWidth	= MulDiv(hmWidth, m_x, HIMETRIC_INCH);
	//	int nHeight	= MulDiv(hmHeight, m_y, HIMETRIC_INCH);
		int nWidth	= MulDiv(hmWidth, 96, HIMETRIC_INCH);
		int nHeight	= MulDiv(hmHeight, 96, HIMETRIC_INCH);
		rcBmp.left   = 0;
		rcBmp.top    = 0;
		rcBmp.right  = nWidth;
		rcBmp.bottom = nHeight;
		rcDis = rcBmp;

		hDIB = NULL;
	}

	InvalidateRect(ghWnd, NULL, TRUE);
}


//Nahradi v retezci "str" znak "z1" za znak "z2"
void nahrad(char *ch, char z1, char z2)
{
	__int16 i, delka;

	delka = strlen(ch);
	for (i=0; i<delka; i++)
	{
		if (ch[i] == z1)
		{
			ch[i] = z2;
		}
	}
}

// pred "." prida "a"
void DejNovyNazev(LPCTSTR szFile, char* szFileN)
{
	int len = strlen(szFile);
	int i,i_zal;
//	LPCTSTR szFileN;
	char sNavrat[MAX_LOADSTRING];
//	LPCTSTR str;

	ZeroMemory(sNavrat, MAX_LOADSTRING);

	for (i = len-1; i >= 0; i--)
	{
		if (szFile[i] == '.')
		{
			i_zal = i;
			break;
		}
	}
	for (i = 0; i < i_zal; i++)
	{
		szFileN[i] = szFile[i];
	}
	strcat(szFileN,"a");
	strcat(szFileN,&szFile[i_zal]);
//	szFileN = sNavrat;
}

void UrciDPI()
{
	LPSTR    pDIB;
	LPBITMAPINFOHEADER lpbmi; // pointer to a Win 3.0-style DIB

	if (hDIB == NULL)
	{
		return;
	}

	pDIB = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);

	//BITMAPINFOHEADER struktura z bitmapy
	lpbmi = (LPBITMAPINFOHEADER)pDIB;
	if (lpbmi->biXPelsPerMeter>0)
	{
		_DPI = (WORD)((double)lpbmi->biXPelsPerMeter/1000*25.4);
	}
	else
	{
		_DPI = 300;
	}
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
//	CString str;
	char ch[MAX_PATH], chCely[MAX_PATH];

	switch (message) 
	{
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
				case IDM_OPEN:
				{
					// get file name to open
					TCHAR szFile[MAX_PATH];
//					TCHAR szFileNew[MAX_LOADSTRING];  //jmeno noveho souboru
					ZeroMemory(szFile, MAX_PATH);
					ZeroMemory(szFileNew, MAX_PATH);	//jmeno noveho souboru
					OPENFILENAME ofn;
					ZeroMemory(&ofn, sizeof(OPENFILENAME));
					ofn.lStructSize	= sizeof(OPENFILENAME);
					ofn.Flags		= OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
					ofn.hwndOwner	= hWnd;

//					// IDS_ALLFILTER: Podporované grafické soubory(*.bmp;*.jpg;*.tiff)|*.bmp;*.jpg;*.tiff|
//					LoadString(NULL,IDS_ALLFILTER,chCely,MAX_PATH);
//					ofn.lpstrFilter	= _T(chCely);
					// IDS_BMPFILTER: BMP formát (*.bmp)|*.bmp|
					LoadString(NULL,IDS_BMPFILTER,chCely,MAX_PATH);
					ofn.lpstrFilter	= _T(chCely);
//					strcat(chCely, ch);
/*					// IDS_DIBFILTER: DIB formát (*.dib)|*.dib|
					LoadString(NULL,IDS_DIBFILTER,ch,MAX_PATH);
					strcat(chCely, ch);
					// IDS_JPGFILTER: JPEG formát (*.jpg)|*.jpg|
					LoadString(NULL,IDS_JPGFILTER,ch,MAX_PATH);
					strcat(chCely, ch);
					// IDS_TIFFILTER: TIFF formát (*.tif)|*.tif|
					LoadString(NULL,IDS_TIFFILTER,ch,MAX_PATH);
					strcat(chCely, ch);
					// IDS_TIFFFILTER: TIFF formát (*.tiff)|*.tiff|
					LoadString(NULL,IDS_TIFFFILTER,ch,MAX_PATH);
					strcat(chCely, ch);*/

					nahrad(chCely,'|',0);

					ofn.lpstrFilter	= _T(chCely);
					LoadString(NULL,IDS_OPEN_PIC,ch,MAX_PATH);
					ofn.lpstrTitle	= _T(ch);
					ofn.lpstrFile	= szFile;
					ofn.nMaxFile	= MAX_PATH;
					if (IDOK == GetOpenFileName(&ofn))
					{
						// load the file
						LoadPictureFile(szFile);
						DejNovyNazev(szFile, szFileNew);
						UrciDPI();
					}
				}
				break;
				case IDM_EXIT:
					DestroyWindow(hWnd);
				break;
/////////////Nepotrebna cast - Fit obrazku /////////////////////////////////////////////////////////////
				case ID_NASTAVENI_FIT:
					RECT rc;

					//zjisti aktualni velikost dokumentu
					GetClientRect(ghWnd, &rc);

					//velikost se upravi podle osy x
					if (((double)rc.right/(double)rcBmp.right) <= ((double)rc.bottom/(double)rcBmp.bottom))
					{
						rcDis.right  = rc.right;
						rcDis.bottom = (long)(rcBmp.bottom*((double)rc.right/(double)rcBmp.right)+0.5);
					}
					//velikost se upravi podle osy y
					else
					{
						rcDis.right  = (long)(rcBmp.right*((double)rc.bottom/(double)rcBmp.bottom)+0.5);
						rcDis.bottom = rc.bottom;
					}

					//obnoveni zobrazeni
					InvalidateRect(NULL, &rc, TRUE);
				break;
////////////////////////////////////////////////////////////////////////////////////////////////////////
				case ID_NASTAVEN_PRAHDPI:
					DialogBox(hInst, "IDD_DIALOG", hWnd, (DLGPROC)Dial);
				break;
					
				case ID_VSLEDKY_OSTROST:
					double d;
					hdc = BeginPaint(hWnd, &ps);

					if (hDIB)
					{
						RECT rec;
						rec.left   = 0;
						rec.top    = 200;
						rec.right  = 3400;
						rec.bottom = 2481;
/*						rec.left   = 0;
						rec.top    = 0;
						rec.right  = 200;
						rec.bottom = 200;*/
//						d = ZjistiOstrostAUloz(hdc,(HDIB)hDIB,rec);
						SetDPI(_DPI);
						SetNaz(szFileNew);
						d = ZjistiOstrostAUloz(hdc,(HDIB)hDIB);
					}
					if(gpPicture)
					{
//						d = ZjistiOstrost(hdc,(HDIB)gpPicture);
//						d = ZjistiOstrost(hdc,(HBITMAP)gpPicture);
					}
				break;
				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
		break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			if (hDIB)
			{
				LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
				int cxDIB = (int) ::DIBWidth(lpDIB);         // Size of DIB - x
				int cyDIB = (int) ::DIBHeight(lpDIB);        // Size of DIB - y
				::GlobalUnlock((HGLOBAL) hDIB);
				RECT rcDIB;
				rcDIB.top = rcDIB.left = 0;
				rcDIB.right = cxDIB;
				rcDIB.bottom = cyDIB;
				::PaintDIB(hdc, &rcDis, hDIB, &rcDIB, palDIB);
			}
			if (gpPicture)
			{
				// get width and height of picture
				long hmWidth;
				long hmHeight;
				gpPicture->get_Width(&hmWidth);
				gpPicture->get_Height(&hmHeight);

				// convert himetric to pixels
//				int nWidth	= MulDiv(hmWidth, GetDeviceCaps(hdc, LOGPIXELSX), HIMETRIC_INCH);
//				int nHeight	= MulDiv(hmHeight, GetDeviceCaps(hdc, LOGPIXELSY), HIMETRIC_INCH);
				int nWidth	= rcDis.right;
				int nHeight	= rcDis.bottom;
				RECT rc;
				GetClientRect(hWnd, &rc);
				// display picture using IPicture::Render
				gpPicture->Render(hdc, 0, 0, nWidth, nHeight, 0, hmHeight, hmWidth, -hmHeight, &rc);
			}
			EndPaint(hWnd, &ps);
		break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}
