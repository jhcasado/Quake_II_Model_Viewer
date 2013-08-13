/* ==============================================================================
	
	  Vmd: Visualizador de modelos del quake II
	  Versión: 1.0.0b

	  Autores: José Angel Montelongo Reyes 
			   Javier Honorio Casado Fernández

	  Universidad de Las Palmas de Gran Canaria	
	  	  
   ============================================================================== */

#include "stdafx.h"

#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "TModelo.h"

extern char *nombre_animacion[DEATH3+1]={
"TODAS",
"STAND",
"RUN",
"ATTACK",
"PAIN1",
"PAIN2",
"PAIN3",
"JUMP",
"FLIP",
"SALUTE",
"TAUNT",
"WAVE",
"POINT",
"CRSTND",
"CRWALK",
"CRATTACK",
"CRPAIN",
"CRDEATH",
"DEATH1",
"DEATH2",
"DEATH3"
};

int frame_ini_fin[21][2] =
{
	{STAND_INI, DEATH3_FIN},
	{STAND_INI, STAND_FIN},
	{RUN_INI, RUN_FIN},
	{ATTACK_INI, ATTACK_FIN},
	{PAIN1_INI, PAIN1_FIN},
	{PAIN2_INI, PAIN2_FIN},
	{PAIN3_INI, PAIN3_FIN},
	{JUMP_INI, JUMP_FIN},
	{FLIP_INI, FLIP_FIN},
	{SALUTE_INI, SALUTE_FIN},
	{TAUNT_INI, TAUNT_FIN},
	{WAVE_INI, WAVE_FIN},
	{POINT_INI, POINT_FIN},
	{CRSTND_INI, CRSTND_FIN},
	{CRWALK_INI, CRWALK_FIN},
	{CRATTACK_INI, CRATTACK_FIN},
	{CRPAIN_INI, CRPAIN_FIN},
	{CRDEATH_INI, CRDEATH_FIN},
	{DEATH1_INI, DEATH1_FIN},
	{DEATH2_INI, DEATH2_FIN},
	{DEATH3_INI, DEATH3_FIN},
};

#define NUM_NORMALES 162

float normales[NUM_NORMALES][3] = {
#include "anorms.h"
};

int TModelo::ntexturas_totales = 0;
///////////////////////////////////////////////////////////////////////////////////////

typedef struct
{
    char	manufacturer;
    char	version;
    char	encoding;
    char	bits_per_pixel;
    unsigned short	xmin,ymin,xmax,ymax;
    unsigned short	hres,vres;
    unsigned char	palette[48];
    char	reserved;
    char	color_planes;
    unsigned short	bytes_per_line;
    unsigned short	palette_type;
    char	filler[58];
    unsigned char	data;			// unbounded
} pcx_t;

int FileLength (FILE *f)
{
	int		pos;
	int		end;

	pos = ftell (f);
	fseek (f, 0, SEEK_END);
	end = ftell (f);
	fseek (f, pos, SEEK_SET);

	return end;
}

int LoadFile (char *filename, void **bufferptr)
{
	FILE *f;
	int length = -1;
	void *buffer = NULL;

	fopen_s(&f, filename, "rb");

	if (f != NULL)
	{
		length = FileLength(f);
		buffer = malloc(length+1);
		((char *)buffer)[length] = 0;
		fread(buffer, 1, length, f);
		fclose(f);

		*bufferptr = buffer;
	}

	return length;
}

void GetData (char *filename, byte **pic, byte **palette, int *width, int *height)
{
	byte	*raw;
	pcx_t	*pcx;
	int		x, y;
	int		len;
	int		dataByte, runLength;
	byte	*out, *pix;

	if (pic)
		*pic = NULL;
	if (palette)
		*palette = NULL;
	if (width)
		*width = 0;
	if (height)
		*height = 0;

	//
	// load the file
	//
	len = LoadFile (filename, (void **)&raw);
	if (len == -1)
		return;

	//
	// parse the PCX file
	//
	pcx = (pcx_t *)raw;
	raw = &pcx->data;

	if (pcx->manufacturer != 0x0a
		|| pcx->version != 5
		|| pcx->encoding != 1
		|| pcx->bits_per_pixel != 8
		|| pcx->xmax >= 640
		|| pcx->ymax >= 480)
		{ printf ("Bad pcx file %s", filename); return; }
	
	if (palette)
	{
		*palette = (byte *)malloc(768);
		memcpy (*palette, (byte *)pcx + len - 768, 768);
	}

	if (width)
		*width = pcx->xmax+1;
	if (height)
		*height = pcx->ymax+1;

	if (!pic)
		return;

	out = (byte *)malloc ( (pcx->ymax+1) * (pcx->xmax+1) );
	if (!out)
		{ printf ("Skin_Cache: couldn't allocate"); return; }

	*pic = out;

	pix = out;

	for (y=0 ; y<=pcx->ymax ; y++, pix += pcx->xmax+1)
	{
		for (x=0 ; x<=pcx->xmax ; )
		{
			dataByte = *raw++;

			if((dataByte & 0xC0) == 0xC0)
			{
				runLength = dataByte & 0x3F;
				dataByte = *raw++;
			}
			else
				runLength = 1;

			while(runLength-- > 0)
				pix[x++] = dataByte;
		}

	}

	if ( raw - (byte *)pcx > len)
		{ printf ("PCX file %s was malformed", filename); return; }

	free (pcx);
}

