////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-2000 Mark Danks.
//    Copyright (c) G�nther Geiger.
//    Copyright (c) 2001-2011 IOhannes m zm�lnig. forum::f�r::uml�ute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "gemvertexbuffer.h"

#include "Gem/State.h"
#include "Gem/Manager.h"
#include <math.h>
#include <stdio.h>

#if defined (__APPLE__) || defined(MACOSX)
   #define GL_SHARING_EXTENSION "cl_APPLE_gl_sharing"
#else
   #define GL_SHARING_EXTENSION "cl_khr_gl_sharing"
#endif

#define UNIX

CPPEXTERN_NEW_WITH_ONE_ARG(gemvertexbuffer, t_floatarg, A_DEFFLOAT);

gemvertexbuffer :: VertexBuffer:: VertexBuffer (unsigned int size_) :
  size(size_),
  vbo(0),
  array(new float[size_]),
  dirty(false),
  enabled(false) {
  ::post("created VertexBuffer[%p] with %d elements at %p", this, size, array);
}
gemvertexbuffer :: VertexBuffer:: ~VertexBuffer (void) {
  ::post("destroying VertexBuffer[%p] with %d elements at %p", this, size, array);
  destroy();
  if(array)
    delete[]array;
  array=NULL;
}
void gemvertexbuffer :: VertexBuffer:: resize (unsigned int size_) {
  if(array)delete[]array; 
  array=new float[size_];
  size=size_;
  dirty=true;
}

bool gemvertexbuffer :: VertexBuffer:: create (void) {
  if(!vbo) {
    glGenBuffers(1, &vbo);
  }
  if(vbo) {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), array, GL_DYNAMIC_DRAW);
  }
  return (0!=vbo);
}
bool gemvertexbuffer :: VertexBuffer:: render (void) {
  // render from the VBO
  if ( enabled ) {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    if ( dirty ) {
      //~ printf("push pos vertex\n");
      glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), array, GL_DYNAMIC_DRAW);
      dirty = false;
    }
  }
  return enabled;
}
void gemvertexbuffer :: VertexBuffer:: destroy (void) {
	if ( vbo ){
		glBindBuffer(1, vbo);
		glDeleteBuffers(1, &vbo);
	}
  vbo=0;
}



