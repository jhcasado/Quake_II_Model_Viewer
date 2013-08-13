/* ==============================================================================
	
	  Vmd: Visualizador de modelos del quake II
	  Versión: 1.0.0b

	  Autores: José Angel Montelongo Reyes 
			   Javier Honorio Casado Fernández

	  Universidad de Las Palmas de Gran Canaria	
	  	  
   ============================================================================== */


// dlg_luces.cpp : implementation file
//

#include "stdafx.h"
#include "vmd.h"
#include "dlg_luces.h"
#include "dlg_luces_op.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// dlg_luces dialog


dlg_luces::dlg_luces(CWnd* pParent /*=NULL*/)
	: CDialog(dlg_luces::IDD, pParent)
{
	//{{AFX_DATA_INIT(dlg_luces)
	m_pos_luz_z = 0.0f;
	m_pos_luz_x = 0.0f;
	m_pos_luz_y = 0.0f;
	m_b_luces_activas = FALSE;
	//}}AFX_DATA_INIT
}


void dlg_luces::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(dlg_luces)
	DDX_Text(pDX, IDC_POS_LUZ_Z, m_pos_luz_z);
	DDX_Text(pDX, IDC_POS_LUZ_X, m_pos_luz_x);
	DDX_Text(pDX, IDC_POS_LUZ_Y, m_pos_luz_y);
	DDX_Check(pDX, IDC_LUCES_ACTIVAS, m_b_luces_activas);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(dlg_luces, CDialog)
	//{{AFX_MSG_MAP(dlg_luces)
	ON_BN_CLICKED(IDC_B_LUCES_CERRAR, OnBLucesCerrar)
	ON_BN_CLICKED(IDC_B_LUZ_APLICAR, OnBLuzAplicar)
	ON_BN_CLICKED(IDC_LUCES_ACTIVAS, OnLucesActivas)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_AMBIENTE, OnCustomdrawSliderAmbiente)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_DIFUSO, OnCustomdrawSliderDifuso)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_ESPECULAR, OnCustomdrawSliderEspecular)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_BRILLOS, OnCustomdrawSliderBrillos)
	ON_BN_CLICKED(IDC_B_LUCES_RESET, OnBLucesReset)
	ON_BN_CLICKED(IDC_B_LUCES_OPCIONES, OnBLucesOpciones)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void dlg_luces::V_Actualizar()
// aplica los cambios de las luces
{
	// ambiente
	CSliderCtrl *sd_ambiente = (CSliderCtrl *) GetDlgItem(IDC_SLIDER_AMBIENTE);
	float la [4];
	float ambiente = ((float)(sd_ambiente->GetPos()) / (float)(sd_ambiente->GetRangeMax()));

	la [0] = (doc->vgl->luz.r * ambiente);
	la [1] = (doc->vgl->luz.g * ambiente);
	la [2] = (doc->vgl->luz.b * ambiente);
	la [3] = doc->vgl->luz.a;

	glLightfv(GL_LIGHT0, GL_AMBIENT , la);	
			
	// difuso 
	CSliderCtrl *sd_difuso = (CSliderCtrl *) GetDlgItem(IDC_SLIDER_DIFUSO);
	float ld [4];
	float difuso = ((float)(sd_difuso->GetPos()) / (float)(sd_difuso->GetRangeMax()));
	
	ld [0] = (doc->vgl->luz.r * difuso);
	ld [1] = (doc->vgl->luz.g * difuso);
	ld [2] = (doc->vgl->luz.b * difuso);
	ld [3] = doc->vgl->luz.a;

	glLightfv(GL_LIGHT0, GL_DIFFUSE, ld);
	
	// especular
	CSliderCtrl *sd_especular = (CSliderCtrl *) GetDlgItem(IDC_SLIDER_ESPECULAR);
	float ls [4];
	float especular = ((float)(sd_especular->GetPos()) / (float)(sd_especular->GetRangeMax()));
	
	ls [0] = (doc->vgl->luz.r * especular);
	ls [1] = (doc->vgl->luz.g * especular);
	ls [2] = (doc->vgl->luz.b * especular);
	ls [3] = doc->vgl->luz.a;
		
	glLightfv(GL_LIGHT0, GL_SPECULAR, ls);
}

/////////////////////////////////////////////////////////////////////////////
// dlg_luces message handlers

