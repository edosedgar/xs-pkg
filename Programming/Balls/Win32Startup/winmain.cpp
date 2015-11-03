#define _CRT_SECURE_NO_DEPRECATE 1
#undef UNICODE
#undef _UNICODE
#include <windows.h>
#include <windowsx.h>
#include <math.h>
#include <time.h>

//================================ Defines ============================================

#define Round(x) (x-floor((x)) >= 0.5 ? ceil((x)) : floor((x))) 
#define RandomFloat() ((double) rand() / (RAND_MAX+1.0))

//******************************** Constants ********************************************

const unsigned int StandartNumber	= 50; //Balls
const unsigned int ClientWidth		= 200;
const unsigned int ClientHeight		= 200;
const unsigned int TimerInterval_ms	= 1;
unsigned long int ForegroundColor	= 0xFFFFFF;
unsigned long int BackgroundColor	= 0x000000;

double StartInterval = 0;

const double MinRadius		=3;
const double MaxRadius		= 3;

const double MinWeight		= 1.0;
const double MaxWeight		= 1.0;

const double MinSpeedX		= -500.0;
const double MaxSpeedX		= 500.0;
const double MinSpeedY		= -500.0;
const double MaxSpeedY		= 500.0;

double Accuracy	= 0.1;

//******************************** Variable ********************************************

struct TBall
{
	unsigned long int Color;
	double XPos;
	double YPos;
	double Weight;
	double Radius;
	double SpeedX;
	double SpeedY;
	int IsVisible;
};

TBall* Balls;

unsigned FPS = 1;
unsigned FPSCounter = 0;
char* FPSString = NULL;
char* AverageSpeedString = NULL;
time_t OldTime = 0;
time_t NewTime = 0;

int RefreshFlag = 0;
	

//**************************************************************************************

//********************************* Prototype ******************************************

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);
TBall* CreateBalls ( int Number );
int DoMove ( TBall* Balls, int Number );
int DrawBalls ( HDC hdc, HWND hwnd, TBall* Balls, int Number );
int DoCollisionWithWall ( TBall* Balls, HWND hwnd, int Number );
int DoBallsCollision ( TBall* Balls, int Number );
int DestroyStartColliseBalls ( TBall* Balls, int Number );
double GetAverageSpeed ( TBall* Balls, int Number );

//********************************* WinMain ********************************************

