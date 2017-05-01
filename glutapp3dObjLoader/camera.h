# include "glut_window.h"
# include "gsim/gs_mat.h"
# include <gsim/gs_array.h>


class Camera {
	float eyePosX, eyePosY, eyePosZ;

	GsMat camview;
	GsVec eye, center, up;

	GsArray<GsVec> controlpoints;
	GsArray<GsVec> interpolation;

public:
	Camera();
	GsMat& getCamview();
	void init();

	void setControlPoints();
	void eval_curve();

	void lookForward(int i);
	void move();
	~Camera();
};