int loadPCX (char *name, int id, int *ancho, int *alto)
{
	byte *buf,*pal, *data;
	int x, w, h;

	GetData(name, &buf, &pal, &w, &h);

	if (buf == NULL || pal == NULL)
		return -1;

	if ( (data = (byte *) malloc((w+1)*(h+1)* 3)) == NULL)
		return -1;

	//convert to rgb format
	for (x=0; x<(w*h); x++)
	{
		data[x * 3] = pal[buf[x] * 3 ];
		data[x * 3 + 1] = pal[buf[x] * 3 + 1];
		data[x * 3 + 2] = pal[buf[x] * 3 + 2];
	}

	glBindTexture (GL_TEXTURE_2D, id);
	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	free(buf);
	free(pal);
	free(data);

	*ancho = w;
	*alto = h;

	return 1;
}
/////////////////////////////////////////////////////////////////////////////////////////

TModelo::TModelo()
{
	int i;

	frame = 0;
	framesig = 1;
	animacion = 0;

	t = 0.0f;
	coeft = 0.02f;

	for (i = 0; i < 3; i++)
	{
		pos[i] = 0.0f;
		rot[i] = 0.0f;
	}

	for (i = 0; i < 5; i++)
	{
		objetos[i] = NULL;
		texturas[i].blind_id = -1;
	}

	for (i = 0; i < DEATH3_FIN+1; i++)
		Inicializa_Caja(caja[i]);

	for (i = 0; i < 4; i++)
	{
		color_modelo[i]  = 1.0f;
		color_caja_ex[i] = 1.0f;
		color_caja_in[i] = 1.0f;
	}
	color_caja_ex[2] = 0.0f;

	for (int j = 0; j < 3; j++)
		for (i = 0; i < 4; i++)
		{
			colores_activos[j][i]   = 1.0f;
			colores_inactivos[j][i] = 0.75f;
		}
	colores_activos[1][2] = 0.0f;

	nobjetos = 0;
	activo = 1;
	stop = 0;
	stripsandfans = 1;
	cuantas_cajas = 1;
	modo_de_dibujo = LINEAS;

	modo_de_dibujo_ant = -1;
	fx = 3;
	coeffx = -1;
	primer_fx = 1; cc = 0.0f;
}

TModelo::~TModelo()
{
	for (int i = 0; i < nobjetos; i++)
	{
		if (objetos [i] != NULL) delete objetos [i];
		objetos [i] = NULL;
	}
}


int TModelo::Num_Objetos()
// número de objetos
{
	return nobjetos;
}


int TModelo::Num_Texturas()
// número de texturas actuales
{
	return ntexturas_totales;
}


int TModelo::Num_Frames(int objeto)
// número de frames
{
	return objetos [objeto]->num_frames;
}


int TModelo::Num_Vertices(int objeto)
// número de vertices
{
	return objetos [objeto]->num_vertices;
}


int TModelo::Num_Caras(int objeto)
// número de caras
{
	return objetos [objeto]->num_triangulos;
}


int TModelo::Num_Gl_Commmand(int objeto)
// número de gl command
{
	return objetos [objeto]->num_comandos;
}


int TModelo::Num_Coordenadas_Tex(int objeto)
// número de coordenadas de textura
{
	return objetos [objeto]->num_coor_text;
}


int TModelo::Leer_MD2(char *fichero)
{
	TCabecera cab;
	TCoor_Text *coor_text;
	TTriangulo *triangulos;
	TFrame *frames;
	int *comandos;

	FILE *md2;
	int i;
	byte buffer[MAX_VERTICES*4+128];
	TFrame_Alias *frame_temp;

	fopen_s(&md2 , fichero, "rb");

	if (!md2)
		return -1;

	//lee la cabecera
	fread (&cab, sizeof(TCabecera),1,md2);
	if ((cab.id != IDALIASHEADER) || (cab.version != ALIAS_VERSION))
		return -1;

/* 
Esta parte es irrelevante ya que siempre está en desuso
*/
/*	//lee los nombres de las diferentes texturas
	if (cab.num_texturas > 0)
	{
		texturas = (TNombre_Textura *) malloc (sizeof(TNombre_Textura) * cab.num_texturas);
		if (!texturas)
			return -1;
		fseek(md2,cab.pos_texturas,SEEK_SET);
		for (i = 0; i < cab.num_texturas; i++)
			fread (&texturas[i], sizeof(TNombre_Textura), 1, md2);
	}
*/
	//lee las coordenadas de las texturas
	if (cab.num_coor_text > 0)
	{
		coor_text = (TCoor_Text *) malloc (sizeof(TCoor_Text) * cab.num_coor_text);
		if (!coor_text)
			return -1;
		fseek(md2,cab.pos_coor_text,SEEK_SET);
		for (i = 0; i < cab.num_coor_text; i++)
			fread (&coor_text[i], sizeof(TCoor_Text), 1, md2);
	}

	//lee los triangulos
	if (cab.num_triangulos > 0)
	{
		triangulos = (TTriangulo *) malloc (sizeof(TTriangulo) * cab.num_triangulos);
		if (!triangulos)
		{
			free (coor_text);
			return -1;
		}
		fseek(md2,cab.pos_triangulos,SEEK_SET);
		for (i = 0; i < cab.num_triangulos; i++)
			fread (&triangulos[i], sizeof(TTriangulo), 1, md2);
	}

	//lee los frames
	if (cab.num_frames > 0)
	{
		frames = (TFrame *) malloc (sizeof(TFrame) * cab.num_frames);
		if (!frames)
		{
			free (coor_text);
			free (triangulos);
			return -1;
		}
		frame_temp = (TFrame_Alias *) buffer;
		fseek(md2,cab.pos_frames,SEEK_SET);
		for (i = 0; i < cab.num_frames; i++)
		{
			fread (frame_temp, 1, cab.tamano_frame, md2);
			strcpy_s(frames[i].nombre, frame_temp->nombre);
			frames[i].vertices = (TVertice *) malloc (sizeof(TVertice) * cab.num_vertices);
			if (!frames[i].vertices)
			{
				free (coor_text);
				free (triangulos);
				free (frames);
				return -1;
			}

			Inicializa_Caja (frames[i].caja);

			for (int j = 0; j < cab.num_vertices; j++)
			{
				frames[i].vertices[j].v[0] = (frame_temp->vertices[j].v[0] * frame_temp->escalar[0]) + frame_temp->transladar[0];
				frames[i].vertices[j].v[1] = (frame_temp->vertices[j].v[2] * frame_temp->escalar[2]) + frame_temp->transladar[2];
				frames[i].vertices[j].v[2] = (frame_temp->vertices[j].v[1] * frame_temp->escalar[1]) + frame_temp->transladar[1];

				frames[i].vertices[j].n[0] = normales[frame_temp->vertices[j].indice_normal][0];
				frames[i].vertices[j].n[1] = normales[frame_temp->vertices[j].indice_normal][2];
				frames[i].vertices[j].n[2] = normales[frame_temp->vertices[j].indice_normal][1];

				Comprobar_Caja(frames[i].caja, frames[i].vertices[j].v);
				Comprobar_Caja(caja[i], frames[i].vertices[j].v);
			}
		}
	}

	//lee los comandos para los strips/fans
	if (cab.num_comandos > 0)
	{
		comandos = (int *) malloc (sizeof(int) * cab.num_comandos);
		if (!comandos)
			cab.num_comandos = 0;
		else
		{
			fseek(md2,cab.pos_comandos,SEEK_SET);
			if (cab.num_comandos != (int)fread(comandos,sizeof(int),cab.num_comandos,md2))
				cab.num_comandos = 0;
		}
	}

	fclose(md2);

	objetos[nobjetos] = new TObjeto;

	objetos[nobjetos]->num_vertices = cab.num_vertices;
	objetos[nobjetos]->num_coor_text = cab.num_coor_text;
	objetos[nobjetos]->num_triangulos = cab.num_triangulos;
	objetos[nobjetos]->num_frames = cab.num_frames;
	objetos[nobjetos]->num_comandos = cab.num_comandos;
	texturas[nobjetos].blind_id = -1;

	objetos[nobjetos]->coor_text = coor_text;
	objetos[nobjetos]->triangulos = triangulos;
	objetos[nobjetos]->frames = frames;
	if (cab.num_comandos > 0)
		objetos[nobjetos]->comandos = comandos;
	else
		objetos[nobjetos]->comandos = NULL;

	altura_efecto = (float)caja[frame][fx];
	altura_efecto_sig = altura_efecto + ((caja[framesig][fx-1]-caja[frame/*sig*/][fx]) / 2.0f);

	return nobjetos++;
}

