//Ostrost.cpp
//obahuje funkce pro vypocet ostrosti

//#include "ostrost.h"

// Picture.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "ostrost.h"

#include <math.h>
#include <time.h>
//#include "dib.h"
//#include "resource.h"

#define PRAH     90
#define PROCENTA 0.8  //pocet procen ke kterym se me priblizit v miste mereni

__int16 svetly, tmavy;  //hodnoty v sede nejtamvsiho a nejsvetlejsiho bodu obrazku
__int16 DPI;            //Parametr do ktereho je mozne zadat DPI
LPCTSTR FileNew;  //jmeno noveho souboru

void SetDPI(__int16 iDPI)
{
	DPI = iDPI;
}
void SetNaz (LPCTSTR naz)
{
	FileNew = naz;
}

double ZjistiOstrost(HDC hdc, HBITMAP hBitmap)
{
	double dRet;
	HDIB hDIB;

	hDIB = (HDIB) ::GlobalLock((HGLOBAL) hBitmap);
	dRet = ZjistiOstrost(hdc,hDIB);

	return dRet;
}

double ZjistiOstrost(HDC hdc, HBITMAP hBitmap, RECT rec)
{
	double dRet;
	HDIB hDIB;

	hDIB = (HDIB) ::GlobalLock((HGLOBAL) hBitmap);
	dRet = ZjistiOstrost(hdc,hDIB,rec);

	return dRet;
}

double ZjistiOstrostAUloz(HDC hdc, HBITMAP hBitmap)
{
	double dRet;
	HDIB hDIB;

	hDIB = (HDIB) ::GlobalLock((HGLOBAL) hBitmap);
	dRet = ZjistiOstrostAUloz(hdc,hDIB);

	return dRet;
}

double ZjistiOstrostAUloz(HDC hdc, HBITMAP hBitmap, RECT rec)
{
	double dRet;
	HDIB hDIB;

	hDIB = (HDIB) ::GlobalLock((HGLOBAL) hBitmap);
	dRet = ZjistiOstrostAUloz(hdc,hDIB,rec);

	return dRet;
}

/*******************************************************************
vrati podle RGB, sedou barvu (pro vsechny 3 slozky barvy stejnou hodnotu)
R,G,B - baravne hodnoty jedne barvy
*******************************************************************/
int Seda(__int16 R,__int16 G,__int16 B)
{
	int seda;

	seda = (int)(0.299*R+0.587*G+0.114*B + .5);

	return seda;
}

/*******************************************************************
vrati podle RGB, sedou barvu (pro vsechny 3 slozky barvy stejnou hodnotu)
barva - z palety, ktere se priradi hodnota v sede od 0..255
*******************************************************************/
int Seda(PALETTEENTRY barva)
{
	int seda;

	seda = (int)(0.299*barva.peRed+0.587*barva.peGreen+0.114*barva.peBlue+.5);

	return seda;
}

