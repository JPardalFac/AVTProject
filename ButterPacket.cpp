#include "ButterPacket.h"

ButterPacket::ButterPacket()
{
}

ButterPacket::ButterPacket(int id, float pos[3], float rotAxis[3], float rot)
{
	Object::init(id, pos, rotAxis, rot);
}

void ButterPacket::collided()
{
}