int TModelo::Leer_Textura(char *fichero, int objeto)
{
	if (loadPCX(fichero, ntexturas_totales, &texturas[objeto].ancho, &texturas[objeto].alto) == -1)
		return 0;

	texturas[objeto].blind_id = ntexturas_totales;
	modo_de_dibujo = TEXTURAS;

	ntexturas_totales ++;

	return ntexturas_totales;
}

int TModelo::Dibujar_Objeto (int objeto)
{
	TFrame *ant, *sig;
	TTriangulo *tri;
	TCoor_Text *ct;
	TTextura *tex;
//	GLboolean t2d;

	if  (objetos [objeto] == NULL)
	{
		return 1;
	}

	if (frame > objetos[objeto]->num_frames - 1)
		return 0;
	if (framesig > objetos[objeto]->num_frames - 1)
		return 0;

	ant = &objetos[objeto]->frames[frame];
	sig = &objetos[objeto]->frames[framesig];
	tri = objetos[objeto]->triangulos;
	ct  = objetos[objeto]->coor_text;
	tex = &texturas[objeto];
/*
	glGetBooleanv(GL_TEXTURE_2D, &t2d);
	if ((texturas[objeto].blind_id >= 0) && (t2d))
		glBindTexture(GL_TEXTURE_2D, texturas[objeto].blind_id);
	else
	{
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glShadeModel(GL_SMOOTH);
	}
*/
	glBegin(GL_TRIANGLES);
		for (int i = 0; i < objetos[objeto]->num_triangulos; i++)
		{
			for (int j = 0; j < 3; j++)
			{
			glTexCoord2f(
				(float) ct[tri[i].indice_text[j]].x / (float) tex->ancho,
				(float) ct[tri[i].indice_text[j]].y / (float) tex->alto
				);

			glNormal3f(
				ant->vertices[tri[i].indice_vert[j]].n[0] + (t * (sig->vertices[tri[i].indice_vert[j]].n[0] - ant->vertices[tri[i].indice_vert[j]].n[0])),
				ant->vertices[tri[i].indice_vert[j]].n[1] + (t * (sig->vertices[tri[i].indice_vert[j]].n[1] - ant->vertices[tri[i].indice_vert[j]].n[1])),
				ant->vertices[tri[i].indice_vert[j]].n[2] + (t * (sig->vertices[tri[i].indice_vert[j]].n[2] - ant->vertices[tri[i].indice_vert[j]].n[2]))
				);

			glVertex3f(
				ant->vertices[tri[i].indice_vert[j]].v[0] + (t * (sig->vertices[tri[i].indice_vert[j]].v[0] - ant->vertices[tri[i].indice_vert[j]].v[0])),
				ant->vertices[tri[i].indice_vert[j]].v[1] + (t * (sig->vertices[tri[i].indice_vert[j]].v[1] - ant->vertices[tri[i].indice_vert[j]].v[1])),
				ant->vertices[tri[i].indice_vert[j]].v[2] + (t * (sig->vertices[tri[i].indice_vert[j]].v[2] - ant->vertices[tri[i].indice_vert[j]].v[2]))
				);
			}
		}
	glEnd();

	if (cuantas_cajas > 1)
	{
		glLineWidth(1.0f);
		glColor4fv(color_caja_in);
		Dibujar_Caja(ant->caja, sig->caja);
		glColor4fv(color_modelo);
	}

	return 1;
}


