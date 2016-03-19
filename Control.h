#ifndef CONTROL_H
#define CONTROL_H

#include "GLEngine.h"
#include <list>
#include <string>
using std::list;
using std::string;
struct MouseState
{
	int LeftButtonDown;
	int RightButtonDown;
	int MiddelButtonDown;

	int x;
	int y;
	MouseState()
	{
		LeftButtonDown = 0;
		RightButtonDown = 0;
		MiddelButtonDown = 0;
		
		x = 0;
		y = 0;
	};
};

class Control
{
public:
	Control(int positionX, int positionY, int width, int height);
	virtual ~Control();

	virtual bool updateControl(MouseState &state);
	virtual void drawControl(void) = 0;
	virtual string getType(void) = 0;

	void setPosition(int x, int y);
	void setSize(int width, int height);

	int getWidth(void);
	int getHeight(void);
public:
	static list<Control *> controls;
private:
protected:
	bool inside;
	int  posX, posY;
	int  width, height;

};
Control *addControl(Control *control);
#endif 