/* ==============================================================================
	
	  Vmd: Visualizador de modelos del quake II
	  Versión: 1.0.0b

	  Autores: José Angel Montelongo Reyes 
			   Javier Honorio Casado Fernández

	  Universidad de Las Palmas de Gran Canaria	
	  	  
   ============================================================================== */


// dlg_modelos_info.cpp : implementation file
//

#include "stdafx.h"
#include "vmd.h"
#include "dlg_modelos_info.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// dlg_modelos_info dialog


dlg_modelos_info::dlg_modelos_info(CWnd* pParent /*=NULL*/)
	: CDialog(dlg_modelos_info::IDD, pParent)
{
	//{{AFX_DATA_INIT(dlg_modelos_info)
	m_modelo_ct = _T("");
	m_modelo_frames = _T("");
	m_modelo_glc = _T("");
	m_arma_caras = _T("");
	m_arma_ct = _T("");
	m_arma_frames = _T("");
	m_arma_glc = _T("");
	m_arma_vertices = _T("");
	m_modelo_caras = _T("");
	m_modelo_vertices = _T("");
	//}}AFX_DATA_INIT
}


void dlg_modelos_info::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(dlg_modelos_info)
	DDX_Text(pDX, IDC_INFO_MODELO_CT, m_modelo_ct);
	DDX_Text(pDX, IDC_INFO_MODELO_FRAMES, m_modelo_frames);
	DDX_Text(pDX, IDC_INFO_MODELO_GLC, m_modelo_glc);
	DDX_Text(pDX, IDC_INFO_ARMA_CARAS, m_arma_caras);
	DDX_Text(pDX, IDC_INFO_ARMA_CT, m_arma_ct);
	DDX_Text(pDX, IDC_INFO_ARMA_FRAMES, m_arma_frames);
	DDX_Text(pDX, IDC_INFO_ARMA_GLC, m_arma_glc);
	DDX_Text(pDX, IDC_INFO_ARMA_VERTICES, m_arma_vertices);
	DDX_Text(pDX, IDC_INFO_MODELO_CARAS, m_modelo_caras);
	DDX_Text(pDX, IDC_INFO_MODELO_VERTICES, m_modelo_vertices);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(dlg_modelos_info, CDialog)
	//{{AFX_MSG_MAP(dlg_modelos_info)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// dlg_modelos_info message handlers
