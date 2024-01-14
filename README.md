# Checkers Playdate Version
![DownloadCountTotal](https://img.shields.io/github/downloads/joyrider3774/checkers_playdate/total?label=total%20downloads&style=plastic) ![DownloadCountLatest](https://img.shields.io/github/downloads/joyrider3774/checkers_playdate/latest/total?style=plastic) ![LatestVersion](https://img.shields.io/github/v/tag/joyrider3774/checkers_playdate?label=Latest%20version&style=plastic) ![License](https://img.shields.io/github/license/joyrider3774/checkers_playdate?style=plastic)

Checkers is the well known board game with four difficulties and a jump heuristic. 
This version is port of my gp2x version made to work with the Playdate handheld.


## Playdate Game Features:
- basic rules of checkers
- 4 difficulties
- jump heuristic
- ability to turn music & sound on/off

## Playing the Game:
You are the white player playing against the black player, and start on the bottom of the screen, you can move the selector around with the dpad and select pieces with the (A) button. 
Once a piece is selected it will display the valid moves it can do and you can do the move by selecting the diagonal direction you want your piece to go using the dpad.
If your turn is over the AI / CPU Player will start thinking about it's move and make the move.

### Checkers Game rules
- You must take a piece whenever you can
- You can not take pieces backwards unless the piece is crowned
- A piece that reaches the last row on the other side of the board gets crowned and the turn ends.
- Crowned pieces can only move 1 spot at a time, they can not "fly" over diagonals!
- if after taking a single piece you can take another piece you must do so, you can not end the turn.
- If a player can not move any of his pieces, he loses the game.
- if only 1 piece remains for each player it is a draw.
- If all pieces of the other player are taken you win.


### Caveats
- Very hard CPU player on Playdate can think a while before it takes it move.
- Easy and Very easy CPU Player may repeat moves near the end of the game.


## Play version with colored graphics
The windows, mac, (self build) linux and html (web assembly) versions can switch out the default playdate graphics with colored graphics if you press the F3 button on the keyboard or the X Button on your (xbox) gamepad. 
The game will restart with the next source folder and remember which source folder you are using the next time you start the game.


## Controls

### Playdate

| Button | Action |
| ------ | ------ |
| Dpad | Select menu's and options. Move the selector on the board. When a piece is selected, diagonals dictate the move |
| A | Confirm in menu's, Selecting a piece on the board|
| B | Back in menus and game |

### Keyboard Windows / Mac / Browser

| Button | Action |
| ------ | ------ |
| Arrow keys | Select menu's and options. Move the selector on the board. When a piece is selected, diagonals dictate the move |
| X | Confirm in menu's, Selecting a piece on the board|
| C | Back in menus and game |
| F | Switch between fullscreen & windowed mode |
| F3 | Switch Source Folders (switch between playdate and colored graphics) |
| F4 | Quit game |

### Game pad Windows / Mac / Browser

| Button | Action |
| ------ | ------ |
| dpad & joystick | Select menu's and options. Move the selector on the board. When a piece is selected, diagonals dictate the move |
| A | Confirm in menu's, Selecting a piece on the board|
| B | Back in menus and game |
| X Button | Switch Source Folders (switch between playdate and colored graphics) |

## Credits
- Checkers for Playdate (and gp2x) is created by Willems Davy.
- Graphics made using Gimp
- Sounds come from [Kenney assets all in one](https://kenney.itch.io/kenney-game-assets)
- Music is "Checkers Rag", made by Harry J. Lincoln (1913), sequenced by John Cowles (29/04/96). (mid file can be found [here](https://web.archive.org/web/20070124193733/http://primeshop.com/midlist3.htm))
