/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    Composite two pix images.

    Copyright (c) 1997-1998 Mark Danks
    Copyright (c) G�nther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM. zmoelnig@iem.kug.ac.at
    Copyright (c) 2002 James Tittle & Chris Clepper
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_PIX_COMPOSITE_H_
#define INCLUDE_PIX_COMPOSITE_H_

#include "Base/GemPixDualObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_composite
    
    Composite two pix images.  Puts the second image over the top using
    alpha blending.

KEYWORDS
    pix
    
DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN pix_composite : public GemPixDualObj
{
    CPPEXTERN_HEADER(pix_composite, GemPixDualObj)

    public:

	    //////////
	    // Constructor
    	pix_composite();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~pix_composite();

    	//////////
    	// Do the processing
    	virtual void 	processDualImage(imageStruct &image, imageStruct &right);
        
	//////////
	virtual void 	processDualYUV(imageStruct &image, imageStruct &right);
};

#endif	// for header file
