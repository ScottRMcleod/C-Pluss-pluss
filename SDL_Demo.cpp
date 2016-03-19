#include "GLEngine.h"
#include "ListBox.h"
#include "Button.h"
#include "Slider.h"

#include "Cube.h"
#include "Plane.h"

const GLsizei windowWidth = 800;
const GLsizei windowHeight = 650;

const float rotateX_speed = 0.5f;
const float rotateY_speed = 0.5f;
const float zoom_speed = 1.0f;

GLfloat rotateX = 45.0f;
GLfloat rotateY = 45.0f;
GLfloat zoom = -56.0f;

Uint8 *keys = NULL; 

bool navigating = false;
Control *controlled = NULL;
MouseState state;

Button *btnCreateCube, *btnCreatePlane;

Slider *sldTranslateX, *sldTranslateY, *sldTranslateZ;
Slider *sldRotateX, *sldRotateY, *sldRotateZ;
Slider *sldScaleX, *sldScaleY, *sldScaleZ;

Button *btnParent, *btnUnparent;

Mesh *selectedNode = NULL;
list<Mesh *> selectedItems;

GLvoid establishProjectionMatrix(GLsizei width, GLsizei height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 500.0f);
}
GLvoid setOrtho(GLsizei width, GLsizei height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(0,width,height,0);
}
void updateControls(void)
{
	sldTranslateX->setValue(&selectedNode->getTransform().translate.x);
	sldTranslateY->setValue(&selectedNode->getTransform().translate.y);
	sldTranslateZ->setValue(&selectedNode->getTransform().translate.z);

	sldRotateX->setValue(&selectedNode->getTransform().rotation.x);
	sldRotateY->setValue(&selectedNode->getTransform().rotation.y);
	sldRotateZ->setValue(&selectedNode->getTransform().rotation.z);

	sldScaleX->setValue(&selectedNode->getTransform().scale.x);
	sldScaleY->setValue(&selectedNode->getTransform().scale.y);
	sldScaleZ->setValue(&selectedNode->getTransform().scale.z);
}
GLvoid initGL(GLsizei width, GLsizei height)
{
	iGLEngine->Intialise(width,height);
	
	establishProjectionMatrix(width, height);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f,0.0f,0.0f,1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnable(GL_PERSPECTIVE_CORRECTION_HINT);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);

	Light *mainLight = NULL;
	
	mainLight = new Light(LIGHT_POINT);
	mainLight->setDiffuse(1.0f, 1.0f, 1.0f, 1.0f);
	mainLight->setPosition(9.34f,150.857f,23.251f);

	Plane *plane = new Plane();
	plane->setDivisions(100);
	plane->setSize(180,180);
	plane->createPrimitive();
	plane->getTransform().setTranslate(0,-10,0);

	selectedNode = plane;

	btnCreateCube = (Button *)addControl(new Button("Create Cube",0,0,97,30));
	btnCreatePlane = new Button("Create Plane",108,5,97,30);

	sldTranslateX = (Slider *)addControl(new Slider("Translate X", -100.0f, 100.0f, 0, 0, 200,20));
	sldTranslateY = (Slider *)addControl(new Slider("Translate Y", -100.0f, 100.0f, 0, 0, 200,20));
	sldTranslateZ = (Slider *)addControl(new Slider("Translate Z", -100.0f, 100.0f, 0, 0, 200,20));

	sldRotateX = (Slider *)addControl(new Slider("Rotate X", -360.0f, 360.0f, 0, 0, 200,20));
	sldRotateY = (Slider *)addControl(new Slider("Rotate Y", -360.0f, 360.0f, 0, 0, 200,20));
	sldRotateZ = (Slider *)addControl(new Slider("Rotate Z", -360.0f, 360.0f, 0, 0, 200,20));

	sldScaleX = (Slider *)addControl(new Slider("Scale X", 0.1f, 10.0f, 0, 0, 200,20));
	sldScaleY = (Slider *)addControl(new Slider("Scale Y", 0.1f, 10.0f, 0, 0, 200,20));
	sldScaleZ = (Slider *)addControl(new Slider("Scale Z", 0.1f, 10.0f, 0, 0, 200,20));

	btnParent = (Button *)addControl(new Button("Parent",0,0,97,30));
	btnUnparent = new Button("Unparent",108,265,97,30);

	updateControls();
}
GLvoid displayFPS(GLvoid)
{
	static long lastTime = SDL_GetTicks();
	static long loops = 0;
	static GLfloat fps = 0.0f;

	int newTime = SDL_GetTicks();

	if(newTime - lastTime > 100)
	{
		float newFPS = (float)loops / float(newTime - lastTime) * 1000.0f;

		fps = (fps + newFPS) / 2.0f;

		lastTime = newTime;

		loops = 0;
	}

		iGLEngine->drawText(5,5,"OpenGL Demo - %.2f", fps);

	loops++;
}
GLvoid drawControls(GLvoid)
{
	for(list<Control *>::iterator it = Control::controls.begin(); it != Control::controls.end(); it++)
	{
		Control *control = (*it);

		control->drawControl();

		if((controlled != NULL && controlled != control))
			continue;
		if(control->updateControl(state))
		{
			controlled = control;

			if(control == btnCreateCube)
			{
				Cube *cube = new Cube();
				cube->createPrimitive();

				selectedNode = cube;

				selectedItems.clear();
				selectedItems.push_back(selectedNode);

				updateControls();
			}
			else if(control == btnCreatePlane)
			{
				Plane *plane = new Plane();
				plane->setDivisions(20);
				plane->setSize(10,10);
				plane->createPrimitive();

				selectedNode = plane;

				selectedItems.clear();
				selectedItems.push_back(selectedNode);

				updateControls();
			}
			else if(control == btnParent &&(int)selectedItems.size() > 1)
			{
				Mesh *parent = (*(--selectedItems.end()));

				for(list<Mesh *>::iterator it = selectedItems.begin(); it != selectedItems.end(); it++)
				{
					Mesh *mesh = (*it);

					if(mesh && mesh !=parent)
						mesh->setParent(NULL);
				}
				for(list<Mesh *>::iterator it = selectedItems.begin(); it != selectedItems.end(); it++)
				{
					Mesh *mesh = (*it);

					if(mesh && mesh !=parent)
						mesh->setParent(parent);
				}
			}
			else if(control == btnUnparent)
			{
				for(list<Mesh *>::iterator it = selectedItems.begin(); it != selectedItems.end(); it++)
				{
					Mesh *mesh = (*it);

					if(mesh)
						mesh->setParent(NULL);
				}
			}
		}
		else if(control == controlled)
		{
			controlled = NULL;
		}
	}
}
GLvoid drawScene(bool selection = false)
{
	if(selection == false)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		establishProjectionMatrix(windowWidth, windowHeight);
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0,0,zoom);
	glRotatef(rotateX, 1, 0, 0);
	glRotatef(rotateY, 0, 1, 0);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);

	for(int i = 0; i <(int)Light::lights.size(); i++)
	{
		Light::lights[i]->updateLight();
	}

	for(list<Node *>::iterator it = Node::getNodes().begin(); it !=Node::getNodes().end(); it++)
	{
		Mesh *mesh = dynamic_cast<Mesh *>(*it);

		if(mesh && mesh->getParent() == NULL)
		{
			mesh->drawMesh();
		}
	}

	list<Mesh *> items = selectedItems;
	while(items.size() > 0)
	{
		for(list<Node *>::iterator it = Node::getNodes().begin(); it !=Node::getNodes().end(); it++)
		{
			Mesh *mesh = dynamic_cast<Mesh *>(*it);

			if(mesh && mesh->getParent() == NULL)
			{
				for(list<Mesh *>::iterator mit = items.begin(); mit != items.end(); mit++)
				{
					Mesh *insideMesh = (*mit);

					glDisable(GL_LIGHTING);
					glColor4f(0.0f, 1.0f, 0.0f, 0.5f);

					if(mesh->drawMesh(true, insideMesh))
					{
						items.remove(insideMesh);
						break;
					}
				}
			}
		}
	}
	glEnable(GL_LIGHTING);
	glColor4f(1.0f, 1.0f, 1.0f, 0.2f);
	
	if(selection == true)
		return;
	
	//Pass 2
	
	glDisable(GL_LIGHTING);

	setOrtho(windowWidth, windowHeight);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	drawControls();
	displayFPS();
	glFlush();

	// this is modified in diffrent interfaces
	SDL_GL_SwapBuffers();
}
void deleteNode(Node *node)
{
	list<Node *> children = node->getChildren();

	for(list<Node *>::iterator it = children.begin(); it != children.end(); it++)
	{
		deleteNode((*it));
	}
	delete node;
}