/*******************************************************************
vrati dvojrozmerne pole s obrazkem prevedych do sedych odstinu

lpDIBBits - obrazek v bitech
lpbmi - LPBITMAPINFOHEADER - informace o obrazku
palBarvy - barvy v palete
rec - odelnik ve kterem se pocita ostrost
*******************************************************************/
BYTE **DejDoPole(LPSTR lpDIBBits, LPBITMAPINFOHEADER lpbmi, LPPALETTEENTRY palBarvy, RECT rec)
{
	BYTE    **bPuvodni;//puvodni obr
	__int32 i,j,ix,jy;
	__int16 ii,i1;     //pro vypocet barev
	__int64 iPoradi;   //Poradi v lpDIBBits
	__int16 iZarovnat; //byty na zarovnani radku na 32 bitovy okraj
	double  d;
	long    Width,Height;

	iZarovnat = 0;
	tmavy  = 255; //nejtmasi bod
	svetly = 0;   //nejsvetlejsi bod

/*	bPuvodni = (BYTE**)calloc(lpbmi->biHeight, sizeof(BYTE*));
	for(i=0; i<lpbmi->biHeight; i++)
	{
		bPuvodni[i] = (BYTE*)calloc(lpbmi->biWidth, sizeof(BYTE));
	}*/
	Width  = abs(rec.right-rec.left);
	Height = abs(rec.top-rec.bottom);
	bPuvodni = (BYTE**)calloc(Height, sizeof(BYTE*));
	for(i=0; i<Height; i++)
	{
		bPuvodni[i] = (BYTE*)calloc(Width, sizeof(BYTE));
	}

	/////////////////////////// PUVODNI OBR //////////////////////////////
	//Pro rychlejsi praci se data nactou do bPuvodni, ale puze ve stupnich sede,
	//projistotu kdyby obrazek nebyl sedy, prepocitava se do odstinu sede
	iPoradi = 0;
	//rozdeleni podle poctu brev
	switch(lpbmi->biBitCount)
	{
		case 1:  //2^1=2 barvy
			//vypocet zbytku radku upraveneho na 32 bitovy okraj
			d = ((double)lpbmi->biWidth/32)-floor(lpbmi->biWidth/32);
			//if ((0.78125 <= d)||(d == 0   )) iZarovnat = 0;
			if ((0.03125 <= d)&&(d <= 0.25)) iZarovnat = 3;
			if ((0.28125 <= d)&&(d <= 0.5 )) iZarovnat = 2;
			if ((0.53125 <= d)&&(d <= 0.75)) iZarovnat = 1;
			ix = 0;
			for(i=0; i<lpbmi->biHeight; i++)
			{
				jy = 0;
				for(j=0; j<lpbmi->biWidth; j++)
				{
					for (i1 = 0; i1 < 8; i1++)
					{
						switch(i1)
						{
							case 0:
								ii = (__int16)(lpDIBBits[iPoradi] & 0x80);
							break;
							case 1:
								ii = (__int16)(lpDIBBits[iPoradi] & 0x40);
								j++;
							break;
							case 2:
								ii = (__int16)(lpDIBBits[iPoradi] & 0x20);
								j++;
							break;
							case 3:
								ii = (__int16)(lpDIBBits[iPoradi] & 0x10);
								j++;
							break;
							case 4:
								ii = (__int16)(lpDIBBits[iPoradi] & 0x8);
								j++;
							break;
							case 5:
								ii = (__int16)(lpDIBBits[iPoradi] & 0x4);
								j++;
							break;
							case 6:
								ii = (__int16)(lpDIBBits[iPoradi] & 0x2);
								j++;
							break;
							case 7:
								ii = (__int16)(lpDIBBits[iPoradi] & 0x1);
								j++;
							break;
							default :
								ii = 0;
							break;
						}
						if (j<lpbmi->biWidth)
						{
							//zjisteni cisla barvy (tady jen 0 nebo 1)
							ii = (__int16)(ii >> (7-i1));
							//zjistuje jestli se jedna o vybranou cast BMP
							if (((rec.left<=j)&&(j<rec.right))&&((rec.top<=i)&&(i<rec.bottom)))
							{
								bPuvodni[i][j] = Seda(palBarvy[ii]);
								if (tmavy>bPuvodni[i][j])
								{
									tmavy = bPuvodni[i][j];
								}
								if (svetly<bPuvodni[i][j])
								{
									svetly = bPuvodni[i][j];
								}
								jy++;
							}
						}
						else //je za rozmerem obrazku
						{
							i1 = 7;
						}
					}
					iPoradi++;
				}
				if (jy>0)
				{
					ix++;
				}
				iPoradi += iZarovnat;
			}
		break;
		case 4:  //2^4=16 barev
			//vypocet zbytku radku upraveneho na 32 bitovy okraj
			d = ((double)lpbmi->biWidth/8)-floor(lpbmi->biWidth/8);
			//if ((d == 0    )||(d == 0.875)) iZarovnat = 0;
			if ((d == 0.125)||(d == 0.25 )) iZarovnat = 3;
			if ((d == 0.375)||(d == 0.5  )) iZarovnat = 2;
			if ((d == 0.625)||(d == 0.75 )) iZarovnat = 1;
			ix = 0;
			for(i=0; i<lpbmi->biHeight; i++)
			{
				jy = 0;
				for(j=0; j<lpbmi->biWidth; j++)
				{
					ii = (__int16)(lpDIBBits[iPoradi] & 0xF0);
					ii = (__int16)(ii >> 4);
					//zjistuje jestli se jedna o vybranou cast BMP
					if (((rec.left<=j)&&(j<rec.right))&&((rec.top<=i)&&(i<rec.bottom)))
					{
						bPuvodni[i][j] = Seda(palBarvy[ii]);
						if (tmavy>bPuvodni[i][j])
						{
							tmavy = bPuvodni[i][j];
						}
						if (svetly<bPuvodni[i][j])
						{
							svetly = bPuvodni[i][j];
						}
						jy++;
					}
					j++;

					if (j<lpbmi->biWidth)
					{
						ii = (__int16)(lpDIBBits[iPoradi] & 0xF);
						//zjistuje jestli se jedna o vybranou cast BMP
						if (((rec.left<=j)&&(j<rec.right))&&((rec.top<=i)&&(i<rec.bottom)))
						{
							bPuvodni[i][j] = Seda(palBarvy[ii]);
							if (tmavy>bPuvodni[i][j])
							{
								tmavy = bPuvodni[i][j];
							}
							if (svetly<bPuvodni[i][j])
							{
								svetly = bPuvodni[i][j];
							}
							jy++;
						}
					}
					iPoradi++;
				}
				if (jy>0)
				{
					ix++;
				}
				iPoradi += iZarovnat;
			}
		break;
		case 8:  //2^8=256 barev
			//vypocet zbytku radku upraveneho na 32 bitovy okraj
			d = ((double)lpbmi->biWidth/4)-floor(lpbmi->biWidth/4);
			//if (d == 0   ) iZarovnat = 0;
			if (d == 0.25) iZarovnat = 3;
			if (d == 0.5 ) iZarovnat = 2;
			if (d == 0.75) iZarovnat = 1;
			ix = 0;
			for(i=0; i<lpbmi->biHeight; i++)
			{
				jy = 0;
				for(j=0; j<lpbmi->biWidth; j++)
				{
					//zjistuje jestli se jedna o vybranou cast BMP
					if (((rec.left<=j)&&(j<rec.right))&&((rec.top<=i)&&(i<rec.bottom)))
					{
						bPuvodni[i][j] = Seda(palBarvy[lpDIBBits[iPoradi]&0xff]);
						if (tmavy>bPuvodni[i][j])
						{
							tmavy = bPuvodni[i][j];
						}
						if (svetly<bPuvodni[i][j])
						{
							svetly = bPuvodni[i][j];
						}
						jy++;
					}
					iPoradi++;
				}
				if (jy>0)
				{
					ix++;
				}
				iPoradi += iZarovnat;
			}
		break;
		case 24: //2^24 = 16777216 barev
			//vypocet zbytku radku upraveneho na 32 bitovy okraj
			d = ((double)lpbmi->biWidth/4)-floor(lpbmi->biWidth/4);
			//if (d == 0   ) iZarovnat = 0;
			if (d == 0.25) iZarovnat = 1;
			if (d == 0.5 ) iZarovnat = 2;
			if (d == 0.75) iZarovnat = 3;
			ix = 0;
			for(i=0; i<lpbmi->biHeight; i++)
			{
				jy = 0;
				for(j=0; j<lpbmi->biWidth; j++)
				{
					//zjistuje jestli se jedna o vybranou cast BMP
					if (((rec.left<=j)&&(j<rec.right))&&((rec.top<=i)&&(i<rec.bottom)))
					{
						bPuvodni[ix][jy] = Seda(lpDIBBits[iPoradi]&0xff,lpDIBBits[iPoradi+1]&0xff,lpDIBBits[iPoradi+2]&0xff);
						if (tmavy>bPuvodni[i][j])
						{
							tmavy = bPuvodni[i][j];
						}
						if (svetly<bPuvodni[i][j])
						{
							svetly = bPuvodni[i][j];
						}
						jy++;
					}
					iPoradi +=3;
				}
				if (jy>0)
				{
					ix++;
				}
				iPoradi += iZarovnat;
			}
		break;
		default:
			for(i=0; i<lpbmi->biHeight; i++)
			{
				for(j=0; j<lpbmi->biWidth; j++)
				{
					bPuvodni[i][j] = 0;
					iPoradi++;
				}
			}
	}
	///// Konec ////////////// PUVODNI OBR //////////////////////////////
	return bPuvodni;
}

