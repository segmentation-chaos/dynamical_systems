#include "clickspace.h"

bool olcClickSpace::OnUserCreate()
{
    return true;
}

bool olcClickSpace::OnUserUpdate(float fElapsedTime)
{   
    switch (RunningID)
    {
        case RUN_MENU:
            RunningID = menu.run_frame(*this);
            if (RunningID > SET_2DMAP &&
                RunningID < SET_1DMAP)
            {
                map2D.SetApplication((*this), RunningID);
                RunningID = RUN_2DMAP;
            }
            if (RunningID > SET_1DMAP)
            {
                map1D.SetApplication((*this), RunningID);
                RunningID = RUN_1DMAP;
            }
            break;
        case RUN_2DMAP:
            RunningID = map2D.run_frame(*this);
            if (RunningID == RUN_MENU) { menu.mm.Open(&menu.mo["main"]); }
            break;
        case RUN_1DMAP:
            RunningID = map1D.run_frame(*this);
            if (RunningID == RUN_MENU) { menu.mm.Open(&menu.mo["main"]); }
            break;
        case EXIT_ALL:
            return false;
        default:
            break;
    }

    return true;
}