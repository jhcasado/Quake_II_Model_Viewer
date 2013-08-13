/* ==============================================================================
	
	  Vmd: Visualizador de modelos del quake II
	  Versión: 1.0.0b

	  Autores: José Angel Montelongo Reyes 
			   Javier Honorio Casado Fernández

	  Universidad de Las Palmas de Gran Canaria	
	  	  
   ============================================================================== */


// dlg_modelos.cpp : implementation file
//

#include "stdafx.h"
#include "vmd.h"
#include "MainFrm.h"
#include "dlg_modelos.h"
#include "dlg_modelos_info.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// dlg_modelos dialog


dlg_modelos::dlg_modelos(CWnd* pParent /*=NULL*/)
	: CDialog(dlg_modelos::IDD, pParent)
{
	//{{AFX_DATA_INIT(dlg_modelos)
	m_strips_and_fans = FALSE;
	//}}AFX_DATA_INIT
}


void dlg_modelos::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(dlg_modelos)
	DDX_Check(pDX, IDC_CSTRIPS_FANS, m_strips_and_fans);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(dlg_modelos, CDialog)
	//{{AFX_MSG_MAP(dlg_modelos)
	ON_BN_CLICKED(IDC_B_MODELOS_CERRAR, OnBModelosCerrar)
	ON_BN_CLICKED(IDC_B_MODELO, OnBModelo)
	ON_BN_CLICKED(IDC_B_MODELO_TEXTURA, OnBModeloTextura)
	ON_CBN_SELCHANGE(IDC_ELEGIR_SOMBREADO, OnSelchangeElegirSombreado)
	ON_CBN_SELCHANGE(IDC_ELEGIR_CAJAS, OnSelchangeElegirCajas)
	ON_BN_CLICKED(IDC_B_COLOR_CAJA, OnBColorCaja)
	ON_BN_CLICKED(IDC_B_BORRAR_MODELO, OnBBorrarModelo)
	ON_BN_CLICKED(IDC_B_ARMA, OnBArma)
	ON_BN_CLICKED(IDC_B_COLOR, OnBColorMalla)
	ON_BN_CLICKED(IDC_D_BORRAR_ARMA, OnDBorrarArma)
	ON_BN_CLICKED(IDC_B_RESET, OnBReset)
	ON_BN_CLICKED(IDC_CSTRIPS_FANS, OnCstripsFans)
	ON_BN_CLICKED(IDC_B_ARMA_TEXTURA, OnBArmaTextura)
	ON_CBN_SELCHANGE(IDC_ELEGIR_MODELOS, OnSelchangeElegirModelos)
	ON_CBN_SELCHANGE(IDC_ELEGIR_ARMAS, OnSelchangeElegirArmas)
	ON_BN_CLICKED(IDC_B_INFO, OnBInfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// dlg_modelos message handlers

BOOL dlg_modelos::OnInitDialog() 
// inicializamos el dialogo
{
	CDialog::OnInitDialog();
	
	CComboBox *cb_mdl = (CComboBox *) GetDlgItem(IDC_ELEGIR_MODELOS);
	cb_mdl->SetCurSel(0);

	CComboBox *cb_armas = (CComboBox *) GetDlgItem(IDC_ELEGIR_ARMAS);
	cb_armas->SetCurSel(0);

	UpdateData(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void dlg_modelos::V_Actualizar()
// actualza los controles de la ventana
{
	CComboBox *cb_som = (CComboBox *) GetDlgItem(IDC_ELEGIR_SOMBREADO);
	CComboBox *cb_cajas = (CComboBox *) GetDlgItem(IDC_ELEGIR_CAJAS);
	CButton *gp_som = (CButton *) GetDlgItem(IDC_CAJA_SOMBREADO);
	CButton *gp_cajas = (CButton *) GetDlgItem(IDC_CAJA_CAJAS);
	CButton *bt_mod_tex = (CButton *) GetDlgItem(IDC_B_BORRAR_MODELO);
	CButton *bt_mod_borrar = (CButton *) GetDlgItem(IDC_B_MODELO_TEXTURA);
	CButton *bt_arma_tex = (CButton *) GetDlgItem(IDC_D_BORRAR_ARMA);
	CButton *bt_arma_borrar = (CButton *) GetDlgItem(IDC_B_ARMA_TEXTURA);
	CButton *bt_color = (CButton *) GetDlgItem(IDC_B_COLOR);
	CButton *bt_color_caja = (CButton *) GetDlgItem(IDC_B_COLOR_CAJA);
	CButton *bt_reset = (CButton *) GetDlgItem(IDC_B_RESET);
	CButton *bt_info = (CButton *) GetDlgItem(IDC_B_INFO);
	CButton *bt_sf = (CButton *) GetDlgItem(IDC_CSTRIPS_FANS);
	
	int p = -1;
	BOOL activo, modelo, arma;
	activo = FALSE;
	modelo = FALSE;
	arma = FALSE;

	if (doc->vgl->i_num_modelos > 0)
	{
		activo = TRUE;
		p = 0;
		
		if (doc->vgl->p_lmodelos [doc->vgl->i_elto_actual] != -1)
		{
			modelo = TRUE;
		}

		if (doc->vgl->p_larmas [doc->vgl->i_elto_actual] != -1)
		{
			arma = TRUE;
		}
	}

	cb_som->SetCurSel(p);
	cb_som->EnableWindow(activo);
	cb_cajas->SetCurSel(p);
	cb_cajas->EnableWindow(activo);
	gp_som->EnableWindow(activo);			
	gp_cajas->EnableWindow(activo);			
	bt_mod_borrar->EnableWindow(modelo);			
	bt_mod_tex->EnableWindow(modelo);			
	bt_arma_borrar->EnableWindow(arma);			
	bt_arma_tex->EnableWindow(arma);			
	bt_color_caja->EnableWindow(activo);			 
	bt_color->EnableWindow(activo);			
	bt_reset->EnableWindow(activo);			
	bt_info->EnableWindow(activo);			
	bt_sf->EnableWindow(activo);			
}

BOOL dlg_modelos::B_Anadir_Modelo()
// añade un modelo a la lista
{
	CComboBox *cb_mdl = (CComboBox *) GetDlgItem(IDC_ELEGIR_MODELOS);
	CComboBox *cb_arma = (CComboBox *) GetDlgItem(IDC_ELEGIR_ARMAS);
	CComboBox *cb_som = (CComboBox *) GetDlgItem(IDC_ELEGIR_SOMBREADO);
	CComboBox *cb_cajas = (CComboBox *) GetDlgItem(IDC_ELEGIR_CAJAS);

	// superamos el maximo
	
	if (cb_mdl->GetCount() >= doc->vgl->i_max_num_modelos)
	{
		return FALSE;
	}
	
	// dialogo para cargar el modelo

	char BASED_CODE szfilter[] = "Modelos (*.md2)|*.md2||";
	CFileDialog *abrir_fichero = NULL;
	BOOL bborrar = TRUE, bnuevo = FALSE;

	abrir_fichero = new CFileDialog(TRUE, "*.md2", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szfilter);
	
	if (!abrir_fichero)
	{
		return FALSE;
	}

	int i_ret = abrir_fichero->DoModal();

	if (i_ret == IDOK)
	{						
		int n = cb_mdl->GetCurSel(); // elto actual
		
		if (doc->vgl->p_lmodelos [n] != -1)
		{
			n = cb_mdl->GetCount();
			bborrar = FALSE;
		}

		// cargamos el modelos
		
		char nfichero [256];
		sprintf_s(nfichero, "%s", LPCTSTR(abrir_fichero->GetPathName()));

		if ((doc->vgl->i_elto_actual != -1) && (doc->vgl->i_elto_actual != n))
		{
			doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Activar_Desactivar();
		}
				
		if (doc->vgl->l_modelos [n] == NULL)
		{
			bnuevo = TRUE;
			doc->vgl->l_modelos [n] = new TModelo;			
			doc->vgl->l_modelos [n]->Trasladar (0.0f, 0.0f, 0.0f);
			doc->vgl->l_modelos [n]->Set_Animacion (TODAS);		
			doc->vgl->i_elto_actual = n;

			TColor color;

			color [0] = 1.0;
			color [1] = 1.0;
			color [2] = 0.0;
			color [3] = 1.0;

			doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Set_Color_Activo(COLOR_CAJA_EX, color);
			doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Set_Color_Activo(COLOR_CAJA_IN, color);

			color [0] *= 0.5;
			color [1] *= 0.5;
			color [2] *= 0.5;

			doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Set_Color_Inactivo(COLOR_CAJA_EX, color);
			doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Set_Color_Inactivo(COLOR_CAJA_IN, color);			
		}

		doc->vgl->p_lmodelos [n] = doc->vgl->l_modelos [n]->Leer_MD2(nfichero);

		if (doc->vgl->p_lmodelos [n] == -1)
		{
			delete doc->vgl->l_modelos [doc->vgl->i_elto_actual];
			doc->vgl->l_modelos [doc->vgl->i_elto_actual] = NULL;

			return FALSE;
		}
		
		if (bnuevo)
		{
			doc->vgl->i_num_modelos ++;
		}
		
		// actualizamos las listas

		if (bborrar)
		{
			cb_mdl->DeleteString(n);
		}

		else 
		{
			n = cb_mdl->GetCount();
			cb_arma->InsertString(n, "<ninguna>");
			cb_arma->SetCurSel(n);
		}
		
		cb_mdl->InsertString(n, abrir_fichero->GetPathName());
		cb_mdl->SetCurSel(n); 						
		
		// acutalizamos los controles
	
		V_Actualizar();

		cb_som->SetCurSel(doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Get_Modo_De_Dibujo());		
		cb_cajas->SetCurSel(doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Get_Cuantas_Cajas());								
		m_strips_and_fans = doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Get_Strips_and_Fans();
		
		UpdateData(FALSE);	
	}
	
	delete abrir_fichero;

	return TRUE;
}

BOOL dlg_modelos::B_Textura_Modelo()
// carga una textura para el modelo actual
{
	CComboBox *cb_mdl = (CComboBox *) GetDlgItem(IDC_ELEGIR_MODELOS);
	CComboBox *cb_som = (CComboBox *) GetDlgItem(IDC_ELEGIR_SOMBREADO);
	
	int n = cb_mdl->GetCurSel(); // elto actual

	if (doc->vgl->p_lmodelos [n] == -1)
	{
		return TRUE;
	}
	
	// cargamos el dialgo de elección del fichero

	char BASED_CODE szfilter[] = "Texturas (*.pcx)|*.pcx||";
	CFileDialog *abrir_fichero;

	abrir_fichero = new CFileDialog(TRUE, "*.pcx", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szfilter);
	
	if (!abrir_fichero)
	{
		return FALSE;
	}

	int i_ret = abrir_fichero->DoModal();

	if (i_ret == IDOK)
	{								
		// cargamos la textura
		
		char nfichero [256];
		sprintf_s(nfichero, "%s", 
			LPCTSTR(abrir_fichero->GetPathName()));
	
		if (doc->vgl->p_lmodelos [n] != -1)
		{
			doc->vgl->l_modelos [n]->Leer_Textura(nfichero, doc->vgl->p_lmodelos [n]);
		}

		else
		{
			return FALSE;	
		}

		// actualiza los controles

		cb_som->SetCurSel(doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Get_Modo_De_Dibujo());
	}

	delete abrir_fichero;

	return TRUE;
}

BOOL dlg_modelos::B_Textura_Arma()
// carga la textura del arma
{
	CComboBox *cb_arma = (CComboBox *) GetDlgItem(IDC_ELEGIR_ARMAS);
	CComboBox *cb_som = (CComboBox *) GetDlgItem(IDC_ELEGIR_SOMBREADO);

	int n = cb_arma->GetCurSel(); // elto actual

	if (doc->vgl->p_larmas [n] == -1)
	{
		return TRUE;
	}
	
	// cargamos un modelo

	char BASED_CODE szfilter[] = "Texturas (*.pcx)|*.pcx||";
	CFileDialog *abrir_fichero;

	abrir_fichero = new CFileDialog(TRUE, "*.pcx", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szfilter);
	
	if (!abrir_fichero)
	{
		return FALSE;
	}

	int i_ret = abrir_fichero->DoModal();

	if (i_ret == IDOK)
	{								
		// cargamos la textura
		
		char nfichero [256];
		sprintf_s(nfichero, "%s", LPCTSTR(abrir_fichero->GetPathName()));
	
		if (doc->vgl->p_larmas [n] != -1)
		{
			doc->vgl->l_modelos [n]->Leer_Textura(nfichero, doc->vgl->p_larmas [n]);
		}

		else
		{
			return FALSE;	
		}

		// actualizamos los controles

		cb_som->SetCurSel(doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Get_Modo_De_Dibujo());
	}	

	delete abrir_fichero;

	return TRUE;
}

BOOL dlg_modelos::B_Anadir_Arma()
// añade un arma a la lista
{
	CComboBox *cb_mdl = (CComboBox *) GetDlgItem(IDC_ELEGIR_MODELOS);
	CComboBox *cb_arma = (CComboBox *) GetDlgItem(IDC_ELEGIR_ARMAS);
	CComboBox *cb_som = (CComboBox *) GetDlgItem(IDC_ELEGIR_SOMBREADO);
	CComboBox *cb_cajas = (CComboBox *) GetDlgItem(IDC_ELEGIR_CAJAS);

	if (cb_mdl->GetCount() >= doc->vgl->i_max_num_modelos)
	{
		return FALSE;
	}

	// cargamos un arma para el modelo

	char BASED_CODE szfilter[] = "Armas (*.md2)|*.md2||";
	CFileDialog *abrir_fichero;
	BOOL bborrar = TRUE, bnuevo = FALSE;

	abrir_fichero = new CFileDialog(TRUE, "*.md2", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szfilter);
	
	if (!abrir_fichero)
	{
		return FALSE;
	}

	int i_ret = abrir_fichero->DoModal();

	if (i_ret == IDOK)
	{		
		int n = cb_arma->GetCurSel(); // elto actual
				
		if (doc->vgl->p_larmas [n] != -1)
		{
			n = cb_arma->GetCount();
			bborrar = FALSE;
		}
		
		// cargamos el arma
		
		char nfichero [256];
		sprintf_s(nfichero, "%s", LPCTSTR(abrir_fichero->GetPathName()));		

		if ((doc->vgl->i_elto_actual != -1) && (doc->vgl->i_elto_actual != n))
		{
			doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Activar_Desactivar();
		}
					
		if (doc->vgl->l_modelos [n] == NULL) 
		{
			bnuevo = TRUE;
			doc->vgl->l_modelos [n] = new TModelo;			
			doc->vgl->l_modelos [n]->Trasladar (0.0f, 0.0f, 0.0f);
			doc->vgl->l_modelos [n]->Set_Animacion (TODAS);
			doc->vgl->i_elto_actual = n;

			TColor color;

			color [0] = 1.0;
			color [1] = 1.0;
			color [2] = 0.0;
			color [3] = 1.0;

			doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Set_Color_Activo(COLOR_CAJA_EX, color);
			doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Set_Color_Activo(COLOR_CAJA_IN, color);

			color [0] *= 0.5;
			color [1] *= 0.5;
			color [2] *= 0.5;

			doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Set_Color_Inactivo(COLOR_CAJA_EX, color);
			doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Set_Color_Inactivo(COLOR_CAJA_IN, color);
		}		

		doc->vgl->p_larmas [n] = doc->vgl->l_modelos [n]->Leer_MD2(nfichero);

		if (doc->vgl->p_larmas [n] == -1)
		{
			delete doc->vgl->l_modelos [doc->vgl->i_elto_actual];
			doc->vgl->l_modelos [doc->vgl->i_elto_actual] = NULL;

			return FALSE;
		}
			
		if (bnuevo) 
		{
			doc->vgl->i_num_modelos ++;
		}

		// actualiza las listas

		if (bborrar)
		{
			cb_arma->DeleteString(n);
		}

		else 
		{
			n = cb_arma->GetCount();
			cb_mdl->InsertString(n, "<ninguno>");
			cb_mdl->SetCurSel(n);
		}
		
		cb_arma->InsertString(n, abrir_fichero->GetPathName());
		cb_arma->SetCurSel(n); 						
		
		// acualiza los controles
		
		V_Actualizar();

		cb_som->SetCurSel(doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Get_Modo_De_Dibujo());
		cb_cajas->SetCurSel(doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Get_Cuantas_Cajas());						
		m_strips_and_fans = doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Get_Strips_and_Fans();

		UpdateData(FALSE);	
	}

	delete abrir_fichero;

	return TRUE;
}

BOOL dlg_modelos::B_Borrar_Modelo()
// borra el modelo actual, el modelo y el arma
{
	CComboBox *cb_mdl = (CComboBox *) GetDlgItem(IDC_ELEGIR_MODELOS);
	CComboBox *cb_arma = (CComboBox *) GetDlgItem(IDC_ELEGIR_ARMAS);
	CComboBox *cb_som = (CComboBox *) GetDlgItem(IDC_ELEGIR_SOMBREADO);
	CComboBox *cb_cajas = (CComboBox *) GetDlgItem(IDC_ELEGIR_CAJAS);	

	int n;
	
	n = cb_mdl->GetCurSel();

	if (doc->vgl->i_num_modelos < 1)
	{
		return TRUE;
	}

	if (doc->vgl->l_modelos [n] == NULL)
	{
		return FALSE;
	}
	
	doc->vgl->i_num_modelos --;		
	delete doc->vgl->l_modelos [n];

	for (int c = n; c < doc->vgl->i_num_modelos; c ++)
	{
		doc->vgl->l_modelos [c] = doc->vgl->l_modelos [c + 1];
		doc->vgl->p_lmodelos [c] = doc->vgl->p_lmodelos [c + 1];
		doc->vgl->p_larmas [c] = doc->vgl->p_larmas [c + 1];
	}

	doc->vgl->l_modelos [doc->vgl->i_num_modelos] = NULL;
	doc->vgl->p_lmodelos [doc->vgl->i_num_modelos] = -1;
	doc->vgl->p_larmas [doc->vgl->i_num_modelos] = -1;

	// actualizamos las listas

	cb_mdl->DeleteString(n);
	cb_arma->DeleteString(n);
	
	if (cb_mdl->GetCount() == 0)
	{
		cb_mdl->AddString("<ninguno>");
		cb_arma->AddString("<ninguna>");
	}

	if (n < 1) 
	{
		n = 1;
	}
		
	cb_mdl->SetCurSel(n - 1);
	cb_arma->SetCurSel(n - 1);
		
	doc->vgl->i_elto_actual = cb_mdl->GetCurSel();

	if (doc->vgl->i_num_modelos > 0)
	{
		doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Activar_Desactivar();	
		
		V_Actualizar();
	
		cb_som->SetCurSel(doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Get_Modo_De_Dibujo());		
		cb_cajas->SetCurSel(doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Get_Cuantas_Cajas());								
		m_strips_and_fans = doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Get_Strips_and_Fans();

		UpdateData(FALSE);		
	}

	else
	{
		V_Actualizar();
	}

	return TRUE;
}

BOOL dlg_modelos::B_Reset()
// borra todos los modelos de la lista
{
	while (doc->vgl->i_num_modelos > 0)
	{
		if (!B_Borrar_Modelo())
		{
			return FALSE;
		}
	}
	
	return TRUE;
}

void dlg_modelos::V_Cambiar_Modelo(int nuevo)
// cambia el modelo actual
{
	CComboBox *cb_mdl = (CComboBox *) GetDlgItem(IDC_ELEGIR_MODELOS);
	CComboBox *cb_arma = (CComboBox *) GetDlgItem(IDC_ELEGIR_ARMAS);
	CComboBox *cb_som = (CComboBox *) GetDlgItem(IDC_ELEGIR_SOMBREADO);
	CComboBox *cb_cajas = (CComboBox *) GetDlgItem(IDC_ELEGIR_CAJAS);	

	if ((doc->vgl->i_num_modelos > 0) && (nuevo >= 0) && 
		(nuevo != doc->vgl->i_elto_actual) && (nuevo < doc->vgl->i_num_modelos))
	{
		doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Activar_Desactivar();		
		doc->vgl->i_elto_actual = nuevo;
		doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Activar_Desactivar();	

		V_Actualizar();

		cb_mdl->SetCurSel(doc->vgl->i_elto_actual);	
		cb_arma->SetCurSel(doc->vgl->i_elto_actual);	
		cb_som->SetCurSel(doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Get_Modo_De_Dibujo());
		cb_cajas->SetCurSel(doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Get_Cuantas_Cajas());							
		m_strips_and_fans = doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Get_Strips_and_Fans();
	
		UpdateData(FALSE);		
	}
}

void dlg_modelos::OnBModelosCerrar() 
// cerramos el dialogo
{
	EndDialog(IDOK);	
}

void dlg_modelos::OnBModelo() 
// al pulsar el borón modelo
{
	if (!B_Anadir_Modelo())
	{
		MessageBox("Imposible añadir el modelo", "¡¡¡ Error !!!", MB_OK | MB_ICONERROR);
	}
}

void dlg_modelos::OnBModeloTextura() 
// al pulsar el botón de la textura
{
	if (!B_Textura_Modelo())
	{
		MessageBox("Imposible cargar la textura", "¡¡¡ Error !!!", MB_OK | MB_ICONERROR);
	}	
}

void dlg_modelos::OnSelchangeElegirSombreado() 
// cambia el mode de sombreado
{
	CComboBox *cb_som = (CComboBox *) GetDlgItem(IDC_ELEGIR_SOMBREADO);

	if (doc->vgl->i_num_modelos > 0)
	{
		doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Set_Modo_De_Dibujo(cb_som->GetCurSel());
	}	
}

void dlg_modelos::OnSelchangeElegirCajas() 
// cambia el numero de cajas a dibujar
{
	CComboBox *cb_cajas = (CComboBox *) GetDlgItem(IDC_ELEGIR_CAJAS);

	if (doc->vgl->i_num_modelos > 0)
	{
		doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Set_Cuantas_Cajas(cb_cajas->GetCurSel());
	}		
}

void dlg_modelos::OnBColorCaja() 
// cambia el color de las cajas
{
	CColorDialog *dlg_color;
	TColor color;	
	BYTE r, g, b;
	
	if (doc->vgl->i_num_modelos < 1)
	{
		return;
	}

	doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Get_Color_Activo(COLOR_CAJA_EX, color);
		
	r = (BYTE)(color [0] * 255.0);
	g = (BYTE)(color [1] * 255.0);
	b = (BYTE)(color [2] * 255.0);

	dlg_color = new CColorDialog(RGB(r, g, b)); 
	int i_ret = dlg_color->DoModal();

	if (i_ret == IDOK)
	{
		COLORREF cs = dlg_color->GetColor();
		
		r = (BYTE)((cs & 0x000000ff));
		g = (BYTE)((cs & 0x0000ff00) >> 8);
		b = (BYTE)((cs & 0x00ff0000) >> 16);

		color [0] = (float)(r / 255.0);
		color [1] = (float)(g / 255.0);		
		color [2] = (float)(b / 255.0);
		color [3] = 1.0;

		doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Set_Color_Activo(COLOR_CAJA_EX, color);
		doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Set_Color_Activo(COLOR_CAJA_IN, color);

		color [0] *= 0.5;
		color [1] *= 0.5;
		color [2] *= 0.5;

		doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Set_Color_Inactivo(COLOR_CAJA_EX, color);
		doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Set_Color_Inactivo(COLOR_CAJA_IN, color);
	}		

	delete dlg_color;
}

void dlg_modelos::OnBBorrarModelo() 
// borra el modelo actual y su arma
{	
	if (!B_Borrar_Modelo())
	{
		MessageBox("Imposible borrar el modelo actual", "¡¡¡ Error !!!", MB_OK | MB_ICONERROR);
	}		
}

void dlg_modelos::OnBArma() 
// al pulsar el boton arma añadimos un arma a la lista
{
	if (!B_Anadir_Arma())
	{
		MessageBox("Imposible añadir el arma", "¡¡¡ Error !!!", MB_OK | MB_ICONERROR);
	}	
}

void dlg_modelos::OnBColorMalla() 
// cambia el color de la malla
{
	CColorDialog *dlg_color;
	TColor color;	
	BYTE r, g, b;
	
	if (doc->vgl->i_num_modelos < 1)
	{
		return;
	}

	doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Get_Color_Activo(COLOR_MODELO, color);
		
	r = (BYTE)(color [0] * 255.0);
	g = (BYTE)(color [1] * 255.0);
	b = (BYTE)(color [2] * 255.0);

	dlg_color = new CColorDialog(RGB(r, g, b)); 
	int i_ret = dlg_color->DoModal();

	if (i_ret == IDOK)
	{
		COLORREF cs = dlg_color->GetColor();
		
		r = (BYTE)((cs & 0x000000ff));
		g = (BYTE)((cs & 0x0000ff00) >> 8);
		b = (BYTE)((cs & 0x00ff0000) >> 16);

		color [0] = (float)(r / 255.0);
		color [1] = (float)(g / 255.0);		
		color [2] = (float)(b / 255.0);
		color [3] = 1.0;

		doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Set_Color_Activo(COLOR_MODELO, color);

		color [0] *= 0.5;
		color [1] *= 0.5;
		color [2] *= 0.5;

		doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Set_Color_Inactivo(COLOR_MODELO, color);
	}	

	delete dlg_color;
}

void dlg_modelos::OnDBorrarArma() 
// borra el modelo actual y su arma
{
	if (!B_Borrar_Modelo())
	{
		MessageBox("Imposible borrar el modelo actual", "¡¡¡ Error !!!", MB_OK | MB_ICONERROR);
	}			
}

void dlg_modelos::OnBReset() 
// al pulsar el botón reset
{
	if (doc->vgl->i_num_modelos > 0)
	{
		if (MessageBox("¿Borrar todos los modelos?", "Reset", MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			if (!B_Reset())
			{
				MessageBox("Imposible borrar todos los modelos", "¡¡¡ Error !!!", MB_OK | MB_ICONERROR);
			}
		}
	}
}

void dlg_modelos::OnCstripsFans() 
// cambia el mode de dibujo mediante Strips and Fans
{
	if (doc->vgl->i_num_modelos > 0)
	{
		doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Strips_and_Fans();
		UpdateData(TRUE);	
	}
}

void dlg_modelos::OnBArmaTextura() 
// elige un textura para el arma
{
	if (!B_Textura_Arma())
	{
		MessageBox("Imposible cargar la textura", "¡¡¡ Error !!!", MB_OK | MB_ICONERROR);
	}		
}

void dlg_modelos::OnSelchangeElegirModelos() 
// cambiamos el modelo actual
{
	CComboBox *cb_mdl = (CComboBox *) GetDlgItem(IDC_ELEGIR_MODELOS);
	CComboBox *cb_arma = (CComboBox *) GetDlgItem(IDC_ELEGIR_ARMAS);
	CComboBox *cb_som = (CComboBox *) GetDlgItem(IDC_ELEGIR_SOMBREADO);
	CComboBox *cb_cajas = (CComboBox *) GetDlgItem(IDC_ELEGIR_CAJAS);	

	if (doc->vgl->i_num_modelos > 0)
	{
		doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Activar_Desactivar();		
		doc->vgl->i_elto_actual = cb_mdl->GetCurSel();
		doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Activar_Desactivar();	
			
		V_Actualizar();

		cb_arma->SetCurSel(doc->vgl->i_elto_actual);	
		cb_som->SetCurSel(doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Get_Modo_De_Dibujo());
		cb_cajas->SetCurSel(doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Get_Cuantas_Cajas());							
		m_strips_and_fans = doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Get_Strips_and_Fans();
	
		UpdateData(FALSE);		
	}
}

void dlg_modelos::OnSelchangeElegirArmas() 
// cambia la eleción del arma
{
	CComboBox *cb_mdl = (CComboBox *) GetDlgItem(IDC_ELEGIR_MODELOS);
	CComboBox *cb_arma = (CComboBox *) GetDlgItem(IDC_ELEGIR_ARMAS);
	CComboBox *cb_som = (CComboBox *) GetDlgItem(IDC_ELEGIR_SOMBREADO);
	CComboBox *cb_cajas = (CComboBox *) GetDlgItem(IDC_ELEGIR_CAJAS);

	if (doc->vgl->i_num_modelos > 0)
	{
		doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Activar_Desactivar();
		doc->vgl->i_elto_actual = cb_arma->GetCurSel();
		doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Activar_Desactivar();	
	
		V_Actualizar();
			
		cb_mdl->SetCurSel(doc->vgl->i_elto_actual);		
		cb_som->SetCurSel(doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Get_Modo_De_Dibujo());	
		cb_cajas->SetCurSel(doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Get_Cuantas_Cajas());							
		m_strips_and_fans = doc->vgl->l_modelos [doc->vgl->i_elto_actual]->Get_Strips_and_Fans();

		UpdateData(FALSE);		
	}
}

void dlg_modelos::OnBInfo() 
// mostramos la info del modelo
{
	dlg_modelos_info d_info;	
	char s [256];

	// modelo

	if ((doc->vgl->i_num_modelos > 0) && (doc->vgl->p_lmodelos [doc->vgl->i_elto_actual] != -1))
	{
		int p = doc->vgl->p_lmodelos [doc->vgl->i_elto_actual];
		TModelo *mod = doc->vgl->l_modelos [doc->vgl->i_elto_actual];

		sprintf_s(s, "Nº de frames : %d", mod->Num_Frames(p));
		CString num_frames(s);

		sprintf_s(s, "Nº de vértices : %d", mod->Num_Vertices(p));
		CString num_vertices(s);

		sprintf_s(s, "Nº de caras : %d", mod->Num_Caras(p));
		CString num_caras(s);

		sprintf_s(s, "Nº de coodenadas de textura : %d", mod->Num_Coordenadas_Tex(p));
		CString num_ct(s);

		sprintf_s(s, "Nº de comandos GL : %d", mod->Num_Gl_Commmand(p));
		CString num_glc(s);

		d_info.m_modelo_frames = num_frames;
		d_info.m_modelo_vertices = num_vertices;
		d_info.m_modelo_caras = num_caras;
		d_info.m_modelo_ct = num_ct;
		d_info.m_modelo_glc = num_glc;
	}

	else 
	{
		sprintf_s(s, "No existe.");
		CString nohay(s);

		d_info.m_modelo_frames = nohay;
	}

	// arma

	if ((doc->vgl->i_num_modelos > 0) && (doc->vgl->p_larmas [doc->vgl->i_elto_actual] != -1))
	{
		int p = doc->vgl->p_larmas [doc->vgl->i_elto_actual];
		TModelo *mod = doc->vgl->l_modelos [doc->vgl->i_elto_actual];

		sprintf_s(s, "Nº de frames : %d", mod->Num_Frames(p));
		CString num_frames(s);

		sprintf_s(s, "Nº de vértices : %d", mod->Num_Vertices(p));
		CString num_vertices(s);

		sprintf_s(s, "Nº de caras : %d", mod->Num_Caras(p));
		CString num_caras(s);

		sprintf_s(s, "Nº de coodenadas de textura : %d", mod->Num_Coordenadas_Tex(p));
		CString num_ct(s);

		sprintf_s(s, "Nº de comandos GL : %d", mod->Num_Gl_Commmand(p));
		CString num_glc(s);

		d_info.m_arma_frames = num_frames;
		d_info.m_arma_vertices = num_vertices;
		d_info.m_arma_caras = num_caras;
		d_info.m_arma_ct = num_ct;
		d_info.m_arma_glc = num_glc;
	}

	else 
	{
		sprintf_s(s, "No existe.");
		CString nohay(s);

		d_info.m_arma_frames = nohay;
	}

	d_info.DoModal();	
}
