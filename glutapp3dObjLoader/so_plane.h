
// Ensure the header file is included only once in multi-file projects
#ifndef SO_PLANE_H
#define SO_PLANE_H

// Include needed header files
# include <gsim/gs_mat.h>
# include <gsim/gs_light.h>
# include <gsim/gs_color.h>
# include <gsim/gs_material.h>
# include <gsim/gs_array.h>
# include "ogl_tools.h"

#include <gsim/gs_material.h>
#include <gsim/gs_shared.h>
#include <gsim/gs_image.h>
#include <gsim/stb_image.h>
#include <gsim/stb_image_write.h>

// Scene objects should be implemented in their own classes; and
// here is an example of how to organize a scene object in a class.
// Scene object axis:
class SoPlane : public GlObjects
{
private:
	GlShader _vshgou, _fshgou, _vshtex, _fshtex;
	GlProgram _proggouraud, _progphong;
	GLuint _texid;
	GsArray<GsVec>   P; // coordinates
	GsArray<GsColor> C; // diffuse colors per face
	GsArray<GsVec>   N; // normals
	GsArray<GsVec2>  T; // texture coords
	GsMaterial _mtl;
	int _numpoints;     // just saves the number of points
	bool _flatn;
public:
	GsArray<GsVec> NL; // normal lines computed in last build
public:
	SoPlane();
	void init(std::string _path);
	void flat(bool b) { _flatn = b; }
	void build(float xmin, float xmax, float zmin, float zmax, float y);
	void draw(const GsMat& tr, const GsMat& pr, const GsLight& l);
};

#endif // SO_PLANE_H
