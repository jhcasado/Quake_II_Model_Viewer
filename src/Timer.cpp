/* ==============================================================================
	
	  Vmd: Visualizador de modelos del quake II
	  Versión: 1.0.0b

	  Autores: José Angel Montelongo Reyes 
			   Javier Honorio Casado Fernández

	  Universidad de Las Palmas de Gran Canaria	
	  	  
   ============================================================================== */


// timer.cpp


#include "stdafx.h"
#include "timer.h"


CTimer::CTimer()
// constructor de la clase
{
    if(!QueryPerformanceFrequency((LARGE_INTEGER *)&m_TicksPerSecond))
	{
        m_TicksPerSecond = 1000;
	}

	m_fFps = 0;
	m_bRunning = FALSE;
}

void CTimer::Start()
// inicia el timer
{
    if(!QueryPerformanceCounter((LARGE_INTEGER *)&m_BaseTicks))
    {
        m_BaseTicks = (UINT64)timeGetTime();
    }

	m_bRunning = TRUE;

	m_fLastUpdate = 0;
	m_dwNumFrames = 0;

	m_fFrameTime = 0;
	m_fDeltaTime = 0;
}

void CTimer::Stop()
// para el timer
{
	if(m_bRunning)
	{
		if(!QueryPerformanceCounter((LARGE_INTEGER *)&m_StopTicks))
		{
			m_StopTicks = (UINT64)timeGetTime();
		}

		m_bRunning = FALSE;
	}
}

void CTimer::Continue()
// reestablese el timer
{
	if(!m_bRunning)
	{
		UINT64 Ticks;

		if(!QueryPerformanceCounter((LARGE_INTEGER *)&Ticks))
		{
			Ticks = (UINT64)timeGetTime();
		}

		m_BaseTicks += Ticks - m_StopTicks;

		m_bRunning = TRUE;
	}
}

float CTimer::GetTime()
// devuelve el tiempo actual
{
	UINT64 Ticks;

	if(m_bRunning)
	{
		if(!QueryPerformanceCounter((LARGE_INTEGER *)&Ticks))
		{
			Ticks = (UINT64)timeGetTime();
		}
	}

	else
	{
		Ticks = m_StopTicks;
	}

	Ticks -= m_BaseTicks;

	return (float)(__int64)Ticks/(float)(__int64)m_TicksPerSecond;
}

void CTimer::Frame_Rate()
// calcula el frame-rate
{
	m_fDeltaTime = GetTime() - m_fFrameTime;
	m_fFrameTime += m_fDeltaTime;

	m_dwNumFrames ++;

	if ((m_fFrameTime - m_fLastUpdate) > FPS_INTERVAL)
	{
		m_fFps = m_dwNumFrames / (m_fFrameTime - m_fLastUpdate);
		m_dwNumFrames = 0;
		m_fLastUpdate = m_fFrameTime;
	}
}

// timer.cpp