/*
 * boot_screen.c
 *
 *  Created on: Jan 12, 2021
 *      Author: David		Original work by Jose (PTDreamer), 2017
 */

#include "boot_screen.h"
#include "oled.h"
#define SPLASH_TIMEOUT 1000


//-------------------------------------------------------------------------------------------------------------------------------
// Boot Screen variables
//-------------------------------------------------------------------------------------------------------------------------------
uint16_t profile;
uint8_t status;
uint32_t splash_time;
//-------------------------------------------------------------------------------------------------------------------------------
// Boot Screen widgets
//-------------------------------------------------------------------------------------------------------------------------------
static widget_t Widget_profile_edit;
static widget_t Widget_profile_OK;

//-------------------------------------------------------------------------------------------------------------------------------
// Boot Screen widget functions
//-------------------------------------------------------------------------------------------------------------------------------
static void * getProfile() {
	return &profile;
}

static void setProfile(uint16_t *val) {
	profile = *val;
}
static int profile_OK(widget_t *w) {
	loadProfile(profile);											// Load profile
	saveSettings(0);												// Save
	return screen_main;
}


// Credits: Jesus Vallejo  https://github.com/jesusvallejo/
const uint8_t splashBMP[] = {
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x3f, 0x3f, 0x1f, 0x8f, 0x8f, 0x87, 0x07,
		0x07, 0x03, 0xff, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x07, 0x07, 0x07, 0x0f, 0x0f,
		0x1f, 0x3f, 0x3f, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xbf, 0xbf, 0xbb, 0xab, 0xab, 0xab, 0x01, 0x70, 0x51, 0x50, 0x51, 0xd0, 0x01,
		0x10, 0x11, 0xf0, 0x11, 0x10, 0x01, 0xf0, 0x31, 0x40, 0x31, 0xf0, 0x01, 0xab, 0xab, 0xbb, 0xbb,
		0xff, 0xff, 0xff, 0x7f, 0x1f, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x83, 0xc1,
		0xe1, 0xe1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xe3, 0xe6, 0xcc, 0x88, 0x08, 0x08,
		0x08, 0x88, 0x88, 0x88, 0x08, 0x09, 0x0f, 0x1f, 0x7f, 0xff, 0xff, 0xff, 0xf3, 0xf3, 0x03, 0x03,
		0xf3, 0xf3, 0xff, 0xff, 0x03, 0x03, 0x9f, 0x9f, 0x03, 0x03, 0xff, 0xff, 0x03, 0x03, 0x93, 0x93,
		0x93, 0xf3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xfb, 0xfb, 0xfb, 0xfb, 0xeb, 0xeb, 0xaa, 0xaa, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x88,
		0xa8, 0xa8, 0x51, 0x00, 0xc8, 0xa8, 0x91, 0x00, 0x00, 0x00, 0x01, 0x00, 0xaa, 0xab, 0xaf, 0xaf,
		0xff, 0xff, 0x07, 0x02, 0x02, 0x02, 0x02, 0x02, 0xfe, 0x00, 0x00, 0x00, 0xf8, 0xfe, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xdf, 0x0f, 0x07, 0x07, 0x0f, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xf8,
		0x00, 0x03, 0xff, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0x1f, 0x1f, 0x9c, 0x9c,
		0x9f, 0x9f, 0xff, 0xff, 0x1c, 0x1c, 0x9f, 0x9f, 0x1c, 0x1c, 0xff, 0xff, 0x1c, 0x1c, 0xfc, 0xfc,
		0xfc, 0xfc, 0xff, 0xff, 0x1f, 0x1f, 0x1f, 0xdf, 0xdf, 0x1f, 0xff, 0xff, 0x1f, 0x1f, 0x9f, 0x9f,
		0x9f, 0x9f, 0xff, 0xff, 0x1f, 0x1f, 0x9f, 0x9f, 0x9f, 0x1f, 0xff, 0xff, 0x9f, 0x9f, 0x1f, 0x1f,
		0x9f, 0x9f, 0xff, 0xff, 0x1f, 0x1e, 0x3e, 0x7e, 0x1e, 0x1c, 0xf0, 0xfc, 0x18, 0x1c, 0x90, 0x9c,
		0x98, 0x9c, 0xf0, 0xfc, 0xe0, 0xfc, 0x80, 0xfc, 0xe0, 0xfc, 0xf8, 0xfc, 0xfe, 0xfe, 0xfe, 0xff,
		0xff, 0xff, 0xff, 0xf0, 0x80, 0x00, 0x00, 0x70, 0x7f, 0x70, 0x00, 0x00, 0x00, 0x03, 0x0f, 0x9f,
		0xff, 0xff, 0xff, 0xff, 0xff, 0x03, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0x9f, 0x1f, 0x13, 0x10,
		0x10, 0x10, 0x1f, 0x00, 0x00, 0x07, 0x07, 0x87, 0xf2, 0xff, 0xff, 0xff, 0xe4, 0xe4, 0xe4, 0xe4,
		0xe0, 0xe0, 0xff, 0xff, 0xe0, 0xe0, 0xe7, 0xe7, 0xe0, 0xe0, 0xff, 0xff, 0xe0, 0xe0, 0xe7, 0xe7,
		0xe7, 0xe7, 0xff, 0xff, 0xe0, 0xe0, 0xe0, 0xef, 0xef, 0xe0, 0xff, 0xff, 0xe0, 0xe0, 0xe4, 0xe4,
		0xe4, 0xe7, 0xff, 0xff, 0xe0, 0xe0, 0xfe, 0xf8, 0xe2, 0xe6, 0xff, 0xff, 0xe7, 0xe7, 0xe0, 0xe0,
		0xe7, 0xe7, 0xff, 0xff, 0xe0, 0xe0, 0xfe, 0xfc, 0xe0, 0xe0, 0xff, 0xff, 0xe0, 0xe0, 0xe7, 0xe4,
		0xe0, 0xe0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0xf8, 0xf0, 0xe0, 0xc0, 0xc0, 0xf0, 0xf8, 0xfe, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xf8, 0xf0,
		0xc0, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x20, 0x20, 0x24, 0x24,
		0x04, 0x04, 0xff, 0xff, 0xfc, 0xfc, 0x00, 0x00, 0xfc, 0xfc, 0xff, 0xff, 0x00, 0x00, 0xe4, 0xe4,
		0x00, 0x00, 0xff, 0xff, 0xfc, 0xfc, 0x00, 0x00, 0xfc, 0xfc, 0xff, 0xff, 0x3c, 0x3c, 0x00, 0x00,
		0x3c, 0x3c, 0xff, 0xff, 0x00, 0x00, 0x3c, 0x3c, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xf9, 0xf3,
		0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x52, 0x6d, 0xdf, 0xab, 0x7f, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0xc0, 0x8f, 0x1f, 0x3f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0xff,
		0x3f, 0x3f, 0x3f, 0x3f, 0xff, 0x3f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f,
		0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f,
		0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0xff, 0x1f, 0x1f, 0x0f, 0xff, 0x7f, 0x7f,
		0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x79, 0x76, 0x55, 0x7e, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xff,
		0xfc, 0xfc, 0xfc, 0xfc, 0xff, 0xfc, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8,
		0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8,
		0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xff, 0xf8, 0xf8, 0xf0, 0xff, 0xfe, 0xfe,
		0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};