/*******************************************************************
detekce hran obrazku, podle Sobelove matice (3x3)
vrati dvojrozmerne pole s obrazkem v cerne a bile barve

lpbmi - LPBITMAPINFOHEADER - informace o obrazku
DPI - Obrazku
rec - obdelnik ve kterem se resi ostrost
*Ostrost - vypoctena hodnota ostrosti; cim vyssi cislo tak tim vyssi ostrost
*******************************************************************/
BYTE **HranySobel(BYTE **bPuvodni, WORD DPI, RECT rec, double *Ostrost)
{
	__int32 iSx,iSy;
	__int32 i,j;
	double  d;
	long    Width,Height;
	BYTE    **bHrany;  //obr hrany
	double  dPrah;     //prepocten podle DPI
	double  dPrumer;   //hodnoota ktera prekrocila prah
	__int64 iPoc;      //pocet prekroceni prahu

/*	bHrany = (BYTE**)calloc(lpbmi->biHeight, sizeof(BYTE*));
	for(i=0; i<lpbmi->biHeight; i++)
	{
		bHrany[i] = (BYTE*)calloc(lpbmi->biWidth, sizeof(BYTE));
	}*/
	Width  = abs(rec.right-rec.left);
	Height = abs(rec.bottom-rec.top);

	bHrany = (BYTE**)calloc(Height, sizeof(BYTE*));
	for(i=0; i<Height; i++)
	{
		bHrany[i] = (BYTE*)calloc(Width, sizeof(BYTE));
	}

	dPrumer = 0;
	iPoc = 0;

	if(DPI == 0)//pro pripad, ze neni DPI zadano
	{
		DPI = 300;
	}

	dPrah = (double)PRAH/DPI*200;

	for(i=0; i<Height; i++)
	{
		for(j=0; j<Width; j++)
		{
			if ((i==0)||(j==0)||(i==(Height-1))||(j==(Width-1)))
			{
				bHrany[i][j] = 255;
			}
			else
			{
				iSx = bPuvodni[i-1][j-1]+2*bPuvodni[i-1][j]+bPuvodni[i-1][j+1]
					-bPuvodni[i+1][j-1]-2*bPuvodni[i+1][j]-bPuvodni[i+1][j+1];
				iSy = bPuvodni[i-1][j-1]-bPuvodni[i-1][j+1]+2*bPuvodni[i][j-1]
					-2*bPuvodni[i][j+1]+bPuvodni[i+1][j-1]-bPuvodni[i+1][j+1];
				d = sqrt(iSx*iSx+iSy*iSy);
				if (d<dPrah)
				{
					bHrany[i][j] = 255;
				}
				else
				{
					bHrany[i][j] = 0;
					dPrumer += d;
					iPoc++;
				}
			}
		}
	}
	if (iPoc != 0)
	{
		*Ostrost = dPrumer/iPoc*DPI/200;
	}
	else
	{
		*Ostrost = 0;
	}

	return bHrany;
}

