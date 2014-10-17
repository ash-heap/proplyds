#ifndef SCIURUS_SCIURUS_H
#define SCIURUS_SCIURUS_H

#define GLM_FORCE_RADIANS
#include <sciurus/types.h>
#include <sciurus/opengl.h>
#include <sciurus/window.h>
#include <sciurus/keyboard.h>
#include <string>

namespace sc
{

bool init();
void quit();
std::string getError();

}

#endif

