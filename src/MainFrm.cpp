/* ==============================================================================
	
	  Vmd: Visualizador de modelos del quake II
	  Versión: 1.0.0b

	  Autores: José Angel Montelongo Reyes 
			   Javier Honorio Casado Fernández

	  Universidad de Las Palmas de Gran Canaria	
	  	  
   ============================================================================== */


// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "vmd.h"

#include "MainFrm.h"
#include "vmdView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_RBUTTONDOWN()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_VENTANA_MODELOS, OnVentanaModelos)
	ON_COMMAND(ID_VENTANA_ANIMACIONES, OnVentanaAnimaciones)
	ON_COMMAND(ID_VENTANA_CAMARA, OnVentanaCamara)
	ON_COMMAND(ID_ANADIR_MODELO, OnAnadirModelo)
	ON_COMMAND(ID_ARCHIVO_TEXTURA_MODELO, OnArchivoTexturaModelo)
	ON_COMMAND(ID_ARCHIVO_BORRAR_MODELO, OnArchivoBorrarModelo)
	ON_COMMAND(ID_ANADIR_ARMA, OnAnadirArma)
	ON_COMMAND(ID_ARCHIVO_TEXTURA_ARMA, OnArchivoTexturaArma)
	ON_COMMAND(ID_VENTANA_LUCES, OnVentanaLuces)
	ON_COMMAND(ID_MENUEMERGENTE_CAMARA, OnMenuemergenteCamara)
	ON_COMMAND(ID_MENUEMERGENTE_MODELO, OnMenuemergenteModelo)
	ON_UPDATE_COMMAND_UI(ID_MENUEMERGENTE_MODELO, OnUpdateMenuemergenteModelo)
	ON_COMMAND(ID_MENUEMERGENTE_LUZ, OnMenuemergenteLuz)
	ON_UPDATE_COMMAND_UI(ID_MENUEMERGENTE_LUZ, OnUpdateMenuemergenteLuz)
	ON_UPDATE_COMMAND_UI(ID_MENUEMERGENTE_CAMARA, OnUpdateMenuemergenteCamara)
	ON_COMMAND(ID_MENUEMERGENTE_XY, OnMenuemergenteXy)
	ON_COMMAND(ID_MENUEMERGENTE_XZ, OnMenuemergenteXz)
	ON_COMMAND(ID_MENUEMERGENTE_ZY, OnMenuemergenteZy)
	ON_UPDATE_COMMAND_UI(ID_MENUEMERGENTE_XY, OnUpdateMenuemergenteXy)
	ON_UPDATE_COMMAND_UI(ID_MENUEMERGENTE_XZ, OnUpdateMenuemergenteXz)
	ON_UPDATE_COMMAND_UI(ID_MENUEMERGENTE_ZY, OnUpdateMenuemergenteZy)
	ON_COMMAND(ID_OPCIONES_PREFERENCIAS, OnOpcionesPreferencias)
	ON_UPDATE_COMMAND_UI(ID_ARCHIVO_TEXTURA_MODELO, OnUpdateArchivoTexturaModelo)
	ON_UPDATE_COMMAND_UI(ID_ARCHIVO_TEXTURA_ARMA, OnUpdateArchivoTexturaArma)
	ON_UPDATE_COMMAND_UI(ID_ARCHIVO_BORRAR_MODELO, OnUpdateArchivoBorrarModelo)
	ON_WM_SETFOCUS()
	ON_WM_TIMER()
	ON_COMMAND(ID_CAMBIAR_MOD_DER, OnCambiarModDer)
	ON_COMMAND(ID_CAMBIAR_MOD_IZQ, OnCambiarModIzq)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
//	ID_INDICATOR_CAPS,
//	ID_INDICATOR_NUM,
//	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	d_modelos = NULL;
	d_animaciones = NULL;	
	d_luces = NULL;
	d_camara = NULL;

	m_b_modelo = TRUE; 
	m_b_luz = FALSE;
	m_b_camara = FALSE;
	
	m_b_xy = TRUE;
	m_b_xz = FALSE;
	m_b_zy = FALSE;
}

