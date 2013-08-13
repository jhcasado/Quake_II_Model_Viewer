/* ==============================================================================
	
	  Vmd: Visualizador de modelos del quake II
	  Versión: 1.0.0b

	  Autores: José Angel Montelongo Reyes 
			   Javier Honorio Casado Fernández

	  Universidad de Las Palmas de Gran Canaria	
	  	  
   ============================================================================== */


#if !defined(AFX_DLG_LUCES_H__68CE3B20_5EE7_11D5_9D1F_D897157F691F__INCLUDED_)
#define AFX_DLG_LUCES_H__68CE3B20_5EE7_11D5_9D1F_D897157F691F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// dlg_luces.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// dlg_luces dialog

#include "vmdDoc.h"

class dlg_luces : public CDialog
{
// Construction
public:
	dlg_luces(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(dlg_luces)
	enum { IDD = IDD_LUCES_DIALOG };
	float	m_pos_luz_z;
	float	m_pos_luz_x;
	float	m_pos_luz_y;
	BOOL	m_b_luces_activas;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(dlg_luces)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void dlg_luces::V_Actualizar(); // aplica los cambios
	CVmdDoc *doc;

protected:

	// Generated message map functions
	//{{AFX_MSG(dlg_luces)
	virtual BOOL OnInitDialog();
	afx_msg void OnBLucesCerrar();
	afx_msg void OnBLuzAplicar();
	afx_msg void OnLucesActivas();
	afx_msg void OnCustomdrawSliderAmbiente(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomdrawSliderDifuso(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomdrawSliderEspecular(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomdrawSliderBrillos(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBLucesReset();
	afx_msg void OnBLucesOpciones();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_LUCES_H__68CE3B20_5EE7_11D5_9D1F_D897157F691F__INCLUDED_)
