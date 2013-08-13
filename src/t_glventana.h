/* ==============================================================================
	
	  Vmd: Visualizador de modelos del quake II
	  Versión: 1.0.0b

	  Autores: José Angel Montelongo Reyes 
			   Javier Honorio Casado Fernández

	  Universidad de Las Palmas de Gran Canaria	
	  	  
   ============================================================================== */


// t_glventana.h


/*
==================================================================

  clase para tratar la ventana 

==================================================================
*/


#include "tmodelo.h"


#ifndef __T_GLVENTANA__
#define __T_GLVENTANA__


#define XY  0
#define XZ	1
#define ZY	2

#define MODELO	0
#define CAMARA	1
#define LUZ		2 


class T_CAMARA
{
	public:
	float f_fovy,
		  f_aspect_x,  
		  f_aspect_y;
	float f_vt, f_vr,
		  px, py, pz, // pos de la camara
	      cx, cy, cz; // a donde miramos

	T_CAMARA();
};


class T_LUZ
{
	public:
	float px, py, pz, // posición
	r, g, b, a, // color 
    f_vlt; // velocidad de traslación 
	BOOL visible;

	T_LUZ::T_LUZ(); // constructor de la clase
};


class T_GLVENTANA
{
	public:

	float f_zNear,   
		  f_zFar;
	DWORD e_calidad;	
	int i_num_modelos, i_max_num_modelos, i_elto_actual;
	TModelo **l_modelos; // lista con los modelos
	float f_cfondo_r, f_cfondo_g, f_cfondo_b, f_cfondo_a; // color de fondo de la ventana
	int *p_larmas, *p_lmodelos;	
	int i_plano, i_objeto;
	float f_vt, f_vr;
	T_CAMARA camara;
	T_LUZ luz;

	float ctx, cty, ctz, crx, cry;
	int i_vancho, i_valto;

	T_GLVENTANA();
	// contructor de la clase

	~T_GLVENTANA();
	// destructor de la clase

	void V_Inicializar_Listas(int max_num_modelos);
	// crea la ventana

	void V_Inicializar_OpenGL();
	// inicia las propiedades de OpenGL

	void V_Escalar_Ventana(int width, int height);
	// función que gestiona el escalado de la ventana

	void V_Dibujar_Escena();
	// función que dibuja los modelos

};


#endif


// t_glventana.h
