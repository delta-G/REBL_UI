/*
 * REBLMenu.h
 *
 *  Created on: Oct 10, 2015
 *      Author: David
 */

#ifndef REBLMENU_H_
#define REBLMENU_H_

#include "Arduino.h"
#include "REBL_UI.h"

#include "MenuClass.h"
//#include "Defines.h"
//#include "REBLInterface.h"
//#include "REBLDisplay.h"


class REBLMenu: public MenuClass {

public:
	int updateSelection();
	boolean selectionMade();
	boolean checkForCancel();
	void displayMenu();

};

extern REBLMenu reblMenu;

#endif /* REBLMENU_H_ */
