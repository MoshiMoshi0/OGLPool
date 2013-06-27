/*
 * TestMenu.cpp
 *
 *  Created on: 24-06-2013
 *      Author: _CORE7
 */

#include "RandomPolygonMenu.h"
#include "Component/SliderComponent.h"
#include "Component/LabelComponent.h"
#include <World/RandomTableWorld.h>
#include <Shape/Polygon/SmoothRandomPolygon.h>
#include <iostream>
#include <sstream>
#include <World/GameWorld.h>
using namespace std;

namespace OGLPool {

RandomPolygonMenu::RandomPolygonMenu( App* app ) : Menu( app ){
	seedSlider = new SliderComponent( vec2(200, 20), this );
	polygonSidesSlider = new SliderComponent( vec2(200, 40), this );
	polygonPointsSlider = new SliderComponent( vec2(200, 60), this );
	polygonNumTriesSlider = new SliderComponent( vec2(200, 80), this );
	polygonScaleSlider = new SliderComponent( vec2(200, 100), this );
	polygonHolesSlider = new SliderComponent( vec2(200, 120), this );

	seedLabel = new LabelComponent( "0", vec2(260, 16), this );
	polygonSidesLabel = new LabelComponent( "5", vec2(260, 36), this );
	polygonPointsLabel = new LabelComponent( "5", vec2(260, 56), this );
	polygonNumTriesLabel = new LabelComponent( "10", vec2(260, 76), this );
	polygonScaleLabel = new LabelComponent( "8", vec2(260, 96), this );
	polygonHolesLabel = new LabelComponent( "0", vec2(260, 116), this );

	polygonTypeCheckbox = new CheckboxComponent( {"Smooth random polygon", "Random polygon"}, vec2(320, 20), this );
	polygonTypeCheckbox->setSelected( 0 );

	startButton = new ButtonComponent( "START", vec2( 650, 50), [=](){
		srand( seedSlider->getPercent() * RAND_MAX );
		app->setWorld( new GameWorld( cueTable ) );
		app->setMenu( 0 );
	}, this );

	errorLabel = new LabelComponent( "", vec2(320,90), this );

	world = new RandomTableWorld();

	addComponent( new LabelComponent( "Seed:", vec2(50, 16), this ) );
	addComponent( new LabelComponent( "Sides:", vec2(50, 36), this ) );
	addComponent( new LabelComponent( "Points", vec2(50, 56), this ) );
	addComponent( new LabelComponent( "NumTries:", vec2(50, 76), this ) );
	addComponent( new LabelComponent( "Scale:", vec2(50, 96), this ) );
	addComponent( new LabelComponent( "Holes:", vec2(50, 116), this ) );

	addComponent( seedSlider );
	addComponent( polygonSidesSlider );
	addComponent( polygonPointsSlider );
	addComponent( polygonNumTriesSlider );
	addComponent( polygonScaleSlider );
	addComponent( polygonHolesSlider );

	addComponent( seedLabel );
	addComponent( polygonSidesLabel );
	addComponent( polygonPointsLabel );
	addComponent( polygonNumTriesLabel );
	addComponent( polygonScaleLabel );
	addComponent( polygonHolesLabel );

	addComponent( polygonTypeCheckbox );
	addComponent( startButton );
	addComponent( errorLabel );

	seedSlider->setValue( 0 );
	polygonSidesSlider->setValue( 0 );
	polygonPointsSlider->setValue( 0 );
	polygonNumTriesSlider->setValue( 0 );
	polygonScaleSlider->setValue( 0 );
	polygonHolesSlider->setValue( 0 );

	generateTable();
}

void RandomPolygonMenu::generateTable(){
	world->clearBodies();
	uint seed = seedSlider->getPercent() * RAND_MAX;
	uint sides = polygonSidesSlider->getValue(5, 30);
	uint points = polygonPointsSlider->getValue(5, 100);
	uint numTries = polygonNumTriesSlider->getValue( 10, 200 );
	uint holes = polygonHolesSlider->getValue( 0, 20 );
	float scale = polygonScaleSlider->getValue( 8, 100 );

	static auto fToStr = [=] ( float t ) -> string { ostringstream os; os<<t; return os.str(); };
	static auto iToStr = [=] ( int t ) -> string { ostringstream os; os<<t; return os.str(); };

	seedLabel->setText( iToStr( seed ) );
	polygonSidesLabel->setText( iToStr( sides ) );
	polygonPointsLabel->setText( iToStr( points ) );
	polygonNumTriesLabel->setText( iToStr( numTries ) );
	polygonScaleLabel->setText( fToStr( scale ) );
	polygonHolesLabel->setText( iToStr( holes ) );

	srand( seed );
	switch ( polygonTypeCheckbox->getSelectedIndex() ) {
		case 0: {
			SmoothRandomPolygon shape;
			if( shape.generate( SmoothRandomPolygon::ROUND, points, sides, scale, 1.0f, numTries ) ){
				cueTable = CueTable( shape, holes );
				world->addBody( cueTable.tableMesh );
				errorLabel->setText( "" );
			}else{
				errorLabel->setText( "INVALID DATA" );
			}
			break;
		}

		case 1: {
			RandomPolygon shape;
			if( shape.generate( points, sides, scale, numTries ) ){
				cueTable = CueTable( shape, holes );
				world->addBody( cueTable.tableMesh );
				errorLabel->setText( "" );
			}else{
				errorLabel->setText( "INVALID DATA" );
			}
			break;
		}
		default:
			break;
	}
}

RandomPolygonMenu::~RandomPolygonMenu(){
	if( world ) delete world;
}

void RandomPolygonMenu::render(){
	world->render();

	window->pushGLStates();
	Menu::render();
	window->popGLStates();
}
void RandomPolygonMenu::update( float dt ){
	world->update( dt );
	Menu::update( dt );

	if( isChanged() ){
		generateTable();
	}
}

} /* namespace OGLPool */