BOOL dlg_luces::OnInitDialog() 
// incializa el dialogo
{
	CDialog::OnInitDialog();
	
	CSliderCtrl *sd_ambiente = (CSliderCtrl *) GetDlgItem(IDC_SLIDER_AMBIENTE);
	CSliderCtrl *sd_difuso = (CSliderCtrl *) GetDlgItem(IDC_SLIDER_DIFUSO);
	CSliderCtrl *sd_especular = (CSliderCtrl *) GetDlgItem(IDC_SLIDER_ESPECULAR);
	CSliderCtrl *sd_br = (CSliderCtrl *) GetDlgItem(IDC_SLIDER_BRILLOS);

	sd_ambiente->SetRangeMax(1000, TRUE);
	sd_ambiente->SetPos(250);
	sd_difuso->SetRangeMax(1000, TRUE);
	sd_difuso->SetPos(500);
	sd_especular->SetRangeMax(1000, TRUE);
	sd_especular->SetPos(0);
	sd_br->SetRangeMax(128, TRUE);
	sd_br->SetPos(0);

	m_pos_luz_x = 0.0f;
	m_pos_luz_y = 0.0f;
	m_pos_luz_z = 0.0f;
	m_b_luces_activas = TRUE;

	UpdateData(FALSE);	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void dlg_luces::OnBLucesCerrar() 
// cerramos el dialogo
{
	EndDialog(IDOK);	
}

void dlg_luces::OnBLuzAplicar() 
// mueve la luz a la pos especificada
{
	float lp [4];
	
	UpdateData(TRUE);	

	doc->vgl->luz.px += m_pos_luz_x;
	doc->vgl->luz.py += m_pos_luz_y;
	doc->vgl->luz.pz += m_pos_luz_z;

	lp [0] = doc->vgl->luz.px;
	lp [1] = doc->vgl->luz.py;
	lp [2] = doc->vgl->luz.pz;
	lp [3] = 1.0f;

	glLightfv(GL_LIGHT0, GL_POSITION, lp);	
}

void dlg_luces::OnLucesActivas() 
// activamos/descativamos las luces
{
	UpdateData(TRUE);

	if (m_b_luces_activas)
	{
		glEnable(GL_LIGHTING);	
	}

	else
	{
		glDisable(GL_LIGHTING);	
	}
}

void dlg_luces::OnCustomdrawSliderAmbiente(NMHDR* pNMHDR, LRESULT* pResult) 
// cambia la cantidad de luz ambiente de la escena
{
	V_Actualizar();
	*pResult = 0;
}

void dlg_luces::OnCustomdrawSliderDifuso(NMHDR* pNMHDR, LRESULT* pResult) 
// cambia valor difuso de la luz
{
	V_Actualizar();	
	*pResult = 0;
}

void dlg_luces::OnCustomdrawSliderEspecular(NMHDR* pNMHDR, LRESULT* pResult) 
// cambia el valor especular de la luz
{
	V_Actualizar();	
	*pResult = 0;
}

void dlg_luces::OnCustomdrawSliderBrillos(NMHDR* pNMHDR, LRESULT* pResult) 
// cambia el brillo del objeto
{
	CSliderCtrl *sd_br = (CSliderCtrl *) GetDlgItem(IDC_SLIDER_BRILLOS);
		
	glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, sd_br->GetPos());
	
	*pResult = 0;
}

void dlg_luces::OnBLucesReset() 
// reseta los valores de la ventana
{
	CSliderCtrl *sd_ambiente = (CSliderCtrl *) GetDlgItem(IDC_SLIDER_AMBIENTE);
	CSliderCtrl *sd_difuso = (CSliderCtrl *) GetDlgItem(IDC_SLIDER_DIFUSO);
	CSliderCtrl *sd_especular = (CSliderCtrl *) GetDlgItem(IDC_SLIDER_ESPECULAR);
	CSliderCtrl *sd_br = (CSliderCtrl *) GetDlgItem(IDC_SLIDER_BRILLOS);

	sd_ambiente->SetPos(250);
	sd_difuso->SetPos(500);
	sd_especular->SetPos(0);
	sd_br->SetPos(0);

	doc->vgl->luz.px = 0.0f; // pos
	doc->vgl->luz.py = 30.0f;
	doc->vgl->luz.pz = 40.0f;
	
	m_pos_luz_x = 0.0f; // mover
	m_pos_luz_y = 0.0f;
	m_pos_luz_z = 0.0f;

	doc->vgl->luz.r = 1.0f; // color
	doc->vgl->luz.g = 1.0f;
	doc->vgl->luz.b = 1.0f;
	doc->vgl->luz.a = 1.0f;

	m_b_luces_activas = TRUE;

	UpdateData(FALSE);		

	OnLucesActivas();
	OnBLuzAplicar();
	V_Actualizar();
		
	glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, sd_br->GetPos());		
}

void dlg_luces::OnBLucesOpciones() 
// dlg con más opciones de las luces
{
	dlg_luces_op d_opciones;
	float lr, lg, lb; 
	
	// color
	lr = doc->vgl->luz.r;
	lg = doc->vgl->luz.g;
	lb = doc->vgl->luz.b;

	// pos
	d_opciones.m_px = doc->vgl->luz.px;
	d_opciones.m_py = doc->vgl->luz.py;
	d_opciones.m_pz = doc->vgl->luz.pz;

	d_opciones.m_mostrar = doc->vgl->luz.visible;

	int r = d_opciones.DoModal();
	
	if (r == IDOK)
	{
		doc->vgl->luz.px = d_opciones.m_px;
		doc->vgl->luz.py = d_opciones.m_py;
		doc->vgl->luz.pz = d_opciones.m_pz;
		
		doc->vgl->luz.visible = d_opciones.m_mostrar;

		OnBLuzAplicar();
	}

	else if (r == IDCANCEL)	
	{
		doc->vgl->luz.r = lr;
		doc->vgl->luz.g = lg; 
		doc->vgl->luz.b = lb; 
	}

	V_Actualizar();	
}
