/*
 * TestMenu.cpp
 *
 *  Created on: 24-06-2013
 *      Author: _CORE7
 */

#include "TestMenu.h"
#include "Component/SliderComponent.h"
#include "Component/LabelComponent.h"
#include <World/RandomTableWorld.h>
#include <Shape/Polygon/SmoothRandomPolygon.h>
#include <iostream>
#include <sstream>
using namespace std;

namespace OGLPool {

TestMenu::TestMenu( RenderWindow* window ) : Menu( window ){
	seedSlider = new SliderComponent( vec2(200, 20), this, window );
	polygonSidesSlider = new SliderComponent( vec2(200, 40), this, window );
	polygonPointsSlider = new SliderComponent( vec2(200, 60), this, window );
	polygonNumTriesSlider = new SliderComponent( vec2(200, 80), this, window );
	polygonScaleSlider = new SliderComponent( vec2(200, 100), this, window );


	seedLabel = new LabelComponent( "0", vec2(260, 16), this, window );
	polygonSidesLabel = new LabelComponent( "0", vec2(260, 36), this, window );
	polygonPointsLabel = new LabelComponent( "0", vec2(260, 56), this, window );
	polygonNumTriesLabel = new LabelComponent( "0", vec2(260, 76), this, window );
	polygonScaleLabel = new LabelComponent( "0", vec2(260, 96), this, window );

	world = new RandomTableWorld();

	addComponent( new LabelComponent( "Seed:", vec2(50, 16), this, window ) );
	addComponent( new LabelComponent( "Sides:", vec2(50, 36), this, window ) );
	addComponent( new LabelComponent( "Points", vec2(50, 56), this, window ) );
	addComponent( new LabelComponent( "NumTries:", vec2(50, 76), this, window ) );
	addComponent( new LabelComponent( "Scale:", vec2(50, 96), this, window ) );

	addComponent( seedSlider );
	addComponent( polygonSidesSlider );
	addComponent( polygonPointsSlider );
	addComponent( polygonNumTriesSlider );
	addComponent( polygonScaleSlider );

	addComponent( seedLabel );
	addComponent( polygonSidesLabel );
	addComponent( polygonPointsLabel );
	addComponent( polygonNumTriesLabel );
	addComponent( polygonScaleLabel );

	seedSlider->setValue( 0 );
	polygonSidesSlider->setValue( 0 );
	polygonPointsSlider->setValue( 0 );
	polygonNumTriesSlider->setValue( 0 );
	polygonScaleSlider->setValue( 0 );
}

void TestMenu::generateTable(){
	world->clearBodies();
	uint seed = seedSlider->getPercent() * RAND_MAX;
	uint sides = polygonSidesSlider->getValue(5, 30);
	uint points = polygonPointsSlider->getValue(5, 100);
	uint numTries = polygonNumTriesSlider->getValue( 10, 1000 );
	float scale = polygonScaleSlider->getValue( 8, 100 );

	static auto fToStr = [=] ( float t ) -> string { ostringstream os; os<<t; return os.str(); };
	static auto iToStr = [=] ( int t ) -> string { ostringstream os; os<<t; return os.str(); };

	seedLabel->setText( iToStr( seed ) );
	polygonSidesLabel->setText( iToStr( sides ) );
	polygonPointsLabel->setText( iToStr( points ) );
	polygonNumTriesLabel->setText( iToStr( numTries ) );
	polygonScaleLabel->setText( fToStr( scale ) );

	srand( seed );
	SmoothRandomPolygon shape;
	if( shape.generate( SmoothRandomPolygon::ROUND, points, sides, scale, 1.0f, numTries ) ){
		cueTable = CueTable( shape, vector<vec2>() );
		world->addBody( cueTable.tableMesh );
	}else{
		cout << "INVALID DATA" << endl;
	}
}

TestMenu::~TestMenu(){
	if( world ) delete world;
}

void TestMenu::render(){
	world->render();

	window->pushGLStates();
	Menu::render();
	window->popGLStates();
}
void TestMenu::update( float dt ){
	world->update( dt );
	Menu::update( dt );

	if( isChanged() ){
		generateTable();
	}
}

} /* namespace OGLPool */
