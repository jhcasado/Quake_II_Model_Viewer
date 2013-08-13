/* ==============================================================================
	
	  Vmd: Visualizador de modelos del quake II
	  Versión: 1.0.0b

	  Autores: José Angel Montelongo Reyes 
			   Javier Honorio Casado Fernández

	  Universidad de Las Palmas de Gran Canaria	
	  	  
   ============================================================================== */


#if !defined(AFX_DLG_MODELOS_INFO_H__FA039EE3_683E_11D5_9D1F_B1D4B0E0401F__INCLUDED_)
#define AFX_DLG_MODELOS_INFO_H__FA039EE3_683E_11D5_9D1F_B1D4B0E0401F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// dlg_modelos_info.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// dlg_modelos_info dialog

class dlg_modelos_info : public CDialog
{
// Construction
public:
	dlg_modelos_info(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(dlg_modelos_info)
	enum { IDD = IDD_INFO_DIALOG };
	CString	m_modelo_ct;
	CString	m_modelo_frames;
	CString	m_modelo_glc;
	CString	m_arma_caras;
	CString	m_arma_ct;
	CString	m_arma_frames;
	CString	m_arma_glc;
	CString	m_arma_vertices;
	CString	m_modelo_caras;
	CString	m_modelo_vertices;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(dlg_modelos_info)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(dlg_modelos_info)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_MODELOS_INFO_H__FA039EE3_683E_11D5_9D1F_B1D4B0E0401F__INCLUDED_)
