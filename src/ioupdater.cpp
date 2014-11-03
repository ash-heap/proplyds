#include <ioupdater.h>
using namespace sc;

void IOUpdater::update()
{
    kb.reset();
    kb.handleevents();
    for(sc::u32 i = 0; i < funcs.size(); i++) funcs[i]->call(kb, dt);
}

