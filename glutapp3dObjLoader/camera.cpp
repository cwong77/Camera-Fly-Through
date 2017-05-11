#include "camera.h"

Camera::Camera() {

}

GsMat& Camera::getCamview() {
	return camview;
}

void Camera::init(GsArray<GsVec> _interpolation) {
	eyePosX = 0.0;
	eyePosY = 0.0;
	eyePosZ = 2.0f;


	up = GsVec(0.0, 1.0, 0.0);	//orientation of eye

	interpolation = _interpolation;
	camIndex = 0;

	eye = interpolation[camIndex];
	center = interpolation[++camIndex];

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
	camview.lookat(eye, center, up);
}

//move by following the curve
void Camera::move() {
	//for (int i = 0; i < interpolation.size(); ++i) {
	//	eye = interpolation[i];	//move to next point
	//	lookForward(i);	//fix direction of look
	//}
	
	//std::cout << "index: " << camIndex << std::endl;
	//std::cout << "where i am: " << eye << "what im looking at: " << center << std::endl;
	if (camIndex < interpolation.size())
		eye = interpolation[camIndex];
	else {
		camIndex = 0;	//reset the camera
		eye = interpolation[camIndex];
	}
	//std::cout << "where i am now: " << eye << "what i am looking at now: " << center << std::endl;
	lookForward(++camIndex);	//also changes camview
	
}

void Camera::rotate(float t) {
	//std::cout << "b4 center: " << center << std::endl;
	GsVec pos = interpolation[camIndex];
	center = GsVec(pos.x + cos(t), pos.y, pos.z + sin(t));
	//std::cout << "after center: " << center << std::endl;
	camview.lookat(eye, center, up);
}

/*
void Camera::rotate360() {
	float interval = (2 * PI) / 60;
	for (float i = 0; i < 2 * PI; i += interval) {
		rotate(i);
	}
}
*/

void Camera::observe(GsVec lookHere) {
	center = lookHere;
	camview.lookat(eye, center, up);
}

Camera::~Camera() {

}

