/* ==============================================================================
	
	  Vmd: Visualizador de modelos del quake II
	  Versión: 1.0.0b

	  Autores: José Angel Montelongo Reyes 
			   Javier Honorio Casado Fernández

	  Universidad de Las Palmas de Gran Canaria	
	  	  
   ============================================================================== */


// dlg_animaciones.cpp : implementation file
//

#include "stdafx.h"
#include "vmd.h"
#include "dlg_animaciones.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// dlg_animaciones dialog


dlg_animaciones::dlg_animaciones(CWnd* pParent /*=NULL*/)
	: CDialog(dlg_animaciones::IDD, pParent)
{
	//{{AFX_DATA_INIT(dlg_animaciones)
	m_frame = 0;
	//}}AFX_DATA_INIT
}


void dlg_animaciones::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(dlg_animaciones)
	DDX_Text(pDX, IDC_EDIT_FRAMES, m_frame);
	DDV_MinMaxInt(pDX, m_frame, 0, 197);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(dlg_animaciones, CDialog)
	//{{AFX_MSG_MAP(dlg_animaciones)
	ON_BN_CLICKED(IDC_B_ANIMACIONES_CERRAR, OnBAnimacionesCerrar)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_INTER, OnCustomdrawSliderInter)
	ON_CBN_SELCHANGE(IDC_ELEGIR_ANIMACION, OnSelchangeElegirAnimacion)
	ON_BN_CLICKED(IDC_B_ANIMACIONES_PLAY, OnBAnimacionesPlay)
	ON_BN_CLICKED(IDC_B_SET_FRAME, OnBSetFrame)
	ON_BN_CLICKED(IDC_B_RESET_ANIM, OnBResetAnim)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// dlg_animaciones message handlers

