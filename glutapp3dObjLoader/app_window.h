
// Ensure the header file is included only once in multi-file projects
#ifndef APP_WINDOW_H
#define APP_WINDOW_H

# include <gsim/gs_color.h>
# include <gsim/gs_array.h>
# include <gsim/gs_light.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"
# include "glut_window.h"
# include "so_axis.h"
# include "so_model.h"
# include "camera.h"
# include "so_lines.h"
# include "curve_eval.h"
# include "so_plane.h"

// The functionality of your application should be implemented inside AppWindow
class AppWindow : public GlutWindow
 { private :
    // OpenGL shaders and programs:
    //GlShader _vshflat, _fshflat, _vshgou, _fshgou, _vshphong, _fshphong;
  //  GlProgram _progflat, _proggou, _progphong;

    // My scene objects:
    SoAxis _axis;
    SoModel _bridge, _house1, _house2, _house3, _house4;
	SoPlane _ground;
	Camera cam;

    // Scene data:
    bool  _viewaxis = false;
    GsModel _gsm1, _gsm2, _gsm3, _gsm4, _gsm5;
    GsLight _light;
	float tx, ty, tz;

	SoLines _lines;

	CurveEval _cameraPath;

	GsArray<GsVec> _cameraControlPoints;
	GsArray<GsVec> _cameraInterpolation;
    
    // App data:
    enum MenuEv { evOption0, evOption1 };
    float _rotx, _roty, _fovy;
    int _w, _h;

   public :
    AppWindow ( const char* label, int x, int y, int w, int h );
    void initPrograms ();
    void loadModel ( int model );
    GsVec2 windowToScene ( const GsVec2& v );

   private : // functions derived from the base class
    virtual void glutMenu ( int m );
    virtual void glutKeyboard ( unsigned char key, int x, int y );
    virtual void glutSpecial ( int key, int x, int y );
    virtual void glutMouse ( int b, int s, int x, int y );
    virtual void glutMotion ( int x, int y );
    virtual void glutDisplay ();
    virtual void glutReshape ( int w, int h );
	virtual void loadCameraCurve();
	void AppWindow::translation(GsMat &transform, float x, float y, float z);
 };

#endif // APP_WINDOW_H
