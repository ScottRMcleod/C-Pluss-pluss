#ifndef	APPFRAME_H
#define APPFRAME_H

#include "stdwx.h"
#include <math.h>
#include "drawEngine.h"
#include "level.h"
#include "mage.h"
#include "pickup.h"

enum ID_List
{
	ID_New = 400,
	ID_Load,
	ID_About,
	ID_Exit,

	ID_Timer
};
enum pickupItems
{
	MANAPOTION = 0
};
enum LevelType
{
	RANDOM_NEW_LEVEL = 0,
	NEXT_PACKAGE_LEVEL
};
enum GameState
{
	STATE_NULL = 0,
	STATE_GAME_IN_PROGRESS,
	STATE_GAME_OVER,
	STATE_PLAYER_WON
};
struct Level_Info
{
	int grid_x;
	int grid_y;
	char **grid;
};
typedef list<Level_Info *> Package;
class AppFrame : public wxFrame
{
public:
	AppFrame(const wxString &title, const wxPoint &pos, const wxSize &size, wxString filename = "", int startingLevel = 0);
	~AppFrame();

	void OnNew(wxCommandEvent &event);
	void OnLoad(wxCommandEvent &event);
	void OnAbout(wxCommandEvent &event);
	void OnExit(wxCommandEvent &event);
	//void Pause();
	void OnSize(wxSizeEvent &event);
	void OnPaint(wxPaintEvent &event);
	void OnTimer(wxTimerEvent &event);
	void OnKey(wxKeyEvent &event);

private:
	bool startNewLevel(LevelType type);
	void updateView(void);
	void updateGame(void);

	void drawInformation(void);
	void stretchGameView(void);
	void flipBackbuffer(void);

	void loadGame(wxString filename, int startingLevel = 1);
private:

	//controls
	wxPanel *m_panel;

	wxPanel *m_info_panel;

	wxWindow *gameWindow;
	wxStaticText *stPlayerLives;
	wxStaticText *stCurrentLevel;
	wxStaticText *stNumEnemies;
	wxStaticText *stManaLevel;

	wxTimer *m_timer;
	//variables
	Level *level;

	Mage *player;
	DrawEngine drawArea;

	wxBitmap *finalBackBuffer;

	wxBitmap *backBuffer;

	int currentLevel;

	GameState gameState;
	
	Package *package;

	bool packageLoaded;
	bool isStarted;
	bool isPaused;
private:
	DECLARE_EVENT_TABLE()
};
#endif