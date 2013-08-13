/* ==============================================================================
	
	  Vmd: Visualizador de modelos del quake II
	  Versión: 1.0.0b

	  Autores: José Angel Montelongo Reyes 
			   Javier Honorio Casado Fernández

	  Universidad de Las Palmas de Gran Canaria	
	  	  
   ============================================================================== */


#if !defined(AFX_DLG_ACERCADE_H__6BF1E380_E595_11D5_9D1F_525400DA446A__INCLUDED_)
#define AFX_DLG_ACERCADE_H__6BF1E380_E595_11D5_9D1F_525400DA446A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// dlg_acercade.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// dlg_acercade dialog

class dlg_acercade : public CDialog
{
// Construction
public:
	dlg_acercade(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(dlg_acercade)
	enum { IDD = IDD_ACERCA_DE };
	CString	m_creditos;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(dlg_acercade)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(dlg_acercade)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_ACERCADE_H__6BF1E380_E595_11D5_9D1F_525400DA446A__INCLUDED_)