int TModelo::Dibujar_Objeto_FX (int objeto,float altura, int modo_ant,int modo_sig)
{
	TFrame *ant, *sig;
	TTriangulo *tri;
	TCoor_Text *ct;
	TTextura *tex;
	GLboolean t2d;
	int ffx = fx / 2;

	if  (objetos [objeto] == NULL)
	{
		return 1;
	}

	if (frame > objetos[objeto]->num_frames - 1)
		return 0;
	if (framesig > objetos[objeto]->num_frames - 1)
		return 0;

	ant = &objetos[objeto]->frames[frame];
	sig = &objetos[objeto]->frames[framesig];
	tri = objetos[objeto]->triangulos;
	ct  = objetos[objeto]->coor_text;
	tex = &texturas[objeto];

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	for (int i = 0; i < objetos[objeto]->num_triangulos; i++)
	{
		int c = 3;
		TColor color_vertice[3];
		float v[3][3] ={
			{ant->vertices[tri[i].indice_vert[0]].v[0] + (t * (sig->vertices[tri[i].indice_vert[0]].v[0] - ant->vertices[tri[i].indice_vert[0]].v[0])),
			ant->vertices[tri[i].indice_vert[0]].v[1] + (t * (sig->vertices[tri[i].indice_vert[0]].v[1] - ant->vertices[tri[i].indice_vert[0]].v[1])),
			ant->vertices[tri[i].indice_vert[0]].v[2] + (t * (sig->vertices[tri[i].indice_vert[0]].v[2] - ant->vertices[tri[i].indice_vert[0]].v[2]))},
			{ant->vertices[tri[i].indice_vert[1]].v[0] + (t * (sig->vertices[tri[i].indice_vert[1]].v[0] - ant->vertices[tri[i].indice_vert[1]].v[0])),
			ant->vertices[tri[i].indice_vert[1]].v[1] + (t * (sig->vertices[tri[i].indice_vert[1]].v[1] - ant->vertices[tri[i].indice_vert[1]].v[1])),
			ant->vertices[tri[i].indice_vert[1]].v[2] + (t * (sig->vertices[tri[i].indice_vert[1]].v[2] - ant->vertices[tri[i].indice_vert[1]].v[2]))},
			{ant->vertices[tri[i].indice_vert[2]].v[0] + (t * (sig->vertices[tri[i].indice_vert[2]].v[0] - ant->vertices[tri[i].indice_vert[2]].v[0])),
			ant->vertices[tri[i].indice_vert[2]].v[1] + (t * (sig->vertices[tri[i].indice_vert[2]].v[1] - ant->vertices[tri[i].indice_vert[2]].v[1])),
			ant->vertices[tri[i].indice_vert[2]].v[2] + (t * (sig->vertices[tri[i].indice_vert[2]].v[2] - ant->vertices[tri[i].indice_vert[2]].v[2]))}
			};

		if (v[0][ffx] > altura) c--;
		if (v[1][ffx] > altura) c--;
		if (v[2][ffx] > altura) c--;

		if ((c<3) && (modo_ant >= 0))
		{
			switch (modo_ant)
			{
				case PUNTOS:
					glDisable(GL_TEXTURE_2D);
					glDisable(GL_CULL_FACE);
					glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
					glShadeModel(GL_SMOOTH);
					break;
				case LINEAS:
					glDisable(GL_TEXTURE_2D);
					glDisable(GL_CULL_FACE);
					glLineWidth(1.0f);
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					glShadeModel(GL_SMOOTH);
					break;
				case FLAT:
					glDisable(GL_TEXTURE_2D);
					glEnable(GL_CULL_FACE);
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					glShadeModel(GL_FLAT);
					break;
				case SMOOTH:
					glDisable(GL_TEXTURE_2D);
					glEnable(GL_CULL_FACE);
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					glShadeModel(GL_SMOOTH);
					break;
				case TEXTURAS:
					glEnable(GL_TEXTURE_2D);
					glEnable(GL_CULL_FACE);
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					glShadeModel(GL_SMOOTH);
					glGetBooleanv(GL_TEXTURE_2D, &t2d);
					if ((texturas[objeto].blind_id >= 0) && (t2d))
						glBindTexture(GL_TEXTURE_2D, texturas[objeto].blind_id);
					else
					{
						glDisable(GL_TEXTURE_2D);
						glEnable(GL_CULL_FACE);
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
						glShadeModel(GL_SMOOTH);
					}
					break;
				default:
					break;
			}

			for (int u = 0; u < 3; u++)
				for (int v = 0; v < 4; v++)
					color_vertice[u][v] = color_modelo[v];

			glBegin(GL_TRIANGLES);
				for (int j = 0; j < 3; j++)
				{
					float dif = v[j][ffx] - altura;

					if (dif < 0)
						color_vertice[j][3] = 0.0f;
					else
						color_vertice[j][3] = (dif / 10.0f);

					if ((modo_ant != TEXTURAS) || (texturas[objeto].blind_id < 0))
					{
						glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE ,color_vertice[j]);
						glColor4fv(color_vertice[j]);
					}
					else 
					{
						TColor ctex = {1.0f,1.0f,1.0f,color_vertice[j][3]};
						glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE ,ctex);
						glColor4fv (ctex);
					}

					glTexCoord2f(
						(float) ct[tri[i].indice_text[j]].x / (float) tex->ancho,
						(float) ct[tri[i].indice_text[j]].y / (float) tex->alto
						);
					glNormal3f(
						ant->vertices[tri[i].indice_vert[j]].n[0] + (t * (sig->vertices[tri[i].indice_vert[j]].n[0] - ant->vertices[tri[i].indice_vert[j]].n[0])),
						ant->vertices[tri[i].indice_vert[j]].n[1] + (t * (sig->vertices[tri[i].indice_vert[j]].n[1] - ant->vertices[tri[i].indice_vert[j]].n[1])),
						ant->vertices[tri[i].indice_vert[j]].n[2] + (t * (sig->vertices[tri[i].indice_vert[j]].n[2] - ant->vertices[tri[i].indice_vert[j]].n[2]))
						);
					glVertex3f(v[j][0], v[j][1], v[j][2]);
				}
			glEnd();			
		}

		if ((c > 0) && (modo_sig >= 0))
		{
			switch (modo_sig)
			{
				case PUNTOS:
					glDisable(GL_TEXTURE_2D);
					glDisable(GL_CULL_FACE);
					glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
					glShadeModel(GL_SMOOTH);
					break;
				case LINEAS:
					glDisable(GL_TEXTURE_2D);
					glDisable(GL_CULL_FACE);
					glLineWidth(1.0f);
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					glShadeModel(GL_SMOOTH);
					break;
				case FLAT:
					glDisable(GL_TEXTURE_2D);
					glEnable(GL_CULL_FACE);
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					glShadeModel(GL_FLAT);
					break;
				case SMOOTH:
					glDisable(GL_TEXTURE_2D);
					glEnable(GL_CULL_FACE);
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					glShadeModel(GL_SMOOTH);
					break;
				case TEXTURAS:
					glEnable(GL_TEXTURE_2D);
					glEnable(GL_CULL_FACE);
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					glShadeModel(GL_SMOOTH);
					glGetBooleanv(GL_TEXTURE_2D, &t2d);
					if ((texturas[objeto].blind_id >= 0) && (t2d))
						glBindTexture(GL_TEXTURE_2D, texturas[objeto].blind_id);
					else
					{
						glDisable(GL_TEXTURE_2D);
						glEnable(GL_CULL_FACE);
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
						glShadeModel(GL_SMOOTH);
					}
					break;
				default:
				break;
			}

			for (int u = 0; u < 3; u++)
				for (int v = 0; v < 4; v++)
					color_vertice[u][v] = color_modelo[v];

			glBegin(GL_TRIANGLES);
				for (int j = 0; j < 3; j++)
				{
					float dif = v[j][ffx] - altura;
					if (dif > 0)
						color_vertice[j][3] = 0.0f;
					else
						color_vertice[j][3] = -(dif / 5.0f);

					if ((modo_sig != TEXTURAS) || (texturas[objeto].blind_id < 0))
					{
						glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE ,color_vertice[j]);
						glColor4fv(color_vertice[j]);
					}
					else 
					{
						TColor ctex = {1.0f,1.0f,1.0f,color_vertice[j][3]};
						glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE ,ctex);
						glColor4fv (ctex);
					}

					glTexCoord2f(
						(float) ct[tri[i].indice_text[j]].x / (float) tex->ancho,
						(float) ct[tri[i].indice_text[j]].y / (float) tex->alto
						);
					glNormal3f(
						ant->vertices[tri[i].indice_vert[j]].n[0] + (t * (sig->vertices[tri[i].indice_vert[j]].n[0] - ant->vertices[tri[i].indice_vert[j]].n[0])),
						ant->vertices[tri[i].indice_vert[j]].n[1] + (t * (sig->vertices[tri[i].indice_vert[j]].n[1] - ant->vertices[tri[i].indice_vert[j]].n[1])),
						ant->vertices[tri[i].indice_vert[j]].n[2] + (t * (sig->vertices[tri[i].indice_vert[j]].n[2] - ant->vertices[tri[i].indice_vert[j]].n[2]))
						);
					glVertex3f(v[j][0], v[j][1], v[j][2]);
				}
			glEnd();
		}
	}

	glDisable(GL_BLEND);

	if (cuantas_cajas > 1)
	{
		glLineWidth(1.0f);
		glColor4fv(color_caja_in);
		Dibujar_Caja(ant->caja, sig->caja);
		glColor4fv(color_modelo);
	}

	return 1;
}

