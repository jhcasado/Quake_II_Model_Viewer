/* ==============================================================================
	
	  Vmd: Visualizador de modelos del quake II
	  Versión: 1.0.0b

	  Autores: José Angel Montelongo Reyes 
			   Javier Honorio Casado Fernández

	  Universidad de Las Palmas de Gran Canaria	
	  	  
   ============================================================================== */


// vmdView.h : interface of the CVmdView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_VMDVIEW_H__3527BA4C_4F7D_11D5_9D1F_C8D24E49A71F__INCLUDED_)
#define AFX_VMDVIEW_H__3527BA4C_4F7D_11D5_9D1F_C8D24E49A71F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "timer.h"


class CVmdView : public CView
{
protected: // create from serialization only
	CVmdView();
	DECLARE_DYNCREATE(CVmdView);

// Attributes
public:
	CVmdDoc *GetDocument();
				
	HGLRC m_hRC;
	CDC *m_pDC;
	
	int m_rx, m_ry;
	CTimer m_fps;

// Operations

private:
	void V_Dibujar_Luz();

public:
	
	void V_OnSize(int cx, int cy); 	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVmdView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CVmdView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CVmdView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in vmdView.cpp
inline CVmdDoc* CVmdView::GetDocument()
   { return (CVmdDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VMDVIEW_H__3527BA4C_4F7D_11D5_9D1F_C8D24E49A71F__INCLUDED_)
