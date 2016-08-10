#ifndef APPFRAME_H
#define APPFRAME_H

#include "wx/wx.h"
#include "wx/image.h"
#include <list>
using namespace std;
enum Menu_IDs
{
	ID_New_Package = 400,
	ID_New_Level,
	ID_Load,
	ID_Save,
	ID_About,
	ID_Exit
};

enum Toolbar_IDs
{
	TLB_WALL = 500,
	TLB_PLAYER,
	TLB_ENEMY,
	TLB_POTION,
	TLB_ERASE,
	TLB_PREVIOUS_LEVEL,
	TLB_NEXT_LEVEL,
	TLB_TEST_LEVEL

};
enum Tile_IDs
{
	TILE_EMPTY,
	TILE_WALL,
	TILE_PLAYER,
	TILE_ENEMY,
	TILE_POTION
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
	AppFrame(const wxString &title, const wxPoint &pos, const wxSize &size);
	
	void OnNewPackage(wxCommandEvent &event);
	void OnNewLevel(wxCommandEvent &event);
	void OnSave(wxCommandEvent &event);
	void OnLoad(wxCommandEvent &event);
	void OnAbout(wxCommandEvent &event);
	void OnExit(wxCommandEvent &event);

	void OnToolBarClicked(wxCommandEvent &event);
	void OnSize(wxSizeEvent &event);
	void OnPaint(wxPaintEvent &event);
	
	void OnMotion(wxMouseEvent &event);
	void OnMouseLeft(wxMouseEvent &event);

private:
	bool createNewLevel(void);
	void setCurrentLevel(Level_Info *level);
	void updateView(void);
	void flipBackBuffer(void);
	void stretchGameView(void);
	void savePackage(wxString filename);
private:
	//Controls
	wxWindow *gameWindow;
	wxToolBar *toolbar;
	wxMenu *menuFile;

	//Game variables
	Package *package;
	int levelIndex;
	Level_Info *currentLevel;

	wxBitmap items[5];
	wxBitmap *backBuffer;
	wxBitmap *finalBackBuffer;
	
	int pos_x, pos_y;
	int grid_width, grid_height;
	int mouse_grid_x, mouse_grid_y;

private:
	DECLARE_EVENT_TABLE()
};
#endif