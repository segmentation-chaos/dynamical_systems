#include "menumanager.h"

void menumanager::Open(menuobject* mo)
{
    Close();
    panels.push_back(mo);
}

void menumanager::Close()
{
    panels.clear();
}

void menumanager::Draw(olc::PixelGameEngine& pge, olc::Sprite* sprGFX, 
                       olc::vi2d vScreenOffset)
{
    if (panels.empty()) return;

    // Draw Visible Menu System
    for (auto& p : panels)
    {
        p->DrawSelf(pge, sprGFX, vScreenOffset);
        vScreenOffset += {10, 10};
    }

    // Draw Cursor
    olc::Pixel::Mode currentPixelMode = pge.GetPixelMode();
    pge.SetPixelMode(olc::Pixel::ALPHA);
    pge.DrawPartialSprite(panels.back()->GetCursorPosition(), sprGFX, olc::vi2d(4, 0) * nPatch, {nPatch * 2, nPatch * 2});
    pge.SetPixelMode(currentPixelMode);

}

void menumanager::OnUp()
{
    if (!panels.empty())
    {
        panels.back()->OnUp();
    }
}

void menumanager::OnDown()
{
    if (!panels.empty())
    {
        panels.back()->OnDown();
    }
}

void menumanager::OnLeft()
{
    if (!panels.empty())
    {
        panels.back()->OnLeft();
    }
}

void menumanager::OnRight()
{
    if (!panels.empty())
    {
        panels.back()->OnRight();
    }
}

void menumanager::OnBack()
{
    if (!panels.empty())
    {
        panels.pop_back();
    }
}

menuobject* menumanager::OnConfirm()
{
    if (panels.empty()) { return nullptr; }

    menuobject* next = panels.back()->OnConfirm();

    if (next == panels.back())
    {
        if (panels.back()->GetSelectedItem()->Enabled())
        {
            return panels.back()->GetSelectedItem();
        }
    }
    else
    {
        if (next->Enabled())
        {
            panels.push_back(next);
        }
    }

    return nullptr;   
}

int menumanager::GetPanelsSize()
{
    return panels.size();
}