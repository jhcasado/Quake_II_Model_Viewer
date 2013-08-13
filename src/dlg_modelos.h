/* ==============================================================================
	
	  Vmd: Visualizador de modelos del quake II
	  Versión: 1.0.0b

	  Autores: José Angel Montelongo Reyes 
			   Javier Honorio Casado Fernández

	  Universidad de Las Palmas de Gran Canaria	
	  	  
   ============================================================================== */


#if !defined(AFX_DLG_MODELOS_H__B8701020_5697_11D5_9D1F_E91A9C1F821E__INCLUDED_)
#define AFX_DLG_MODELOS_H__B8701020_5697_11D5_9D1F_E91A9C1F821E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// dlg_modelos.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// dlg_modelos dialog

#include "vmdDoc.h"

class dlg_modelos : public CDialog
{
// Construction
public:
	dlg_modelos(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(dlg_modelos)
	enum { IDD = IDD_MODELOS_DIALOG };
	BOOL	m_strips_and_fans;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(dlg_modelos)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void V_Actualizar(); // actualiza los controles de la ventana
	BOOL B_Anadir_Modelo(); // añade un modelo a la lista
	BOOL B_Anadir_Arma(); // añade un arma a la escena
	BOOL B_Textura_Modelo(); // carga una textura para el modelo actual
	BOOL B_Textura_Arma(); // carga una textura para el arma
	BOOL B_Borrar_Modelo(); // borra el modelo actual
	BOOL B_Reset(); // borra todos los modelos
	void V_Cambiar_Modelo(int nuevo); // cambia el modelos actual

	CVmdDoc *doc;

protected:

	// Generated message map functions
	//{{AFX_MSG(dlg_modelos)
	afx_msg void OnBModelosCerrar();
	virtual BOOL OnInitDialog();
	afx_msg void OnBModelo();
	afx_msg void OnBModeloTextura();
	afx_msg void OnSelchangeElegirSombreado();
	afx_msg void OnSelchangeElegirCajas();
	afx_msg void OnBColorCaja();
	afx_msg void OnBBorrarModelo();
	afx_msg void OnBArma();
	afx_msg void OnBColorMalla();
	afx_msg void OnDBorrarArma();
	afx_msg void OnBReset();
	afx_msg void OnCstripsFans();
	afx_msg void OnBArmaTextura();
	afx_msg void OnSelchangeElegirModelos();
	afx_msg void OnSelchangeElegirArmas();
	afx_msg void OnBInfo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_MODELOS_H__B8701020_5697_11D5_9D1F_E91A9C1F821E__INCLUDED_)
