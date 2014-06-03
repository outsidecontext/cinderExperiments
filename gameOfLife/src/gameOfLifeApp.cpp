#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include "cinder/params/Params.h"

#include "Cell.h"

#define MAX_X_CELLS 1000
#define MAX_Y_CELLS 1000

using namespace ci;
using namespace ci::app;
using namespace std;

class gameOfLifeApp : public AppBasic {
  public:
    
    // params gui
    params::InterfaceGl mParams;
    
    // cells
    Cell cellsXY[MAX_X_CELLS][MAX_Y_CELLS];
    // cell dimensions
    float cellHeight;
    float cellWidth;
    // x/y max
    int maxx;
    int maxy;
    // override cell colour
    ColorA colour;
    
    void prepareSettings( Settings *settings );
	void setup();
    void setupCells();
    void keyDown( KeyEvent event );
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
    
    
};


void gameOfLifeApp::prepareSettings( Settings *settings )
{
	settings->setWindowSize( 800, 600 );
	settings->setFrameRate( 30.0f );
}


void gameOfLifeApp::setup()
{
    
    // defaults
    cellHeight = 10;
    cellWidth = 10;
    
    // Setup the parameters
	mParams = params::InterfaceGl( "App parameters", Vec2i( 200, 400 ) );
	mParams.addParam( "cell height", &cellHeight, "min=5 max=50 step=1 keyIncr=a keyDecr=A" );
	mParams.addParam( "cell width", &cellWidth, "min=5 max=50 step=1 keyIncr=z keyDecr=Z" );
	mParams.addButton( "restart", std::bind( &gameOfLifeApp::setupCells, this ) );
	mParams.addParam( "color", &colour, "" );	
    /*
	mParams.addText( "text", "label=`This is a label without a parameter.`" );
	mParams.addParam( "String ", &mString, "" );
     */
    
    setupCells();
}

void gameOfLifeApp::setupCells()
{
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

void gameOfLifeApp::mouseDown( MouseEvent event )
{
    // restart
    setupCells();
}


void gameOfLifeApp::update()
{
    // update next state based on rules
    for (int x=0; x<maxx && x<MAX_X_CELLS; x++) {
        for (int y=0; y<maxy && x<MAX_Y_CELLS; y++) {
            Cell& cell = cellsXY[x][y];
            cell.colour = cell.colour.lerp(0.5, colour);
            cell.updateState();
        }
    }
}


void gameOfLifeApp::draw()
{
    // bg colour
    gl::clear( Color( 0.2, 0.2, 0.2 ) );
    
    
    // Draw the interface
	params::InterfaceGl::draw();
    
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


void gameOfLifeApp::keyDown(KeyEvent event)
{
    // fullscreen
    if ( event.getChar() == 'f') setFullScreen(!isFullScreen());
	// quit
	if ( event.getChar() == KeyEvent::KEY_ESCAPE ) quit();
    
}


CINDER_APP_BASIC( gameOfLifeApp, RendererGl )
