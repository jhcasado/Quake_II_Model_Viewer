/* ==============================================================================
	
	  Vmd: Visualizador de modelos del quake II
	  Versión: 1.0.0b

	  Autores: José Angel Montelongo Reyes 
			   Javier Honorio Casado Fernández

	  Universidad de Las Palmas de Gran Canaria	
	  	  
   ============================================================================== */


// dlg_luces_op.cpp : implementation file
//

#include "stdafx.h"
#include "vmd.h"
#include "dlg_luces_op.h"
#include "dlg_luces.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// dlg_luces_op dialog


dlg_luces_op::dlg_luces_op(CWnd* pParent /*=NULL*/)
	: CDialog(dlg_luces_op::IDD, pParent)
{
	//{{AFX_DATA_INIT(dlg_luces_op)
	m_px = 0.0f;
	m_py = 0.0f;
	m_pz = 0.0f;
	m_mostrar = FALSE;
	//}}AFX_DATA_INIT
}


void dlg_luces_op::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(dlg_luces_op)
	DDX_Text(pDX, IDC_LUZ_EDIT_PX, m_px);
	DDX_Text(pDX, IDC_LUZ_EDIT_PY, m_py);
	DDX_Text(pDX, IDC_LUZ_EDIT_PZ, m_pz);
	DDX_Check(pDX, IDC_LUCES_MOSTRAR, m_mostrar);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(dlg_luces_op, CDialog)
	//{{AFX_MSG_MAP(dlg_luces_op)
	ON_BN_CLICKED(IDC_LUCES_COLOR, OnLucesColor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// dlg_luces_op message handlers

void dlg_luces_op::OnLucesColor() 
// cambia el color de la luz
{
	BYTE r, g, b;
	CVmdDoc *doc = (CVmdDoc *)(GetParentFrame()->GetActiveDocument());		
	
	r = (BYTE)(doc->vgl->luz.r * 255.0);
	g = (BYTE)(doc->vgl->luz.g * 255.0);
	b = (BYTE)(doc->vgl->luz.b * 255.0);
	
	CColorDialog dlg_color(RGB(r, g, b));

	int i_ret = dlg_color.DoModal();

	if (i_ret == IDOK)
	{
		COLORREF cs = dlg_color.GetColor();
		
		r = (BYTE)((cs & 0x000000ff));
		g = (BYTE)((cs & 0x0000ff00) >> 8);
		b = (BYTE)((cs & 0x00ff0000) >> 16);

		doc->vgl->luz.r = (float)(r / 255.0);
		doc->vgl->luz.g = (float)(g / 255.0);
		doc->vgl->luz.b = (float)(b / 255.0);

		((dlg_luces *)GetParent())->V_Actualizar();

		GetParentFrame()->Invalidate(); 
	}		
}
