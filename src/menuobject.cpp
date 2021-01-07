#include "menuobject.h"

menuobject::menuobject()
{
    sName = "root";
}

menuobject::menuobject(const std::string& name)
{
    sName = name;
}

menuobject& menuobject::SetTable(int nColumns, int nRows)
{
    vCellTable = {nColumns, nRows};
    return *this;
}

menuobject& menuobject::SetID(int32_t id)
{
    nID = id;
    return *this;
}

menuobject& menuobject::Enable(bool e)
{
    bEnabled = e;
    return *this;
}

menuobject& menuobject::operator[](const std::string& name)
{
    if (itemPointer.count(name) == 0)
    {
        itemPointer[name] = items.size();
        items.push_back(menuobject(name));
    }

    return items[itemPointer[name]];
}

void menuobject::DrawSelf(olc::PixelGameEngine& pge, olc::Sprite* sprGFX, 
                          olc::vi2d vScreenOffset)
{
    // Record current pixel mode user is using
    olc::Pixel::Mode currentPixelMode = pge.GetPixelMode();
    pge.SetPixelMode(olc::Pixel::ALPHA);

    // Draw Panel & Border
    olc::vi2d vPatchPos = {0, 0};
    for (vPatchPos.x = 0; vPatchPos.x < vSizeInPatches.x; vPatchPos.x += 1)
    {
        for (vPatchPos.y = 0; vPatchPos.y < vSizeInPatches.y; vPatchPos.y += 1)
        {
            // Determine position in screen space
            olc::vi2d vScreenLocation = vPatchPos * nPatch + vScreenOffset;

            // Calculate which patch is needed
            olc::vi2d vSourcePatch = {0, 0};
            if (vPatchPos.x > 0) { vSourcePatch.x = 1; }
            if (vPatchPos.x == (vSizeInPatches.x - 1)) { vSourcePatch.x = 2; }
            if (vPatchPos.y > 0) { vSourcePatch.y = 1; }
            if (vPatchPos.y == (vSizeInPatches.y - 1)) { vSourcePatch.y = 2; }

            // Draw Actual Patch
            pge.DrawPartialSprite(vScreenLocation, sprGFX, vSourcePatch * nPatch, vPatchSize);
        }
    }

    // === Draw Panel Contents
    olc::vi2d vCell = {0, 0};
    vPatchPos = {1, 1};

    // Work out visible items
    int32_t nTopLeftItem = nTopVisibleRow * vCellTable.x;
    int32_t nBottomRightItem = vCellTable.y * vCellTable.x + nTopLeftItem;

    // Clamp to size of child item vector
    nBottomRightItem = std::min(int32_t(items.size()), nBottomRightItem);
    int32_t nVisibleItems = nBottomRightItem - nTopLeftItem;

    // Draw Upward Scroll Marker (if required)
    if (nTopVisibleRow > 0)
    {
        vPatchPos = {vSizeInPatches.x - 2, 0};
        olc::vi2d vScreenLocation = vPatchPos * nPatch + vScreenOffset;
        olc::vi2d vSourcePatch = {3, 0};
        pge.DrawPartialSprite(vScreenLocation, sprGFX, vSourcePatch * nPatch, vPatchSize);
    }

    // Draw Downward Scroll Marker (if required)
    if ((nTotalRows - nTopVisibleRow) > vCellTable.y)
    {
        vPatchPos = {vSizeInPatches.x - 2, vSizeInPatches.y - 1};
        olc::vi2d vScreenLocation = vPatchPos * nPatch + vScreenOffset;
        olc::vi2d vSourcePatch = {3, 2};
        pge.DrawPartialSprite(vScreenLocation, sprGFX, vSourcePatch * nPatch, vPatchSize);
    }

    // Draw Visible Items
    for (int32_t i = 0; i < nVisibleItems; i++)
    {
        // Cell location
        vCell.x = i % vCellTable.x;
        vCell.y = i / vCellTable.x;

        // Patch location (including border offset and padding)
        vPatchPos.x = vCell.x * (vCellSize.x + vCellPadding.x) + 1;
        vPatchPos.y = vCell.y * (vCellSize.y + vCellPadding.y) + 1;

        // Actual screen location in pixels
        olc::vi2d vScreenLocation = vPatchPos * nPatch + vScreenOffset;

        // Display Item Header
        pge.DrawString(vScreenLocation, items[nTopLeftItem + i].sName, 
                        items[nTopLeftItem + i].bEnabled ? olc::WHITE : olc::DARK_GREY, nPixelScale);

        // Display indicator that panel has a sub panel
        if (items[nTopLeftItem + i].HasChildren())
        {
            vPatchPos.x = vCell.x * (vCellSize.x + vCellPadding.x) + 1 + vCellSize.x;
            vPatchPos.y = vCell.y * (vCellSize.y + vCellPadding.y) + 1;
            olc::vi2d vSourcePatch = {3, 1};
            vScreenLocation = vPatchPos * nPatch + vScreenOffset;
            pge.DrawPartialSprite(vScreenLocation, sprGFX, vSourcePatch * nPatch, vPatchSize);
        }
    }

    // Calculate cursor position in screen space in case system draws it
    vCursorPos.x = (vCellCursor.x * (vCellSize.x + vCellPadding.x)) * nPatch + vScreenOffset.x - nPatch;
    vCursorPos.y = ((vCellCursor.y - nTopVisibleRow) * (vCellSize.y + vCellPadding.y)) * nPatch + vScreenOffset.y + nPatch; 

    pge.SetPixelMode(currentPixelMode);
}

