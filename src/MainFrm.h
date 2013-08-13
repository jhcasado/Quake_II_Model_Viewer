/* ==============================================================================
	
	  Vmd: Visualizador de modelos del quake II
	  Versión: 1.0.0b

	  Autores: José Angel Montelongo Reyes 
			   Javier Honorio Casado Fernández

	  Universidad de Las Palmas de Gran Canaria	
	  	  
   ============================================================================== */


// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__3527BA48_4F7D_11D5_9D1F_C8D24E49A71F__INCLUDED_)
#define AFX_MAINFRM_H__3527BA48_4F7D_11D5_9D1F_C8D24E49A71F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dlg_acercade.h"
#include "dlg_modelos.h"
#include "dlg_animaciones.h"
#include "dlg_luces.h"
#include "dlg_camara.h"
#include "dlg_preferencias.h"


class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	CMenu *menu_emergente; // menu emergente
	dlg_modelos *d_modelos; // ventana con los modelos
	dlg_animaciones *d_animaciones; // ventana con las animaciones
	dlg_luces *d_luces; // ventana con las luces
	dlg_camara *d_camara ; // ventana con los controles de la camara

// Operations
public:
	void V_Menu_Emergente(CPoint & p); // muestra el menu emergente
	void V_Crear_Dlg_Modelos(); // crea la ventana con los modelos
	void V_Crear_Dlg_Animaciones(); // crea la ventana con las animaciones
	void V_Crear_Dlg_Luces(); // crea la ventana con las luces
	void V_Crear_Dlg_Camara(); // crea la ventana con los controles de la camara
	void V_Actualizar_Barra_Estado(); // adivina

	// variables para el chekeo del menu emergente

	BOOL m_b_modelo, 
		 m_b_luz,
		 m_b_camara,
		 m_b_xy,
		 m_b_xz,
		 m_b_zy;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL DestroyWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnVentanaModelos();
	afx_msg void OnVentanaAnimaciones();
	afx_msg void OnVentanaCamara();
	afx_msg void OnAnadirModelo();
	afx_msg void OnArchivoTexturaModelo();
	afx_msg void OnArchivoBorrarModelo();
	afx_msg void OnAnadirArma();
	afx_msg void OnArchivoTexturaArma();
	afx_msg void OnVentanaLuces();
	afx_msg void OnMenuemergenteCamara();
	afx_msg void OnMenuemergenteModelo();
	afx_msg void OnUpdateMenuemergenteModelo(CCmdUI* pCmdUI);
	afx_msg void OnMenuemergenteLuz();
	afx_msg void OnUpdateMenuemergenteLuz(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMenuemergenteCamara(CCmdUI* pCmdUI);
	afx_msg void OnMenuemergenteXy();
	afx_msg void OnMenuemergenteXz();
	afx_msg void OnMenuemergenteZy();
	afx_msg void OnUpdateMenuemergenteXy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMenuemergenteXz(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMenuemergenteZy(CCmdUI* pCmdUI);
	afx_msg void OnOpcionesPreferencias();
	afx_msg void OnUpdateArchivoTexturaModelo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateArchivoTexturaArma(CCmdUI* pCmdUI);
	afx_msg void OnUpdateArchivoBorrarModelo(CCmdUI* pCmdUI);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnCambiarModDer();
	afx_msg void OnCambiarModIzq();
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__3527BA48_4F7D_11D5_9D1F_C8D24E49A71F__INCLUDED_)
