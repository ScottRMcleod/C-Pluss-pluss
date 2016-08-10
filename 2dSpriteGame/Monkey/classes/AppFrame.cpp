#include "stdwx.h"

#include "AppFrame.h"
#define GRID_SIZE 32
#define LEVEL_X 20
#define LEVEL_Y 10
#define UPDATE_TIME 20

BEGIN_EVENT_TABLE(AppFrame, wxFrame)
	EVT_MENU (ID_New, AppFrame::OnNew)
	EVT_MENU (ID_Load, AppFrame::OnLoad)
	EVT_MENU (ID_About, AppFrame::OnAbout)
	EVT_MENU (ID_Exit, AppFrame::OnExit)
	EVT_TIMER (ID_Timer, AppFrame::OnTimer)
END_EVENT_TABLE()

AppFrame::AppFrame(const wxString & title, const wxPoint &pos, const wxSize &size, wxString filename, int startingLevel)
: wxFrame((wxFrame *)NULL, -1, title,pos,size)
{
	player = NULL;
	level = NULL;
	backBuffer = NULL;

	finalBackBuffer = NULL;
	packageLoaded = false;
	package = NULL;

	wxMenuBar *menu_bar = new wxMenuBar;

	wxMenu *menuFile = new wxMenu;

	menuFile->Append(ID_New, "&New");
	menuFile->AppendSeparator();
	menuFile->Append(ID_Load, "&Load");
	menuFile->AppendSeparator();
	menuFile->Append(ID_About, "&About");
	menuFile->AppendSeparator();
	menuFile->Append(ID_Exit, "E&xit");

	menu_bar->Append(menuFile,"&File");

	SetMenuBar(menu_bar);

	m_panel = new wxPanel(this, -1);

	m_info_panel = new wxPanel(m_panel, -1, wxDefaultPosition, wxSize(200,400), wxBORDER_STATIC);

	gameWindow = new wxWindow(m_panel, -1, wxDefaultPosition, wxSize(200,200), wxBORDER_STATIC);
	
	gameWindow->Connect(-1,-1,wxEVT_PAINT,
		(wxObjectEventFunction) &AppFrame::OnPaint, NULL, this);

	gameWindow->Connect(-1,-1, wxEVT_KEY_DOWN,
		(wxObjectEventFunction) &AppFrame::OnKey, NULL, this);
	gameWindow->Connect(-1,-1, wxEVT_SIZE,
		(wxObjectEventFunction) &AppFrame::OnSize, NULL, this);
	
	stPlayerLives = new wxStaticText(m_info_panel,-1, _T("Lives:"), wxDefaultPosition, wxSize(100,15), wxST_NO_AUTORESIZE);
	stCurrentLevel = new wxStaticText(m_info_panel,-1, _T("Level:"), wxDefaultPosition, wxSize(100,15), wxST_NO_AUTORESIZE);
	stNumEnemies = new wxStaticText(m_info_panel,-1, _T("Enemies:"), wxDefaultPosition, wxSize(100,15), wxST_NO_AUTORESIZE);
	stManaLevel = new wxStaticText(m_info_panel,-1, _T("Mana Level:"), wxDefaultPosition, wxSize(100,15), wxST_NO_AUTORESIZE);

	wxBoxSizer *mainsizer = new wxBoxSizer(wxHORIZONTAL);

	mainsizer->Add(gameWindow, 1, wxGROW|wxALL,5);
	mainsizer->Add(m_info_panel, 0, wxGROW|wxALL|wxSTRETCH_NOT,5);

	wxBoxSizer *subsizer = new wxBoxSizer(wxVERTICAL);

	subsizer->Add(stPlayerLives, 0, wxLEFT|wxTOP,5);
	subsizer->Add(stCurrentLevel, 0, wxLEFT|wxTOP,5);
	subsizer->Add(stNumEnemies, 0, wxLEFT|wxTOP,5);
	subsizer->Add(stManaLevel, 0, wxLEFT|wxTOP,5);

	m_panel->SetAutoLayout(TRUE);
	m_panel->SetSizer(mainsizer);

	m_info_panel->SetAutoLayout(TRUE);
	m_info_panel->SetSizer(subsizer);

	drawArea.createBackgroundTile(TILE_EMPTY, &wxBITMAP(EmptyTile));
	drawArea.createBackgroundTile(TILE_WALL, &wxBITMAP(WallTile));

	drawArea.createSprite(SPRITE_PLAYER, &wxBITMAP(PlayerTile));
	drawArea.createSprite(SPRITE_ENEMY, &wxBITMAP(EnemyTile));
	drawArea.createSprite(SPRITE_FIREBALL, &wxBITMAP(FireballTile));
	//drawArea.createSprite(SPRITE_ICEBALL, &wxBITMAP(IceballTile));
	drawArea.createSprite(SPRITE_MANAPOTION, &wxBITMAP(potionTile));

	m_timer = new wxTimer(this, ID_Timer);
	m_timer->Start(UPDATE_TIME);

	srand(wxGetElapsedTime(false));
	wxStartTimer();
	
	gameState = STATE_NULL;

	if(filename != "")
		loadGame(filename, startingLevel);
}
AppFrame::~AppFrame()
{
	gameState = STATE_NULL;

	m_timer->Stop();

	delete m_timer;

	if(player)
		delete player;

	if(level)
		delete level;

	if(backBuffer)
		delete backBuffer;

	if(finalBackBuffer)
		delete finalBackBuffer;
}
void AppFrame::OnNew(wxCommandEvent &event)
{
	m_timer->Stop();

	if(player)
		delete player;

	player = new Mage(level, &drawArea, 0);

	startNewLevel(RANDOM_NEW_LEVEL);

	level->addEnemies(3, 0);
	level->addPickup(3, MANAPOTION);

	currentLevel = 1;

	gameState = STATE_GAME_IN_PROGRESS;

	m_timer->Start(UPDATE_TIME);
	level->draw();
	level->addPlayer(player);
	
	level->setPlayerStart();
	level->upDate(wxGetElapsedTime(false));
}
void AppFrame::OnLoad(wxCommandEvent &event)
{
		wxFileDialog dialog(this,
		_T("Open Package"),
		_T(""),
		_T(""),
		_T("Packages (*.pkg)|*.pkg"),
		0);
	
	if(dialog.ShowModal() == wxID_OK)
	{
		loadGame(dialog.GetPath().c_str());
	}
}
void AppFrame::OnAbout(wxCommandEvent &event)
{
	wxMessageBox(_T("The Evil Monkeys Game Of Doom Use the controls W,A,S,D to move the character around Press the space bar to kill the evil monkeys"), _T("About EvilMokeys"));
}
void AppFrame::OnExit(wxCommandEvent &event)
{
	Close(TRUE);
}
//void AppFrame::Pause()
//{
//	if(!isStarted)
//		return;

