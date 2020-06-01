//
// Created by superbob on 2020-05-31.
//

#ifndef OSM_A_STAR_SEARCH_MYAPP_HPP
#define OSM_A_STAR_SEARCH_MYAPP_HPP

#include "wx/wx.h"
#include "wx/sizer.h"

#include <thread>
#include <future>

class MyApp;

class Graphics;

// dots drawn on the map
struct spot
{
	spot( const wxBrush &brush, int x, int y, int id );
	
	const wxBrush &brush;
	int x;
	int y;
	
	int id;
};

// from wxWidgets example on drawing simple graphics. A pane that can draw spots on a background.
class TrafficSimPane : public wxPanel
{
	wxSize _originalSize {};
	wxImage _image;
	wxString _imgFile;
	
	std::vector<spot> _spots {};
	
	wxTimer _timer;
	Graphics *_graphics;
public:
	TrafficSimPane( wxFrame *parent, Graphics *graphics );
	
	void paintEvent( wxPaintEvent &evt );
	
	void sizeEvent( wxSizeEvent &evt );
	
	void timerEvent( wxTimerEvent &evt );
	
	void paintNow();
	
	std::mutex _mutex;
	
	void loadBackground( const std::string &path );
	
	void updateBackground();
	
	void render( wxDC &dc );

DECLARE_EVENT_TABLE()
};

class MyApp : public wxApp
{
	// std::unique_ptr<Graphics> _graphics;
	// - there's no shutdown mechanism in endless loops, so deleting this on exit crashes the program
	//   so we'll just leak this here, like it was in original implementation
	
	Graphics *_graphics;
	
	bool OnInit();
	
	wxFrame *frame;
	TrafficSimPane *trafficSimPane;
};


#endif //OSM_A_STAR_SEARCH_MYAPP_HPP
