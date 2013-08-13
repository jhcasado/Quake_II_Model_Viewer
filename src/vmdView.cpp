/* ==============================================================================
	
	  Vmd: Visualizador de modelos del quake II
	  Versión: 1.0.0b

	  Autores: José Angel Montelongo Reyes 
			   Javier Honorio Casado Fernández

	  Universidad de Las Palmas de Gran Canaria	
	  	  
   ============================================================================== */


// vmdView.cpp : implementation of the CVmdView class
//

#include "stdafx.h"
#include "vmd.h"

#include "vmdDoc.h"
#include "vmdView.h"

#include "dlg_modelos.h"
#include "dlg_animaciones.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// dialogos


/////////////////////////////////////////////////////////////////////////////
// CVmdView

IMPLEMENT_DYNCREATE(CVmdView, CView)

BEGIN_MESSAGE_MAP(CVmdView, CView)
	//{{AFX_MSG_MAP(CVmdView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVmdView construction/destruction

CVmdView::CVmdView()
{
	m_rx = -1;
	m_ry = -1;
}

CVmdView::~CVmdView()
{
}

BOOL CVmdView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.lpszClass = ::AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_OWNDC,
		::LoadCursor(NULL, IDC_ARROW), NULL, NULL);
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CVmdView drawing

void CVmdView::OnDraw(CDC* pDC)
// dibuja la pantalla
{
	CVmdDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC); 

	// render
	pDoc->vgl->V_Dibujar_Escena();
	
	// luz
	if (pDoc->vgl->luz.visible)
	{
		V_Dibujar_Luz();
	}

	// intercambia los buffer
	SwapBuffers(m_pDC->GetSafeHdc()); 
	
	m_fps.Frame_Rate(); // calcula el frame rate
}

/////////////////////////////////////////////////////////////////////////////
// CVmdView diagnostics

#ifdef _DEBUG
void CVmdView::AssertValid() const
{
	CView::AssertValid();
}

void CVmdView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CVmdDoc* CVmdView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CVmdDoc)));
	return (CVmdDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CVmdView métodos privados

void CVmdView::V_Dibujar_Luz()
// dibuja la luz en su pos
{
	GLboolean luz, texturas, cull_face;
	GLint polygon_mode[2];	
	GLfloat punto;
	CVmdDoc* pDoc = GetDocument();
	
	// obtenemos el estado de OpenGL
	glGetFloatv(GL_POINT_SIZE, &punto);
	glGetBooleanv(GL_LIGHTING, &luz);
	glGetBooleanv(GL_TEXTURE_2D, &texturas);
	glGetBooleanv(GL_CULL_FACE, &cull_face);
	glGetIntegerv(GL_POLYGON_MODE, polygon_mode);

	if (luz)
	{
		glDisable(GL_LIGHTING);
	}

    if (texturas) 
	{
		glDisable(GL_TEXTURE_2D);
	}

	if (cull_face) 
	{
		glDisable(GL_CULL_FACE);
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

	// tamaño y color de la luz
	glPointSize(8.0f);	
	glColor4f(pDoc->vgl->luz.r, pDoc->vgl->luz.g, pDoc->vgl->luz.b, pDoc->vgl->luz.a);
	
	// dibujamos la luz, un punto
	glBegin(GL_POINTS);
		glVertex3f(pDoc->vgl->luz.px, pDoc->vgl->luz.py, pDoc->vgl->luz.pz);
	glEnd();

	// restauramos el estado anterior
	if (luz) 
	{
		glEnable(GL_LIGHTING);
	}

    if (texturas) 
	{
		glEnable(GL_TEXTURE_2D);
	}

	if (cull_face) 
	{
		glEnable(GL_CULL_FACE);
	}

	glPolygonMode(GL_FRONT_AND_BACK, polygon_mode[0]);

	glPointSize(punto);
}

/////////////////////////////////////////////////////////////////////////////
// CVmdView message handlers

void CVmdView::V_OnSize(int cx, int cy) 
// reescalamos la ventana
{
	CVmdDoc* pDoc = GetDocument();
	
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC); 
	pDoc->vgl->V_Escalar_Ventana(cx, cy);
}

int CVmdView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
// elegimos el formato del pixel
{
	if (CView::OnCreate(lpCreateStruct) == -1)	
		return -1;

	m_pDC = new CClientDC(this);

	if (m_pDC == NULL)
	{
		return FALSE;
	}
	
	// elegimos el formato del pixel
	PIXELFORMATDESCRIPTOR pfd = 
	{
		sizeof(PIXELFORMATDESCRIPTOR),    // size of this pfd
		1,                                // version number
		PFD_DRAW_TO_WINDOW |              // support window
		PFD_SUPPORT_OPENGL |              // support OpenGL
		PFD_DOUBLEBUFFER,                 // double buffered
		PFD_TYPE_RGBA,                    // RGBA type
		24,                               // color depth
		0, 0, 0, 0, 0, 0,                 // color bits ignored
		0,                                // no alpha buffer
		0,                                // shift bit ignored
		0,                                // no accumulation buffer
		0, 0, 0, 0,                       // accumulation bits ignored
		32,                               // z-buffer
		0,                                // no stencil buffer
		0,                                // no auxiliary buffer
		PFD_MAIN_PLANE,                   // main layer
		0,                                // reserved
		0, 0, 0                           // layer masks ignored
	};

	// obtenemos formato del pixel que mejor se adapta a nuestas necesidades
	int m_nPixelFormat = ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd);

	TRACE1("\n * m_nPixelFormat = %d\n",  m_nPixelFormat );
	
    if (m_nPixelFormat == 0)
	{
        return FALSE;
	}
	
    SetPixelFormat(m_pDC->GetSafeHdc(), m_nPixelFormat, &pfd);

	m_hRC = wglCreateContext(m_pDC->GetSafeHdc());

    if (m_hRC == 0)
	{
        return FALSE;
	}

	if (wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC) == FALSE)
	{
		return FALSE;
	}

	// inicilizamos

	CVmdDoc* pDoc = GetDocument();
	
	pDoc->vgl = new T_GLVENTANA;
	pDoc->vgl->V_Inicializar_OpenGL();
	pDoc->vgl->V_Inicializar_Listas(100);

	// crono para los fps

	m_fps.Start();

	return 0;
}

