/* ==============================================================================
	
	  Vmd: Visualizador de modelos del quake II
	  Versión: 1.0.0b

	  Autores: José Angel Montelongo Reyes 
			   Javier Honorio Casado Fernández

	  Universidad de Las Palmas de Gran Canaria	
	  	  
   ============================================================================== */


// dlg_camara.cpp : implementation file
//

#include "stdafx.h"
#include "vmd.h"
#include "dlg_camara.h"
#include "vmdView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// dlg_camara dialog


dlg_camara::dlg_camara(CWnd* pParent /*=NULL*/)
	: CDialog(dlg_camara::IDD, pParent)
{
	//{{AFX_DATA_INIT(dlg_camara)
	m_fov = 0.0f;
	m_aspect_x = 0.0f;
	m_aspect_y = 0.0f;
	//}}AFX_DATA_INIT
}


void dlg_camara::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(dlg_camara)
	DDX_Text(pDX, IDC_EDIT_FOV, m_fov);
	DDV_MinMaxFloat(pDX, m_fov, 0.f, 180.f);
	DDX_Text(pDX, IDC_EDIT_ASPECX, m_aspect_x);
	DDX_Text(pDX, IDC_EDIT_ASPECY, m_aspect_y);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(dlg_camara, CDialog)
	//{{AFX_MSG_MAP(dlg_camara)
	ON_BN_CLICKED(IDC_B_CAMARA_CERRAR, OnBCamaraCerrar)
	ON_BN_CLICKED(IDC_B_CAMARA_APLICAR, OnBCamaraAplicar)
	ON_BN_CLICKED(IDC_B_CAMARA_RESET, OnBCamaraReset)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// dlg_camara message handlers

BOOL dlg_camara::OnInitDialog() 
// inicializa los valores por defecto de la camara
{
	CDialog::OnInitDialog();
	
	m_fov = 60.0f;
	m_aspect_x = 1.0f;
	m_aspect_y = 1.0f;

	UpdateData(FALSE);	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void dlg_camara::OnBCamaraCerrar() 
// cerramos el dialogo
{
	EndDialog(IDOK);	
}

void dlg_camara::OnBCamaraAplicar() 
// aplica los cambios a la camara
{
	CRect r;

	UpdateData(TRUE);

	doc->vgl->camara.f_fovy = m_fov;
	doc->vgl->camara.f_aspect_x = 1.0f / m_aspect_x;
	doc->vgl->camara.f_aspect_y = 1.0f / m_aspect_y;

	CVmdView *view = (CVmdView *)(GetParentFrame()->GetActiveView());

	view->GetClientRect(r);
	view->V_OnSize(r.Width(), r.Height());
}

void dlg_camara::OnBCamaraReset() 
// cargamos los valores por defecto
{
	doc->vgl->camara.px = 0.0f; 
	doc->vgl->camara.py = 0.0f; 
	doc->vgl->camara.pz = 100.0f; 

	doc->vgl->camara.cx = 0.0f; 
	doc->vgl->camara.cy = 0.0f;  
	doc->vgl->camara.cz = 0.0f; 

	m_fov = 60.0f;
	m_aspect_x = 1.0f;
	m_aspect_y = 1.0f;

	UpdateData(FALSE);	
	
	OnBCamaraAplicar();	
}
