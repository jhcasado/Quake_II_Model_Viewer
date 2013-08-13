/* ==============================================================================
	
	  Vmd: Visualizador de modelos del quake II
	  Versión: 1.0.0b

	  Autores: José Angel Montelongo Reyes 
			   Javier Honorio Casado Fernández

	  Universidad de Las Palmas de Gran Canaria	
	  	  
   ============================================================================== */

// vmdDoc.h : interface of the CVmdDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_VMDDOC_H__3527BA4A_4F7D_11D5_9D1F_C8D24E49A71F__INCLUDED_)
#define AFX_VMDDOC_H__3527BA4A_4F7D_11D5_9D1F_C8D24E49A71F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "t_glventana.h"


class CVmdDoc : public CDocument
{
protected: // create from serialization only
	CVmdDoc();
	DECLARE_DYNCREATE(CVmdDoc)

// Attributes
public:
	T_GLVENTANA *vgl;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVmdDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CVmdDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CVmdDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VMDDOC_H__3527BA4A_4F7D_11D5_9D1F_C8D24E49A71F__INCLUDED_)
