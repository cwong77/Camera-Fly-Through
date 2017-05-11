# include <iostream>
# include <gsim/gs.h>
# include "app_window.h"


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
   degrees = 0;
   rotatey(rotd, degrees);
   translation(transd, -.05*cos(degrees), 0, .05*sin(degrees));
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
	//starting point
	/*
	_cameraControlPoints.push(GsVec(5.0, 0.25, -2.0));
	_cameraControlPoints.push(GsVec(3.00, .5, -2.0));
	_cameraControlPoints.push(GsVec(2.5, .5, -2.0));
	_cameraControlPoints.push(GsVec(1.0, .5, -2.0));
	_cameraControlPoints.push(GsVec(-1.0, .5, -2.0));
	_cameraControlPoints.push(GsVec(-1.0, .5, 1.0));
	_cameraControlPoints.push(GsVec(-1.0, .5, 2.0));
	*/
	_cameraControlPoints.push(GsVec(-2.0, .5, 2.0));
	_cameraControlPoints.push(GsVec(-4.0, .01, 2.0));
	_cameraControlPoints.push(GsVec(-3.0, .05, 1.5));
	_cameraControlPoints.push(GsVec(-2.3, .05, .75));
	
	//second floor
	_cameraControlPoints.push(GsVec(-2.3, .35, 1.9));
	_cameraControlPoints.push(GsVec(-2.2, .35, 1.9));

	_cameraControlPoints.push(GsVec(-1.7, .35, 1.9));
	_cameraControlPoints.push(GsVec(-1.6, .35, 1.9));

	//out the second door
	
	_cameraControlPoints.push(GsVec(-1.7, .35, 1.4));
	/*
	_cameraControlPoints.push(GsVec(-1.7, .35, 1.2));
	_cameraControlPoints.push(GsVec(-1.63, .35, 1.2));
	_cameraControlPoints.push(GsVec(-1.7, .35, 1.3));
	_cameraControlPoints.push(GsVec(-1.65, .35, 1.9));
	_cameraControlPoints.push(GsVec(-1.55, .35, 1.9));
	
	//third floor
	_cameraControlPoints.push(GsVec(-1.55, .65, 1.4));
	_cameraControlPoints.push(GsVec(-1.55, .65, 1.2));

	_cameraControlPoints.push(GsVec(-1.7, .65, 1.3));
	_cameraControlPoints.push(GsVec(-1.7, .65, 1.9));

	_cameraControlPoints.push(GsVec(-1.65, .65, 1.9));
	_cameraControlPoints.push(GsVec(-1.55, .65, 1.9));
	
	_cameraControlPoints.push(GsVec(-1.55, .65, 1.8));
	_cameraControlPoints.push(GsVec(-1.55, .95, 1.3));
	
	//top floor
	_cameraControlPoints.push(GsVec(-1.55, .95, 1.2));
	_cameraControlPoints.push(GsVec(-1.7, .95, 1.2));
	_cameraControlPoints.push(GsVec(-1.7, .95, 2.5));
	//the great jump
	_cameraControlPoints.push(GsVec(-1.7, 2.0, 3.0));
	_cameraControlPoints.push(GsVec(-1.5, 1.5, 3.5));
	_cameraControlPoints.push(GsVec(-1.5, 1.5, 4.0));
	*/



	/***************************************************************/

	//interpolate them
	float interval = (float)(_cameraControlPoints.size()) / 2048;
	for (float i = 2; i < _cameraControlPoints.size(); i += interval){
		_cameraInterpolation.push(_cameraPath.eval_bspline(i, 3, _cameraControlPoints));
	}

	//for (int i = 0; i < _cameraInterpolation.size(); ++i) {
	//	std::cout << _cameraInterpolation[i] << std::endl;
	//}

	//give them to the camera
	cam.init(_cameraInterpolation);
}