void CVmdView::OnDestroy() 
// destruimos la ventana
{
	CView::OnDestroy();

	wglMakeCurrent(0, 0);
	wglDeleteContext(m_hRC);

	if (m_pDC)
	{
		delete m_pDC;
	}

	m_pDC = NULL;	
}

void CVmdView::OnSize(UINT nType, int cx, int cy) 
// reescalamos la ventana
{
	CView::OnSize(nType, cx, cy);
		
	V_OnSize(cx, cy);
}

BOOL CVmdView::OnEraseBkgnd(CDC* pDC) 
// evitamos el parpadeo al repintar la pantalla
{
	return TRUE;
}

void CVmdView::OnMouseMove(UINT nFlags, CPoint point) 
// actualiza los valores para las transfromaciones
{
	CVmdDoc* pDoc = GetDocument();

	if(pDoc->vgl->i_num_modelos > 0)
	{
		if (nFlags & MK_LBUTTON)
		{
			if ((m_rx != -1) && (m_ry != -1))
			{		
				if (nFlags & MK_CONTROL)
				{
					switch (pDoc->vgl->i_plano)
					{	
						case XY:
							pDoc->vgl->ctx = ((float)point.x - m_rx);
							pDoc->vgl->cty = -((float)point.y - m_ry);	
							pDoc->vgl->ctz = 0.0f;
						break;

						case XZ:
							pDoc->vgl->ctx = ((float)point.x - m_rx);
							pDoc->vgl->cty = 0.0f;	
							pDoc->vgl->ctz = -((float)point.y - m_ry);
						break;
		
						case ZY:
							pDoc->vgl->ctx = 0.0f;
							pDoc->vgl->cty = -((float)point.y - m_ry);	
							pDoc->vgl->ctz = ((float)point.x - m_rx);
						break;
					}
				}

				else
				{
					pDoc->vgl->cry = ((float)point.x - m_rx);
					pDoc->vgl->crx = ((float)point.y - m_ry);	
				}

				m_rx = point.x;
				m_ry = point.y;
			}
		}
	}
	
	CView::OnMouseMove(nFlags, point);
}

void CVmdView::OnLButtonDown(UINT nFlags, CPoint point) 
// capturamos las coordenadas actuales del ratón
{
	CVmdDoc* pDoc = GetDocument();

	// obtenemos todos los privilegios sobre el ratón
	SetCapture();

	if(pDoc->vgl->i_num_modelos > 0)
	{
		m_rx = point.x;
		m_ry = point.y;
	}

	CView::OnLButtonDown(nFlags, point);
}

void CVmdView::OnLButtonUp(UINT nFlags, CPoint point) 
// reseteamos los valores al levantar el botón izq
{
	m_rx = -1;
	m_ry = -1;

	// liberamos los privilegios sobre el ratón
	ReleaseCapture();
	
	CView::OnLButtonUp(nFlags, point);
}