UG_BMP_MONO splash = {
		p: splashBMP,
		width: 128,
		height: 64
};


int boot_screen_processInput(screen_t * scr, RE_Rotation_t input, RE_State_t *state) {

	if(HAL_GetTick() - splash_time > SPLASH_TIMEOUT){		// After splash timeout
		if(systemSettings.setupMode){
			FillBuffer(C_BLACK,fill_dma);					// Clear screen
			Widget_profile_edit.enabled=1;					// Enable widgets
			Widget_profile_OK.enabled=1;
			UG_FontSelect(&FONT_8X14_reduced);
			UG_PutString(19,0,"First boot!");//11			// Draw message
			UG_PutString(7,15,"Select profile");//14
			UG_DrawLine(0, 30, UG_GetXDim()-1,30, C_WHITE );
		}
		else{
			return screen_main;
		}
	}
	else{
		return -1;
	}

	return default_screenProcessInput(scr, input, state);	// Draw screen
}


void boot_screen_init(screen_t * scr){
	profile=systemSettings.settings.currentProfile;

	if(		(profile!=Profile_T12)	&&					// If unknown tip
			(profile!=Profile_C210)	&&
			(profile!=Profile_C245))	{

		profile=Profile_C210;							// For safety, set C210 profile by default, has the lowest Output TC
		systemSettings.setupMode=1;
	}
	splash_time = HAL_GetTick();
	UG_DrawBMP_MONO(0,0, &splash);
	default_init(scr);
}

void boot_screen_setup(screen_t *scr) {
	widget_t* w;
	screen_setDefaults(scr);
	scr->processInput = &boot_screen_processInput;
	scr->init = &boot_screen_init;

	// Profile select
	w = &Widget_profile_edit;
	screen_addWidget(w,scr);
	widgetDefaultsInit(w, widget_multi_option);
	w->posX = 12;
	w->posY = 40;
	w->font_size = &FONT_10X16_reduced;
	w->editable.inputData.getData = &getProfile;
	w->editable.inputData.type = field_uinteger16;
	w->editable.big_step = 1;
	w->editable.step = 1;
	w->editable.selectable.tab = 0;
	w->editable.setData = (void (*)(void *))&setProfile;
	w->editable.max_value = ProfileSize-1;
	w->editable.min_value = 0;
	w->displayWidget.hasEndStr = 0;
	w->reservedChars = 4;
	w->multiOptionWidget.options = profileStr;
	w->multiOptionWidget.numberOfOptions = 3;
	w->enabled=0;

	// OK Button
	w = &Widget_profile_OK;
	screen_addWidget(w,scr);
	widgetDefaultsInit(w, widget_button);
	w->font_size = &FONT_10X16_reduced;
	w->posX = 95;
	w->posY = 40                                                                                                                                                                                                                                                                                                                                                                                                                    ;
	strcpy(w->displayString, "OK");
	w->reservedChars = 2;
	w->buttonWidget.selectable.tab = 1;
	w->buttonWidget.action = &profile_OK;
	w->enabled=0;

}
