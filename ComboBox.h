// ComboBox.h

#pragma once

#include <windows.h>
#include <commctrl.h>

#include "Resource.h"

#include "..\Common\Ascii.h"
#include "..\Common\Common.h"

#include "..\Classes\ArgumentListClass.h"
#include "..\Classes\ButtonWindowClass.h"
#include "..\Classes\ComboBoxWindowClass.h"
#include "..\Classes\DroppedFilesClass.h"
#include "..\Classes\FontClass.h"
#include "..\Classes\ListBoxWindowClass.h"
#include "..\Classes\MessageClass.h"
#include "..\Classes\MenuClass.h"
#include "..\Classes\StatusBarWindowClass.h"
#include "..\Classes\WindowClass.h"
#include "..\Classes\WindowClassClass.h"

#define MAIN_WINDOW_CLASS_NAME													"Main ComboBox Window Class"

#define MAIN_WINDOW_CLASS_MENU_NAME												MAKEINTRESOURCE( IDR_MAIN_MENU )

#define MAIN_WINDOW_TEXT														"ComboBox"

#define COMBO_BOX_FILE_NAME														"ComboBox.txt"

#define BUTTON_WINDOW_ID														( WM_USER + 1 )
#define BUTTON_WINDOW_TEXT														"Browse"
#define BUTTON_WINDOW_WIDTH														50

#define ABOUT_MESSAGE_TEXT														"ComboBox\r\n"						\
																				"\r\n"								\
																				"Written by Jim Smith\r\n"			\
																				"\r\n"								\
																				"January 2025"
