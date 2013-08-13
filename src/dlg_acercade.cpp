/* ==============================================================================
	
	  Vmd: Visualizador de modelos del quake II
	  Versión: 1.0.0b

	  Autores: José Angel Montelongo Reyes 
			   Javier Honorio Casado Fernández

	  Universidad de Las Palmas de Gran Canaria	
	  	  
   ============================================================================== */


// dlg_acercade.cpp : implementation file
//

#include "stdafx.h"
#include "vmd.h"
#include "dlg_acercade.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// dlg_acercade dialog


dlg_acercade::dlg_acercade(CWnd* pParent /*=NULL*/)
	: CDialog(dlg_acercade::IDD, pParent)
{
	//{{AFX_DATA_INIT(dlg_acercade)
	m_creditos = _T("");
	//}}AFX_DATA_INIT
}


void dlg_acercade::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(dlg_acercade)
	DDX_Text(pDX, IDC_CREDITOS, m_creditos);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(dlg_acercade, CDialog)
	//{{AFX_MSG_MAP(dlg_acercade)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// dlg_acercade message handlers
