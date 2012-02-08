#include "cinder/app/AppCocoaTouch.h"
#include "cinder/app/Renderer.h"

#include "cinder/Rand.h"
#include "Cell.h"

#define MAX_X_CELLS 1000
#define MAX_Y_CELLS 1000

using namespace ci;
using namespace ci::app;

class gameOfLifeiOSApp : public AppCocoaTouch {
  public:
	virtual void	setup();
	virtual void	resize( ResizeEvent event );
	virtual void	update();
	virtual void	draw();
	virtual void	mouseDown( MouseEvent event );
    virtual void	touchesMoved( TouchEvent event );
    
    // max cell count
    Cell cellsXY[MAX_X_CELLS][MAX_Y_CELLS];
    // cell dimensions
    float cellHeight;
    float cellWidth;
    // x/y max
    int maxx;
    int maxy;
};

void gameOfLifeiOSApp::setup()
{
	// 10 x 10 cells
    cellHeight = 20;
    cellWidth = 20;
    
    // set max based on current window dimensions and cell dimensions
    maxx = (int)(getWindowWidth() / cellWidth);
    maxy = (int)(getWindowHeight() / cellHeight);
    
    // create cells
    for (int x=0; x<maxx && x<MAX_X_CELLS; x++) {
        for (int y=0; y<maxy && x<MAX_Y_CELLS; y++) {
            
            // new cell
            Cell cell;
            cell.x = x * cellWidth;
            cell.y = y * cellHeight;
            cell.width = cellWidth;
            cell.height = cellHeight;
            cell.setup();
            
            // Initial random state
            if(Rand::randFloat() > 0.5)
                cell.isAlive = cell.nextIsAlive = true;
            else
                cell.isAlive = cell.nextIsAlive = false;
            
            cellsXY[x][y] = cell;
        }
    }
    
    /*
     // Specific starting patterns...
     cellsXY[10][10].isAlive = cellsXY[10][10].nextIsAlive = true;
     cellsXY[11][10].isAlive = cellsXY[11][10].nextIsAlive = true;
     cellsXY[12][10].isAlive = cellsXY[12][10].nextIsAlive = true;
     cellsXY[11][9].isAlive = cellsXY[11][9].nextIsAlive = true;
     cellsXY[12][11].isAlive = cellsXY[12][11].nextIsAlive = true;
     */
    
    // add neighbour pointers
    for (int x=0; x<maxx && x<MAX_X_CELLS; x++) {
        for (int y=0; y<maxy && x<MAX_Y_CELLS; y++) {
            
            // neighbour ref
            Cell& cell = cellsXY[x][y];
            
            // positions
            int above = y-1;
            int below = y+1;
            int left = x-1;
            int right = x+1;
            
            // loop
            if (above < 0) above = maxy-1;
            if (below == maxy) below = 0;
            if (left < 0) left = maxx-1;
            if (right == maxx) right = 0;
            
            cell.neighbours.clear();
            cell.neighbours.push_back(&cellsXY[left][above]);
            cell.neighbours.push_back(&cellsXY[left][y]);
            cell.neighbours.push_back(&cellsXY[left][below]);
            cell.neighbours.push_back(&cellsXY[x][below]);
            cell.neighbours.push_back(&cellsXY[right][below]);
            cell.neighbours.push_back(&cellsXY[right][y]);
            cell.neighbours.push_back(&cellsXY[right][above]);
            cell.neighbours.push_back(&cellsXY[x][above]);
            
            //console() << "neighbour count = " << cell.neighbours.size() << endl;
        }
    }
}

void gameOfLifeiOSApp::resize( ResizeEvent event )
{
	// restart
    setup();
}

void gameOfLifeiOSApp::touchesMoved( TouchEvent event )
{
    // restart
    //setup();
    float xRatio, yRatio;
    bool isAlive;
    for (vector<TouchEvent::Touch>::const_iterator touchIt = event.getTouches().begin(); touchIt != event.getTouches().end(); ++touchIt)
	{
        xRatio = touchIt->getX()/getWindowWidth();
        yRatio = touchIt->getY()/getWindowHeight();
        
        Cell& cell = cellsXY[ int(maxx * xRatio) ][ int(maxy * yRatio)  ];
        
        for (int i = 0; i < cell.neighbours.size(); i++) {
            if (Rand::randFloat() > 0.8) isAlive = true;
            else isAlive = false;
            cell.neighbours[i]->isAlive = isAlive;
        }
        
        cell.isAlive = isAlive;
	}
    
   
}

void gameOfLifeiOSApp::mouseDown( MouseEvent event )
{
	console() << "Mouse down @ " << event.getPos() << std::endl;
    // restart
    setup();
}

void gameOfLifeiOSApp::update()
{
	// update next state based on rules
    for (int x=0; x<maxx && x<MAX_X_CELLS; x++) {
        for (int y=0; y<maxy && x<MAX_Y_CELLS; y++) {
            Cell& cell = cellsXY[x][y];
            cell.updateState();
        }
    }
}

void gameOfLifeiOSApp::draw()
{
	// bg colour
    gl::clear( Color( 0.2, 0.2, 0.2 ) );
    
    // nudge in
    gl::pushMatrices();
    gl::translate(cellWidth*.5, cellHeight*.5);
    
    // update and draw cells
    for (int x=0; x<maxx && x<MAX_X_CELLS; x++) {
        for (int y=0; y<maxy && x<MAX_Y_CELLS; y++) {
            Cell& cell = cellsXY[x][y];
            cell.update();
            cell.draw();
        }
    }
    
    gl::popMatrices();
}

CINDER_APP_COCOA_TOUCH( gameOfLifeiOSApp, RendererGl )
