
# include <iostream>
# include <gsim/gs.h>
# include "app_window.h"
const float PI = 3.1415926535f;

AppWindow::AppWindow ( const char* label, int x, int y, int w, int h )
          :GlutWindow ( label, x, y, w, h )
 {
   initPrograms ();
   addMenuEntry ( "Option 0", evOption0 );
   addMenuEntry ( "Option 1", evOption1 );
   _viewaxis = true;
   _fovy = GS_TORAD(60.0f);
   _rotx = 0.05f;
   _roty = 0;
   _w = w;
   _h = h;
   //tx = ty = 0.0f;
   //tz = 2.0f;
 }

void AppWindow::loadCameraCurve() {
	//std::cout << "loading cam curve\n";

	//push control points
	/**********************Fill these in Tyler**********************/
	/*******************VERY IMPORTANT: NO SUPPORT TO CHECK WHEN THE CAMERA HAS GONE THROUGH THE WHOLE CURVE
						SO THE CURVE MUST LOOP OR JUST CALL ME TO FIX IT	
						NOT HEAVILY TESTED SO CALL ME IF IT DOESN'T WORK
						CAMERA ONLY LOOKS FORWARD
	*****************************************/
	//these are random control points
	_cameraControlPoints.push(GsVec(0.0, 0.0, 0.0));
	_cameraControlPoints.push(GsVec(.75, 0.25, 0.0));
	_cameraControlPoints.push(GsVec(.5, .5, 0.0));
	_cameraControlPoints.push(GsVec(.25, .75, 0.0));
	_cameraControlPoints.push(GsVec(0.0, 0.0, 1.0));
	/***************************************************************/

	//interpolate them
	float interval = (float)(_cameraControlPoints.size()) / 128;
	for (float i = 2; i < _cameraControlPoints.size(); i += interval){
		_cameraInterpolation.push(_cameraPath.eval_bspline(i, 3, _cameraControlPoints));
	}

	for (int i = 0; i < _cameraInterpolation.size(); ++i) {
		std::cout << _cameraInterpolation[i] << std::endl;
	}

	//give them to the camera
	cam.init(_cameraInterpolation);
}

void AppWindow::initPrograms ()
 {
   // Init my scene objects:
   _axis.init ();
   _bridge.init("../models/Texture.png");
   _house1.init("../models/House_1.png");
   _house2.init("../models/House_2.png");
   _house3.init("../models/House_3.png");
   _house4.init("../models/House_4.png");
   _cloud.init();

   _ground.init("../models/Ground.png");

   // set light:
   _light.set ( GsVec(0,5,10), GsColor(90,90,90,255), GsColor::white, GsColor::white );

   _lines.init();	//normal lines for testing purposes
   loadCameraCurve();	//initializes the camera with a curve to follow

   _lines.build(_house1.NL, GsColor::red);

   // Load demo model:
   loadModel ( 1 );
 }

static void printInfo ( GsModel& m )
 {
   //std::cout<<"V:  "<<m.V.size()<<"\n";
   //std::cout<<"F:  "<<m.F.size()<<"\n";
   //std::cout<<"N:  "<<m.N.size()<<"\n";
   //std::cout<<"M:  "<<m.M.size()<<"\n";
   //std::cout<<"Fn: "<<m.Fn.size()<<"\n";
   //std::cout<<"Fm: "<<m.Fm.size()<<"\n";
 }

void AppWindow::loadModel ( int model )
 {
   float f;
   GsString file1, file2, file3, file4, file5, file6;
   f = 0.4f; 
   file1 = "../models/Bridges.obj";
   file2 = "../models/House_1.obj";
   file3 = "../models/House_2.obj";
   file4 = "../models/House_3.obj";
   file5 = "../models/House_4.obj";
   file6 = "../models/cloud.obj";
   
   //std::cout << "Loading "<< file1 << "...\n";
   if (!_gsm1.load(file1)) std::cout << "Error!\n";
   //printInfo (_gsm1);
   _gsm1.scale ( f ); // to fit our camera space
   _bridge.build(_gsm1);
   
   //std::cout << "Loading " << file2 << "...\n";
   if (!_gsm2.load(file2)) std::cout << "Error!\n";
   //printInfo(_gsm2);
   _gsm2.scale(f); // to fit our camera space
   _house1.build(_gsm2);
   
   //std::cout << "Loading " << file3 << "...\n";
   if (!_gsm3.load(file3)) std::cout << "Error!\n";
   //printInfo(_gsm3);
   _gsm3.scale(f); // to fit our camera space
   _house2.build(_gsm3);
   
   //std::cout << "Loading " << file4 << "...\n";
   if (!_gsm4.load(file4)) std::cout << "Error!\n";
   //printInfo(_gsm4);
   _gsm4.scale(f); // to fit our camera space
   _house3.build(_gsm4);
   
   //std::cout << "Loading " << file5 << "...\n";
   if (!_gsm5.load(file5)) std::cout << "Error!\n";
   //printInfo(_gsm5);
   _gsm5.scale(f); // to fit our camera space
   _house4.build(_gsm5);

   //std::cout << "Loading " << file6 << "...\n";
   if (!_gsm6.load(file6)) std::cout << "Error!\n";
   //printInfo(_gsm6);
   _gsm6.scale(f); // to fit our camera space
   _cloud.build(_gsm6);
   
   _lines.build(_house2.NL, GsColor::red);

   redraw();
 }

