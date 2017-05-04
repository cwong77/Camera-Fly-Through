#include "camera.h"

Camera::Camera() {

}

Camera::Camera(GsArray<GsVec>& ctrl) {
	controlpoints = ctrl;
}

GsMat& Camera::getCamview() {
	return camview;
}

void Camera::init() {
	eyePosX = 0.0;
	eyePosY = 0.0;
	eyePosZ = 2.0f;
	eye = GsVec(eyePosX, eyePosY, eyePosZ);	//position of eye
	center = GsVec(eyePosX, eyePosY, eyePosZ - 2.0f);	//what the eye is looking at
	up = GsVec(0.0, 1.0, 0.0);	//orientation of eye

	camview.lookat(eye, center, up);
}

//gets the points to fly-through
void Camera::eval_curve() {
	//interpolate points
	//use PA7
}

//look forward
//takes in the index of  current position in curve
void Camera::lookForward(int i) {
	center = interpolation[++i];	//look at where we are going next
}

//move by following the curve
void Camera::move() {
	for (int i = 0; i < interpolation.size(); ++i) {
		eye = interpolation[i];	//move to next point
		lookForward(i);	//fix direction of look
	}
}

Camera::~Camera() {

}

