#include "Control.h"

list<Control *> Control::controls;
Control::Control(int positionX, int positionY, int w, int h)
{
	controls.push_back(this);
	posX = positionX;
	posY = positionY;

	width = w;
	height = h;
}
Control::~Control()
{
	controls.remove(this);
}
bool Control::updateControl(MouseState &state)
{
	int x = state.x;
	int y = state.y;

	inside = false;
	if(x >= posX && x <=posX + width &&
	   y >= posY && y <= posY + height )
	{
		inside = true;
	}

	return false;

}
void Control::setPosition(int x, int y)
{
	posX = x;
	posY = y;
}
void Control::setSize(int w, int h)
{
	width = w;
	height = h;
}
int Control::getWidth(void)
{
	return width;
}
int Control::getHeight(void)
{
	return height;
}
Control *addControl(Control *control)
{
	static int lastX = 5;
	static int lastY = 5;

	control->setPosition(lastX, lastY);
	lastY += control->getHeight() + 5;

	return control;

}