int TModelo::Dibujar_Objeto_Strips_And_Fans (int objeto)
{
	TFrame *ant, *sig;
	int *comandos;
	int c, v;
	float x, y;
//	GLboolean t2d;
	
	if  (objetos [objeto] == NULL)
	{
		return 1;
	}

	if (frame > objetos[objeto]->num_frames - 1)
		return 0;
	if (framesig > objetos[objeto]->num_frames - 1)
		return 0;

	ant = &objetos[objeto]->frames[frame];
	sig = &objetos[objeto]->frames[framesig];
	comandos = objetos[objeto]->comandos;
/*
	glGetBooleanv(GL_TEXTURE_2D, &t2d);
	if ((texturas[objeto].blind_id >= 0) && (t2d))
		glBindTexture(GL_TEXTURE_2D, texturas[objeto].blind_id);
	else
	{
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glShadeModel(GL_SMOOTH);
	}
*/
	while (c = *comandos++)
	{
		if (c > 0)
			glBegin(GL_TRIANGLE_STRIP);
		else
		{
			glBegin(GL_TRIANGLE_FAN);
			c = -c;
		}
		while (c--)
		{
			x = *(float *) (comandos++);
			y = *(float *) (comandos++);
			v = *(comandos++);

			glTexCoord2f(x, y);
			glNormal3f(
				ant->vertices[v].n[0] + (t * (sig->vertices[v].n[0] - ant->vertices[v].n[0])),
				ant->vertices[v].n[1] + (t * (sig->vertices[v].n[1] - ant->vertices[v].n[1])),
				ant->vertices[v].n[2] + (t * (sig->vertices[v].n[2] - ant->vertices[v].n[2]))
				);
			glVertex3f(
				ant->vertices[v].v[0] + (t * (sig->vertices[v].v[0] - ant->vertices[v].v[0])),
				ant->vertices[v].v[1] + (t * (sig->vertices[v].v[1] - ant->vertices[v].v[1])),
				ant->vertices[v].v[2] + (t * (sig->vertices[v].v[2] - ant->vertices[v].v[2]))
				);
		}
		glEnd();
	}

	if (cuantas_cajas > 1)
	{
		glLineWidth(1.0f);
		glColor4fv(color_caja_in);
		Dibujar_Caja(ant->caja, sig->caja);
		glColor4fv(color_modelo);
	}

	return 1;
}

