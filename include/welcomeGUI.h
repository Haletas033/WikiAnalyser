//
// Created by halet on 2/18/2026.
//

#ifndef WELCOMEGUI_H
#define WELCOMEGUI_H

#define WIKI_OPTION(i) i+6
#define ZIG_OPTION(i) i+9
#define FINISH_SETUP 18

#define WELCOME_TEXT_START 0
#define WELCOME_TEXT_END 10

#define PERFORM_CHECK_TEXT_START 10
#define PERFORM_CHECK_TEXT_END 11

#define ZIG_OPTIONS_TEXT_START 11
#define ZIG_OPTIONS_TEXT_END 16

#define SUCCESS_TEXT_START 16
#define SUCCESS_TEXT_END 18

#define DEFAULT_BUTTON_SIZE 15
#define TYPING_SPEED 50
#define WAIT_AFTER_TEXT 500

#include "GUI.h"
#ifdef __WIN32__
#include "../include/WINGUI.h"
#endif

void WelcomeGUI();

#endif //WELCOMEGUI_H