/*******************************************************************
Ulozi obrazek s hranami do a.bmp
lpbmi - LPBITMAPINFOHEADER - informace o obrazku
**bHrany - matice s hranami
*******************************************************************/
void UlozHrany(LPBITMAPINFOHEADER lpbmi, BYTE **bHrany, RECT rec)
{
	BOOL bRead;
	LPSTR pDIB;
	BITMAPFILEHEADER bmfHeader;
	BITMAPINFOHEADER bmiHeader;
	PALETTEENTRY palBarvy[2]; //paleta barev jen cerna a bila
	__int32 i,j;
	__int16 ii,i1;
	__int32 iPoradi;   //Poradi v pDIB
	long    Width,Height;

	DWORD dwBytesRead = 0;
	__int16 iZarovnat; //byty na zarovnani radku na 32 bitovy okraj
	double  d;

	iZarovnat = 0;

	Width  = abs(rec.right-rec.left);
	Height = abs(rec.bottom-rec.top);

//	LPCTSTR szFile = "a.bmp";
	// vytvori a.bmp
	if (FileNew == "")
	{
		LPCTSTR szFile = "a.bmp";
		FileNew = "a.bmp";
	}
	HANDLE hFile = CreateFile(FileNew, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
	_ASSERTE(INVALID_HANDLE_VALUE != hFile);

	/////////////// BITMAPFILEHEADER
	bmfHeader.bfType = 0x4d42;
	bmfHeader.bfSize = 0x3e+(DWORD)ceil((double)Width/32)*4*Height;
	bmfHeader.bfReserved1 = 0;
	bmfHeader.bfReserved2 = 0;
	bmfHeader.bfOffBits = 0x3e;

	bRead = WriteFile(hFile, (LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER), &dwBytesRead, NULL);
	_ASSERTE(FALSE != bRead);

	//BITMAPINFOHEADER
	bmiHeader.biSize = lpbmi->biSize;
	bmiHeader.biWidth = Width;
	bmiHeader.biHeight = Height;
	bmiHeader.biPlanes = lpbmi->biPlanes;
	bmiHeader.biBitCount = 1;
	bmiHeader.biCompression = lpbmi->biCompression;
	bmiHeader.biSizeImage = (DWORD)ceil((double)Width/32)*4*Height;
	bmiHeader.biXPelsPerMeter = lpbmi->biXPelsPerMeter;
	bmiHeader.biYPelsPerMeter = lpbmi->biYPelsPerMeter;
	bmiHeader.biClrUsed = 0;
	bmiHeader.biClrImportant = 0;
	bRead = WriteFile(hFile, (LPSTR)&bmiHeader, sizeof(BITMAPINFOHEADER), &dwBytesRead, NULL);
	_ASSERTE(FALSE != bRead);

	//PALETA - 2 barvy
	palBarvy[0].peRed   = 0;
	palBarvy[0].peGreen = 0;
	palBarvy[0].peBlue  = 0;
	palBarvy[0].peFlags = 0;
	palBarvy[1].peRed   = 0xff;
	palBarvy[1].peGreen = 0xff;
	palBarvy[1].peBlue  = 0xff;
	palBarvy[1].peFlags = 0;
	bRead = WriteFile(hFile, (LPSTR)&palBarvy, 2*sizeof(PALETTEENTRY), &dwBytesRead, NULL);
	_ASSERTE(FALSE != bRead);

	//PIXELY

	pDIB = (LPSTR)calloc (bmiHeader.biSizeImage, sizeof(BYTE));

	//vypocet zbytku radku upraveneho na 32 bitovy okraj
	d = ((double)Width/32)-floor(Width/32);
	//if ((0.78125 <= d)||(d == 0   )) iZarovnat = 0;
	if ((0.03125 <= d)&&(d <= 0.25)) iZarovnat = 3;
	if ((0.28125 <= d)&&(d <= 0.5 )) iZarovnat = 2;
	if ((0.53125 <= d)&&(d <= 0.75)) iZarovnat = 1;

	iPoradi = 0;
	for(i=0; i<Height; i++)
	{
		ii = 0;
		i1 = 0;
		for(j=0; j<Width; j++)
		{
			switch(i1)
			{
				case 0:
					if (bHrany[i][j] == 255)
					{
						ii = 0x80;
					}
				break;
				case 1:
					if (bHrany[i][j] == 255)
					{
						ii += 0x40;
					}
				break;
				case 2:
					if (bHrany[i][j] == 255)
					{
						ii += 0x20;
					}
				break;
				case 3:
					if (bHrany[i][j] == 255)
					{
						ii += 0x10;
					}
				break;
				case 4:
					if (bHrany[i][j] == 255)
					{
						ii += 0x8;
					}
				break;
				case 5:
					if (bHrany[i][j] == 255)
					{
						ii += 0x4;
					}
				break;
				case 6:
					if (bHrany[i][j] == 255)
					{
						ii += 0x2;
					}
				break;
				case 7:
					if (bHrany[i][j] == 255)
					{
						ii += 0x1;
					}
					pDIB[iPoradi] = (char)ii;
					iPoradi++;
					ii = 0;
					i1 = -1;
				break;
				default :
					ii += 0;
				break;
			}
			i1++;
		}
		if (i1 != 0) //nepropbehlo zapamotovani pixelu
		{
			pDIB[iPoradi] = (char)ii;
			iPoradi++;
		}
		for(i1=0; i1<iZarovnat; i1++)
		{
			pDIB[iPoradi] = 0;
			iPoradi++;
		}
	}

	bRead = WriteFile(hFile, pDIB, iPoradi, &dwBytesRead, NULL);
	_ASSERTE(FALSE != bRead);

	CloseHandle(hFile);

}

/*******************************************************************
prevede paletu barev na barvy
lpbmi - LPBITMAPINFOHEADER - informace o obrazku
hPal - paleta obrazku
*******************************************************************/
LPPALETTEENTRY BarvyPalety(LPBITMAPINFOHEADER lpbmi, HPALETTE hPal)
{
	LPPALETTEENTRY palBarvy; //paleta barev z puvodniho obrazku
	__int32 PocetBarev;

	//paleta berev puvodniho obrazku
	PocetBarev = (__int32)pow(2,lpbmi->biBitCount);
	if (hPal != NULL)
	{
		palBarvy = (LPPALETTEENTRY)calloc(PocetBarev, sizeof(PALETTEENTRY));
		UINT i = GetPaletteEntries(hPal, 0, PocetBarev, palBarvy);
	}

	return palBarvy;
}

/*******************************************************************
detekce hran obrazku a v navratove hodnote ostrost
lpDIBBits - obrazek v bitech
lpbmi - LPBITMAPINFOHEADER - informace o obrazku
hPal - paleta obrazku
*******************************************************************/
double DetekceHran(LPSTR lpDIBBits, LPBITMAPINFOHEADER lpbmi, HPALETTE hPal, RECT rec)
{
	LPPALETTEENTRY palBarvy; //paleta barev z puvodniho obrazku
	BYTE    **bPuvodni;//puvodni obr
	BYTE    **bHrany;  //obr hrany
	WORD    wDPI;
	double  Ostrost;  //vypoctena hodnota ostrosti; cim vyssi cislo tak tim vyssi ostrost

	//paleta berev puvodniho obrazku
	palBarvy = BarvyPalety(lpbmi, hPal);

	/////////////////////////// PUVODNI OBR //////////////////////////////
	bPuvodni = DejDoPole(lpDIBBits, lpbmi, palBarvy, rec);

	/////////////////////////// DPI //////////////////////////////////////
	wDPI = 300;
	if (lpbmi->biXPelsPerMeter != 0)
	{
		wDPI = (WORD)((double)lpbmi->biXPelsPerMeter/1000*25.4);
	}
	else //DPI neni u obrzku a tak zjisti jestli nebylo zadane
	{
		if(DPI!=0)
		{
			wDPI = DPI;
		}
	}
	/////////////////////////// OBR HRANY SOBELOVE ///////////////////////
	bHrany = HranySobel(bPuvodni, wDPI, rec, &Ostrost);

	return Ostrost;
}

/*******************************************************************
detekce hran obrazku, ktery ulozi do a.bmp a v navratove hodnote ostrost
lpDIBBits - obrazek v bitech
lpbmi - LPBITMAPINFOHEADER - informace o obrazku
hPal - paleta obrazku
*******************************************************************/
double DetekceHranAUloz(LPSTR lpDIBBits, LPBITMAPINFOHEADER lpbmi, HPALETTE hPal, RECT rec)
{
	LPPALETTEENTRY palBarvy; //paleta barev z puvodniho obrazku
	BYTE    **bPuvodni;//puvodni obr
	BYTE    **bHrany;  //obr hrany
	WORD    wDPI;
	double  Ostrost;  //vypoctena hodnota ostrosti; cim vyssi cislo tak tim vyssi ostrost

	//paleta berev puvodniho obrazku
	palBarvy = BarvyPalety(lpbmi, hPal);

	/////////////////////////// PUVODNI OBR //////////////////////////////
	bPuvodni = DejDoPole(lpDIBBits, lpbmi, palBarvy, rec);

	/////////////////////////// DPI //////////////////////////////////////
	wDPI = 300;
/*	if (lpbmi->biXPelsPerMeter != 0)
	{
		wDPI = (WORD)((double)lpbmi->biXPelsPerMeter/1000*25.4);
	}
	else //DPI neni u obrzku a tak zjisti jestli nebylo zadane
	{
		if(DPI!=0)
		{
			wDPI = DPI;
		}
	}*/
	if(DPI!=0)
	{
		wDPI = DPI;
	}
	/////////////////////////// OBR HRANY SOBELOVE ///////////////////////
	bHrany = HranySobel(bPuvodni, wDPI, rec, &Ostrost);
	UlozHrany(lpbmi, bHrany, rec);

	return Ostrost;
}

double ZjistiOstrostAUloz(HDC hdc, HDIB hDIB)
{
	LPSTR    pDIB;
	LPBITMAPINFOHEADER lpbmi; // pointer to a Win 3.0-style DIB
	double   dVysl;
	RECT     rec;

	dVysl = -1;

	if (hDIB == NULL)
	{
		return dVysl;
	}

	pDIB = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);

	//BITMAPINFOHEADER struktura z bitmapy
	lpbmi = (LPBITMAPINFOHEADER)pDIB;

	rec.left   = 0;
	rec.bottom = 0;
	rec.right  = lpbmi->biWidth;
	rec.top    = lpbmi->biHeight;

	dVysl = ZjistiOstrostAUloz(hdc, hDIB, rec);
	return dVysl;
}

