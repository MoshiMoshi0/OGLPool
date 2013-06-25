/*
 * TestMenu.cpp
 *
 *  Created on: 24-06-2013
 *      Author: _CORE7
 */

#include "TestMenu.h"
#include "Component/SliderComponent.h"
#include <World/RandomTableWorld.h>
#include <Shape/Polygon/RandomPolygon.h>
#include <iostream>
using namespace std;

namespace OGLPool {

TestMenu::TestMenu( RenderWindow* window ) : Menu( window ){
	seedSlider = new SliderComponent( vec2( 200, 20 ), this, window );
	polygonSidesSlider = new SliderComponent( vec2( 200, 40 ), this, window );
	polygonPointsSlider = new SliderComponent( vec2( 200, 60 ), this, window );
	polygonNumTriesSlider = new SliderComponent( vec2( 200, 80 ), this, window );
	polygonScaleSlider = new SliderComponent( vec2( 200, 100 ), this, window );

	world = new RandomTableWorld();
	cueTable = 0;

	addComponent( seedSlider );
	addComponent( polygonSidesSlider );
	addComponent( polygonPointsSlider );
	addComponent( polygonNumTriesSlider );
	addComponent( polygonScaleSlider );

	seedSlider->setValue( 0 );
	polygonSidesSlider->setValue( 0 );
	polygonPointsSlider->setValue( 0 );
	generateTable();
}

void TestMenu::generateTable(){
	world->clearBodies();
	srand( (uint)( seedSlider->getPercent() * RAND_MAX ) );

	uint sides = polygonSidesSlider->getValue(5, 30);
	uint points = polygonPointsSlider->getValue(sides, 300);
	uint numTries = polygonNumTriesSlider->getValue( 10, 1000 );
	float scale = polygonScaleSlider->getValue( 5, 100 );

	RandomPolygon shape;
	if( shape.generate( sides, points, scale, numTries ) ){
		cueTable = new CueTable( shape, vector<vec2>() );
		world->addBody( cueTable->tableMesh );
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
