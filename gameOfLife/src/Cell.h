//
//  Cell.h
//  gameOfLife
//
//  Created by Chris on 29/11/2011.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef gameOfLife_Cell_h
#define gameOfLife_Cell_h

using namespace std;

class Cell {
public:
    
    bool isAlive;
    bool nextIsAlive;
    
    float width;
    float height;
    
    float x,y;
    float life;
    cinder::ColorA colour;
    float brightness;
    
    vector<Cell*> neighbours;
    
	void setup();
	void update();
    void updateState();
	void draw();
};

#endif