double ZjistiOstrostAUloz(HDC hdc, HDIB hDIB, RECT rec)
{
	LPSTR    pDIB;
	LPBITMAPINFOHEADER lpbmi; // pointer to a Win 3.0-style DIB
	LPSTR    lpDIBHdr;        // Pointer to BITMAPINFOHEADER
	LPSTR    lpDIBBits;       // Pointer to DIB bits
	HPALETTE hPal;
	double   dVysl;

	dVysl = -1;

	if (hDIB == NULL)
	{
		return dVysl;
	}

	pDIB = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);

	//BITMAPINFOHEADER struktura z bitmapy
	lpbmi = (LPBITMAPINFOHEADER)pDIB;
	//jednotlive pixely
	lpDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
	lpDIBBits = ::FindDIBBits(lpDIBHdr);

	//paleta
	hPal = NULL;
	if (::CreateDIBPalette(hDIB, &hPal) == NULL)
	{
		// neni paleta
		//2^24=16777216 nema pelatu
		delete hPal;
		hPal = NULL;
	}

	//BITMAPA je ulozena odspodu
	//prehozeni RECT do pojeti jak je to v BMP
	rec.bottom = lpbmi->biHeight-rec.bottom;
	rec.top    = lpbmi->biHeight-rec.top;
	//serazeni polozek rec
	if (rec.top  > rec.bottom)
	{
		rec.top   += rec.bottom;
		rec.bottom = rec.top-rec.bottom;
		rec.top   -= rec.bottom;
	}
	if (rec.left > rec.right)
	{
		rec.left += rec.right;
		rec.right = rec.left-rec.right;
		rec.left -= rec.right;
	}

	dVysl = DetekceHranAUloz(lpDIBBits, lpbmi, hPal, rec);		//ULOZENI

	DeleteObject(hPal);

	return dVysl;
}

