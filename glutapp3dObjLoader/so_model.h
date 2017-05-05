
// Ensure the header file is included only once in multi-file projects
#ifndef SO_MODEL_H
#define SO_MODEL_H

// Include needed header files
# include <gsim/gs_mat.h>
# include <gsim/gs_light.h>
# include <gsim/gs_color.h>

#include <gsim/gs_material.h>

# include <gsim/gs_array.h>
# include <gsim/gs_model.h>
# include "ogl_tools.h"

#include <gsim/gs_shared.h>
#include <gsim/gs_image.h>
#include <gsim/stb_image.h>
#include <gsim/stb_image_write.h>

// Scene objects should be implemented in their own classes; and
// here is an example of how to organize a scene object in a class.
// Scene object axis:
class SoModel : public GlObjects
 { private :
    GlShader _vshtex, _fshtex;
	GlProgram _progtex;

	GLuint _texid;

    GsArray<GsPnt>   P; // coordinates
    GsArray<GsColor> C; // diffuse colors per face
    GsArray<GsVec>   N; // normals
	GsArray<GsPnt2>	T;	//texture


    GsMaterial _mtl;    // main material
    int _numpoints;     // just saves the number of points
    bool _phong;
   public :
	GsArray<GsVec> NL;

    SoModel ();
    void phong ( bool b ) { _phong=b; }
    bool phong () const { return _phong; }
    void init (std::string _path);
    void build ( GsModel& m );
    void draw ( const GsMat& tr, const GsMat& pr, const GsLight& l );
 };

#endif // SO_MODEL_H
