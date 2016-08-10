#include "AppFrame.h"

#define TOOLBAR_SIZE 16
#define GRID_SIZE 32

BEGIN_EVENT_TABLE(AppFrame, wxFrame)
	EVT_MENU( ID_New_Package, AppFrame::OnNewPackage)
	EVT_MENU( ID_New_Level, AppFrame::OnNewLevel)
	EVT_MENU( ID_Save, AppFrame::OnSave)
	EVT_MENU( ID_Load, AppFrame::OnLoad)
	EVT_MENU( ID_About, AppFrame::OnAbout)
	EVT_MENU( ID_Exit, AppFrame::OnExit)
	
	EVT_TOOL_RANGE(	TLB_PREVIOUS_LEVEL, TLB_TEST_LEVEL, AppFrame::OnToolBarClicked)
END_EVENT_TABLE()

AppFrame::AppFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
: wxFrame((wxFrame*)NULL,-1,title,pos,size, wxDEFAULT_FRAME_STYLE | wxCLIP_CHILDREN)
{
	mouse_grid_x = mouse_grid_y = -1;

	finalBackBuffer = NULL;
	backBuffer = NULL;
	package = NULL;

	wxMenuBar *menuBar = new wxMenuBar;
	menuFile = new wxMenu;

	
	menuFile->Append(ID_New_Package, "New &Package");
	menuFile->Append(ID_New_Level, "&New Level");
	menuFile->AppendSeparator();

	menuFile->Append(ID_Load, "&Load");
	menuFile->Append(ID_Save, "&Save");
	menuFile->AppendSeparator();
	menuFile->Append(ID_About, "&About");
	menuFile->AppendSeparator();
	menuFile->Append(ID_Exit, "E&xit");
	menuBar->Append(menuFile, "&File");

	SetMenuBar(menuBar);

	gameWindow = new wxWindow(this, -1);
	
	gameWindow->Connect(-1,-1, wxEVT_SIZE,
		(wxObjectEventFunction) &AppFrame::OnSize, NULL, this);
	gameWindow->Connect(-1,-1, wxEVT_PAINT,
		(wxObjectEventFunction) &AppFrame::OnPaint, NULL, this);
	gameWindow->Connect(-1,-1, wxEVT_MOTION,
		(wxObjectEventFunction) &AppFrame::OnMotion, NULL, this);
	gameWindow->Connect(-1,-1, wxEVT_LEFT_DOWN,
		(wxObjectEventFunction) &AppFrame::OnMouseLeft, NULL, this);

	CreateStatusBar(3);

	SetStatusText("Create a New Pakage to begin creating levels...");
	
	toolbar = CreateToolBar(wxTB_FLAT);

	wxImage wallImage = wxBITMAP(WallTile).ConvertToImage().Scale(TOOLBAR_SIZE,TOOLBAR_SIZE);
	wallImage.SetMask(false);
	wxBitmap wallBitmap = (wxBitmap)wallImage;

	wxImage playerImage = wxBITMAP(PlayerTile).ConvertToImage().Scale(TOOLBAR_SIZE,TOOLBAR_SIZE);
	playerImage.SetMaskColour(255,255,255);
	wxBitmap playerBitmap = (wxBitmap)playerImage;
	
	wxImage enemyImage = wxBITMAP(EnemyTile).ConvertToImage().Scale(TOOLBAR_SIZE,TOOLBAR_SIZE);
	enemyImage.SetMaskColour(255,255,255);
	wxBitmap enemyBitmap = (wxBitmap)enemyImage;

	wxImage potionImage = wxBITMAP(PotionTile).ConvertToImage().Scale(TOOLBAR_SIZE,TOOLBAR_SIZE);
	potionImage.SetMaskColour(255,255,255);
	wxBitmap potionBitmap = (wxBitmap)potionImage;

	wxBitmap eraseBitmap = wxBITMAP(Eraser).ConvertToImage().Scale(TOOLBAR_SIZE,TOOLBAR_SIZE);
	wxBitmap arrowLeftBitmap = (wxBitmap)wxBITMAP(ArrowLeft);
	wxBitmap arrowRightBitmap = (wxBitmap)wxBITMAP(ArrowRight);
	wxBitmap executeBitmap = (wxBitmap)wxBITMAP(Execute);
	
	toolbar->AddRadioTool(TLB_WALL, "Place Wall", wallBitmap, wxNullBitmap, "Place walls");
	toolbar->AddRadioTool(TLB_PLAYER, "Place Player", playerBitmap, wxNullBitmap, "Place Player");
	toolbar->AddRadioTool(TLB_ENEMY, "Place Enemy", enemyBitmap, wxNullBitmap, "Place Enemy");
	toolbar->AddRadioTool(TLB_POTION, "Place Potion", potionBitmap, wxNullBitmap, "Place Potion");
	toolbar->AddRadioTool(TLB_ERASE, "Erase Items", eraseBitmap, wxNullBitmap, "Erase Items");
	toolbar->AddSeparator();

	toolbar->AddTool(TLB_PREVIOUS_LEVEL, "Previous Level", arrowLeftBitmap,"Previous Level");
	toolbar->AddTool(TLB_NEXT_LEVEL, "Next Level", arrowRightBitmap,"Next Level");
	toolbar->AddTool(TLB_TEST_LEVEL, "Test Level", executeBitmap,"Test Level");

	toolbar->EnableTool(TLB_PREVIOUS_LEVEL, false);
	toolbar->EnableTool(TLB_NEXT_LEVEL, false);

	toolbar->Realize();

	wallImage = wxBITMAP(WallTile).ConvertToImage();
	wallImage.SetMask(false);
	wallBitmap = (wxBitmap)wallImage;

	playerImage = wxBITMAP(PlayerTile).ConvertToImage();
	playerImage.SetMaskColour(255,255,255);
	playerBitmap = (wxBitmap)playerImage;

	enemyImage = wxBITMAP(EnemyTile).ConvertToImage();
	enemyImage.SetMaskColour(255,255,255);
	enemyBitmap = (wxBitmap)enemyImage;

	potionImage = wxBITMAP(PotionTile).ConvertToImage();
	potionImage.SetMaskColour(255,255,255);
	potionBitmap = (wxBitmap)potionImage;


	wxBitmap emptyBitmap = wxBITMAP(EmptyTile);
	
	items[0] = emptyBitmap;
	items[1] = wallBitmap;
	items[2] = playerBitmap;
	items[3] = enemyBitmap;
	items[4] = potionBitmap;

}
void AppFrame::OnNewPackage(wxCommandEvent &event)
{
	Package *old_package = package;
	int old_levelIndex = levelIndex;

	package = new Package;
	levelIndex = 0;

	if(!createNewLevel())
	{
		delete package;
		package = old_package;
		levelIndex = old_levelIndex;
	}
	else
	{
		delete old_package;

		menuFile->Enable(ID_New_Level, true);

		toolbar->EnableTool(TLB_PREVIOUS_LEVEL, false);
		toolbar->EnableTool(TLB_NEXT_LEVEL, false);
	}
}
void AppFrame::OnNewLevel(wxCommandEvent &event)
{
	createNewLevel();
}
void AppFrame::OnSave(wxCommandEvent &event)
{
	wxFileDialog dialog(this,
		_T("Save Package"),
		_T(""),
		_T("Package1.pkg"),
		_T("Package (*.pkg)|*.pkg"),
		wxSAVE|wxOVERWRITE_PROMPT);

	if(dialog.ShowModal() == wxID_OK)
	{
		savePackage(dialog.GetPath().c_str());
	}
}
void AppFrame::OnLoad(wxCommandEvent &event)
{
	wxFileDialog dialog(this,
		_T("Load Package"),
		_T(""),
		_T(""),
		_T("Package (*.pkg)|*.pkg"),
		0);
	
	if(dialog.ShowModal() == wxID_OK)
	{
		FILE *stream;

		const wxChar *filename = dialog.GetPath().c_str();
		
		stream = fopen(filename, "r+b");

		int numLevels;

		fread(&numLevels, sizeof(int), 1, stream);

		if(package)
			delete package;

		package = new Package;

		for(int i = 0; i < numLevels; i++)
		{
			Level_Info *level = new Level_Info;

			fread(&level->grid_x, sizeof(int), 1, stream);
			fread(&level->grid_y, sizeof(int), 1, stream);

			level->grid = new char *[level->grid_x];

			for(int x = 0; x < level->grid_x; x++)
			{
				level->grid[x] = new char[level->grid_y];
			}
			for (int x = 0; x < level->grid_x; x++)
				fread(level->grid[x], sizeof(char), level->grid_y, stream);
			
			package->push_back(level);

			if(i == 0)
			{
				levelIndex = 1;
				setCurrentLevel(level);
			}
		}

		fclose(stream);

		toolbar->EnableTool(TLB_PREVIOUS_LEVEL, levelIndex != 1);
		toolbar->EnableTool(TLB_NEXT_LEVEL, levelIndex != package->size());

		menuFile->Enable(ID_New_Level, true);

		updateView();
	}

}
void AppFrame::OnAbout(wxCommandEvent &event)
{
}
void AppFrame::OnExit(wxCommandEvent &event)
{
}
void AppFrame::OnToolBarClicked(wxCommandEvent &event)
{
	bool changed = false;
	switch(event.GetId())
	{
		case TLB_TEST_LEVEL:
		{
			savePackage("temp.pkg");
			wxExecute(wxString::Format("EvilMonkeys.exe temp.pkg %d", levelIndex));
			break;
		}
		case TLB_PREVIOUS_LEVEL:
		{
			if(levelIndex > 1)
				levelIndex--;

			changed = true;
		}
		break;

		case TLB_NEXT_LEVEL:
		{
				if(levelIndex < (int)package->size())
					levelIndex++;
				changed = true;
		}
		break;
	}

	if(changed)
	{
		list<Level_Info *>::iterator it;
		
		int i = 0;
		for(it = package->begin(); it != package->end(); it++, i++)
		{
			if(i == levelIndex - 1)
			{
				setCurrentLevel((*it));
				break;
			}
		}
		
		toolbar->EnableTool(TLB_PREVIOUS_LEVEL, levelIndex != 1);
		toolbar->EnableTool(TLB_NEXT_LEVEL, levelIndex != package->size());
	}
}

