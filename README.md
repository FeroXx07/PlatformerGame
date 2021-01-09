# Frost's adventures

## Description

A platformer game made for the subject of video game development of the grade of development and design of video games at the UPC in Barcelona.

## Key Features

 - XML configuration file loading.
 - Load and Save game state using XML file.
 - Tiled TMX map loading and drawing with orthographic view.
 - Map collision detection (platforms of the game).
 - Map navigation: player movement and jumping.
 - Logo screen with fade-in fade-out.
 - Title screen (press ENTER to continue).
 - Player walk/jump/die/idle animations.
 - Ending screen (on dying).
 - DEBUG keys.
 - Destroy enemies by shooting.
 - Game items to recover health / lives.
 - Game collectable items to accumulate points: coins, stars...
 - Checkpoint autosave with feedback on passing.
 - Map checkpoints teleportation.
 - Audio feedback.
 - Walking type enemy that can pathfind to the player.
 - Flying enemy that can pathfind to the player avoiding non-walkable areas.
 - Load and Save that considers each enemy state.
 - Game capped to stable 60 frames per second.
 - Game's movement normalized using dt (deltaTime).
 - NEW! Entity System: All the elements in the game (Player, Enemies, Props, Coins / Collectibles) must
hereby from a base Entity class and an EntityManager class must manage them (Initialize, include in
a list, Update, Draw, CleanUp…)
- NEW! GUI: Title Screen Main Menu: It includes the following GuiButtons:
- PLAY: Smooth transition to the gameplay screen.
- CONTINUE: Only enabled if there is a saved game. It must transition to the last saved
game. Use a different visible state for disabled!
- SETTINGS: Open options menu. Including the following options:
- Adjust music volume, with a GuiSlider.
- Adjust fx volume, with a GuiSlider.
- Toggle fullscreen mode, with a GuiCheckBox.
- Toggle VSync, with a GuiCheckBox.
- CREDITS: Open a credits panel where you can read the authors and the license.
- EXIT: Quit the game.
- NOTE: Buttons must be responsive and include:
- Visible state change on mouse-hover (FOCUSED)
- Visible state change on mouse-pressed (PRESSED)
- Generate an OnMouseClick event to be processed
- Audio feedback on mouse-hover and mouse-click.
- NEW! GUI: Gameplay Screen HUD: In-game GUI with the following elements:
○ Player lives: The game will cycle from the beginning of each level until the
player consumes all its lifes. At that point, the game must transition to the ending
screen or title screen.
○ Coins: There must be some sort of coins that the player must
be able to collect throughout the game.
○ Timer: It contains the time so far accumulated from the player while playing the level or
the remaining time to finish the level.
- NEW! GUI: Gameplay Screen Pause Menu: Pressing ESCAPE triggers an in-game Pause Menu with
the following options GuiButtons:
○ RESUME: Continue playing the game after displaying the menu.
○ SETTINGS: Shows settings menu (same one from Title Screen).
○ BACK to TITLE: Returns to title screen.
○ EXIT: Quits the game.
 
## Controls

 - "A" to move to the left.
 - "D" to move to he right.
 - "Space" to jump.
 - "Enter" to start the game.
 - "Left Click" to shoot.
 - "F1" to start the first level.
 - "F3" to start from the beginning of the current level.
 - "F5" to save the current game state.
 - "F6" to load the previous state.
 - "F7" to die instantly.
 - "F9" to view colliders and logic.
 - "F10" to enter the God mode (fly around and cannot be killed).
 - "F11" to enable or disable the FPS to 30.
 - "F12" to win the level.
 - "ESCAPE" to pause menu in-game.

## Developers

 - Shahid, Ali Hassan - Programmer
 - López, Carles - Programmer & Map designer

## License

This project is licensed under an unmodified MIT license, which is an OSI-certified license that allows static linking with closed source software. Check [LICENSE](LICENSE) for further details.

[SDL license](https://github.com/FeroXx07/PlatformerGame/blob/master/Output/SDL%20License.txt)

[SDL_Image license](https://github.com/FeroXx07/PlatformerGame/blob/master/Output/SDL%20Image%20License.txt)

[SDL_Mixer license](https://github.com/FeroXx07/PlatformerGame/blob/master/Output/SDL%20Mixer%20License.txt)

[Pugi license](https://github.com/FeroXx07/PlatformerGame/blob/master/Output/Pugi%20License.txt)

Assets licenses:
(https://github.com/FeroXx07/PlatformerGame/blob/master/Output/Assets/textures/Creative%20Common%20License.txt
(https://github.com/FeroXx07/PlatformerGame/blob/master/Output/Assets/textures/License.txt) (https://github.com/FeroXx07/PlatformerGame/blob/master/Output/Assets/textures/Readme.txt)
