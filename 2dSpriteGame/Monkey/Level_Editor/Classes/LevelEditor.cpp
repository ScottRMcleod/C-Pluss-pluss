#include "AppFrame.h"

class EvilMonkeysApp : public wxApp
{
	virtual bool OnInit();
};

IMPLEMENT_APP(EvilMonkeysApp);

bool EvilMonkeysApp::OnInit()
{
	AppFrame *frame = new AppFrame("Level Editor for Evil Monkeys", wxPoint(100,100),
		wxSize(470,515));

	frame->Show(TRUE);
	SetTopWindow(frame);

	return TRUE;
}