void AppFrame::OnSize(wxSizeEvent &event)
{
	event.Skip();

	if(finalBackBuffer)
	{
		delete finalBackBuffer;
	}

	wxSize clientArea = gameWindow->GetClientSize();

	finalBackBuffer = new wxBitmap(clientArea.GetWidth(), clientArea.GetHeight());

	if(backBuffer)
	{
		stretchGameView();
		flipBackBuffer();
	}
}
void AppFrame::OnPaint(wxPaintEvent &event)
{
	wxPaintDC dc(gameWindow);
	
	wxSize clientArea = gameWindow->GetClientSize();

	if(!backBuffer)
	{
		wxClientDC screenDC(gameWindow);

		screenDC.Clear();

		screenDC.DrawRectangle(0,0, clientArea.GetWidth(), clientArea.GetHeight());
	}
	else
	{
		stretchGameView();
		flipBackBuffer();
	}
}
void AppFrame::updateView(void)
{
	if(!backBuffer)
	{
		return;
	}
	wxMemoryDC backBufferDC;

	backBufferDC.SelectObject(*backBuffer);
	backBufferDC.Clear();

	for(int x = 0; x < currentLevel->grid_x; x++)
	{
		for(int y = 0; y < currentLevel->grid_y; y++)
		{
			int item = currentLevel->grid[x][y];

			if(item >= 0 && item <= 4)
			{
				backBufferDC.DrawBitmap(items[0], x *(GRID_SIZE + 1) + 1, y * (GRID_SIZE + 1));
				backBufferDC.DrawBitmap(items[item], x *(GRID_SIZE + 1) + 1, y * (GRID_SIZE + 1), true);

			}
		}
	}
	backBufferDC.SetPen(wxPen(wxColour(128,128,128)));

	for(int x = 0; x <= currentLevel->grid_x; x++)
		backBufferDC.DrawLine(wxPoint(x*(GRID_SIZE + 1), 0), wxPoint(x * (GRID_SIZE  + 1), currentLevel->grid_y * (GRID_SIZE + 1)));

	for(int y = 0; y <= currentLevel->grid_y; y++)
		backBufferDC.DrawLine(wxPoint(0, y*(GRID_SIZE + 1)), wxPoint(currentLevel->grid_x * (GRID_SIZE + 1 ), y * (GRID_SIZE + 1)));

	backBufferDC.SelectObject(wxNullBitmap);

	stretchGameView();
	flipBackBuffer();
}
void AppFrame::stretchGameView(void)
{
	wxSize clientArea = gameWindow->GetClientSize();

	wxSize stretchedSize;

	if(clientArea.GetWidth() * currentLevel->grid_y / currentLevel->grid_x < clientArea.GetHeight())
	{
		stretchedSize.Set(clientArea.GetWidth(), clientArea.GetWidth() * currentLevel->grid_y / currentLevel->grid_x);
	}
	else
	{
		stretchedSize.Set(clientArea.GetHeight() * currentLevel->grid_x / currentLevel->grid_y, clientArea.GetHeight());
	}

	wxImage stretchedImage = backBuffer->ConvertToImage();

	stretchedImage = stretchedImage.Scale(stretchedSize.GetWidth(), stretchedSize.GetHeight());

	wxMemoryDC finalDC;
	wxMemoryDC imageDC;

	finalDC.SelectObject(*finalBackBuffer);
	imageDC.SelectObject(stretchedImage);

	finalDC.SetBackground(*wxBLACK_BRUSH);
	finalDC.Clear();

	wxPoint center;

	center.x = (clientArea.GetWidth() - stretchedSize.GetWidth()) / 2;
	center.y = (clientArea.GetHeight() - stretchedSize.GetHeight()) / 2;

	finalDC.Blit(center, stretchedSize, &imageDC, wxPoint(0,0));

	finalDC.SetBrush(*wxTRANSPARENT_BRUSH);
	finalDC.DrawRectangle(wxPoint(0,0), clientArea);

	imageDC.SelectObject(wxNullBitmap);
	finalDC.SelectObject(wxNullBitmap);

	pos_x = center.x;
	pos_y = center.y;
	grid_width = stretchedSize.GetWidth();
	grid_height = stretchedSize.GetHeight();

}
void AppFrame::flipBackBuffer(void)
{
	wxSize clientArea = gameWindow->GetClientSize();

	wxMemoryDC finalDC;

	wxClientDC screenDC(gameWindow);

	finalDC.SelectObject(*finalBackBuffer);

	screenDC.Blit(wxPoint(0,0), clientArea, &finalDC, wxPoint(0,0));

	finalDC.SelectObject(wxNullBitmap);
}
bool AppFrame::createNewLevel(void)
{
	long x = wxGetNumberFromUser(_T("Enter the number of colums for the new level."), _T("Colums"),_T("New Level"), 10, 0, 100, this);

	if(x < 1)
	{
		return false;
	}
	
	long y = wxGetNumberFromUser(_T("Enter the number of rows for the new level."), _T("Rows"),_T("New Level"), 10, 0, 100, this);

	if(y < 1)
	{
		return false;
	}
	currentLevel = new Level_Info;

	package->push_back(currentLevel);
	levelIndex = (int)package->size();

	currentLevel->grid_x = x;
	currentLevel->grid_y = y;

	currentLevel->grid = new char *[currentLevel->grid_x];

	for(int i = 0; i < currentLevel->grid_x; i++)
	{
		currentLevel->grid[i] = new char[currentLevel->grid_y];
	}
	for (int x = 0; x < currentLevel->grid_x; x++)
	{
		for(int y = 0; y < currentLevel->grid_y; y++)
			currentLevel->grid[x][y] = 0;
	}
	
	setCurrentLevel(currentLevel);

	if(levelIndex >= 2)
	{
		toolbar->EnableTool(TLB_PREVIOUS_LEVEL, true);
		toolbar->EnableTool(TLB_NEXT_LEVEL, false);
	}

	return true;
}
void AppFrame::setCurrentLevel(Level_Info *level)
{
	currentLevel = level;
	backBuffer = new wxBitmap(currentLevel->grid_x * (GRID_SIZE + 1) + 1, currentLevel->grid_y * (GRID_SIZE + 1) + 1);
	
	wxString new_title = "Level Editor for Evil Monkeys - level";
	new_title += wxString::Format("%d", levelIndex);
	SetTitle(new_title);

	wxString status_info = "Level Size";
	status_info += wxString::Format("%dx%d", currentLevel->grid_x, currentLevel->grid_y);
	SetStatusText(status_info, 1);
	
	status_info = "Current Level";
	status_info += wxString::Format("%d", levelIndex);
	SetStatusText(status_info, 2);

	updateView();
}
void AppFrame::OnMotion(wxMouseEvent &event)
{
	wxPoint mp = event.GetPosition();

	if (mp.x >= pos_x && mp.x <= pos_x + grid_width && 
		mp.y >= pos_y && mp.y <= pos_y + grid_height)
	{
		int tempX = (mp.x - pos_x) / (grid_width / currentLevel->grid_x);
		int tempY = (mp.y - pos_y) / (grid_height / currentLevel->grid_y);

		if(tempX != mouse_grid_x || tempY != mouse_grid_y)
		{
			mouse_grid_x = tempX;
			mouse_grid_y = tempY;

			if(event.LeftIsDown())
				OnMouseLeft(event);
		}
	}
	else
	{
		if(mouse_grid_x != -1 && mouse_grid_y != -1)
		{
			mouse_grid_x = mouse_grid_y = -1;
			updateView();
		}
	}
}
void AppFrame::OnMouseLeft(wxMouseEvent &event)
{
	if(mouse_grid_x != -1 && mouse_grid_y != -1)
	{
		int selected = 0;

		if(toolbar->GetToolState(TLB_ERASE))
			selected = TILE_EMPTY;
		else if(toolbar->GetToolState(TLB_WALL))
			selected = TILE_WALL;
		else if(toolbar->GetToolState(TLB_PLAYER))
			selected = TILE_PLAYER;
		else if(toolbar->GetToolState(TLB_ENEMY))
			selected = TILE_ENEMY;
		else if(toolbar->GetToolState(TLB_POTION))
			selected = TILE_POTION;
		else
			return;

		if(backBuffer)
		{
			wxMemoryDC backBufferDC;

			backBufferDC.SelectObject(*backBuffer);
			
			if(selected == 2)
			{
				for(int x = 0; x < currentLevel->grid_x; x++)
					for(int y = 0; y < currentLevel->grid_y; y++)
						if(currentLevel->grid[x][y] == 2)
						{
							currentLevel->grid[x][y] = 0;
							backBufferDC.DrawBitmap(items[0], x * (GRID_SIZE + 1) + 1, y * (GRID_SIZE + 1) + 1);

						}

			}
			if(mouse_grid_x < currentLevel->grid_x && mouse_grid_y < currentLevel->grid_y)
				currentLevel->grid[mouse_grid_x][mouse_grid_y] = selected;

			backBufferDC.DrawBitmap(items[0], mouse_grid_x * (GRID_SIZE + 1) + 1, mouse_grid_y * (GRID_SIZE + 1) + 1);
			backBufferDC.DrawBitmap(items[selected], mouse_grid_x * (GRID_SIZE + 1) + 1, mouse_grid_y * (GRID_SIZE + 1) + 1, true);

			backBufferDC.SelectObject(wxNullBitmap);

			stretchGameView();
			flipBackBuffer();
		}
	}
}
void AppFrame::savePackage(wxString filename)
{
	FILE *stream;

	stream = fopen(filename, "w+b");
	
	int numLevels = (int)package->size();

	fwrite(&numLevels, sizeof(int), 1, stream);

	list<Level_Info *>::iterator it;
	
	for(it = package->begin(); it != package->end(); it++)
	{
		Level_Info *level = (*it);
		
		fwrite(&level->grid_x, sizeof(int), 1, stream);
		fwrite(&level->grid_y, sizeof(int), 1, stream);

		for(int x = 0; x < level->grid_x; x++)
			fwrite(level->grid[x], sizeof(char), level->grid_y, stream);
	}
	
	fclose(stream);
}