// mouse events are in window coordinates, but your 2D scene is in [0,1]x[0,1],
// so make here the conversion when needed
GsVec2 AppWindow::windowToScene ( const GsVec2& v )
 {
   // GsVec2 is a lighteweight class suitable to return by value:
   return GsVec2 ( (2.0f*(v.x/float(_w))) - 1.0f,
                    1.0f - (2.0f*(v.y/float(_h))) );
 }

// Called every time there is a window event
void AppWindow::glutKeyboard ( unsigned char key, int x, int y )
 {
   switch ( key )
    { case ' ': _viewaxis = !_viewaxis; redraw(); break;
	  case 27 : exit(1); // Esc was pressed
      case 's' : std::cout<<"Smoothing normals...\n";
                 _gsm1.smooth (GS_TORAD(35));
				 _gsm2.smooth(GS_TORAD(35));
				 _gsm3.smooth(GS_TORAD(35));
				 _gsm4.smooth(GS_TORAD(35));
				 _gsm5.smooth(GS_TORAD(35));
				 _gsm6.smooth(GS_TORAD(35));
                 _bridge.build(_gsm1);
				 _house1.build(_gsm2);
				 _house2.build(_gsm3);
				 _house3.build(_gsm4);
				 _house4.build(_gsm5);
				 _cloud.build(_gsm6);
                 redraw(); 
                 break;
      case 'f' : std::cout<<"Flat normals...\n";
                _gsm1.flat();
				_gsm2.flat();
				_gsm3.flat();
				_gsm4.flat();
				_gsm5.flat();
				_gsm6.flat();
				_bridge.build(_gsm1);
				_house1.build(_gsm2);
				_house2.build(_gsm3);
				_house3.build(_gsm4);
				_house4.build(_gsm5);
				_cloud.build(_gsm6);
                 redraw(); 
                 break;
				 /* I don't think we should keep this functionality
      case 'p' : if ( !_bridge.phong() )
                  { std::cout<<"Switching to phong shader...\n";
                    _bridge.phong(true);
					_house1.phong(true);
					_house2.phong(true);
					_house3.phong(true);
					_house4.phong(true);
					_cloud.phong(true);
                  }
                 redraw(); 
                 break;
      case 'g' : if ( _bridge.phong() )
                  { std::cout<<"Switching to gouraud shader...\n";
                    _bridge.phong(false);
					_house1.phong(false);
					_house2.phong(false);
					_house3.phong(false);
					_house4.phong(false);
                  }
                 redraw(); 
                 break;
				 */
	  case 'i': ty += 0.05f; redraw(); break;
	  case 'k': ty -= 0.05f; redraw(); break;
	  case 'j': tx -= 0.05f; redraw(); break;
	  case 'l': tx += 0.05f; redraw(); break;
	  case '7': tz -= 0.05f; redraw(); break;
	  case '8': tz += 0.05f; redraw(); break;

	  case 'n': 
		  /*****************Hold n (or whatever button you want to use) down***********************/
		  /*****************Don't know how to get it to loop because it only redraws at the end****/
		  //for (int i = 0; i < _cameraInterpolation.size(); ++i) {
			  //std::cout << "moving\n";
			  cam.move();
			  redraw();
		  //}
		  break;

      default : loadModel ( int(key-'0') );
                break;
	}
 }

void AppWindow::glutSpecial ( int key, int x, int y )
 {
   bool rd=true;
   const float incr=GS_TORAD(2.5f);
   const float incf=0.05f;
   switch ( key )
    { case GLUT_KEY_LEFT:      _roty-=incr; break;
      case GLUT_KEY_RIGHT:     _roty+=incr; break;
      case GLUT_KEY_UP:        _rotx+=incr; break;
      case GLUT_KEY_DOWN:      _rotx-=incr; break;
      case GLUT_KEY_PAGE_UP:   _fovy-=incf; break;
      case GLUT_KEY_PAGE_DOWN: _fovy+=incf; break;
      default: return; // return without rendering
	}
   if (rd) redraw(); // ask the window to be rendered when possible
 }

