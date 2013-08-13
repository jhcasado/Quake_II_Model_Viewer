/* ==============================================================================
	
	  Vmd: Visualizador de modelos del quake II
	  Versión: 1.0.0b

	  Autores: José Angel Montelongo Reyes 
			   Javier Honorio Casado Fernández

	  Universidad de Las Palmas de Gran Canaria	
	  	  
   ============================================================================== */


#if !defined(AFX_DLG_CAMARA_H__5CF47DA0_6220_11D5_986D_977D9FBE9026__INCLUDED_)
#define AFX_DLG_CAMARA_H__5CF47DA0_6220_11D5_986D_977D9FBE9026__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// dlg_camara.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// dlg_camara dialog

#include "vmdDoc.h"

class dlg_camara : public CDialog
{
// Construction
public:
	dlg_camara(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(dlg_camara)
	enum { IDD = IDD_CAMARA_DIALOG };
	float	m_fov;
	float	m_aspect_x;
	float	m_aspect_y;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(dlg_camara)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
		CVmdDoc *doc;

protected:

	// Generated message map functions
	//{{AFX_MSG(dlg_camara)
	virtual BOOL OnInitDialog();
	afx_msg void OnBCamaraCerrar();
	afx_msg void OnBCamaraAplicar();
	afx_msg void OnBCamaraReset();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_CAMARA_H__5CF47DA0_6220_11D5_986D_977D9FBE9026__INCLUDED_)
