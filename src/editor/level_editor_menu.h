#ifndef LEVEL_EDITOR_MENU_H
#define LEVEL_EDITOR_MENU_H

#include "raylib.h"

enum class MenuAction {
    None,
    New,
    Open,
    Save,
    SaveAs,
    Undo,
    ToggleGrid,
    GeneratePowerUps,
    ShowHelp
};

class LevelEditorMenu {
public:
    LevelEditorMenu();
    void Update(bool &shouldClose, MenuAction &action);
    void Draw();
    int GetMenuHeight() const;
    bool IsAnyMenuOpen() const;
    bool IsPointInsideMenu(const Vector2 &point) const;

private:
    int menuHeight;
    bool fileOpen;
    bool editOpen;
    bool viewOpen;
    bool helpOpen;

    void CloseAllMenus();
    void DrawMenuBar();
    void DrawMenuWindows(MenuAction &action, bool &shouldClose);
    void DrawMenuWindows();
    void HandleKeyboardShortcuts(bool &shouldClose, MenuAction &action);
};

#endif
