/* ==============================================================================
	
	  Vmd: Visualizador de modelos del quake II
	  Versión: 1.0.0b

	  Autores: José Angel Montelongo Reyes 
			   Javier Honorio Casado Fernández

	  Universidad de Las Palmas de Gran Canaria	
	  	  
   ============================================================================== */


// dlg_preferencias.cpp : implementation file
//

#include "stdafx.h"
#include "vmd.h"
#include "dlg_preferencias.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// dlg_preferencias dialog


dlg_preferencias::dlg_preferencias(CWnd* pParent /*=NULL*/)
	: CDialog(dlg_preferencias::IDD, pParent)
{
	//{{AFX_DATA_INIT(dlg_preferencias)
	m_vel_mod_tras = 0.0f;
	m_vel_mod_rot = 0.0f;
	m_vel_tras_luz = 0.0f;
	m_vel_cam_tras = 0.0f;
	m_vel_cam_rot = 0.0f;
	//}}AFX_DATA_INIT
}


void dlg_preferencias::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(dlg_preferencias)
	DDX_Text(pDX, IDC_PREFERENCIAS_EDIT_VEL_TRAS_MOD, m_vel_mod_tras);
	DDX_Text(pDX, IDC_PREFERENCIAS_EDIT_VEL_ROT_MOD, m_vel_mod_rot);
	DDX_Text(pDX, IDC_PREFERENCIAS_EDIT_VEL_TRAS_LUZ, m_vel_tras_luz);
	DDX_Text(pDX, IDC_PREFERENCIAS_EDIT_VEL_CAM_TRAS, m_vel_cam_tras);
	DDX_Text(pDX, IDC_PREFERENCIAS_EDIT_VEL_CAM_ROT, m_vel_cam_rot);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(dlg_preferencias, CDialog)
	//{{AFX_MSG_MAP(dlg_preferencias)
	ON_BN_CLICKED(IDC_B_COLOR_FONDO, OnBColorFondo)
	ON_BN_CLICKED(ID_PREFERENCIAS_RESET, OnPreferenciasReset)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// dlg_preferencias message handlers

void dlg_preferencias::OnBColorFondo() 
// cambia el color de fondo
{
	CColorDialog *dlg_color;
	BYTE r, g, b;
	CVmdDoc *doc = (CVmdDoc *)(GetParentFrame()->GetActiveDocument());		
	
	r = (BYTE)(doc->vgl->f_cfondo_r * 255.0);
	g = (BYTE)(doc->vgl->f_cfondo_g * 255.0);
	b = (BYTE)(doc->vgl->f_cfondo_b * 255.0);

	dlg_color = new CColorDialog(RGB(r, g, b)); 
	int i_ret = dlg_color->DoModal();

	if (i_ret == IDOK)
	{
		COLORREF cs = dlg_color->GetColor();
		
		r = (BYTE)((cs & 0x000000ff));
		g = (BYTE)((cs & 0x0000ff00) >> 8);
		b = (BYTE)((cs & 0x00ff0000) >> 16);

		doc->vgl->f_cfondo_r = (float)(r / 255.0);
		doc->vgl->f_cfondo_g = (float)(g / 255.0);		
		doc->vgl->f_cfondo_b = (float)(b / 255.0);

		GetParentFrame()->Invalidate(); // redibujamos la pantalla
	}			
}

BOOL dlg_preferencias::OnInitDialog() 
// inicializa los valores por defecto del dialogo
{
	CDialog::OnInitDialog();
	
	CVmdDoc *doc = (CVmdDoc *)(GetParentFrame()->GetActiveDocument());		
	
	m_vel_cam_tras = doc->vgl->camara.f_vt;
	m_vel_cam_rot = doc->vgl->camara.f_vr;
	m_vel_mod_tras = doc->vgl->f_vt;
	m_vel_mod_rot = doc->vgl->f_vr; 
	m_vel_tras_luz = doc->vgl->luz.f_vlt;
		
	UpdateData(FALSE);	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void dlg_preferencias::OnPreferenciasReset() 
// restaura los valores iniciales
{
	CVmdDoc *doc = (CVmdDoc *)(GetParentFrame()->GetActiveDocument());		
	
	// color de fondo
	doc->vgl->f_cfondo_r = 0.0f;
	doc->vgl->f_cfondo_g = 0.2f;		
	doc->vgl->f_cfondo_b = 0.5f;

	m_vel_cam_tras = 0.5f;
	m_vel_cam_rot = 0.5f;
	m_vel_mod_tras = 0.1f;
	m_vel_mod_rot = 1.0f; 
	m_vel_tras_luz = 1.0f;
		
	UpdateData(FALSE);	

	GetParentFrame()->Invalidate(); // redibujamos la pantalla	
}
