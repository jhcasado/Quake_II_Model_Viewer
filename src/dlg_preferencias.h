/* ==============================================================================
	
	  Vmd: Visualizador de modelos del quake II
	  Versión: 1.0.0b

	  Autores: José Angel Montelongo Reyes 
			   Javier Honorio Casado Fernández

	  Universidad de Las Palmas de Gran Canaria	
	  	  
   ============================================================================== */


#if !defined(AFX_DLG_PREFERENCIAS_H__2203F380_65E8_11D5_9D1F_E0B6519F9D1F__INCLUDED_)
#define AFX_DLG_PREFERENCIAS_H__2203F380_65E8_11D5_9D1F_E0B6519F9D1F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// dlg_preferencias.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// dlg_preferencias dialog

#include "vmdDoc.h"

class dlg_preferencias : public CDialog
{
// Construction
public:
	dlg_preferencias(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(dlg_preferencias)
	enum { IDD = IDD_PREFERENCIAS_DIALOG };
	float	m_vel_mod_tras;
	float	m_vel_mod_rot;
	float	m_vel_tras_luz;
	float	m_vel_cam_tras;
	float	m_vel_cam_rot;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(dlg_preferencias)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
	
protected:

	// Generated message map functions
	//{{AFX_MSG(dlg_preferencias)
	afx_msg void OnBColorFondo();
	virtual BOOL OnInitDialog();
	afx_msg void OnPreferenciasReset();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_PREFERENCIAS_H__2203F380_65E8_11D5_9D1F_E0B6519F9D1F__INCLUDED_)
