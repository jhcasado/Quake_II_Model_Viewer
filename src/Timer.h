/* ==============================================================================
	
	  Vmd: Visualizador de modelos del quake II
	  Versión: 1.0.0b

	  Autores: José Angel Montelongo Reyes 
			   Javier Honorio Casado Fernández

	  Universidad de Las Palmas de Gran Canaria	
	  	  
   ============================================================================== */


// timer.h


#ifndef __TIMER_H__
#define __TIMER_H__


#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>


#define FPS_INTERVAL 0.5f


class CTimer
{
public:

// atributos

	BOOL   m_bRunning;	
	UINT64 m_BaseTicks;
	UINT64 m_StopTicks;
	UINT64 m_TicksPerSecond;
	float  m_fFrameTime;
	float  m_fDeltaTime;
	DWORD  m_dwNumFrames;
	float  m_fLastUpdate;
	float  m_fFps;

// métodos

	CTimer();
	void  Start();
	void  Stop();
	void  Continue();
	float GetTime();
	void  Frame_Rate();
};

#endif	

// timer.h