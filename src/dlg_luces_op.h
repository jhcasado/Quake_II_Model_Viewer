/* ==============================================================================
	
	  Vmd: Visualizador de modelos del quake II
	  Versión: 1.0.0b

	  Autores: José Angel Montelongo Reyes 
			   Javier Honorio Casado Fernández

	  Universidad de Las Palmas de Gran Canaria	
	  	  
   ============================================================================== */


#if !defined(AFX_DLG_LUCES_OP_H__60A64DE0_6F0C_11D5_9D1F_E564A9CD811F__INCLUDED_)
#define AFX_DLG_LUCES_OP_H__60A64DE0_6F0C_11D5_9D1F_E564A9CD811F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// dlg_luces_op.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// dlg_luces_op dialog

#include "vmdDoc.h"

class dlg_luces_op : public CDialog
{
// Construction
public:
	dlg_luces_op(CWnd* pParent = NULL);   // standard constructor
	
// Dialog Data
	//{{AFX_DATA(dlg_luces_op)
	enum { IDD = IDD_LUCES_OPCIONES_DIALOG };
	float	m_px;
	float	m_py;
	float	m_pz;
	BOOL	m_mostrar;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(dlg_luces_op)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:

protected:

	// Generated message map functions
	//{{AFX_MSG(dlg_luces_op)
	afx_msg void OnLucesColor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_LUCES_OP_H__60A64DE0_6F0C_11D5_9D1F_E564A9CD811F__INCLUDED_)
