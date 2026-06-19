#include "level_editor_menu.h"
#include "raygui.h"

LevelEditorMenu::LevelEditorMenu()
    : menuHeight(26), fileOpen(false), editOpen(false), viewOpen(false), helpOpen(false) {}

void LevelEditorMenu::CloseAllMenus() {
    fileOpen = editOpen = viewOpen = helpOpen = false;
}

int LevelEditorMenu::GetMenuHeight() const {
    return menuHeight;
}

bool LevelEditorMenu::IsAnyMenuOpen() const {
    return fileOpen || editOpen || viewOpen || helpOpen;
}

bool LevelEditorMenu::IsPointInsideMenu(const Vector2 &point) const {
    if (point.y <= (float)menuHeight) return true;
    if (fileOpen && point.x >= 6 && point.x <= 166 && point.y <= menuHeight + 122) return true;
    if (editOpen && point.x >= 66 && point.x <= 226 && point.y <= menuHeight + 92) return true;
    if (viewOpen && point.x >= 126 && point.x <= 286 && point.y <= menuHeight + 92) return true;
    if (helpOpen && point.x >= 186 && point.x <= 406 && point.y <= menuHeight + 112) return true;
    return false;
}

void LevelEditorMenu::DrawMenuBar() {
    DrawRectangle(0, 0, GetScreenWidth(), menuHeight, LIGHTGRAY);
    Rectangle fileBtn = { 6.0f, 3.0f, 56.0f, 20.0f };
    Rectangle editBtn = { 66.0f, 3.0f, 56.0f, 20.0f };
    Rectangle viewBtn = { 126.0f, 3.0f, 56.0f, 20.0f };
    Rectangle helpBtn = { 186.0f, 3.0f, 56.0f, 20.0f };

    if (GuiButton(fileBtn, "File")) {
        fileOpen = !fileOpen;
        editOpen = viewOpen = helpOpen = false;
    }
    if (GuiButton(editBtn, "Edit")) {
        editOpen = !editOpen;
        fileOpen = viewOpen = helpOpen = false;
    }
    if (GuiButton(viewBtn, "View")) {
        viewOpen = !viewOpen;
        fileOpen = editOpen = helpOpen = false;
    }
    if (GuiButton(helpBtn, "Help")) {
        helpOpen = !helpOpen;
        fileOpen = editOpen = viewOpen = false;
    }
}

void LevelEditorMenu::DrawMenuWindows(MenuAction &action, bool &shouldClose) {
    if (fileOpen) {
        Rectangle fileWin = { 6.0f, (float)menuHeight + 2.0f, 160.0f, 170.0f };
        GuiWindowBox(fileWin, "File");
        Rectangle btnNew = { fileWin.x + 8, fileWin.y + 24, fileWin.width - 16, 24 };
        Rectangle btnOpen = { fileWin.x + 8, fileWin.y + 52, fileWin.width - 16, 24 };
        Rectangle btnSave = { fileWin.x + 8, fileWin.y + 80, fileWin.width - 16, 24 };
        Rectangle btnSaveAs = { fileWin.x + 8, fileWin.y + 108, fileWin.width - 16, 24 };
        Rectangle btnExit = { fileWin.x + 8, fileWin.y + 136, fileWin.width - 16, 24 };

        if (GuiButton(btnNew, "New")) { action = MenuAction::New; fileOpen = false; }
        if (GuiButton(btnOpen, "Open")) { action = MenuAction::Open; fileOpen = false; }
        if (GuiButton(btnSave, "Save")) { action = MenuAction::Save; fileOpen = false; }
        if (GuiButton(btnSaveAs, "Save As")) { action = MenuAction::SaveAs; fileOpen = false; }
        if (GuiButton(btnExit, "Exit")) { shouldClose = true; fileOpen = false; }
    }

    if (editOpen) {
        Rectangle editWin = { 66.0f, (float)menuHeight + 2.0f, 160.0f, 120.0f };
        GuiWindowBox(editWin, "Edit");
        Rectangle btnUndo = { editWin.x + 8, editWin.y + 24, editWin.width - 16, 24 };
        Rectangle btnGenerate = { editWin.x + 8, editWin.y + 52, editWin.width - 16, 24 };
        if (GuiButton(btnUndo, "Undo")) { action = MenuAction::Undo; editOpen = false; }
        if (GuiButton(btnGenerate, "Gen Power-ups")) { action = MenuAction::GeneratePowerUps; editOpen = false; }
    }

    if (viewOpen) {
        Rectangle viewWin = { 126.0f, (float)menuHeight + 2.0f, 160.0f, 90.0f };
        GuiWindowBox(viewWin, "View");
        Rectangle btnGrid = { viewWin.x + 8, viewWin.y + 24, viewWin.width - 16, 24 };
        if (GuiButton(btnGrid, "Toggle Grid")) { action = MenuAction::ToggleGrid; viewOpen = false; }
    }

    if (helpOpen) {
        Rectangle helpWin = { 186.0f, (float)menuHeight + 2.0f, 220.0f, 110.0f };
        GuiWindowBox(helpWin, "Help");
        DrawText("Level Editor v0.1\nRight-click drag to pan.\nMouse wheel to zoom.\nCtrl+N/O/S/Z shortcuts.", (int)helpWin.x + 8, (int)helpWin.y + 28, 12, DARKGRAY);
    }
}

void LevelEditorMenu::HandleKeyboardShortcuts(bool &shouldClose, MenuAction &action) {
    if (IsKeyPressed(KEY_ESCAPE)) {
        if (IsAnyMenuOpen()) {
            CloseAllMenus();
            return;
        }
        shouldClose = true;
        return;
    }

    if (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) {
        if (IsKeyPressed(KEY_N)) { action = MenuAction::New; CloseAllMenus(); }
        if (IsKeyPressed(KEY_O)) { action = MenuAction::Open; CloseAllMenus(); }
        if (IsKeyPressed(KEY_S)) {
            if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
                action = MenuAction::SaveAs;
            } else {
                action = MenuAction::Save;
            }
            CloseAllMenus();
        }
        if (IsKeyPressed(KEY_Z)) { action = MenuAction::Undo; CloseAllMenus(); }
    }

    if (IsKeyPressed(KEY_F1)) {
        action = MenuAction::ShowHelp;
        CloseAllMenus();
        helpOpen = true;
    }
}

void LevelEditorMenu::Update(bool &shouldClose, MenuAction &action) {
    HandleKeyboardShortcuts(shouldClose, action);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mouse = GetMousePosition();
        if (mouse.y > (float)menuHeight && !IsPointInsideMenu(mouse)) {
            CloseAllMenus();
        }
    }
}

void LevelEditorMenu::Draw() {
    DrawMenuBar();
    DrawMenuWindows();
}

void LevelEditorMenu::DrawMenuWindows() {
    MenuAction dummyAction = MenuAction::None;
    bool dummyClose = false;
    DrawMenuWindows(dummyAction, dummyClose);
}
