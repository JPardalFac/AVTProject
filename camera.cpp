#include "camera.h"

void camera::setCameraType(CameraType newActiveCam)
{
	activeCam = newActiveCam;
}

//when camera is switch to MP, this functions  initializes the ortho matrix
void camera::setFixedOrtho()
{
	//loadIdentity(VIEW);
	loadIdentity(PROJECTION);
	ortho(orthoBox[0], orthoBox[1], orthoBox[2], orthoBox[3], orthoBox[4], orthoBox[5]);
}

//when camera is switch to FP, this functions initializes the projection matrix
void camera::setFixedPerspective(float* ratio)
{
	loadIdentity(PROJECTION);
	perspective(fixedPerspectiveFov, *ratio, 0.1f, 1000.0f);
}

//when camera is switch to MP, this functions initializes the projection matrix
void camera::setMovingPerspective(float* ratio)
{
	loadIdentity(PROJECTION);
	perspective(movingPerspectiveFov, *ratio, 0.1f, 1000.0f);
}

//updates lookAt values depending on the current active camera
void camera::updateLookAt(float carPos[])
{
	switch(activeCam){
		case FIXEDORTHO:
			lookAt(eye[0], eye[1], eye[2], at[0], at[1], at[2], up[0], up[1], up[2]);
		break; 
		case FIXEDPERSPECTIVE:
			lookAt(eye[0], eye[1], eye[2], at[0], at[1], at[2], up[0], up[1], up[2]);
		break;
		case MOVINGPERSPECTIVE:
			lookAt( carPos[0] - 5 + camRot[0], carPos[1] + 3 + camRot[1], carPos[2] + camRot[2],
					carPos[0], carPos[1], carPos[2],
					0,1,0);
			// carPos[0] + 6
		break;
	}
}

//updates the projection matrix, checking if the current camera is set to perspective or ortho
void camera::updateProjection(float * ratio)
{
	switch (activeCam) 
	{
		case(FIXEDORTHO):
			ortho(orthoBox[0], orthoBox[1], orthoBox[2], orthoBox[3], orthoBox[4], orthoBox[5]);
			break;
		case(FIXEDPERSPECTIVE):
			perspective(fixedPerspectiveFov, *ratio, 0.1f, 1000.0f);
			break;
		case(MOVINGPERSPECTIVE):
			perspective(movingPerspectiveFov, *ratio, 0.1f, 1000.0f);
			break;
	}
}

void camera::sendCamCoords(float x, float y, float z)
{
	camRot[0] = x;
	camRot[1] = std::max(y, 0.1f);
	camRot[2] = z;

}

camera::camera()
{
}


camera::~camera()
{
}