GLboolean updateNavigation(GLvoid)
{
	static int lastX = -1;
	static int lastY = -1;

	if(lastX == -1 && lastY == -1 )
	{
		lastX = state.x;
		lastY = state.y;
	}

	int changeX = lastX - state.x;
	int changeY = lastY - state.y;

	lastX = state.x;
	lastY = state.y;

	if(state.LeftButtonDown && controlled == NULL)
	{
		SDL_WM_GrabInput(SDL_GRAB_ON);
		rotateX -=(float)changeY * rotateX_speed;
		rotateY -= (float)changeX * rotateY_speed;

		navigating = true;
	}
	else if(state.RightButtonDown && controlled ==NULL)
	{
		SDL_WM_GrabInput(SDL_GRAB_ON);
		zoom -=(float)changeX * zoom_speed;
		navigating = true;
	}
	else
	{
		SDL_WM_GrabInput(SDL_GRAB_OFF);
		navigating = false;
	}

	if(keys[SDLK_DELETE])
	{
		for(list<Mesh *>::iterator it = selectedItems.begin(); it != selectedItems.end(); it++)
		{
			Mesh *mesh = dynamic_cast<Mesh *>(*it);

			if(mesh == NULL)
				continue;

			mesh->setParent(NULL);
		}
		
		for(list<Mesh *>::iterator it = selectedItems.begin(); it != selectedItems.end(); it++)
		{
			Mesh *mesh = dynamic_cast<Mesh *>(*it);

			if(mesh == NULL)
				continue;

			deleteNode(mesh);
		}
		selectedItems.clear();
	}
	if (keys[SDLK_ESCAPE])
		return true;
	
	return false;
}
void handleSelections(void)
{
	static bool buttonDown = false;

	if(state.MiddelButtonDown)
	{
		buttonDown = true;
	}
	else if (buttonDown)
	{
		buttonDown = false;

		GLuint buffer[512];
		GLint viewport[4];

		glGetIntegerv(GL_VIEWPORT, viewport);
		glSelectBuffer(512,buffer);

		glRenderMode(GL_SELECT);
		glInitNames();

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();

		glLoadIdentity();

		gluPickMatrix(state.x, viewport[3] - state.y, 1.0f,1.0f,viewport);
		gluPerspective(45.0f, (GLfloat) windowWidth / (GLfloat) windowHeight,0.1f, 500.0f );

		drawScene(true);

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

		glMatrixMode(GL_MODELVIEW);
		int totalHits = glRenderMode(GL_RENDER);

		if(totalHits > 0)
		{
			int choose = buffer[3];
			int depth = buffer[1];

			for(int i = 1; i < totalHits; i++)
			{
				if(buffer[i * 4 +1] < (GLuint)depth)
				{
					choose = buffer[i * 4 + 3];
					depth = buffer[i * 4 + 1];
				}
			}

			selectedNode = reinterpret_cast<Mesh *>(choose);

			if(!keys[SDLK_LSHIFT])
			{
				selectedItems.clear();
			}
			bool exists = false;
			for(list<Mesh *>::iterator it = selectedItems.begin(); it != selectedItems.end(); it++ )
			{
				if((*it) == selectedNode)
				{
					exists = true;
				}
			}
			if(!exists)
			{
				selectedItems.push_back(selectedNode);
			}
			updateControls();
		}
		else
			selectedItems.clear();
	}
}
int main(int argc, char **argv)
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		fprintf(stderr, "Unable to start %s", SDL_GetError());
		exit(1);
	}
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);

	if(SDL_SetVideoMode(windowWidth, windowHeight, 0 , SDL_OPENGL) == NULL)
	{
		fprintf(stderr, "Unable to start %s", SDL_GetError());
		exit(2);
	}
	initGL(windowWidth, windowHeight);
	int done = 0;
	while(!done)
	{
		SDL_GetMouseState(&state.x, &state.y);

		state.LeftButtonDown = SDL_GetMouseState(NULL,NULL) & SDL_BUTTON(1);
		state.MiddelButtonDown = SDL_GetMouseState(NULL,NULL) & SDL_BUTTON(2);
		state.RightButtonDown = SDL_GetMouseState(NULL,NULL) & SDL_BUTTON(3);

		drawScene();

		handleSelections();
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			if(event.type == SDL_QUIT)
			{
				done = 1;
			}
			keys = SDL_GetKeyState(NULL);
		}
			if(updateNavigation())
				done = 1;
	}

	while(Node::getNodes().size() > 1)
		delete (*Node::getNodes().begin());

	while(Light::lights.size() > 1)
		delete (*Light::lights.begin());

	while(Control::controls.size() > 1)
		delete(*Control::controls.begin());

	GLEngine::Uninitialise();
	
	SDL_Quit();

	return 1;
}