int TModelo::Inicializa_Caja(float *caja)
{
	caja[DERECHA] = -9999.0f;
	caja[IZQUIERDA] = 9999.0f;
	caja[ARRIBA] = -9999.0f;
	caja[ABAJO] = 9999.0f;
	caja[DELANTE] = -9999.0f;
	caja[DETRAS] = 9999.0f;
	return 1;
}

int TModelo::Comprobar_Caja (float *c, float *v)
{
	if (v[0] > c[DERECHA])
		c[DERECHA] = v[0];
	else if (v[0] < c[IZQUIERDA])
		c[IZQUIERDA] = v[0];

	if (v[1] > c[ARRIBA])
		c[ARRIBA] = v[1];
	else if (v[1] < c[ABAJO])
		c[ABAJO] = v[1];

	if (v[2] > c[DELANTE])
		c[DELANTE] = v[2];
	else if (v[2] < c[DETRAS])
		c[DETRAS] = v[2];

	return 1;
}

int TModelo::Dibujar_Caja (float *v1, float *v2)
//v[0] = derecha
//v[1] = izquierda
//v[2] = arriba
//v[3] = abajo
//v[4] = delante
//v[5] = atras
{
	float v[6];
	GLboolean luz, texturas,cull_face;
	GLint polygon_mode[2];

	glGetBooleanv(GL_LIGHTING, &luz);
	glGetBooleanv(GL_TEXTURE_2D, &texturas);
	glGetBooleanv(GL_CULL_FACE, &cull_face);
	glGetIntegerv(GL_POLYGON_MODE, polygon_mode);

	if (luz) glDisable(GL_LIGHTING);
    if (texturas) glDisable(GL_TEXTURE_2D);
	if (cull_face) glDisable(GL_CULL_FACE);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	v[0] = v1[0] + (t * (v2[0] - v1[0]));
	v[1] = v1[1] + (t * (v2[1] - v1[1]));
	v[2] = v1[2] + (t * (v2[2] - v1[2]));
	v[3] = v1[3] + (t * (v2[3] - v1[3]));
	v[4] = v1[4] + (t * (v2[4] - v1[4]));
	v[5] = v1[5] + (t * (v2[5] - v1[5]));

	glBegin(GL_QUADS);
		glVertex3f(v[DERECHA],v[ABAJO],v[DETRAS]);
		glVertex3f(v[DERECHA],v[ABAJO],v[DELANTE]);
		glVertex3f(v[DERECHA],v[ARRIBA],v[DELANTE]);
		glVertex3f(v[DERECHA],v[ARRIBA],v[DETRAS]);

		glVertex3f(v[IZQUIERDA],v[ABAJO],v[DETRAS]);
		glVertex3f(v[IZQUIERDA],v[ABAJO],v[DELANTE]);
		glVertex3f(v[IZQUIERDA],v[ARRIBA],v[DELANTE]);
		glVertex3f(v[IZQUIERDA],v[ARRIBA],v[DETRAS]);

		glVertex3f(v[IZQUIERDA],v[ARRIBA],v[DETRAS]);
		glVertex3f(v[IZQUIERDA],v[ARRIBA],v[DELANTE]);
		glVertex3f(v[DERECHA],v[ARRIBA],v[DELANTE]);
		glVertex3f(v[DERECHA],v[ARRIBA],v[DETRAS]);

		glVertex3f(v[IZQUIERDA],v[ABAJO],v[DETRAS]);
		glVertex3f(v[IZQUIERDA],v[ABAJO],v[DELANTE]);
		glVertex3f(v[DERECHA],v[ABAJO],v[DELANTE]);
		glVertex3f(v[DERECHA],v[ABAJO],v[DETRAS]);

		glVertex3f(v[IZQUIERDA],v[ABAJO],v[DELANTE]);
		glVertex3f(v[DERECHA],v[ABAJO],v[DELANTE]);
		glVertex3f(v[DERECHA],v[ARRIBA],v[DELANTE]);
		glVertex3f(v[IZQUIERDA],v[ARRIBA],v[DELANTE]);

		glVertex3f(v[IZQUIERDA],v[ABAJO],v[DETRAS]);
		glVertex3f(v[DERECHA],v[ABAJO],v[DETRAS]);
		glVertex3f(v[DERECHA],v[ARRIBA],v[DETRAS]);
		glVertex3f(v[IZQUIERDA],v[ARRIBA],v[DETRAS]);
	glEnd();

	if (luz) glEnable(GL_LIGHTING);
    if (texturas) glEnable(GL_TEXTURE_2D);
	if (cull_face) glEnable(GL_CULL_FACE);

	glPolygonMode(GL_FRONT_AND_BACK, polygon_mode[0]);

	return 1;
}