BOOL dlg_animaciones::OnInitDialog() 
// inicializamos le dialogo con las animaciones
{
	CDialog::OnInitDialog();
	
	CComboBox *cb_anim = (CComboBox *) GetDlgItem(IDC_ELEGIR_ANIMACION);
	cb_anim->SetCurSel(0);

	CSliderCtrl *sd_inter = (CSliderCtrl *) GetDlgItem(IDC_SLIDER_INTER);
	sd_inter->SetRangeMax(1000, TRUE);

	CSpinButtonCtrl *sp_frames = (CSpinButtonCtrl *) GetDlgItem(IDC_CAMBIO_FRAME);
	sp_frames->SetRange(0, 197);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void dlg_animaciones::OnBAnimacionesCerrar() 
// cierra el dialogo
{
	EndDialog(IDOK);		
}

void dlg_animaciones::V_Actualizar()
// actualiza la ventana de las animaciones
{
	CComboBox *cb_anim = (CComboBox *) GetDlgItem(IDC_ELEGIR_ANIMACION);
	CSliderCtrl *sd_inter = (CSliderCtrl *) GetDlgItem(IDC_SLIDER_INTER);
	CEdit *ed_frames = (CEdit *) GetDlgItem(IDC_EDIT_FRAMES);
	CButton *bt_play = (CButton *) GetDlgItem(IDC_B_ANIMACIONES_PLAY);
	CButton *bt_set = (CButton *) GetDlgItem(IDC_B_SET_FRAME);
	CSpinButtonCtrl *sp_frames = (CSpinButtonCtrl *) GetDlgItem(IDC_CAMBIO_FRAME);
	CButton *bt_pause = (CButton *) GetDlgItem(IDC_B_ANIMACIONES_PLAY);
	CButton *bt_reset = (CButton *) GetDlgItem(IDC_B_RESET_ANIM);
	CButton *cj_opciones = (CButton *) GetDlgItem(IDC_CAJA_ANIMACIONES);
	CButton *cj_frame = (CButton *) GetDlgItem(IDC_CAJA_FRAME);
	CButton *cj_inter = (CButton *) GetDlgItem(IDC_CAJA_INTERPOLACION);
	BOOL bs = FALSE;
	char buf [100];

	if (doc->vgl->i_num_modelos > 0)
	{	
		cb_anim->SetCurSel(doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Get_Animacion());
		sd_inter->SetPos((int)(doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Get_Coeft() * (float)(sd_inter->GetRangeMax()))); 
		m_frame = doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Get_Frame();
		
		sprintf_s(buf, "%d", m_frame);
		ed_frames->SetWindowText(buf);		

		if (doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Get_Play_Pause())
		{
			bt_play->SetWindowText("Play");
			ed_frames->EnableWindow(TRUE);
			sp_frames->EnableWindow(TRUE);
			bt_set->EnableWindow(TRUE);
			sp_frames->SetPos(doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Get_Frame());
		}

		else 
		{
			bt_play->SetWindowText("Pause");
			ed_frames->EnableWindow(FALSE);
			sp_frames->EnableWindow(FALSE); 
			bt_set->EnableWindow(FALSE);
		}

		bs = TRUE;
	}
	
	else
	{
		cb_anim->SetCurSel(-1);
			
		sprintf_s(buf, " ");
		ed_frames->SetWindowText(buf);		
		ed_frames->EnableWindow(FALSE);
		sp_frames->EnableWindow(FALSE); 
		bt_set->EnableWindow(FALSE);
	}
	
	bt_pause->EnableWindow(bs);
	bt_reset->EnableWindow(bs);
	sd_inter->EnableWindow(bs);
	cb_anim->EnableWindow(bs);
	cj_opciones->EnableWindow(bs); 
	cj_frame->EnableWindow(bs); 
	cj_inter->EnableWindow(bs); 
}

void dlg_animaciones::OnCustomdrawSliderInter(NMHDR* pNMHDR, LRESULT* pResult) 
// varia la velocidad de interpolación
{
	CSliderCtrl *sd_inter = (CSliderCtrl *) GetDlgItem(IDC_SLIDER_INTER);
	
	if (doc->vgl->i_num_modelos > 0)
	{		
		doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Set_Coeft((float)(sd_inter->GetPos()) / (float)(sd_inter->GetRangeMax()));
	}
	
	*pResult = 0;
}

void dlg_animaciones::OnSelchangeElegirAnimacion() 
// cambia la animación actual
{
	CComboBox *cb_anim = (CComboBox *) GetDlgItem(IDC_ELEGIR_ANIMACION);
	
	if (doc->vgl->i_num_modelos > 0)
	{	
		doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Set_Animacion(cb_anim->GetCurSel());	
		V_Actualizar();
	}
}

void dlg_animaciones::OnBAnimacionesPlay() 
// play / pause de la animación
{
	if (doc->vgl->i_num_modelos > 0)
	{
		doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Play_Pause();
		V_Actualizar();
	}	
}

void dlg_animaciones::OnBSetFrame() 
// cambia el frame actual
{
	CSpinButtonCtrl *sp_frames = (CSpinButtonCtrl *) GetDlgItem(IDC_CAMBIO_FRAME);

	if (doc->vgl->i_num_modelos > 0)
	{
		m_frame = sp_frames->GetPos();

		if ((m_frame < 0) || (m_frame > 197))
		{
			MessageBox("Frame no válido, rango [0...197]", "¡¡¡ Error !!!", MB_OK | MB_ICONERROR);			
			m_frame = 0;
		}

		doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Set_Animacion(TODAS);
		doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Set_Frame(m_frame);

		V_Actualizar();
	}
}

void dlg_animaciones::OnBResetAnim() 
// deja la anim como al principio
{
	if (doc->vgl->i_num_modelos > 0)
	{	
		doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Set_Frame(0);
		doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Set_Coeft(0.02f);	
		doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Set_Animacion(TODAS);
		doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Play_Pause();

		V_Actualizar();
	}
}
