/*
 * TestMenu.h
 *
 *  Created on: 24-06-2013
 *      Author: _CORE7
 */

#ifndef TESTMENU_H_
#define TESTMENU_H_

#include "Menu.h"
#include <World/World.h>
#include <Util/Table/CueTable.h>
#include "Component/SliderComponent.h"
#include "Component/LabelComponent.h"
#include "Component/CheckboxComponent.h"
#include "Component/ButtonComponent.h"

namespace OGLPool {

class RandomPolygonMenu : public Menu {
public:
	RandomPolygonMenu( App* app );
	virtual ~RandomPolygonMenu();

	void render();
	void update( float dt );
	void generateTable();

	CueTable cueTable;
	World* world;

	SliderComponent* seedSlider;
	SliderComponent* polygonSidesSlider;
	SliderComponent* polygonPointsSlider;
	SliderComponent* polygonNumTriesSlider;
	SliderComponent* polygonScaleSlider;
	SliderComponent* polygonHolesSlider;

	LabelComponent* seedLabel;
	LabelComponent* polygonSidesLabel;
	LabelComponent* polygonPointsLabel;
	LabelComponent* polygonNumTriesLabel;
	LabelComponent* polygonScaleLabel;
	LabelComponent* polygonHolesLabel;

	CheckboxComponent* polygonTypeCheckbox;

	ButtonComponent* startButton;
	LabelComponent* errorLabel;
};

} /* namespace OGLPool */
#endif /* TESTMENU_H_ */