void AppWindow::loadFlyCurve() {
	//for balloon that loops
	_flyControlPoints.push(GsVec(-1.0, 4.0, 0.0));
	_flyControlPoints.push(GsVec(0.0, 4.0, 1.0));
	_flyControlPoints.push(GsVec(1.0, 4.0, 0.0));
	_flyControlPoints.push(GsVec(0.0, 4.0, -1.0));
	_flyControlPoints.push(GsVec(-1.0, 4.0, 0.0));
	_flyControlPoints.push(GsVec(0.0, 4.0, 1.0));

	float interval = (float)(_flyControlPoints.size()) / 2048;
	for (float i = 2; i < _flyControlPoints.size(); i += interval) {
		_flyInterpolation.push(_cameraPath.eval_bspline(i, 3, _flyControlPoints));
	}

	//for balloon that goes back and forth
	_flyControlPoints2.push(GsVec(-7.5, 4.0, -2.5));
	_flyControlPoints2.push(GsVec(-5.0, 4.0, 0.0));
	_flyControlPoints2.push(GsVec(0.0, 4.0, 0.0));
	_flyControlPoints2.push(GsVec(0.0, 4.0, 5.0));
	_flyControlPoints2.push(GsVec(5.5, 4.0, 5.5));

	interval = (float)(_flyControlPoints2.size()) / 2048;
	for (float i = 2; i < _flyControlPoints2.size(); i += interval) {
		_flyInterpolation2.push(_cameraPath.eval_bspline(i, 3, _flyControlPoints2));
	}
}

void AppWindow::initPrograms ()
 {

	musicplayer.init();

   // Init my scene objects:
   _axis.init ();
   _bridge.init("../models/Texture.png");
   _house1.init("../models/House_1.png");
   _house2.init("../models/House_2.png");
   _house3.init("../models/House_3.png");
   _house4.init("../models/House_4.png");
   _door1.init("../models/Porta_casa.png");
   _house5.init();
   _balloon.init("../models/Stone_Brushed_Khaki.png");
   _balloon2.init("../models/blue.png");

   _ground.init("../models/Ground.png");
   _skytop.init("../models/sky.png");
   _skyback.init("../models/sky.png");
   _skyleft.init("../models/sky.png");
   _skyright.init("../models/sky.png");
   _skyfront.init("../models/sky.png");

   // set light:
   _light.set ( GsVec(0,5,10), GsColor(90,90,90,255), GsColor::white, GsColor::white );

   _lines.init();	//normal lines for testing purposes
   _curveVisualization.init();
   _flyVisualization.init();
   _flyVisualization2.init();
   loadCameraCurve();	//initializes the camera with a curve to follow
   loadFlyCurve();
   rotd.identity();
   _lines.build(_house1.NL, GsColor::red);
   _curveVisualization.build(_cameraInterpolation, GsColor::green);
   _flyVisualization.build(_flyInterpolation, GsColor::red);
   _flyVisualization2.build(_flyInterpolation2, GsColor::white);


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

   GsString file1, file2, file3, file4, file5, file6, file7, file8;
   f = 0.4f; 
   file1 = "../models/Bridges.obj";
   file2 = "../models/House_1.obj";
   file3 = "../models/House_2.obj";
   file4 = "../models/House_3.obj";
   file5 = "../models/House_4.obj";
   file6 = "../models/Door.obj";
   file7 = "../models/Hot_Air_Balloon.obj";
   file8 = "../models/Tower-House Design.obj";

   

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
   //std::cout << "Loading " << file5 << "...\n";
   if (!_gsm6.load(file6)) std::cout << "Error!\n";
   //printInfo(_gsm5);
   _gsm6.scale(.01f); // to fit our camera space
   _door1.build(_gsm6);

   //std::cout << "building cloud " << file7 << "...\n";
   if (!_gsm7.load(file7)) std::cout << "Error!\n";
   //printInfo(_gsm6);
   _gsm7.scale(.001f); // to fit our camera space
   _balloon.build(_gsm7);

   //std::cout << "building cloud " << file7 << "...\n";
   if (!_gsm7.load(file7)) std::cout << "Error!\n";
   //printInfo(_gsm6);
   _gsm7.scale(.001f); // to fit our camera space
   _balloon2.build(_gsm7);

   if (!_gsm8.load(file8)) std::cout << "Error!\n";
   //printInfo(_gsm6);
   _gsm8.scale(.1f); // to fit our camera space
   _house5.build(_gsm8);

   _lines.build(_house2.NL, GsColor::red);
   _curveVisualization.build(_cameraInterpolation, GsColor::green);

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
	  case 'q': degrees+=5;
		  rotatey(rotd, degrees);
		  theta = degrees * PI / 180;
		  translation(transd, -.05*cos(theta), 0, .05*sin(theta));
		  redraw();
		  break;
	  case 'a': degrees -= 5;
		  rotatey(rotd, degrees);
		  theta = degrees * PI / 180;
		  translation(transd, -.05*cos(theta), 0, .05*sin(theta));
		  redraw();
		  break;
      case 's' : std::cout<<"Smoothing normals...\n";
                 _gsm1.smooth (GS_TORAD(35));
				 _gsm2.smooth(GS_TORAD(35));
				 _gsm3.smooth(GS_TORAD(35));
				 _gsm4.smooth(GS_TORAD(35));
				 _gsm5.smooth(GS_TORAD(35));
				 _gsm7.smooth(GS_TORAD(35));
                 _bridge.build(_gsm1);
				 _house1.build(_gsm2);
				 _house2.build(_gsm3);
				 _house3.build(_gsm4);
				 _house4.build(_gsm5);
				 _balloon.build(_gsm7);
				 
                 redraw(); 
                 break;
      case 'f' : std::cout<<"Flat normals...\n";
                _gsm1.flat();
				_gsm2.flat();
				_gsm3.flat();
				_gsm4.flat();
				_gsm5.flat();
				_gsm7.flat();
				_bridge.build(_gsm1);
				_house1.build(_gsm2);
				_house2.build(_gsm3);
				_house3.build(_gsm4);
				_house4.build(_gsm5);
				_balloon.build(_gsm7);

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
	  case 'j': tx -= 0.05f; redraw(); /*std::cout << "tx: " << tx << std::endl;*/ break;
	  case 'l': tx += 0.05f; redraw(); break; 
	  case '7': tz -= 0.05f; redraw(); /*std::cout << "tz: " << tz << std::endl;*/ break;
	  case '8': tz += 0.05f; redraw(); break;

	  case 'n': 
		  /*****************Hold n (or whatever button you want to use) down***********************/
		  /*****************Don't know how to get it to loop because it only redraws at the end****/
		  //for (int i = 0; i < _cameraInterpolation.size(); ++i) {
			  //std::cout << "moving\n";
			  //cam.move();
			  //redraw();
		  //}
		  moveCamera = true;
		  break;
		
	  //pause curve movement and look around
	  case 'r':
		  //musicplayer.raiseVolume();
		  /*
		  for (float i = 0; i < 2 * PI; i += interval) {
			  cam.rotate(i);
			  redraw();
		  }
		  */
		  
		  moveCamera = false;	//pause camera movement
		  rotateCam = true;		//start rotating

		  /*
		  std::cout << "b4 parameter: " << parameter << std::endl;
		  parameter += interval;
		  std::cout << "after parameter: " << parameter << std::endl;

		  cam.rotate(parameter);
		  redraw();
		  */
		  break;
		  
	  case 'p':
		  pause = !pause;
		  break;

      default : loadModel ( int(key-'0') );
                break;
	}
 }

