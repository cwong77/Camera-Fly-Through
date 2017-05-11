
// Ensure the header file is included only once in multi-file projects
#ifndef APP_WINDOW_H
#define APP_WINDOW_H

#define PI 3.1419526

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
# include "Jukebox.h"

// The functionality of your application should be implemented inside AppWindow
class AppWindow : public GlutWindow
 { private :
    // OpenGL shaders and programs:
    //GlShader _vshflat, _fshflat, _vshgou, _fshgou, _vshphong, _fshphong;
  //  GlProgram _progflat, _proggou, _progphong;

    // My scene objects:
    SoAxis _axis;


    SoModel _bridge, _house1, _house2, _house3, _house4, _door1, _balloon, _balloon2, _house5;

	SoPlane _ground, _skytop, _skyfront, _skyback, _skyleft, _skyright;

	//Variables for the door
	GsMat rotd, transd, location;
	int degrees;
	float theta;

    // Scene data:
	Jukebox musicplayer;

    bool  _viewaxis = false;

    GsModel _gsm1, _gsm2, _gsm3, _gsm4, _gsm5,_gsm6,_gsm7,_gsm8;

    GsLight _light;
	float tx = 0, ty = 0, tz = 0;

	GsMat _transBridge, _transHouse1, _transHouse2, _transHouse3, _transHouse4, _transHouse5, _transballoon, _transskytop, _transskyfront, _transskyback, _transskyleft, _transskyright;
	GsMat _rotBridge, _rotHouse1, _rotHouse2, _rotHouse3, _rotHouse4, _rotskyfront, _rotskyback, _rotskyleft, _rotskyright;

	SoLines _lines;

	/********************Flying stuff************/
	CurveEval _flyPath;
	CurveEval _flyPath2;
	SoLines _flyVisualization;
	SoLines _flyVisualization2;
	GsArray<GsVec> _flyControlPoints;
	GsArray<GsVec> _flyControlPoints2;
	GsArray<GsVec> _flyInterpolation;
	GsArray<GsVec> _flyInterpolation2;
	GsMat _fly;
	GsMat _fly2;
	int flyIndex = 0;
	int flyIndex2 = 0;
	bool flyBackwards = false;
	
	/********************Camera and friends*****************************/
	Camera cam;
	CurveEval _cameraPath;
	GsArray<GsVec> _cameraControlPoints;
	GsArray<GsVec> _cameraInterpolation;
	float interval = (2 * PI) / 1440;									//rotation angle
	SoLines _curveVisualization;
	bool moveCamera = false;											//whether or not to move camera
	bool rotateCam = false;												//whether or not to rotate camera
	float parameter = 0;												//this is for rotating camera by parameter
	bool pause = true;
    
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
	virtual void glutIdle();
	void AppWindow::translation(GsMat &transform, float x, float y, float z);

	void AppWindow::rotatey(GsMat &rotatey, int degrees);

	void AppWindow::rotationy(GsMat &rotate, float theta);
	void AppWindow::rotationx(GsMat &rotate, float theta);
	void AppWindow::rotationz(GsMat &rotate, float theta);

	void AppWindow::loadFlyCurve();
	void AppWindow::fly(GsMat &fly, GsVec flyHere);

 };

#endif // APP_WINDOW_H
