/* ==============================================================================
	
	  Vmd: Visualizador de modelos del quake II
	  Versión: 1.0.0b

	  Autores: José Angel Montelongo Reyes 
			   Javier Honorio Casado Fernández

	  Universidad de Las Palmas de Gran Canaria	
	  	  
   ============================================================================== */


#if !defined(AFX_DLG_ANIMACIONES_H__B8701021_5697_11D5_9D1F_E91A9C1F821E__INCLUDED_)
#define AFX_DLG_ANIMACIONES_H__B8701021_5697_11D5_9D1F_E91A9C1F821E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// dlg_animaciones.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// dlg_animaciones dialog

#include "vmdDoc.h"

class dlg_animaciones : public CDialog
{
// Construction
public:
	dlg_animaciones(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(dlg_animaciones)
	enum { IDD = IDD_ANIMACIONES_DIALOG };
	int		m_frame;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(dlg_animaciones)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void V_Actualizar(); // actualiza los controles de la ventana

	CVmdDoc *doc;

protected:

	// Generated message map functions
	//{{AFX_MSG(dlg_animaciones)
	virtual BOOL OnInitDialog();
	afx_msg void OnBAnimacionesCerrar();
	afx_msg void OnCustomdrawSliderInter(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeElegirAnimacion();
	afx_msg void OnBAnimacionesPlay();
	afx_msg void OnBSetFrame();
	afx_msg void OnBResetAnim();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_ANIMACIONES_H__B8701021_5697_11D5_9D1F_E91A9C1F821E__INCLUDED_)