void AppWindow::glutIdle() {
	//flying stuff
	//for balloon that goes back and forth
	if (flyIndex2 >= _flyInterpolation2.size())	//if we are going past our curve, go backwards through the curve
		flyBackwards = true;
	else if (flyIndex2 < 0)	//go forwards
		flyBackwards = false;
	if (!flyBackwards)
		flyIndex2++;
	else
		flyIndex2--;
	
	fly(_fly2, _flyInterpolation2[flyIndex2]);

	//for balloon that loops
	if (flyIndex >= _flyInterpolation.size())
		flyIndex = 0;

	fly(_fly, _flyInterpolation[flyIndex++]);

	//camera stuff
	//cam.observe(_flyInterpolation[flyIndex]);
	
	if (moveCamera && !pause) {
		cam.move();
	}
	if (rotateCam && parameter < 2*PI) {
		//std::cout << "rotating\n";
		parameter += interval;	//increment parameter
		cam.rotate(parameter);	//rotate camera
	}
	else if (parameter > 2 * PI) {	//once we have fully looped
		//std::cout << "done rotating\n";
		rotateCam = false;	//stop rotating
		parameter = 0;		//reset parameter
		//moveCamera = true;	//start moving camera again
	}
	redraw();
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
   if (_skytop.changed) // needs update
   {
	   _skytop.build(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f); // xz plane at y=0.0f
   }
   if (_skyfront.changed) // needs update
   {
	   _skyfront.build(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f); // xz plane at y=0.0f
   }
   if (_skyback.changed) // needs update
   {
	   _skyback.build(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f); // xz plane at y=0.0f
   }
   if (_skyleft.changed) // needs update
   {
	   _skyleft.build(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f); // xz plane at y=0.0f
   }
   if (_skyright.changed) // needs update
   {
	   _skyright.build(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f); // xz plane at y=0.0f
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
   //translation(_transballoon, 0.0f, 1.0f, 0.0f);
   translation(_transHouse5, -2.0f, 0.0f, 1.5f);
   //translation(_transballoon, 0.0f, 1.0f, 0.0f);
   translation(_transskytop, 0.0f, 6.0f, 0.0f);
   translation(_transskyfront, 0.0f, 0.0f, 10.0f);
   translation(_transskyback, 0.0f, 0.0f, -10.0f);
   translation(_transskyleft, 10.0f, 0.0f, 0.0f);
   translation(_transskyright, -10.0f, 0.0f, 0.0f);
   translation(location, -1.63f, .325f, 1.55f);

   //Make all the rotations
   rotationx(_rotskyfront, PI / 2);
   rotationx(_rotskyback, PI / 2);
   rotationy(_rotBridge, PI);
   rotationy(_rotHouse1, PI);
   rotationy(_rotHouse4, PI);
   rotationz(_rotskyleft, PI / 2);
   rotationz(_rotskyright, PI / 2);

   // Draw:
   if (_viewaxis) _axis.draw(stransf, sproj);

   _bridge.draw(stransf*_transBridge*_rotBridge, sproj, _light);
   _house1.draw(stransf*_transHouse1*_rotHouse1, sproj, _light);
   _house2.draw(stransf*_transHouse2, sproj, _light);
   _house3.draw(stransf*_transHouse3, sproj, _light);
   _house4.draw(stransf*_transHouse4*_rotHouse4, sproj, _light);
   _balloon.draw(stransf*_fly, sproj, _light);
   _balloon2.draw(stransf*_fly2, sproj, _light);
   _door1.draw(stransf*location*transd*rotd, sproj, _light);
   _house5.draw(stransf*_transHouse5, sproj, _light);

   //_lines.draw(stransf, sproj);
   _flyVisualization.draw(stransf, sproj);
   _flyVisualization2.draw(stransf, sproj);
	_curveVisualization.draw(stransf, sproj);
   _ground.draw(stransf, sproj, _light);
   _skytop.draw(stransf*_transskytop, sproj, _light);
   _skyback.draw(stransf*_transskyback*_rotskyback, sproj, _light);
   _skyfront.draw(stransf*_transskyfront*_rotskyfront, sproj, _light);
   _skyleft.draw(stransf*_transskyleft*_rotskyleft, sproj, _light);
   _skyright.draw(stransf*_transskyright*_rotskyright, sproj, _light);

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

void AppWindow::rotatey(GsMat &rotatey, int degrees)
{
	float thetay = degrees* PI / 180;
	rotatey.setl1(cos(thetay), 0.0f, sin(thetay), 0.0f);
	rotatey.setl2(0.0f, 1.0f, 0.0f, 0.0f);
	rotatey.setl3(-sin(thetay), 0.0f, cos(thetay), 0.0f);
	rotatey.setl4(0.0f, 0.0f, 0.0f, 1.0f);

}
void AppWindow::rotationx(GsMat &rotate, float theta)
{
	rotate.setl1(1.0f, 0.0f, 0.0f, 0.0f);
	rotate.setl2(0.0f, cos(theta), -sin(theta), 0.0f);
	rotate.setl3(0.0f, sin(theta), cos(theta), 0.0f);
	rotate.setl4(0.0f, 0.0f, 0.0f, 1.0f);
}
void AppWindow::rotationy(GsMat &rotate, float theta)
{
	rotate.setl1(cos(theta), 0.0f, sin(theta), 0.0f);
	rotate.setl2(0.0f, 1.0f, 0.0f, 0.0f);
	rotate.setl3(-sin(theta), 0.0f, cos(theta), 0.0f);
	rotate.setl4(0.0f, 0.0f, 0.0f, 1.0f);
}
void AppWindow::rotationz(GsMat &rotate, float theta)
{
	rotate.setl1(cos(theta), -sin(theta), 0.0f, 0.0f);
	rotate.setl2(sin(theta), cos(theta), 0.0f, 0.0f);
	rotate.setl3(0.0f, 0.0f, 1.0f, 0.0f);
	rotate.setl4(0.0f, 0.0f, 0.0f, 1.0f);
}

void AppWindow::fly(GsMat &fly, GsVec flyHere) {
	fly.setl1(1.0f, 0.0, 0.0, flyHere.x);
	fly.setl2(0.0, 1.0, 0.0, flyHere.y);
	fly.setl3(0.0, 0.0, 1.0, flyHere.z);
	fly.setl4(0.0, 0.0, 0.0, 1.0);
}