void AppWindow::glutMouse ( int b, int s, int x, int y )
 {
   // The mouse is not used in this example.
   // Recall that a mouse click in the screen corresponds
   // to a whole line traversing the 3D scene.
 }

void AppWindow::glutMotion ( int x, int y )
 {
 }

void AppWindow::glutMenu ( int m )
 {
   std::cout<<"Menu Event: "<<m<<std::endl;
 }

void AppWindow::glutReshape ( int w, int h )
 {
   // Define that OpenGL should use the whole window for rendering
   glViewport( 0, 0, w, h );
   _w=w; _h=h;
 }

// here we will redraw the scene according to the current state of the application.
void AppWindow::glutDisplay ()
 {
   // Clear the rendering window
   glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   // Build a cross with some lines (if not built yet):
   if ( _axis.changed ) // needs update
    { _axis.build(1.0f); // axis has radius 1.0
    }

   // Build ground (if not built yet):
   if (_ground.changed) // needs update
   {
	   _ground.build(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f); // xz plane at y=0.0f
   }

   // Define our scene transformation:
   GsMat rx, ry, stransf;
   rx.rotx ( _rotx );
   ry.roty ( _roty );
   stransf = rx*ry; // set the scene transformation matrix

   // Define our projection transformation:
   // (see demo program in gltutors-projection.7z, we are replicating the same behavior here)
   GsMat persp, sproj;

   //GsVec eye(tx, ty, tz), center(tx,ty,tz-2), up(0,1,0);
   //camview.lookat ( eye, center, up ); // set our 4x4 "camera" matrix

   float aspect=1.0f, znear=0.1f, zfar=50.0f;
   persp.perspective ( _fovy, aspect, znear, zfar ); // set our 4x4 perspective matrix

   // Our matrices are in "line-major" format, so vertices should be multiplied on the 
   // right side of a matrix multiplication, therefore in the expression below camview will
   // affect the vertex before persp, because v' = (persp*camview)*v = (persp)*(camview*v).

   sproj = persp * cam.getCamview(); // set final scene projection

   //  Note however that when the shader receives a matrix it will store it in column-major 
   //  format, what will cause our values to be transposed, and we will then have in our 
   //  shaders vectors on the left side of a multiplication to a matrix.

   //Make all the translations
   translation(_transBridge, -3.75f, 3.5f, 4.0f);
   translation(_transHouse1, -2.0f, 0.0f, -0.6f);
   translation(_transHouse2, 0.25f, 0.0f, -3.15f);
   translation(_transHouse3, -2.0f, 0.0f, -3.15f);
   translation(_transHouse4, 0.25f, 0.0f, -0.83f);

   //Make all the rotations
   rotation(_rotBridge, PI);
   rotation(_rotHouse1, PI);
   rotation(_rotHouse4, PI);

   // Draw:
   if (_viewaxis) _axis.draw(stransf, sproj);

   _bridge.draw(stransf*_transBridge*_rotBridge, sproj, _light);
   _house1.draw(stransf*_transHouse1*_rotHouse1, sproj, _light);
   _house2.draw(stransf*_transHouse2, sproj, _light);
   _house3.draw(stransf*_transHouse3, sproj, _light);
   _house4.draw(stransf*_transHouse4*_rotHouse4, sproj, _light);
   _cloud.draw(stransf, sproj, _light);
   //_lines.draw(stransf, sproj);
   _ground.draw(stransf, sproj, _light);

   // Swap buffers and draw:
   glFlush();         // flush the pipeline (usually not necessary)
   glutSwapBuffers(); // we were drawing to the back buffer, now bring it to the front
}

void AppWindow::translation(GsMat &translate, float x, float y, float z)
{
	translate.setl1(1.0f, 0.0f, 0.0f, x);
	translate.setl2(0.0f, 1.0f, 0.0f, y);
	translate.setl3(0.0f, 0.0f, 1.0f, z);
	translate.setl4(0.0f, 0.0f, 0.0f, 1.0f);
}

void AppWindow::rotation(GsMat &rotate, float theta)
{
	rotate.setl1(cos(theta), 0.0f, sin(theta), 0.0f);
	rotate.setl2(0.0f, 1.0f, 0.0f, 0.0f);
	rotate.setl3(-sin(theta), 0.0f, cos(theta), 0.0f);
	rotate.setl4(0.0f, 0.0f, 0.0f, 1.0f);
}