double ZjistiOstrost(HDC hdc, HDIB hDIB)
{
	LPSTR    pDIB;
	LPBITMAPINFOHEADER lpbmi; // pointer to a Win 3.0-style DIB
	double   dVysl;
	RECT     rec;

	dVysl = -1;

	if (hDIB == NULL)
	{
		return dVysl;
	}

	pDIB = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);

	//BITMAPINFOHEADER struktura z bitmapy
	lpbmi = (LPBITMAPINFOHEADER)pDIB;

	rec.left   = 0;
	rec.bottom = 0;
	rec.right  = lpbmi->biWidth;
	rec.top    = lpbmi->biHeight;

	dVysl = ZjistiOstrost(hdc, hDIB, rec);

	return dVysl;
}

double ZjistiOstrost(HDC hdc, HDIB hDIB, RECT rec)
{
	LPSTR    pDIB;
	LPBITMAPINFOHEADER lpbmi; // pointer to a Win 3.0-style DIB
	LPSTR    lpDIBHdr;        // Pointer to BITMAPINFOHEADER
	LPSTR    lpDIBBits;       // Pointer to DIB bits
	HPALETTE hPal;
	double   dVysl;

	dVysl = -1;

	if (hDIB == NULL)
	{
		return dVysl;
	}

	pDIB = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);

	//BITMAPINFOHEADER struktura z bitmapy
	lpbmi = (LPBITMAPINFOHEADER)pDIB;
	//jednotlive pixely
	lpDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
	lpDIBBits = ::FindDIBBits(lpDIBHdr);

	//paleta
	hPal = NULL;
	if (::CreateDIBPalette(hDIB, &hPal) == NULL)
	{
		// neni paleta
		//2^24=16777216 nema pelatu
		delete hPal;
		hPal = NULL;
	}

	//BITMAPA je ulozena odspodu
	//prehozeni RECT do pojeti jak je to v BMP
	rec.bottom -= lpbmi->biHeight;
	rec.top     = lpbmi->biHeight-rec.top;

	dVysl = DetekceHran(lpDIBBits, lpbmi, hPal, rec);

	DeleteObject(hPal);

	return dVysl;
}
