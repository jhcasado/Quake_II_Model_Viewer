/* ==============================================================================
	
	  Vmd: Visualizador de modelos del quake II
	  Versión: 1.0.0b

	  Autores: José Angel Montelongo Reyes 
			   Javier Honorio Casado Fernández

	  Universidad de Las Palmas de Gran Canaria	
	  	  
   ============================================================================== */


// t_glventana.cpp


/*
==================================================================

  clase para tratar la ventana con los modelos

==================================================================
*/


// ficheros de inclusión

#include "stdafx.h"

#include "tmodelo.h"
#include "t_glventana.h"


T_CAMARA::T_CAMARA()
// constructor de la clase
{
	f_fovy = 60.0f; // propiedades 
	f_aspect_x = 1.0f;  
	f_aspect_y = 1.0f;

	f_vt = 0.5f; // velocidad de traslación
	f_vr = 0.5f; // velocidad de rotación

	px = py = 0.0f; // pos de la camara
	pz = 100.0f; 
	cx = cy = cz = 0.0f; // a donde miramos
}

T_LUZ::T_LUZ()
// constructor de la luz
{
	px = 0.0f; // pos de la luz
	py = 30.0f;
	pz = 40.0f;
	
	r = g = b = a = 1.0f, // color de la luz

	f_vlt = 1.0f; // velocidad de traslación para la luz
	visible = TRUE;
}

T_GLVENTANA::T_GLVENTANA()
// contructor de la clase
{
	f_zNear = 1.0f; // plano de recorte   
	f_zFar = 500.0;

	e_calidad = GL_NICEST; // calidad del render	
	
	i_num_modelos = 0; // listas de modelos
	i_max_num_modelos = 0;
	l_modelos = NULL; 	
	p_larmas = NULL;
	p_lmodelos = NULL;
	i_elto_actual = -1;

	f_cfondo_r = 0.0f; // color de fondo 
	f_cfondo_g = 0.2f;
	f_cfondo_b = 0.5f;
	f_cfondo_a = 1.0f;
	
	i_plano = XY;

	i_objeto = MODELO;

	f_vt = 0.1f; // velocidad de traslación
	f_vr = 1.0f; // velocidad de rotación

	i_vancho = -1;
	i_valto = -1;

	ctx = 0.0f; // variables para las transformaciones
	cty = 0.0f; 
	ctz = 0.0f;
	crx = 0.0f;
	cry = 0.0f;
}
	

T_GLVENTANA::~T_GLVENTANA()
// destructor de la clase
{
	if (l_modelos != NULL)
	{
		for (int c = 0; c < i_num_modelos; c ++)
		{
			if (l_modelos [c] != NULL)
			{
				delete l_modelos [c];
			}
		}

		delete l_modelos;
	}

	if (p_lmodelos != NULL)
	{
		delete p_lmodelos;
	}

	if (p_larmas != NULL)
	{
		delete p_larmas;
	}	
}


void T_GLVENTANA::V_Inicializar_Listas(int max_num_modelos)
// crea la ventana
{
	// inicializamos el vector de modelos
	i_max_num_modelos = max_num_modelos;

	l_modelos = new TModelo* [max_num_modelos];

	for (int c = 0; c < max_num_modelos; c ++)
	{
		l_modelos [c] = NULL;
	}

	p_lmodelos = new int [max_num_modelos];
	p_larmas = new int [max_num_modelos];

	for (int c = 0; c < max_num_modelos; c ++)
	{
		p_lmodelos [c] = -1;
		p_larmas [c] = -1;
	}
}


void T_GLVENTANA::V_Inicializar_OpenGL()
// inicia las propiedades de OpenGL
{
	// z-buffer
	glDepthFunc(GL_LEQUAL);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);	
	
	// calidad del render
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, e_calidad);

	// no trabajar con las caras ocultas
	glEnable(GL_CULL_FACE);
	
	// incializamos las matrices
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// luces
	glEnable(GL_LIGHTING);

	GLfloat lp [4];
	
	lp [0] = luz.px;
	lp [1] = luz.py;
	lp [2] = luz.pz;
	lp [3] = 1.0f;

	GLfloat la [4] = { 0.25f, 0.25f, 0.25f, 1.0f };
	GLfloat ld [4] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat ls [4] = { 0.0f, 0.0f, 0.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_POSITION, lp);
	glLightfv(GL_LIGHT0, GL_AMBIENT , la);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, ld);
	glLightfv(GL_LIGHT0, GL_SPECULAR, ls);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);

	glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 0);

	glEnable(GL_LIGHT0);

	// fuente 
	glfInit();

	glfLoadFont("font\\Arial1.glf");
}


void T_GLVENTANA::V_Escalar_Ventana(int width, int height)
// función que gestiona el escalado de la ventana
{
	float f_aspect;

	if (height == 0)
	{
		height = 1;
	}

	if (camara.f_aspect_y == 0.0f)
	{
		camara.f_aspect_y = 0.0001f;
	}

	f_aspect = (width * camara.f_aspect_x) / (height * camara.f_aspect_y);		

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(camara.f_fovy, f_aspect, f_zNear, f_zFar);
	
	// camara
	gluLookAt(camara.px, camara.py, camara.pz, 
			  camara.cx, camara.cy, camara.cz, 
			  0.0f, 1.0f, 0.0f);
	
	i_vancho = width;
	i_valto = height;
}


void T_GLVENTANA::V_Dibujar_Escena()
// función que dibuja los modelos
{
	// actualizamos las coordenadas de los objetos
	switch(i_objeto)
	{
		case MODELO:
			if (i_num_modelos > 0)
			{
				l_modelos [i_elto_actual]->Rotar(crx * f_vr, cry * f_vr, 0.0f);
				l_modelos [i_elto_actual]->Trasladar(ctx * f_vt, cty * f_vt, ctz * f_vt);
			}
		break;

		case CAMARA:						
			// cambiamos la camara y el centro de visión
			camara.px += -(ctx * camara.f_vt);
			camara.py += -(cty * camara.f_vt);
			camara.pz += -(ctz * camara.f_vt);

			camara.cx += -(ctx * camara.f_vt);
			camara.cy += -(cty * camara.f_vt);
			camara.cz += -(ctz * camara.f_vt);

			// cambiamos el centro de visión
			camara.cx += -(cry * camara.f_vt);
			camara.cy += (crx * camara.f_vt);

			V_Escalar_Ventana(i_vancho, i_valto);
		break;

		case LUZ:
			float lp [4];

			luz.px += (ctx * luz.f_vlt);
			luz.py += (cty * luz.f_vlt);
			luz.pz += (ctz * luz.f_vlt);

			lp [0] = luz.px;
			lp [1] = luz.py;
			lp [2] = luz.pz;
			lp [3] = 1.0f;

			glLightfv(GL_LIGHT0, GL_POSITION, lp);				
		break;
	}

	// dibujamos los modelos

	glClearColor(f_cfondo_r, f_cfondo_g, f_cfondo_b, f_cfondo_a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	for (int c = 0; c < i_num_modelos; c ++)
	{		
		if (l_modelos [c] != NULL) 
		{
			l_modelos [c]->Dibujar_Modelo();
		}
	}

	ctx = 0.0f;
	cty = 0.0f; 
	ctz = 0.0f;
	crx = 0.0f;
	cry = 0.0f;
}


// t_glventana.cpp