//	isPaused = !isPaused;
//	if(isPaused)
//	{
//		m_timer->Stop();
//		m_stsbar->SetStatusText(wxT("paused"));
//	}
//	else
//	{
//		timer->Start(300);
//		wxString str;
//		str.Printf(wxT("%d"), numLinesRemoved);
//		m_stsbar->SetStatusText(str);
//	}
//	Refresh();
//}
void AppFrame::OnPaint(wxPaintEvent &event)
{
	wxPaintDC dc(gameWindow);

	dc.SetBackground(*wxBLACK_BRUSH);
	dc.Clear();
	
	updateView();
}
void AppFrame::OnTimer(wxTimerEvent &event)
{
	if(gameState == STATE_GAME_IN_PROGRESS)
	{
		updateGame();
	}
}
void AppFrame::OnKey(wxKeyEvent &event)
{
		if(gameState == STATE_GAME_IN_PROGRESS)
			level->keyPress(event.GetKeyCode() | 32);

	updateView();
}
void AppFrame::OnSize(wxSizeEvent &event)
{
	event.Skip();

	if(finalBackBuffer)
		delete finalBackBuffer; 

	wxSize clientArea = gameWindow->GetClientSize();
	finalBackBuffer = new wxBitmap(clientArea.GetWidth(),clientArea.GetHeight());

	updateView();
}
bool AppFrame::startNewLevel(LevelType type)
{
	if(level)
	{
		delete level;
		level = NULL;
	}
	switch(type)
	{
		case RANDOM_NEW_LEVEL:
		{
			level = new Level(&drawArea, LEVEL_X, LEVEL_Y);
			break;
		}

		case NEXT_PACKAGE_LEVEL:
		{
			list<Level_Info *>::iterator it;
			int i = 0;

			if(currentLevel > (int)package->size())
				return true;

			for(it = package->begin(); it!= package->end(); it++, i++)
			{
				if(i == currentLevel - 1)
				{
					Level_Info *new_level = *it;

					level = new Level(&drawArea, new_level->grid, new_level->grid_x, new_level->grid_y);

					break;
				}
			}
			//level = new Level(&drawArea, LEVEL_X, LEVEL_Y);
			//break;
		}	
	}
	
	if(backBuffer)
	{
		delete backBuffer;
		backBuffer = NULL;
	}

	backBuffer = new wxBitmap(level->getWidth()* GRID_SIZE, level->getHeight() *GRID_SIZE);
	
	drawArea.setWindow(backBuffer, level->getWidth(), level->getHeight());

	level->addPlayer(player);
	level->draw();

	player->setLevel(level);
	player->update();

	return false;
}
void AppFrame::stretchGameView(void)
{
	wxSize clientArea = gameWindow->GetClientSize();

	wxSize stretchedSize;

	if(clientArea.GetWidth() * level->getHeight() / level->getWidth() < clientArea.GetHeight())
	{
		stretchedSize.Set(clientArea.GetWidth(), clientArea.GetWidth() * level->getHeight() / level->getWidth());
	}
	else
	{
		stretchedSize.Set(clientArea.GetHeight() * level->getWidth() / level->getHeight(), clientArea.GetHeight());
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


}
void AppFrame::flipBackbuffer(void)
{
	wxSize clientArea = gameWindow->GetClientSize();

	wxMemoryDC finalDC;

	wxClientDC screenDC(gameWindow);

	finalDC.SelectObject(*finalBackBuffer);

	screenDC.Blit(wxPoint(0,0), clientArea, &finalDC, wxPoint(0,0));

	finalDC.SelectObject(wxNullBitmap);
}
void AppFrame::drawInformation(void)
{
	wxSize clientArea = gameWindow->GetClientSize();

	wxMemoryDC finalDC;
	
	finalDC.SelectObject(*finalBackBuffer);

	finalDC.Clear();

	finalDC.DrawRectangle(0,0, clientArea.GetWidth(), clientArea.GetHeight());

	wxString message;

	if(gameState == STATE_NULL)
		message = "Go to file > new to start a new game";
	else if (gameState == STATE_PLAYER_WON)
		message = "You owned this game... Lengend";
	else if (gameState == STATE_GAME_OVER)
		message = "You got owned NEWB!";

	int textWidth, textHeight;

	finalDC.GetTextExtent(message, &textWidth, &textHeight);
	wxPoint center((clientArea.GetWidth() - textWidth) / 2, (clientArea.GetHeight() - textHeight) / 2);

	finalDC.DrawText(message, center);

	finalDC.SelectObject(wxNullBitmap);

}
void AppFrame::loadGame(wxString filename, int startingLevel)
{
	package = new Package();

	player = new Mage(NULL, &drawArea, 0);

	FILE *stream = fopen (filename, "r+b");

	int numLevels;

	fread(&numLevels, sizeof(int), 1, stream);
	
	Level_Info *firstLevel;

	for(int i = 0; i < numLevels; i++)
	{
		Level_Info *new_level = new Level_Info;

		fread(&new_level->grid_x, sizeof(int), 1, stream);
		fread(&new_level->grid_y, sizeof(int), 1, stream);

		new_level->grid = new char *[new_level->grid_x];

		for(int x = 0; x < new_level->grid_x; x++)
			new_level->grid[x] = new char[new_level->grid_y];

		for(int x = 0; x < new_level->grid_x; x++)
			fread(new_level->grid[x], sizeof(char), new_level->grid_y, stream);
		
		package->push_back(new_level);

		if(i == startingLevel - 1)
		{
			currentLevel = startingLevel;
			firstLevel = new_level;
		}
	}

	fclose(stream);

	packageLoaded = true;

	startNewLevel(NEXT_PACKAGE_LEVEL);

	level->addEnemies(0);
	level->addPickup(0, MANAPOTION);

	currentLevel = 1;

	gameState = STATE_GAME_IN_PROGRESS;

	m_timer->Start(UPDATE_TIME);
}
void AppFrame::updateView(void)
{
	wxClientDC area(gameWindow);

	if(gameState == STATE_GAME_IN_PROGRESS)
		stretchGameView();	
	else if (finalBackBuffer)
		drawInformation();

	flipBackbuffer();
}
void AppFrame::updateGame(void)
{
	wxString info_Lives = "Lives :" + wxString::Format("%d", player->getLives());
    wxString info_Level = "Level :" + wxString::Format("%d", currentLevel);
	wxString info_NumEnemies = "Enemies :" + wxString::Format("%d", level->numEnemies());
	wxString info_ManaLevel = "Mana Level :" + wxString::Format("%d", player->getMana());

	stPlayerLives->SetLabel(info_Lives);
	stCurrentLevel->SetLabel(info_Level);
	stNumEnemies->SetLabel(info_NumEnemies);
	stManaLevel->SetLabel(info_ManaLevel);

	if(level->numEnemies() == 0)
	{
		m_timer->Stop();

		currentLevel++;

		if(packageLoaded)
		{
			if(startNewLevel(NEXT_PACKAGE_LEVEL))
			{
				packageLoaded = false;

				gameState = STATE_PLAYER_WON;

				updateView();

				return;
			}

			level->addEnemies(0);
			level->addPickup(0, MANAPOTION);
			level->setPlayerStart();
		}
		else
		{
			startNewLevel(RANDOM_NEW_LEVEL);

			int numEnemies = currentLevel * 4;
			if(numEnemies > 15) numEnemies = 15;

			int newSpeed = pow(2,currentLevel);
			if(newSpeed > 100) newSpeed = 100;

			level->addEnemies(numEnemies, newSpeed);
			level->addPickup(3, MANAPOTION);
			
			level->setPlayerStart();
		}
		m_timer->Start(UPDATE_TIME);
	}
	else if(level->numEnemies() > 0 && !player->isAlive())
	{
		packageLoaded = false;
		gameState = STATE_GAME_OVER;

		m_timer->Stop();
	}
	else
		level->upDate(wxGetElapsedTime(false));

	updateView();
}