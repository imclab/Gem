 /* ------------------------------------------------------------------
  * GEM - Graphics Environment for Multimedia
  *
  *  Copyright (c) 2002-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
  *	zmoelnig@iem.kug.ac.at
  *  For information on usage and redistribution, and for a DISCLAIMER
  *  OF ALL WARRANTIES, see the file, "GEM.LICENSE.TERMS"
  *
  *  this file has been generated...
  * ------------------------------------------------------------------
  */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLCOLOR_F_H_
#define _INCLUDE__GEM_OPENGL_GEMGLCOLOR_F_H_

#include "Base/GemGLBase.h"

/*
 CLASS
	GEMglColor3f
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glColor3f( GLfloat red, GLfloat green, GLfloat blue)"
 */

class GEM_EXTERN GEMglColor3f : public GemGLBase
{
	CPPEXTERN_HEADER(GEMglColor3f, GemGLBase);

	public:
	  // Constructor
	  GEMglColor3f (t_float, t_float, t_float);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglColor3f ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLfloat	red;		// VAR
	  virtual void	redMess(t_float);	// FUN

	  GLfloat	green;		// VAR
	  virtual void	greenMess(t_float);	// FUN

	  GLfloat	blue;		// VAR
	  virtual void	blueMess(t_float);	// FUN


	private:

	// we need some inlets
	  t_inlet *m_inlet[3];

	// static member functions
	  static void	 redMessCallback (void*, t_float);
	  static void	 greenMessCallback (void*, t_float);
	  static void	 blueMessCallback (void*, t_float);
};
#endif // for header file