int TModelo::Dibujar_Modelo ()
{
	GLboolean t2d;
	int c = 0;
	
	for (int i = 0; i < 5; i++)
		if (objetos[i] != NULL) c++;
	if (!c) return 0;

	glPushMatrix();

	glTranslatef(pos[0],pos[1],pos[2]);
	glRotatef (rot[0],1.0f,0.0f,0.0f);
	glRotatef (rot[1],0.0f,1.0f,0.0f);
	glRotatef (rot[2],0.0f,0.0f,1.0f);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE ,color_modelo);
	glColor4fv(color_modelo);

	if (primer_fx)
	{
		if (coeffx <= 0) 
		{
			coeffx = t;
			Set_Frame (67);
			Play_Pause();
			cuantas_cajas = 0;
			coeft = 0.08f;
		}
		fx = 3;
		float alt = altura_efecto + ((t - coeffx) * (altura_efecto_sig - altura_efecto));
		for (int i = 0; i < nobjetos; i++)
			Dibujar_Objeto_FX (i, alt, -1, modo_de_dibujo);
		if (frame == JUMP_FIN)
		{
			cuantas_cajas = 1;
			framesig = 0;
			primer_fx = 0;
			fx = -1;
			cc = 0.0f;
			coeft = 0.02f;
		}
		else if (activo)
		{
			glLineWidth(1.5f);
		}
		else
		{
			glLineWidth(1.0f);
		}
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(color_caja_ex[0],color_caja_ex[1],color_caja_ex[2],color_caja_ex[3]*cc);
		Dibujar_Caja(caja[frame], caja[framesig]);		
		cc += coeft/5;
		glDisable(GL_BLEND);
	}
	else	if (fx >= 0)
	{
		if (coeffx <= 0) 
			coeffx = t;
		float alt = altura_efecto + ((t - coeffx) * (altura_efecto_sig - altura_efecto));
		for (int i = 0; i < nobjetos; i++)
			Dibujar_Objeto_FX (i, alt, modo_de_dibujo_ant, modo_de_dibujo);
		if (altura_efecto >= caja[framesig][fx-1])
			fx = -1;
	}
	else
	{
		for (int i = 0; i < nobjetos; i++)
		{
			switch (modo_de_dibujo)
			{
				case PUNTOS:
					glDisable(GL_TEXTURE_2D);
					glDisable(GL_CULL_FACE);
					glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
					glShadeModel(GL_SMOOTH);
					break;
				case LINEAS:
					glDisable(GL_TEXTURE_2D);
					glDisable(GL_CULL_FACE);
					glLineWidth(1.0f);
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					glShadeModel(GL_SMOOTH);
					break;
				case FLAT:
				   glDisable(GL_TEXTURE_2D);
					glEnable(GL_CULL_FACE);
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					glShadeModel(GL_FLAT);
					break;
				case SMOOTH:
					glDisable(GL_TEXTURE_2D);
					glEnable(GL_CULL_FACE);
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					glShadeModel(GL_SMOOTH);
					break;
				case TEXTURAS:
				   glEnable(GL_TEXTURE_2D);
					glEnable(GL_CULL_FACE);
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					glShadeModel(GL_SMOOTH);
					glGetBooleanv(GL_TEXTURE_2D, &t2d);
					if ((texturas[i].blind_id >= 0) && (t2d))
					{
						glColor4f (1.0f,1.0f,1.0f,1.0f);
						glBindTexture(GL_TEXTURE_2D, texturas[i].blind_id);
					}
					else
					{
						glDisable(GL_TEXTURE_2D);
						glEnable(GL_CULL_FACE);
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
						glShadeModel(GL_SMOOTH);
						glColor4fv(color_modelo);
					}
					break;
				default:
					break;
			}

			if (!stripsandfans)
				Dibujar_Objeto (i);
			else
				Dibujar_Objeto_Strips_And_Fans (i);
		}
	}

	if (cuantas_cajas > 0)
	{
		if (activo)
		{
			glLineWidth(1.5f);
		}
		else
		{
			glLineWidth(1.0f);
		}
		glColor4fv(color_caja_ex);
		Dibujar_Caja(caja[frame], caja[framesig]);
	}

	glGetBooleanv(GL_LIGHTING, &t2d);
	if (t2d) glDisable(GL_LIGHTING);

	glDisable(GL_CULL_FACE);
	glDisable(GL_TEXTURE_2D);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glTranslatef (
		caja[frame][DERECHA] + (t * (caja[framesig][DERECHA] - caja[frame][DERECHA])),
		caja[frame][ABAJO] + 0.8f +  (t * (caja[framesig][ABAJO] - caja[frame][ABAJO])),
		caja[frame][DELANTE]-1.0f + (t * (caja[framesig][DELANTE] - caja[frame][DELANTE]))
		);
	glRotatef (90.0f,0.0f,1.0f,0.0f);
	glScalef(0.75f,0.75f,0.75f);
	float f = frame + t;
	char s[25];
	sprintf_s(s,"%s - %f",nombre_animacion[animacion],f);
	glfDrawSolidString(s);

	if (t2d) glEnable(GL_LIGHTING);

	if (!stop)
		Calcular_T_Siguiente();
	
	glPopMatrix();

	return 1;
}

int TModelo::Play_Pause ()
{
	if (stop)
		stop = 0;
	else
		stop = 1;

	return stop;
}

int TModelo::Get_Play_Pause ()
{
	return stop;
}

void TModelo::Calcular_Frame_Siguiente ()
{
	frame = framesig++;
	if (framesig > frame_ini_fin[animacion][1])
		framesig = frame_ini_fin[animacion][0];
}

void TModelo::Calcular_T_Siguiente ()
{
	t += coeft;
	if (t >= 1)
	{
		Calcular_Frame_Siguiente ();
		t -= 1;
		if (fx >= 0)
		{
			altura_efecto = altura_efecto_sig;
			altura_efecto_sig += ((caja[framesig][fx-1] - caja[frame/*sig*/][fx]) / 2.0f);
		}
	}	
}

