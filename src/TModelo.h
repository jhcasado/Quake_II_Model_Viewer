/* ==============================================================================
	
	  Vmd: Visualizador de modelos del quake II
	  Versión: 1.0.0b

	  Autores: José Angel Montelongo Reyes 
			   Javier Honorio Casado Fernández

	  Universidad de Las Palmas de Gran Canaria	
	  	  
   ============================================================================== */


#ifndef __TMODELO_H_
#define __TMODELO_H_


/*
==================================================================

  Información estándar sobre los frames de los modelos .md2

==================================================================
*/
#define TODAS        0
#define STAND			1
#define RUN				2
#define ATTACK			3
#define PAIN1			4
#define PAIN2			5
#define PAIN3			6
#define JUMP			7
#define FLIP			8
#define SALUTE			9
#define TAUNT			10
#define WAVE			11
#define POINT			12
#define CRSTND			13
#define CRWALK			14
#define CRATTACK		15
#define CRPAIN			16
#define CRDEATH		17
#define DEATH1			18
#define DEATH2			19
#define DEATH3			20

extern char *nombre_animacion[DEATH3+1];

#define STAND_INI		0
#define STAND_FIN		39

#define RUN_INI		40
#define RUN_FIN		45

#define ATTACK_INI	46
#define ATTACK_FIN	53

#define PAIN1_INI		54
#define PAIN1_FIN		57

#define PAIN2_INI		58
#define PAIN2_FIN		61

#define PAIN3_INI		62
#define PAIN3_FIN		65

#define JUMP_INI		66
#define JUMP_FIN		71

#define FLIP_INI		72
#define FLIP_FIN		83

#define SALUTE_INI	84
#define SALUTE_FIN	94

#define TAUNT_INI		95
#define TAUNT_FIN		111

#define WAVE_INI		112
#define WAVE_FIN		122

#define POINT_INI		123
#define POINT_FIN		134

#define CRSTND_INI	135
#define CRSTND_FIN	153

#define CRWALK_INI	154
#define CRWALK_FIN   159

#define CRATTACK_INI	160
#define CRATTACK_FIN 168

#define CRPAIN_INI	169
#define CRPAIN_FIN	172

#define CRDEATH_INI	173
#define CRDEATH_FIN	177

#define DEATH1_INI	178
#define DEATH1_FIN	183

#define DEATH2_INI	184
#define DEATH2_FIN	189

#define DEATH3_INI	190
#define DEATH3_FIN	197



/*
==================================================================

	Modos de dibujar el modelo
		int Set_Modo_De_Dibujo (int m);
		int Get_Modo_De_Dibujo ();

==================================================================
*/
#define PUNTOS				0
#define LINEAS				1
#define FLAT				2
#define SMOOTH				3
#define TEXTURAS			4
#define LINEAS_TEXTURAS 5



/*
==================================================================

	Definir para que objeto se quiere cambiar el color
		int Set_Color_Activo (int c, TColor color);
		int Get_Color_Activo (int c, TColor *color);
		int Set_Color_Inactivo (int c, TColor color);
		int Get_Color_Inactivo (int c, TColor *color);

==================================================================
*/
#define COLOR_MODELO  0
#define COLOR_CAJA_EX 1
#define COLOR_CAJA_IN 2



/*
==================================================================

  Lados de las cajas (de uso interno

==================================================================
*/
//Dibujar_Caja
//v[0] = derecha
//v[1] = izquierda
//v[2] = arriba
//v[3] = abajo
//v[4] = delante
//v[5] = atras
#define DERECHA 0
#define IZQUIERDA 1
#define ARRIBA 2
#define ABAJO 3
#define DELANTE 4
#define DETRAS 5


/*
==================================================================

  Estructura de los .md2

==================================================================
*/
#define IDALIASHEADER	(('2'<<24)+('P'<<16)+('D'<<8)+'I')
#define ALIAS_VERSION	8
#define MAX_VERTICES		2048

