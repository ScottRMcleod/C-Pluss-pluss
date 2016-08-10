#include "stdwx.h"
#include "drawEngine.h"

#define GRID_SIZE 32
DrawEngine::DrawEngine(int xSize, int ySize)
{
	screenWidth = xSize;
	screenHeight = ySize;

	map = 0;
}
DrawEngine::~DrawEngine()
{

}
int DrawEngine::createSprite(int index, wxBitmap *sprite)
{
	if(index >= 0 && index < 17)
	{
		wxImage img = sprite->ConvertToImage();
		img.SetMaskColour(255,255,255);
		spriteImage[index] = new wxBitmap(img);
		return index;
	}
	return -1;
}
void DrawEngine::deleteSprite(int index)
{
	// in this imp we don't need it yet
}
void DrawEngine::drawSprite(int index, int posx, int posy)
{
	wxMemoryDC dc;

	dc.SelectObject(*winCanvas);

	dc.DrawBitmap(*spriteImage[index], wxPoint(posx * GRID_SIZE, posy * GRID_SIZE), true);

	dc.SelectObject(wxNullBitmap);
}
void DrawEngine::eraseSprite(int posx, int posy)
{
	wxMemoryDC dc;

	dc.SelectObject(*winCanvas);

	dc.DrawBitmap(*tileImage[0], wxPoint(posx * GRID_SIZE,posy * GRID_SIZE), true);

	dc.SelectObject(wxNullBitmap);
}
void DrawEngine::setMap(char **data)
{
	map = data;
}
void DrawEngine::createBackgroundTile(int index, wxBitmap *tile)
{
	if(index >= 0 && index < 17)
	{
		wxImage img = tile->ConvertToImage();
		img.SetMaskColour(255,255,255);
		tileImage[index] = new wxBitmap(img);
	}
}
void DrawEngine::drawBackground(void)
{
	wxMemoryDC dc;
	dc.SelectObject(*winCanvas);

	if(map)
	{
		for(int y = 0; y < screenHeight; y++)
		{
			
			for(int x = 0; x < screenWidth; x++)
			{
				if(map[x][y] !=3 && map[x][y] < 4 )
					dc.DrawBitmap(*tileImage[map[x][y]], wxPoint(x * GRID_SIZE, y * GRID_SIZE), true);
			}
		}
	}
	
	dc.SelectObject(wxNullBitmap);
}
void DrawEngine::setWindow(wxBitmap *in, int width, int height)
{
	winCanvas = in;

	screenWidth = width;
	screenHeight = height;
}