/////////////////////////////////////////////////////////
//
// gemvertexbuffer
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
gemvertexbuffer :: gemvertexbuffer(t_floatarg size) :
  vbo_size(size>0?size:(256*256)),
	size_change_flag(false),
  m_position(vbo_size*3), m_texture(vbo_size*2), m_color(vbo_size*4), m_normal(vbo_size*3)
{ 
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
gemvertexbuffer :: ~gemvertexbuffer(void)
{
}

/////////////////////////////////////////////////////////
// renderShape
//
/////////////////////////////////////////////////////////
void gemvertexbuffer :: renderShape(GemState *state)
{
	if ( m_drawType == GL_DEFAULT_GEM ) m_drawType = GL_POINTS;
	if ( !m_position.vbo || !m_texture.vbo || !m_color.vbo || !m_normal.vbo || size_change_flag ) {
//		printf("create VBO\n");
		createVBO();
		size_change_flag = false;
	}
  // render from the VBO
  if(m_position.render())
    glVertexPointer(3, GL_FLOAT, 0, 0);
  if(m_texture.render())
    glTexCoordPointer(2, GL_FLOAT, 0, 0);
  if(m_color.render())
			glColorPointer(4, GL_FLOAT, 0, 0);
  if(m_normal.render())
			glNormalPointer(GL_FLOAT, 0, 0);
		
  if ( m_position.enabled ) glEnableClientState(GL_VERTEX_ARRAY);
  if ( m_color.enabled ) glEnableClientState(GL_COLOR_ARRAY);
  if ( m_texture.enabled ) glEnableClientState(GL_TEXTURE_COORD_ARRAY); 
  if ( m_normal.enabled ) glEnableClientState(GL_NORMAL_ARRAY); 
		
  glDrawArrays(m_drawType, 0, vbo_size);
		
  if ( m_position.enabled ) glDisableClientState(GL_VERTEX_ARRAY);
  if ( m_color.enabled ) glDisableClientState(GL_COLOR_ARRAY);
  if ( m_texture.enabled ) glDisableClientState(GL_TEXTURE_COORD_ARRAY); 
  if ( m_normal.enabled ) glDisableClientState(GL_NORMAL_ARRAY);	
}

/////////////////////////////////////////////////////////
// Setup callback functions
//
/////////////////////////////////////////////////////////
void gemvertexbuffer :: obj_setupCallback(t_class *classPtr)
{
	class_addmethod(classPtr, reinterpret_cast<t_method>(&gemvertexbuffer::posxMessCallback), 
		gensym("posX"), A_GIMME, A_NULL);
	class_addmethod(classPtr, reinterpret_cast<t_method>(&gemvertexbuffer::posyMessCallback), 
		gensym("posY"), A_GIMME, A_NULL);
	class_addmethod(classPtr, reinterpret_cast<t_method>(&gemvertexbuffer::poszMessCallback), 
		gensym("posZ"), A_GIMME, A_NULL);
	class_addmethod(classPtr, reinterpret_cast<t_method>(&gemvertexbuffer::colrMessCallback), 
		gensym("colorR"), A_GIMME, A_NULL);
	class_addmethod(classPtr, reinterpret_cast<t_method>(&gemvertexbuffer::colgMessCallback), 
		gensym("colorG"), A_GIMME, A_NULL);
	class_addmethod(classPtr, reinterpret_cast<t_method>(&gemvertexbuffer::colbMessCallback), 
		gensym("colorB"), A_GIMME, A_NULL);
	class_addmethod(classPtr, reinterpret_cast<t_method>(&gemvertexbuffer::colaMessCallback), 
		gensym("colorA"), A_GIMME, A_NULL);
	class_addmethod(classPtr, reinterpret_cast<t_method>(&gemvertexbuffer::texuMessCallback), 
		gensym("textureU"), A_GIMME, A_NULL);
	class_addmethod(classPtr, reinterpret_cast<t_method>(&gemvertexbuffer::texvMessCallback), 
		gensym("textureV"), A_GIMME, A_NULL);
	class_addmethod(classPtr, reinterpret_cast<t_method>(&gemvertexbuffer::normxMessCallback), 
		gensym("normalX"), A_GIMME, A_NULL);
	class_addmethod(classPtr, reinterpret_cast<t_method>(&gemvertexbuffer::normyMessCallback), 
		gensym("normalY"), A_GIMME, A_NULL);
	class_addmethod(classPtr, reinterpret_cast<t_method>(&gemvertexbuffer::normzMessCallback), 
		gensym("normalZ"), A_GIMME, A_NULL);
	class_addmethod(classPtr, reinterpret_cast<t_method>(&gemvertexbuffer::resizeMessCallback), 
		gensym("resize"), A_FLOAT, A_NULL);
	class_addmethod(classPtr, reinterpret_cast<t_method>(&gemvertexbuffer::posVBO_enableMessCallback), 
		gensym("m_position.enabled"), A_FLOAT, A_NULL);
	class_addmethod(classPtr, reinterpret_cast<t_method>(&gemvertexbuffer::colVBO_enableMessCallback), 
		gensym("m_color.enabled"), A_FLOAT, A_NULL);
	class_addmethod(classPtr, reinterpret_cast<t_method>(&gemvertexbuffer::texVBO_enableMessCallback), 
		gensym("m_texture.enabled"), A_FLOAT, A_NULL);
	class_addmethod(classPtr, reinterpret_cast<t_method>(&gemvertexbuffer::normVBO_enableMessCallback), 
		gensym("m_normal.enabled"), A_FLOAT, A_NULL);			
}

void gemvertexbuffer :: posxMessCallback(void *data, t_symbol *symbol, int argc, t_atom *argv)
{	
	GetMyClass(data)->tabMess(argc,argv, GetMyClass(data)->m_position, 3, 0);
	GetMyClass(data)->m_position.dirty = true;
	GetMyClass(data)->m_position.enabled = true;
	   
}
void gemvertexbuffer :: posyMessCallback(void *data, t_symbol *symbol, int argc, t_atom *argv)
{
	GetMyClass(data)->tabMess(argc,argv, GetMyClass(data)->m_position, 3, 1);
	GetMyClass(data)->m_position.dirty = true;
	GetMyClass(data)->m_position.enabled = true;
}
void gemvertexbuffer :: poszMessCallback(void *data, t_symbol *symbol, int argc, t_atom *argv)
{
	GetMyClass(data)->tabMess(argc,argv, GetMyClass(data)->m_position, 3, 2);
	GetMyClass(data)->m_position.dirty = true;
	GetMyClass(data)->m_position.enabled = true;
}

void gemvertexbuffer :: colrMessCallback(void *data, t_symbol *symbol, int argc, t_atom *argv)
{
	GetMyClass(data)->tabMess(argc,argv, GetMyClass(data)->m_color, 4, 0);
	GetMyClass(data)->m_color.dirty = true;
	GetMyClass(data)->m_color.enabled = true;
}

void gemvertexbuffer :: colgMessCallback(void *data, t_symbol *symbol, int argc, t_atom *argv)
{
	GetMyClass(data)->tabMess(argc,argv, GetMyClass(data)->m_color, 4, 1);
	GetMyClass(data)->m_color.dirty = true;
	GetMyClass(data)->m_color.enabled = true;
}

void gemvertexbuffer :: colbMessCallback(void *data, t_symbol *symbol, int argc, t_atom *argv)
{
	GetMyClass(data)->tabMess(argc,argv, GetMyClass(data)->m_color, 4, 2);
	GetMyClass(data)->m_color.dirty = true;
	GetMyClass(data)->m_color.enabled = true;
}

void gemvertexbuffer :: colaMessCallback(void *data, t_symbol *symbol, int argc, t_atom *argv)
{
	GetMyClass(data)->tabMess(argc,argv, GetMyClass(data)->m_color, 4, 3);
	GetMyClass(data)->m_color.dirty = true;
	GetMyClass(data)->m_color.enabled = true;
}

void gemvertexbuffer :: texuMessCallback(void *data, t_symbol *symbol, int argc, t_atom *argv)
{
	GetMyClass(data)->tabMess(argc,argv, GetMyClass(data)->m_texture, 2, 0);
	GetMyClass(data)->m_texture.dirty = true;
	GetMyClass(data)->m_texture.enabled = true;
}

void gemvertexbuffer :: texvMessCallback(void *data, t_symbol *symbol, int argc, t_atom *argv)
{
	GetMyClass(data)->tabMess(argc,argv, GetMyClass(data)->m_texture, 2, 1);
	GetMyClass(data)->m_texture.dirty = true;
	GetMyClass(data)->m_texture.enabled = true;
}

void gemvertexbuffer :: normxMessCallback(void *data, t_symbol *symbol, int argc, t_atom *argv)
{
	GetMyClass(data)->tabMess(argc,argv, GetMyClass(data)->m_normal, 3, 0);
	GetMyClass(data)->m_normal.dirty = true;
	GetMyClass(data)->m_normal.enabled = true;
}

void gemvertexbuffer :: normyMessCallback(void *data, t_symbol *symbol, int argc, t_atom *argv)
{
	GetMyClass(data)->tabMess(argc,argv, GetMyClass(data)->m_normal, 3, 1);
	GetMyClass(data)->m_normal.dirty = true;
	GetMyClass(data)->m_normal.enabled = true;
}

void gemvertexbuffer :: normzMessCallback(void *data, t_symbol *symbol, int argc, t_atom *argv)
{
	GetMyClass(data)->tabMess(argc,argv, GetMyClass(data)->m_normal, 3, 2);
	GetMyClass(data)->m_normal.dirty = true;
	GetMyClass(data)->m_normal.enabled = true;
}

void gemvertexbuffer :: resizeMessCallback(void *data, float size)
{
	GetMyClass(data)->resizeMess(size);
}

void gemvertexbuffer :: posVBO_enableMessCallback(void *data, float flag)
{
	GetMyClass(data)->m_position.enabled = (flag != 0);
}

void gemvertexbuffer :: colVBO_enableMessCallback(void *data, float flag)
{
	GetMyClass(data)->m_color.enabled = (flag != 0);
}

void gemvertexbuffer :: texVBO_enableMessCallback(void *data, float flag)
{
	GetMyClass(data)->m_texture.enabled = (flag != 0);
}

void gemvertexbuffer :: normVBO_enableMessCallback(void *data, float flag)
{
	GetMyClass(data)->m_normal.enabled = (flag != 0);
}

void gemvertexbuffer :: tabMess(int argc, t_atom *argv, VertexBuffer&array, int stride, int offset)
{
	int offset2 = 0;
	t_symbol *tab_name;
	if ( argv[0].a_type != A_SYMBOL ) 
	{
		error("first arg must be symbol (table name)");
		return;
	}
	if ( argc > 1 ) 
	{
		if ( argv[1].a_type != A_FLOAT )
		{
			error("second arg must be float (offset)");
		}
		else offset2 = argv[1].a_w.w_float;
	}
	offset2 = offset2<0?0:offset2;
	tab_name = argv[0].a_w.w_symbol;
	copyArray(tab_name, array, stride, offset2 * stride + offset);
}

void gemvertexbuffer :: resizeMess(float size)
{
	vbo_size = size>1?size:1;
	//~ printf("cleanup\n");
  m_position.resize(vbo_size*3);
  m_texture .resize(vbo_size*2);
  m_color   .resize(vbo_size*4);
  m_normal  .resize(vbo_size*3);

	size_change_flag = true;
}

// Create VBO
//*****************************************************************************
void gemvertexbuffer :: createVBO(void)
{
  m_position.create();
  m_texture.create();
  m_color.create();
  m_texture.create();
}

void gemvertexbuffer :: copyArray(t_symbol *tab_name, VertexBuffer&buf, unsigned int stride, unsigned int offset)
{
	t_garray *a;
	int npoints, i;
	t_word *vec;
	t_float posx;

  t_float*array=buf.array;
	//~ printf("copy table %s to array\n", tab_name->s_name);
	pd_findbyclass(tab_name, garray_class);
	if (!(a = (t_garray *)pd_findbyclass(tab_name, garray_class)))
		error("%s: no such array", tab_name->s_name);
    else if (!garray_getfloatwords(a, &npoints, &vec))
        error("%s: bad template for tabLink", tab_name->s_name);
	else
    {		
		npoints = npoints>vbo_size?vbo_size:npoints;
		//~ printf("start copying %d values\n",npoints);
		for ( i = 0 ; i < npoints ; i++ )
		{	
			array[offset + i*stride] = vec[i].w_float;
		}
	}
	//~ printf("copy done\n");
}
