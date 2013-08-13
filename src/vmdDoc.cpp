/* ==============================================================================
	
	  Vmd: Visualizador de modelos del quake II
	  Versión: 1.0.0b

	  Autores: José Angel Montelongo Reyes 
			   Javier Honorio Casado Fernández

	  Universidad de Las Palmas de Gran Canaria	
	  	  
   ============================================================================== */


// vmdDoc.cpp : implementation of the CVmdDoc class
//

#include "stdafx.h"
#include "vmd.h"

#include "vmdDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVmdDoc

IMPLEMENT_DYNCREATE(CVmdDoc, CDocument)

BEGIN_MESSAGE_MAP(CVmdDoc, CDocument)
	//{{AFX_MSG_MAP(CVmdDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVmdDoc construction/destruction

CVmdDoc::CVmdDoc()
{
	vgl = NULL;
}

CVmdDoc::~CVmdDoc()
{
	if (vgl != NULL) 
	{
		delete vgl;
		vgl = NULL;
	}
}

BOOL CVmdDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CVmdDoc serialization

void CVmdDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CVmdDoc diagnostics

#ifdef _DEBUG
void CVmdDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CVmdDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CVmdDoc commands