typedef struct
{
	int id;
	int version;
	int skin_w;
	int skin_h;
	int tamano_frame;
	int num_texturas;
	int num_vertices;
	int num_coor_text;
	int num_triangulos;
	int num_comandos;
	int num_frames;
	int pos_texturas;
	int pos_coor_text;
	int pos_triangulos;
	int pos_frames;
	int pos_comandos;
	int pos_final;
} TCabecera;

typedef char TNombre_Textura[64];

typedef struct
{
	short x;
	short y;
} TCoor_Text;

typedef struct
{
	short indice_vert[3];
	short indice_text[3];
} TTriangulo;

typedef unsigned char byte;

typedef struct
{
	byte v[3];
	byte indice_normal;
} TVertice_Alias;

typedef struct
{
	float escalar[3];
	float transladar[3];
	char nombre[16];
	TVertice_Alias vertices[1];
} TFrame_Alias;

//Estas serán mis estructuras internas para almacenar los vértices de cada frame
typedef struct
{
	float v[3];
	float n[3];
} TVertice;

typedef struct
{
	TVertice *vertices;
	float caja[6];
	char nombre[16];
} TFrame;

typedef struct
{
	int num_vertices;
	int num_coor_text;
	int num_triangulos;
	int num_comandos;
	int num_frames;
	TFrame *frames;
	TTriangulo *triangulos;
	TCoor_Text *coor_text;
	int *comandos;
} TObjeto;

typedef struct
{
	int ancho,alto;
	int blind_id;
} TTextura;

typedef float TColor[4];



/*
==================================================================

  Objeto TMD2

==================================================================
*/
class TModelo
{
public:
	TModelo();
	~TModelo();

	int Leer_MD2(char *fichero);
	
	int Num_Objetos();
	int Num_Texturas();

	int Num_Frames(int objeto);
	int Num_Vertices(int objeto);
	int Num_Caras(int objeto);
	int Num_Gl_Commmand(int objeto);
	int Num_Coordenadas_Tex(int objeto);

	int Leer_Textura(char *fichero, int objeto);

	int Dibujar_Modelo ();

	void Trasladar (float x, float y, float z);
	void Rotar (float x, float y, float z);

	int Set_Animacion (int a);
	int Get_Animacion ();

	int Set_Frame (int f);
	int Get_Frame ();

	int Activar_Desactivar ();
	int Get_Activar_Desactivar ();


	int Play_Pause ();
	int Get_Play_Pause ();

	float Set_Coeft (float c);
	float Get_Coeft ();

	int Strips_and_Fans ();
	int Get_Strips_and_Fans ();

	int Set_Cuantas_Cajas (int a);
	int Get_Cuantas_Cajas ();

	int Set_Modo_De_Dibujo (int m);
	int Get_Modo_De_Dibujo ();

	int Set_Color_Activo (int c, TColor color);
	int Get_Color_Activo (int c, TColor color);

	int Set_Color_Inactivo (int c, TColor color);
	int Get_Color_Inactivo (int c, TColor color);

private:
	int Dibujar_Objeto (int objeto);
	int Dibujar_Objeto_FX (int objeto,float altura, int modo_ant,int modo_sig);
	int Dibujar_Objeto_Strips_And_Fans (int objeto);
	void Calcular_Frame_Siguiente ();
	void Calcular_T_Siguiente ();
	int Inicializa_Caja (float *caja);
	int Dibujar_Caja (float *v1, float *v2);
	int Comprobar_Caja (float *c, float *v);
	void Refrescar_Colores();

	
	static int ntexturas_totales;
	int nobjetos;
	TObjeto *objetos[5];
	TTextura texturas[5];
	float caja[DEATH3_FIN+1][6];
	float t;
	float coeft;
	float pos[3];
	float rot[3];
	TColor color_modelo, color_caja_ex, color_caja_in;
	TColor colores_activos[3], colores_inactivos[3];
	int animacion;
	int frame, framesig;
	int activo;
	int stop;
	int stripsandfans;
	int cuantas_cajas;
	int modo_de_dibujo;
	int modo_de_dibujo_ant;
	int fx;
	float coeffx;
	float altura_efecto, altura_efecto_sig;
	int primer_fx;float cc;
};


#endif