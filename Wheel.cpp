#include "Wheel.h"



void Wheel::rotate(int dir) {
	switch (dir) {
	case back:
		currentRot += 4;
		break;
	case forward:
		currentRot -= 4;
		break;
	}
}

//void Wheel::setColor(MyMesh mesh[10]) {
//	Object::setColor(mesh,amb,diff,spec,emissive,shininess,texcount);
//}
//
//void Wheel::createObj() {
//	createTorus(0.1f, 0.2f, 15, 5);
//}