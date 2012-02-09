//
//  Cell.cpp
//  gameOfLife
//
//  Created by Chris on 29/11/2011.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include <iostream>

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

#include "Cell.h"


using namespace ci;
using namespace ci::app;
using namespace std;


void Cell::setup()
{
    // totally random colour
    colour.r = Rand::randFloat();
    colour.g = Rand::randFloat();
    colour.b = Rand::randFloat();
}


void Cell::update()
{
    // next state
    isAlive = nextIsAlive;
    
    // update life
    if(isAlive) life += 2;
    else life -= 0.05;
    
    // clamp life
    life = constrain(life, 0.0f, width-2);
    
    // map brightness
    brightness = lmap(life, .0f, width-2, 0.0f, 1.0f);
}


void Cell::updateState()
{
    // neighbours
    int t = 0;
    for (int i=0; i<neighbours.size(); i++)
        if (neighbours[i]->isAlive) t++;
    
    // GOL rules
    if (t < 2 || t > 3) {
        nextIsAlive = false;
    } else if (t == 3) {
        nextIsAlive = true;
    }
    
    
}


void Cell::draw()
{
    gl::color(colour.r*brightness, colour.g*brightness, colour.b*brightness);
    Rectf rect( x, y, x+life, y + life );
    rect.offsetCenterTo(Vec2f(x,y));
    gl::drawSolidRect( rect );
}