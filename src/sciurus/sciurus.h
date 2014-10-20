#ifndef SCIURUS_SCIURUS_H
#define SCIURUS_SCIURUS_H

#define GLM_FORCE_RADIANS
#include <string>

namespace sc
{

bool sc_init();
void sc_quit();
std::string sc_getError();

}

#endif