int WINAPI WinMain ( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{     
	static char *classname = "WinClassName";
	MSG msg; 
	HWND hwnd;
	WNDCLASSEX wndclass;

	wndclass.cbSize        = sizeof ( wndclass );
	wndclass.style         = 0;
	wndclass.lpfnWndProc   = WndProc;
	wndclass.cbClsExtra    = 0;
	wndclass.cbWndExtra    = 0;
	wndclass.hInstance     = hInstance;
	wndclass.hIcon         = LoadIcon ( NULL, IDI_APPLICATION );
	wndclass.hCursor       = LoadCursor ( NULL, IDC_ARROW );
	wndclass.hbrBackground = ( HBRUSH ) GetStockObject ( BLACK_BRUSH );
	wndclass.lpszMenuName  = NULL;
	wndclass.lpszClassName = classname;
	wndclass.hIconSm       = LoadIcon ( NULL, IDI_APPLICATION );

	RegisterClassEx (&wndclass);

	//*************************** CreateWindow ***************************

    hwnd = CreateWindow (	classname,								// window class name
							"Balls",								// window caption
							WS_OVERLAPPEDWINDOW | WS_SYSMENU,		// window style
							CW_USEDEFAULT,							// initial x position
							CW_USEDEFAULT,							// initial y position
							ClientWidth + 7,					    // initial x size
							ClientHeight + 30,						// initial y size
							0,										// parent window handle
							0,										// window menu handle
							hInstance,								// program instance handle
							NULL );									// creation parameters

	ShowWindow ( hwnd, SW_SHOWNORMAL );
    UpdateWindow ( hwnd );

	//*********************************************************************

	while ( GetMessage ( &msg , 0, 0, 0 ) )
	{  
		time (&NewTime);
		if (NewTime-OldTime == 1)
		{
			FPS = FPSCounter;
			FPSCounter = 0;
		}
		OldTime=NewTime;
		TranslateMessage ( &msg );
		DispatchMessage ( &msg );
	}
    return msg.wParam;
}


/*********************************************************************************************/
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//
/*********************************************************************************************/		


LRESULT CALLBACK WndProc ( HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam )
{     
	HDC hdc;  
	HDC MemoryHDC;
	PAINTSTRUCT ps;  

	switch ( iMsg )
	{   
	//************************************************************
	case WM_CREATE:
	{
		Balls = CreateBalls ( StandartNumber );
		SetTimer ( hwnd, 1, TimerInterval_ms, NULL );
		FPSString = (char*)calloc(10, sizeof(char));
		AverageSpeedString = (char*)calloc(100, sizeof(char));
		strcpy(FPSString, "FPS: ");
		strcpy(AverageSpeedString, "Average Speed: ");
		return 0;
	}
	//************************************************************
    case WM_PAINT:
	{
		RECT ClientSize;
		GetClientRect ( hwnd, &ClientSize );
		hdc = BeginPaint ( hwnd, &ps );

		MemoryHDC = CreateCompatibleDC ( hdc );		
		HBITMAP BitmapHDC = CreateCompatibleBitmap( hdc, ClientSize.right, ClientSize.bottom );
		SelectObject ( MemoryHDC, BitmapHDC );


		if ( RefreshFlag )
		{
			DoMove ( Balls, StandartNumber );
			DoCollisionWithWall ( Balls, hwnd, StandartNumber );
			DoBallsCollision ( Balls, StandartNumber );
			DrawBalls ( MemoryHDC, hwnd, Balls, StandartNumber );
			itoa(FPS, FPSString+5, 10);
			itoa(Round(GetAverageSpeed(Balls, StandartNumber)), AverageSpeedString+15, 10 );
			TextOut (MemoryHDC, 5, 5, (LPCSTR)FPSString, strlen(FPSString));
			TextOut (MemoryHDC, 5, 20, (LPCSTR)AverageSpeedString, strlen(AverageSpeedString));
			BitBlt ( hdc, 0, 0, ClientSize.right, ClientSize.bottom, MemoryHDC, 0, 0, SRCCOPY );
			RefreshFlag = 0;
			FPSCounter++;
		};

		DeleteObject ( BitmapHDC );
		DeleteDC ( MemoryHDC );

		EndPaint ( hwnd, &ps );
		return 0;
	}
	//************************************************************
	case WM_TIMER:
	{
		RefreshFlag = 1;
		InvalidateRect( hwnd, NULL, FALSE );
		return 0;
	}
	//************************************************************
	case WM_DESTROY:
	{
		free(FPSString);
		free(AverageSpeedString);
		KillTimer ( hwnd, 1 );
		PostQuitMessage( 0 );
		free ( Balls );
		return 0;
	}
	//************************************************************
	}; 
    return DefWindowProc (hwnd, iMsg, wParam, lParam);
 }  

/*********************************************************************************************/
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//
/*********************************************************************************************/

int DrawBalls ( HDC hdc, HWND hwnd, TBall* Balls, int Number )
{
	static HPEN HPen;
	static HBRUSH HBrush;

	HBrush = CreateSolidBrush ( BackgroundColor );

	RECT ClientSize;
	GetClientRect( hwnd, &ClientSize );
	FillRect( hdc, &ClientSize, HBrush );

	DeleteObject ( HBrush );

	for ( unsigned int i = 0; i < Number; i++ )
	{
		HPen = CreatePen ( 0, 1, ForegroundColor );
		HBrush = CreateSolidBrush ( Balls[i].Color );
		SelectBrush ( hdc, HBrush );
		SelectPen ( hdc, HPen );
		//If IsVisible == 1 then draw ellipse -------------------------
		Balls[i].IsVisible && \
		Ellipse ( hdc,(int)(Round(Balls[i].XPos - Balls[i].Radius)), \
					  (int)(Round(Balls[i].YPos - Balls[i].Radius)), \
					  (int)(Round(Balls[i].XPos + Balls[i].Radius)), \
					  (int)(Round(Balls[i].YPos + Balls[i].Radius)) ); 
		//=================================== -------------------------
		DeleteObject ( HBrush );
		DeleteObject ( HPen );
	}
	return 0;
};

/*********************************************************************************************/
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//
/*********************************************************************************************/

TBall* CreateBalls ( int Number )
{
	srand( ( int ) ( time ( NULL ) ) );
	TBall* Balls = ( TBall* ) calloc ( Number, sizeof ( TBall ) );
	for ( unsigned int i = 0; i < Number; i++ )
	{
		Balls[i].IsVisible = 1;
		Balls[i].Color = rand ();
		/*if ( i==0 )
			Balls[i].Color = 0x0000FF;
		else
			Balls[i].Color = 0x000000;*/
		Balls[i].Radius = RandomFloat()*(MaxRadius - MinRadius)+MinRadius;
		Balls[i].Weight = RandomFloat()*(MaxWeight - MinWeight)+MinWeight;
		Balls[i].XPos = RandomFloat()*((double)(ClientWidth) \
									   - Balls[i].Radius)+Balls[i].Radius;
		Balls[i].YPos = RandomFloat()*((double)(ClientHeight ) \
									   - Balls[i].Radius)+Balls[i].Radius;
		Balls[i].SpeedX = MaxSpeedX - RandomFloat()*(MaxSpeedX - MinSpeedX);
		Balls[i].SpeedY = MaxSpeedY - RandomFloat()*(MaxSpeedY - MinSpeedY);
		//================================================================
		/*if ( i < Number/2 )
		{
			Balls[i].XPos = RandomFloat()*((double)(ClientWidth/4) \
										   - Balls[i].Radius)+Balls[i].Radius;
			Balls[i].YPos = (double)(7*ClientHeight/16) + RandomFloat()*((double)(ClientHeight/8) \
										   - Balls[i].Radius)+Balls[i].Radius;
		} else
		{
			Balls[i].XPos = (double)(3*ClientWidth/4) + RandomFloat()*((double)(ClientWidth/4) \
										   - Balls[i].Radius)+Balls[i].Radius;
			Balls[i].YPos = (double)(7*ClientHeight/16) + RandomFloat()*((double)(ClientHeight/8) \
										   - Balls[i].Radius)+Balls[i].Radius;
		}
		if ( i < Number/2 )
		{
			Balls[i].SpeedX = RandomFloat()*(MaxSpeedX - MinSpeedX);
			//Balls[i].SpeedY = MaxSpeedY - RandomFloat()*(MaxSpeedY - MinSpeedY);
		} else
		{
			Balls[i].SpeedX =  - RandomFloat()*(MaxSpeedX - MinSpeedX);
			//Balls[i].SpeedY = MaxSpeedY - RandomFloat()*(MaxSpeedY - MinSpeedY);
		}*/
		//================================================================
		if ( fabs ( Balls[i].SpeedX ) <= Accuracy ) 
			Balls[i].SpeedX = MinSpeedX;
		if ( fabs ( Balls[i].SpeedY ) <= Accuracy ) 
			Balls[i].SpeedY = MinSpeedY;
		//================================================================
	}
	//DestroyStartColliseBalls( Balls, Number );
	return Balls;
}

/*********************************************************************************************/
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//
/*********************************************************************************************/

int DoMove ( TBall* Balls, int Number )
{
	if ( FPS > 70 || FPS < 60 ) 
		return 0;
	for ( int i = 0; i < Number; i++ )
	{
		Balls[i].XPos += (Balls[i].SpeedX)/FPS;
		Balls[i].YPos += (Balls[i].SpeedY)/FPS;
	}

	return 0;
}

/*********************************************************************************************/
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//
/*********************************************************************************************/

int DoCollisionWithWall ( TBall* Balls, HWND hwnd, int Number )
{
	RECT ClientSize;
	GetClientRect( hwnd, &ClientSize );

	for ( int i = 0; i < StandartNumber; i++ ) 
	{
		if (Balls[i].XPos < Balls[i].Radius) 
			Balls[i].XPos = Balls[i].Radius;

		if (Balls[i].XPos > ClientSize.right - Balls[i].Radius) 
			Balls[i].XPos = ClientSize.right - Balls[i].Radius;

		if (Balls[i].YPos < Balls[i].Radius) 
			Balls[i].YPos = Balls[i].Radius;

		if (Balls[i].YPos > ClientSize.bottom - Balls[i].Radius) 
			Balls[i].YPos = ClientSize.bottom - Balls[i].Radius;

		if ( (Balls[i].XPos - Balls[i].Radius <= 0 && Balls[i].SpeedX < 0) || \
			 (Balls[i].XPos + Balls[i].Radius >= (ClientSize.right - ClientSize.left) && Balls[i].SpeedX > 0) )
			Balls[i].SpeedX = -Balls[i].SpeedX;

		if ( (Balls[i].YPos - Balls[i].Radius <= 0 && Balls[i].SpeedY < 0) || \
			 (Balls[i].YPos + Balls[i].Radius >= (ClientSize.bottom - ClientSize.top) &&
			 Balls[i].SpeedY > 0) )
			Balls[i].SpeedY = -Balls[i].SpeedY;
	}
	return 0;
}

/*********************************************************************************************/
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//
/*********************************************************************************************/

int DoBallsCollision ( TBall* Balls, int Number )
{
	for ( int i = 0; i < StandartNumber; i++ )
	for ( int j = i + 1; j < StandartNumber; j++ )
	{
		double dist = sqrt( (Balls[i].XPos - Balls[j].XPos) * (Balls[i].XPos - Balls[j].XPos) + \
						    (Balls[i].YPos - Balls[j].YPos) * (Balls[i].YPos - Balls[j].YPos) );

		if ( dist < (Balls[i].Radius + Balls[j].Radius) && Balls[i].IsVisible == 1 \
														&& Balls[j].IsVisible == 1 )
		{
			double a = Balls[i].XPos - Balls[j].XPos;
			double b = Balls[i].YPos - Balls[j].YPos;

			double p1 = a * b / (dist * dist);
			double p2 = a * a / (dist * dist);
			double p3 = b * b / (dist * dist);

			double d1 = Balls[i].SpeedY * p1 + Balls[i].SpeedX * p2 - Balls[j].SpeedY * p1 - Balls[j].SpeedX * p2;
			double d2 = Balls[i].SpeedX * p1 + Balls[i].SpeedY * p3 - Balls[j].SpeedX * p1 - Balls[j].SpeedY * p3;

			Balls[i].SpeedX = Balls[i].SpeedX - d1;
			Balls[i].SpeedY = Balls[i].SpeedY - d2;
			Balls[j].SpeedX = Balls[j].SpeedX + d1;
			Balls[j].SpeedY = Balls[j].SpeedY + d2;

			p3 = (Balls[i].Radius + Balls[j].Radius - dist) / 2;
			p1 = p3 * (a / dist);
			p2 = p3 * (b / dist);

			Balls[i].XPos = Balls[i].XPos + p1;
			Balls[i].YPos = Balls[i].YPos + p2;
			Balls[j].XPos = Balls[j].XPos - p1;
			Balls[j].YPos = Balls[j].YPos - p2;
		}
	}
	return 0;
}

/*********************************************************************************************/
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//
/*********************************************************************************************/

int DestroyStartColliseBalls ( TBall* Balls, int Number )
{
	for ( int i = 0; i < Number; i++ )
	{
		for ( int j = i + 1; j < Number; j++ )
		{
			if ( sqrt ( pow(Balls[i].XPos-Balls[j].XPos, 2 ) + \
						pow(Balls[i].YPos-Balls[j].YPos, 2 ) ) < \
						Balls[i].Radius + Balls[j].Radius + StartInterval )
			{
				Balls[i].IsVisible = 0;
				Balls[j].IsVisible = 0;
			}
		}
	}
	return 0;
}

/*********************************************************************************************/
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$//
/*********************************************************************************************/

double GetAverageSpeed ( TBall* Balls, int Number )
{
	double Sum = 0;
	for ( int i = 0; i < Number; i++ )
		Sum += sqrt ( (Balls[i].SpeedX)*(Balls[i].SpeedX) + (Balls[i].SpeedY)*(Balls[i].SpeedY) );
	return Sum;
}