int TModelo::Set_Frame (int f)
{
	if (f >= frame_ini_fin[TODAS][1])
	{
		frame = frame_ini_fin[TODAS][1];
		framesig = frame_ini_fin[TODAS][0];
	}	
	else 
	{
		if (f <= frame_ini_fin[TODAS][0])
			frame = frame_ini_fin[TODAS][0];
		else
			frame = f;
		framesig = f + 1;
	}

	animacion = TODAS;
	stop = 1;
	t = 0.0f;

	return frame;
}

int TModelo::Get_Frame ()
{
	return frame;
}

int TModelo::Activar_Desactivar ()
{
	if (activo)
	{
		for (int i = 0; i < 4; i++)
		{
			color_modelo[i]  = colores_inactivos[COLOR_MODELO][i];
			color_caja_ex[i] = colores_inactivos[COLOR_CAJA_EX][i];
			color_caja_in[i] = colores_inactivos[COLOR_CAJA_IN][i];
		}
		activo = 0;
	}
	else
	{
		for (int i = 0; i < 4; i++)
		{
			color_modelo[i]  = colores_activos[COLOR_MODELO][i];
			color_caja_ex[i] = colores_activos[COLOR_CAJA_EX][i];
			color_caja_in[i] = colores_activos[COLOR_CAJA_IN][i];
		}
		activo = 1;
	}

	return activo;
}

int TModelo::Get_Activar_Desactivar ()
{
	return activo;
}

void TModelo::Trasladar (float x, float y, float z)
{
	pos[0] += x;
	pos[1] += y;
	pos[2] += z;
}

void TModelo::Rotar (float x, float y, float z)
{
	rot[0] += x;
	rot[1] += y;
	rot[2] += z;
}

int TModelo::Set_Animacion (int a)
{
	if (a < 0)
		a = 0;
	if (a > 20)
		a = 20;
	animacion = a;
	
	frame = frame_ini_fin[animacion][0];
	framesig = frame + 1;

	return a;
}

int TModelo::Get_Animacion ()
{
	return animacion;
}

float TModelo::Set_Coeft (float c)
{
	if (c <= 0.0f)
		coeft = 0.0f;
	else if (c >= 1.0f)
		coeft = 1.0f;
	else
		coeft = c;

	return coeft;
}

float TModelo::Get_Coeft ()
{
	return coeft;
}

int TModelo::Strips_and_Fans ()
{
	if (stripsandfans)
		stripsandfans = 0;
	else
		stripsandfans = 1;
	
	return stripsandfans;
}

int TModelo::Get_Strips_and_Fans ()
{
	return stripsandfans;
}

int TModelo::Set_Cuantas_Cajas (int a)
{
	if (a < 0) a = 0;
	if (a > 2) a = 2;

	cuantas_cajas = a;

	return cuantas_cajas;
}

int TModelo::Get_Cuantas_Cajas ()
{
	return cuantas_cajas;
}

int TModelo::Set_Modo_De_Dibujo (int m)
{
	if (m < 0) m = 0;
	if (m > TEXTURAS) m = TEXTURAS;

	if (stop)
	{
		modo_de_dibujo = m;
		return modo_de_dibujo;
	}

	modo_de_dibujo_ant = modo_de_dibujo;
	modo_de_dibujo = m;

	float r = (float)rand() / (float)RAND_MAX;
	if (r < 0.33)
		fx = 1;
	else if (r < 0.66)
		fx = 5;
	else
		fx = 3;

	altura_efecto = (float)caja[frame][fx];
	altura_efecto_sig = altura_efecto + ((caja[framesig][fx-1]-caja[frame/*sig*/][fx]) / 2.0f);
	coeffx = -1;

	return modo_de_dibujo;
}

int TModelo::Get_Modo_De_Dibujo ()
{
	return modo_de_dibujo;
}

void TModelo::Refrescar_Colores()
{
	if (activo)
		for (int i = 0; i < 4; i++)
		{
			color_modelo[i]  = colores_activos[COLOR_MODELO][i];
			color_caja_ex[i] = colores_activos[COLOR_CAJA_EX][i];
			color_caja_in[i] = colores_activos[COLOR_CAJA_IN][i];
		}
	else
		for (int i = 0; i < 4; i++)
		{
			color_modelo[i]  = colores_inactivos[COLOR_MODELO][i];
			color_caja_ex[i] = colores_inactivos[COLOR_CAJA_EX][i];
			color_caja_in[i] = colores_inactivos[COLOR_CAJA_IN][i];
		}
}

int TModelo::Set_Color_Activo (int c, TColor color)
{
	if (c < COLOR_MODELO) return 0;
	if (c > COLOR_CAJA_IN) return 0;
	for (int i = 0; i < 4; i++)
		colores_activos[c][i] = color[i];
	Refrescar_Colores();
	return 1;
}

int TModelo::Get_Color_Activo (int c, TColor color)
{
	if (c < COLOR_MODELO) return 0;
	if (c > COLOR_CAJA_IN) return 0;

	for (int i = 0; i < 4; i++)
		color[i] = colores_activos[c][i];

	return 1;
}

int TModelo::Set_Color_Inactivo (int c, TColor color)
{
	if (c < COLOR_MODELO) return 0;
	if (c > COLOR_CAJA_IN) return 0;
	for (int i = 0; i < 4; i++)
		colores_inactivos[c][i] = color[i];
	Refrescar_Colores();
	return 1;
}

int TModelo::Get_Color_Inactivo (int c, TColor color)
{
	if (c < COLOR_MODELO) return 0;
	if (c > COLOR_CAJA_IN) return 0;

	for (int i = 0; i < 4; i++)
		color[i] = colores_inactivos[c][i];

	return 1;
}
