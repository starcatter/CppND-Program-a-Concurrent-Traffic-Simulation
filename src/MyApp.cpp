//
// Created by superbob on 2020-05-31.
//

#include <future>
#include "Graphics.h"

Graphics *setupSimulation();

bool MyApp::OnInit()
{
	wxInitAllImageHandlers();
	
	wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
	frame = new wxFrame((wxFrame *) NULL, -1, wxT("Concurrent Traffic Simulation"), wxPoint(50, 50), wxSize(1680, 1024));
	
	// _graphics.reset( setupSimulation() );
	_graphics = setupSimulation();
	
	trafficSimPane = new TrafficSimPane((wxFrame *) frame, _graphics /*.get()*/);
	trafficSimPane->SetDoubleBuffered(true);
	
	sizer->Add(trafficSimPane, 1, wxEXPAND);
	
	frame->SetSizer(sizer);
	frame->SetAutoLayout(true);
	
	frame->Show();
	
	return true;
}

TrafficSimPane::TrafficSimPane( wxFrame *parent, Graphics *graphics )
		: wxPanel(parent), _graphics(graphics), _timer(this, 0)
{
	loadBackground(graphics->getBgFilename());
	_timer.Start(1000 / 60);
}

void TrafficSimPane::loadBackground( const std::string &path )
{
	const static std::string imgBasePath = "./";
	_imgFile = imgBasePath + path;
	
	updateBackground();
}

void TrafficSimPane::sizeEvent( wxSizeEvent &evt )
{
	if ( _image.IsOk())
	{
		std::lock_guard<std::mutex> uLock(_mutex);
		updateBackground();
	}
}

void TrafficSimPane::updateBackground()
{
	wxImage imageFull;
	imageFull.LoadFile(_imgFile);
	if ( imageFull.IsOk())
	{
		_originalSize = imageFull.GetSize();
		
		wxSize sz = this->GetSize();
		_image = imageFull.Rescale(sz.GetWidth(), sz.GetHeight(), wxIMAGE_QUALITY_HIGH);
	}
}

void TrafficSimPane::timerEvent( wxTimerEvent &evt )
{
	if ( _graphics )
	{
		auto &&newSpots = _graphics->getTrafficObjectsAsSpots();
		{
			std::lock_guard<std::mutex> uLock(_mutex);
			_spots.swap(newSpots);
		}
		paintNow();
	}
}

void TrafficSimPane::paintEvent( wxPaintEvent &evt )
{
	wxPaintDC dc(this);
	render(dc);
}

void TrafficSimPane::paintNow()
{
	wxClientDC dc(this);
	render(dc);
}

void TrafficSimPane::render( wxDC &dc )
{
	float xFactor = 0.5;
	float yFactor = 0.5;
	
	if ( _image.IsOk())
	{
		dc.DrawBitmap(_image, 0, 0, false);
		wxSize sz = this->GetSize();
		xFactor = (float) sz.x / (float) _originalSize.x;
		yFactor = (float) sz.y / (float) _originalSize.y;
	}
	
	{
		std::lock_guard<std::mutex> uLock(_mutex);
		
		for ( auto &spot : _spots )
		{
			int spotX = spot.x * xFactor;
			int spotY = spot.y * yFactor;
			int circleRadius = 10;
			
			dc.SetBrush(spot.brush);
			dc.SetPen(wxPen(wxColor(0, 0, 0), 2));
			dc.DrawCircle(wxPoint(spotX, spotY), circleRadius /* radius */ );
			
			dc.SetTextForeground(wxColor(0, 0, 0));
			dc.DrawText(std::to_string(spot.id), wxPoint(spotX - circleRadius / 2, spotY - circleRadius / 2));
		}
	}
}

IMPLEMENT_APP(MyApp)

BEGIN_EVENT_TABLE(TrafficSimPane, wxPanel)EVT_SIZE(TrafficSimPane::sizeEvent)
										  EVT_PAINT(TrafficSimPane::paintEvent)
										  EVT_TIMER(0, TrafficSimPane::timerEvent) END_EVENT_TABLE()

spot::spot( const wxBrush &brush, int x, int y, int id )
		: brush(brush), x(x), y(y), id(id){}