CMainFrame::~CMainFrame()
// destruimos los objetos
{
	if (d_modelos != NULL)
	{
		delete d_modelos;
	}

	if (d_animaciones != NULL)
	{
		delete d_animaciones;
	}
	
	if (d_luces != NULL)
	{
		delete d_luces;
	}

	if (d_camara != NULL)
	{
		delete d_camara;
	}
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
// se crea el marco principal
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	
	// creamos el menu emergente

	menu_emergente = new CMenu();
    
	if (!menu_emergente)
	{
		return -1;
	}

    if (!menu_emergente->LoadMenu(IDR_MENU_EMERGENTE))
	{
		return -1;
	}

	SetTimer(101, 1000, 0);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// Metodos propios

void CMainFrame::V_Menu_Emergente(CPoint & p)
// dibuja el menu emergente
{
    CMenu *psubmenu = menu_emergente->GetSubMenu(0);
    psubmenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, p.x, p.y, this);            
}

void CMainFrame::V_Actualizar_Barra_Estado()
// actualiza la barra de estado
{
	char s [255];
		
	if (d_modelos != NULL)
	{
		CVmdView *view = (CVmdView *)(GetActiveView());		

		if (d_modelos->doc->vgl->i_num_modelos == 0)
		{
			sprintf_s(s, "No hay modelos cargados.", 
				    d_modelos->doc->vgl->i_num_modelos);
		}

		else
		{
			sprintf_s(s, "Número de modelos : %d.    Modelo actual : %d.    FPS: %f", 
				    d_modelos->doc->vgl->i_num_modelos, 
					(d_modelos->doc->vgl->i_elto_actual + 1),
					view->m_fps.m_fFps);
		}
		
		m_wndStatusBar.SetPaneText(ID_SEPARATOR, s, TRUE);
	}	
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnRButtonDown(UINT nFlags, CPoint point) 
// muestra el menu emergente al pulser el botón derecho
{
	V_Menu_Emergente(point);

	CFrameWnd::OnRButtonDown(nFlags, point);
}

void CMainFrame::OnContextMenu(CWnd* pWnd, CPoint point) 
// muestra el menu emergente
{	
    V_Menu_Emergente(point);
}

void CMainFrame::V_Crear_Dlg_Modelos()
// crea la ventana con los modelos
{	
	if (d_modelos == NULL)
	{
		d_modelos = new dlg_modelos();	

		d_modelos->Create(IDD_MODELOS_DIALOG, this);			
		
		int cx = GetSystemMetrics(SM_CXSCREEN);
		int cy = GetSystemMetrics(SM_CYSCREEN);
		CRect r;
		
		d_modelos->GetWindowRect(r);
		d_modelos->MoveWindow((cx - r.Width()) - 10, 70, r.Width(), r.Height(), FALSE);
		d_modelos->doc = (CVmdDoc *)(GetActiveDocument());
	}

	V_Actualizar_Barra_Estado();
}

void CMainFrame::V_Crear_Dlg_Animaciones()
// crea la ventana con los modelos
{	
	if (d_animaciones == NULL)
	{
		d_animaciones = new dlg_animaciones();	

		d_animaciones->Create(IDD_ANIMACIONES_DIALOG, this);			
		
		int cx = GetSystemMetrics(SM_CXSCREEN);
		int cy = GetSystemMetrics(SM_CYSCREEN);
		CRect r;
		
		d_animaciones->GetWindowRect(r);
		d_animaciones->MoveWindow((cx - r.Width()) - 10, 70, r.Width(), r.Height(), FALSE);
		d_animaciones->doc = (CVmdDoc *)(GetActiveDocument());
	}
}

void CMainFrame::V_Crear_Dlg_Luces()
// crea la ventana con las luces
{	
	if (d_luces == NULL)
	{
		d_luces = new dlg_luces();	

		d_luces->Create(IDD_LUCES_DIALOG, this);			
		
		int cx = GetSystemMetrics(SM_CXSCREEN);
		int cy = GetSystemMetrics(SM_CYSCREEN);
		CRect r;
		
		d_luces->GetWindowRect(r);
		d_luces->MoveWindow((cx - r.Width()) - 10, 70, r.Width(), r.Height(), FALSE);
		d_luces->doc = (CVmdDoc *)(GetActiveDocument());
	}
}

void CMainFrame::V_Crear_Dlg_Camara()
// crea la ventana con los controles de la camara
{	
	if (d_camara == NULL)
	{
		d_camara = new dlg_camara();	

		d_camara->Create(IDD_CAMARA_DIALOG, this);			
		
		int cx = GetSystemMetrics(SM_CXSCREEN);
		int cy = GetSystemMetrics(SM_CYSCREEN);
		CRect r;
		
		d_camara->GetWindowRect(r);
		d_camara->MoveWindow((cx - r.Width()) - 10, 70, r.Width(), r.Height(), FALSE);
		d_camara->doc = (CVmdDoc *)(GetActiveDocument());
	}
}

void CMainFrame::OnVentanaModelos() 
// muestra la ventana con los modelos
{
	V_Crear_Dlg_Modelos();
	
	if (d_animaciones != NULL)
	{
		d_animaciones->ShowWindow(SW_HIDE);
	}

	if (d_camara != NULL)
	{
		d_camara->ShowWindow(SW_HIDE);
	}

	if (d_luces != NULL)
	{
		d_luces->ShowWindow(SW_HIDE);
	}
	
	d_modelos->SetActiveWindow();
	d_modelos->ShowWindow(SW_SHOWNORMAL);						
}

void CMainFrame::OnVentanaCamara() 
// muestra la ventana con los controles de la camara
{
	V_Crear_Dlg_Camara();

	if (d_modelos != NULL)
	{
		d_modelos->ShowWindow(SW_HIDE);
	}

	if (d_animaciones != NULL)
	{
		d_animaciones->ShowWindow(SW_HIDE);
	}

	if (d_luces != NULL)
	{
		d_luces->ShowWindow(SW_HIDE);
	}

	d_camara->SetActiveWindow();
	d_camara->ShowWindow(SW_SHOWNORMAL);						
}

void CMainFrame::OnVentanaAnimaciones() 
// muestra la ventana con las animaciones
{
	V_Crear_Dlg_Animaciones();
	
	if (d_modelos != NULL)
	{
		d_modelos->ShowWindow(SW_HIDE);
	}

	if (d_camara != NULL)
	{
		d_camara->ShowWindow(SW_HIDE);
	}

	if (d_luces != NULL)
	{
		d_luces->ShowWindow(SW_HIDE);
	}

	d_animaciones->V_Actualizar();	
	d_animaciones->SetActiveWindow();
	d_animaciones->ShowWindow(SW_SHOWNORMAL);						
}

void CMainFrame::OnAnadirModelo() 
// añadimos el modelo desde el menú
{	
	V_Crear_Dlg_Modelos();
	V_Crear_Dlg_Animaciones();

	if (!d_modelos->B_Anadir_Modelo())
	{
		MessageBox("Imposible añadir el modelo", "¡¡¡ Error !!!", MB_OK | MB_ICONERROR);
	}
	
	else
	{
		d_animaciones->V_Actualizar();	
		V_Actualizar_Barra_Estado();
	}	
}

void CMainFrame::OnArchivoTexturaModelo() 
// carga la textura del modelo desde el menú
{
	V_Crear_Dlg_Modelos();

	if (!d_modelos->B_Textura_Modelo())
	{
		MessageBox("Imposible cargar la textura", "¡¡¡ Error !!!", MB_OK | MB_ICONERROR);
	}	
}

BOOL CMainFrame::DestroyWindow() 
// liberamos los objetos
{
	CVmdDoc *doc = (CVmdDoc *)(GetActiveDocument());

	if (doc->vgl != NULL)
	{
		delete doc->vgl;
		doc->vgl = NULL;
	}

	KillTimer(101);
		
	return CFrameWnd::DestroyWindow();
}

void CMainFrame::OnArchivoBorrarModelo() 
// borra el modelo actual
{
	V_Crear_Dlg_Modelos();
	V_Crear_Dlg_Animaciones();

	if (!d_modelos->B_Borrar_Modelo())
	{
		MessageBox("Imposible borrar el modelo actual", "¡¡¡ Error !!!", MB_OK | MB_ICONERROR);
	}			

	else
	{
		d_animaciones->V_Actualizar();	
		V_Actualizar_Barra_Estado();
	}
}

void CMainFrame::OnAnadirArma() 
// añade el arma del modelo
{
	V_Crear_Dlg_Modelos();
	V_Crear_Dlg_Animaciones();

	if (!d_modelos->B_Anadir_Arma())
	{
		MessageBox("Imposible añadir el arma", "¡¡¡ Error !!!", MB_OK | MB_ICONERROR);
	}
	
	else
	{
		d_animaciones->V_Actualizar();	
		V_Actualizar_Barra_Estado();
	}
}

void CMainFrame::OnArchivoTexturaArma() 
// carga la textura del arma
{
	V_Crear_Dlg_Modelos();

	if (!d_modelos->B_Textura_Arma())
	{
		MessageBox("Imposible cargar la textura", "¡¡¡ Error !!!", MB_OK | MB_ICONERROR);
	}
}

void CMainFrame::OnVentanaLuces() 
// muestra las ventana con las luces
{
	V_Crear_Dlg_Luces();
	
	if (d_modelos != NULL)
	{
		d_modelos->ShowWindow(SW_HIDE);
	}

	if (d_animaciones != NULL)
	{
		d_animaciones->ShowWindow(SW_HIDE);
	}

	if (d_camara != NULL)
	{
		d_camara->ShowWindow(SW_HIDE);
	}

	d_luces->SetActiveWindow();
	d_luces->ShowWindow(SW_SHOWNORMAL);							
}

void CMainFrame::OnMenuemergenteCamara() 
// transfromaciones con la camara
{
	CVmdDoc *doc = (CVmdDoc *)(GetActiveDocument());
	
	doc->vgl->i_objeto = CAMARA;

	m_b_camara = TRUE;
	m_b_modelo = FALSE;	
	m_b_luz = FALSE;	
}

void CMainFrame::OnMenuemergenteModelo() 
// transformaciones con el modelo, por defecto
{
	CVmdDoc *doc = (CVmdDoc *)(GetActiveDocument());

	doc->vgl->i_objeto = MODELO;	

	m_b_modelo = TRUE;
	m_b_luz = FALSE;
	m_b_camara = FALSE;
}

void CMainFrame::OnMenuemergenteLuz() 
// transformaciones con la luz
{
	CVmdDoc *doc = (CVmdDoc *)(GetActiveDocument());

	doc->vgl->i_objeto = LUZ;	

    m_b_luz = TRUE;	
	m_b_modelo = FALSE;
	m_b_camara = FALSE;
}

void CMainFrame::OnUpdateMenuemergenteModelo(CCmdUI* pCmdUI) 
// actualiza el menu emergente
{
	pCmdUI->SetCheck(m_b_modelo);	
}

void CMainFrame::OnUpdateMenuemergenteLuz(CCmdUI* pCmdUI) 
// actualiza el menu emergente
{
	pCmdUI->SetCheck(m_b_luz);		
}


void CMainFrame::OnUpdateMenuemergenteCamara(CCmdUI* pCmdUI) 
// actualiza el menu emergente
{
	pCmdUI->SetCheck(m_b_camara);	
}

void CMainFrame::OnMenuemergenteXy() 
// cambia el plano de transformación
{
	CVmdDoc *doc = (CVmdDoc *)(GetActiveDocument());

	doc->vgl->i_plano = XY;	

	m_b_xy = TRUE;
	m_b_xz = FALSE;
	m_b_zy = FALSE;
}

void CMainFrame::OnMenuemergenteXz() 
// cambia el plano de transformación
{
	CVmdDoc *doc = (CVmdDoc *)(GetActiveDocument());

	doc->vgl->i_plano = XZ;	
	
	m_b_xz = TRUE;
	m_b_xy = FALSE;
	m_b_zy = FALSE;
}

void CMainFrame::OnMenuemergenteZy() 
// cambia el plano de transformación
{
	CVmdDoc *doc = (CVmdDoc *)(GetActiveDocument());

	doc->vgl->i_plano = ZY;	
	
	m_b_zy = TRUE;
	m_b_xz = FALSE;
	m_b_xy = FALSE;		
}

void CMainFrame::OnUpdateMenuemergenteXy(CCmdUI* pCmdUI) 
// actualiza el menu emergente
{
	pCmdUI->SetCheck(m_b_xy);		
}

void CMainFrame::OnUpdateMenuemergenteXz(CCmdUI* pCmdUI) 
// actualiza el menu emergente
{
	pCmdUI->SetCheck(m_b_xz);		
}

void CMainFrame::OnUpdateMenuemergenteZy(CCmdUI* pCmdUI) 
// actualiza el menu emergente
{
	pCmdUI->SetCheck(m_b_zy);		
}

void CMainFrame::OnOpcionesPreferencias() 
// muestra la ventana con las preferencias
{
	dlg_preferencias *d_preferencias;
	float r, g, b, a;
	CVmdDoc *doc = (CVmdDoc *)(GetActiveDocument());
	
	d_preferencias = new dlg_preferencias();	

	r = doc->vgl->f_cfondo_r;
	g = doc->vgl->f_cfondo_g;
	b = doc->vgl->f_cfondo_b;
	a = doc->vgl->f_cfondo_a;

	int result = d_preferencias->DoModal();

	if (result == IDOK)
	{
		doc->vgl->camara.f_vr =	d_preferencias->m_vel_cam_rot;
		doc->vgl->camara.f_vt = d_preferencias->m_vel_cam_tras;
		doc->vgl->f_vr = d_preferencias->m_vel_mod_rot;
		doc->vgl->f_vt = d_preferencias->m_vel_mod_tras;
		doc->vgl->luz.f_vlt = d_preferencias->m_vel_tras_luz;		
	}

	else if (result == IDCANCEL)
	{
		doc->vgl->f_cfondo_r = r;
		doc->vgl->f_cfondo_g = g;
		doc->vgl->f_cfondo_b = b;
		doc->vgl->f_cfondo_a = a;
	}
}

void CMainFrame::OnUpdateArchivoTexturaModelo(CCmdUI* pCmdUI) 
// activa/descativa el menu Archivo->Textura modelo
{
	CVmdDoc *doc = (CVmdDoc *)(GetActiveDocument());

	if ((doc->vgl->i_num_modelos > 0) && (doc->vgl->p_lmodelos [doc->vgl->i_elto_actual] != -1))
	{
		pCmdUI->Enable(TRUE);
	}
	
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CMainFrame::OnUpdateArchivoTexturaArma(CCmdUI* pCmdUI) 
// activa/descativa el menu Archivo->Textura arma
{
	CVmdDoc *doc = (CVmdDoc *)(GetActiveDocument());

	if ((doc->vgl->i_num_modelos > 0) && (doc->vgl->p_larmas [doc->vgl->i_elto_actual] != -1))
	{
		pCmdUI->Enable(TRUE);
	}
	
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CMainFrame::OnUpdateArchivoBorrarModelo(CCmdUI* pCmdUI) 
// activa desactiva el menu Archivo->Borrar
{
	CVmdDoc *doc = (CVmdDoc *)(GetActiveDocument());

	if (doc->vgl->i_num_modelos > 0)
	{
		pCmdUI->Enable(TRUE);
	}
	
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CMainFrame::OnSetFocus(CWnd* pOldWnd) 
// actualiza la barra de estado
{
	CFrameWnd::OnSetFocus(pOldWnd);
	
	V_Actualizar_Barra_Estado();	
}

void CMainFrame::OnTimer(UINT nIDEvent) 
// actualiza los FPS
{
	V_Actualizar_Barra_Estado();

	CFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::OnCambiarModDer() 
// cambia el modelo actual al siguiente
{
	CVmdDoc *doc = (CVmdDoc *)(GetActiveDocument());
	int n = doc->vgl->i_elto_actual + 1;

	if (n >= doc->vgl->i_num_modelos)
	{
		n = 0;
	}
		
	if (d_modelos != NULL)
	{
		d_modelos->V_Cambiar_Modelo(n);
		V_Actualizar_Barra_Estado();
		d_animaciones->V_Actualizar();	
	}
}

void CMainFrame::OnCambiarModIzq() 
// cambia el modelo actual al anterior
{
	CVmdDoc *doc = (CVmdDoc *)(GetActiveDocument());
	int n = doc->vgl->i_elto_actual - 1;

	if (n < 0)
	{
		n = doc->vgl->i_num_modelos - 1;
	}
		
	if (d_modelos != NULL)
	{
		d_modelos->V_Cambiar_Modelo(n);
		V_Actualizar_Barra_Estado();
		d_animaciones->V_Actualizar();	
	}	
}

void CMainFrame::OnAppAbout() 
// muesta el dialogo de acerca de...
{
	dlg_acercade d_acercade;	
	
	d_acercade.m_creditos = "\r\nAutores:\r\n\r\n"
						    "José Ángel Montelongo Reyes\r\n"
							"ja.montelongo@gmail.com\r\n\r\n"
							"Javier Honorio Casado Fernández\r\n"
							"jhcasado@gmail.com\r\n\r\n"
							"Universidad de Las Palmas de Gran Canaria";
							
	d_acercade.DoModal();
}
