#include "olcPixelGameEngine.h"

#include <unordered_map>

// Manually set pixel ratio to keep PGE standard as (1,1)
// Must be fixed as [nPixelScale = 4]
constexpr int32_t nPixelScale = 4;
constexpr int32_t nPatch = 8 * nPixelScale;

class menuobject
{
public:
    menuobject();
    menuobject(const std::string& name);
    
    menuobject& SetTable(int nColumns, int nRows);
    menuobject& SetID(int32_t id);
    menuobject& Enable(bool e);
    menuobject& operator[](const std::string& name);

    int32_t GetID() { return nID; }
    bool Enabled() { return bEnabled; }
    std::string& GetName() { return sName; }
    olc::vi2d GetSize() { return {int32_t(sName.size()), 1}; }
    olc::vi2d GetSizeInPatches() { return vSizeInPatches; }
    olc::vi2d& GetCursorPosition() { return vCursorPos; }
    menuobject* GetSelectedItem() { return &items[nCursorItem]; }

    bool HasChildren() { return !items.empty(); }

    void DrawSelf(olc::PixelGameEngine& pge, olc::Sprite* sprGFX, olc::vi2d vScreenOffset);
    void Build();

    void ClampCursor();
    void OnUp();
    void OnDown();
    void OnLeft();
    void OnRight();

    menuobject* OnConfirm();

protected:
    std::string sName;
    bool bEnabled = true;
    int32_t nID = -1;
    int32_t nTotalRows = 0;
    int32_t nTopVisibleRow = 0;
    int32_t nCursorItem = 0;
    olc::vi2d vCellTable = {1, 0};
    olc::vi2d vCellSize = {0, 0};
    olc::vi2d vCellPadding = {2, 1};
    olc::vi2d vSizeInPatches = {0, 0};    
    olc::vi2d vCellCursor = {0, 0};
    olc::vi2d vCursorPos = {0, 0};
    const olc::vi2d vPatchSize = {nPatch, nPatch};

    std::unordered_map<std::string, size_t> itemPointer;
    std::vector<menuobject> items;
};