void menuobject::Build()
{
    // Recursively build all children, determining their size.
    // This size is further used to indicate cell sizes if this 
    // object contains more than one item.   
    for (auto& m : items)
    {
        if (m.HasChildren())
        {
            m.Build();
        }

        // Longest child name determines cell width
        vCellSize.x = std::max(m.GetSize().x, vCellSize.x);
        vCellSize.y = std::max(m.GetSize().y, vCellSize.y);
    }

    // Adjust size of this object (in patches) if it were rendered as a panel
    vSizeInPatches.x = vCellTable.x * vCellSize.x + (vCellTable.x - 1) * vCellPadding.x + 2;
    vSizeInPatches.y = vCellTable.y * vCellSize.y + (vCellTable.y - 1) * vCellPadding.y + 2;

    // Calculate how many rows this item has to hold
    nTotalRows = (items.size() / vCellTable.x) + (((items.size() % vCellTable.x) > 0) ? 1 : 0);
}

void menuobject::ClampCursor()
{
    // Find item in children
    nCursorItem = vCellCursor.y * vCellTable.x + vCellCursor.x;

    // Clamp Cursor
    if (nCursorItem >= int32_t(items.size()))
    {
        vCellCursor.x = (items.size() % vCellTable.x) - 1;
        vCellCursor.y = (items.size() / vCellTable.x);
        nCursorItem = items.size() - 1;
    }
}
 
void menuobject::OnUp()
{
    vCellCursor.y -= 1;
    
    if (vCellCursor.y < 0) { vCellCursor.y = 0; }
    
    if (vCellCursor.y < nTopVisibleRow) 
    { 
        nTopVisibleRow -= 1;
        if (nTopVisibleRow < 0)
        {
            nTopVisibleRow = 0;
        }
    }

    ClampCursor();
}

void menuobject::OnDown()
{
    vCellCursor.y += 1;

    if (vCellCursor.y == nTotalRows) { vCellCursor.y = nTotalRows - 1; }

    if (vCellCursor.y > (nTopVisibleRow + vCellTable.y - 1))
    {
        nTopVisibleRow += 1;
        if (nTopVisibleRow > (nTotalRows - vCellTable.y))
        {
            nTopVisibleRow = nTotalRows - vCellTable.y;
        }
    }

    ClampCursor();
}

void menuobject::OnLeft()
{
    vCellCursor.x -= 1;
    if (vCellCursor.x < 0) { vCellCursor.x = 0; }
    ClampCursor();
}

void menuobject::OnRight()
{
    vCellCursor.x += 1;
    if (vCellCursor.x == vCellTable.x) { vCellCursor.x = vCellTable.x - 1; }
    ClampCursor();
}

menuobject* menuobject::OnConfirm()
{
    // Check if selected item has children
    if (items[nCursorItem].HasChildren())
    {
        return &items[nCursorItem];
    }
    else
    {
        return this;
    }   
}