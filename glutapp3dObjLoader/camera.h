# include "glut_window.h"
# include "gsim/gs_mat.h"
# include <gsim/gs_array.h>


class Camera {
	float eyePosX, eyePosY, eyePosZ;

	GsMat camview;
	GsVec eye, center, up;

	GsArray<GsVec> controlpoints;
	GsArray<GsVec> interpolation;

	int camIndex;	//where we are in the curve;

public:
	Camera();
	GsMat& getCamview();
	void init(GsArray<GsVec> interpolation);

	void eval_curve();

	void lookForward(int i);
	void move();
	~Camera();
};