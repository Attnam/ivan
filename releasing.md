To do for each release
----------------------

### Pull from master

### Things to change around in the code:

 - Update the IVAN version number in the root CMakeLists.txt from 0.50.9 to 0.51, depending on what the next version is
 - Search for "increment" in the project files and increment the number associated with the bone files, savefiles and high score versions etc
 - Update this file (releasing.md) to the _next_ release number with the instructions for the next release

### Things to change around on the web:

 - Announce the release on the RogueBasin main page at www.roguebasin.com.
 - Update the [IVAN wiki page at RogueBasin](http://www.roguebasin.com/index.php?title=IVAN) with the new version number and release date.

### Compile the game

### Include the following files in a zip folder for each release:

**Folder: IvanWin051**

**Files = 12**

  - AUTHORS
  - COPYING
  - IVAN.EXE
  - LICENSING
  - msvcp50.dll
  - msvcrt.dll
  - NEWS
  - README
  - README.md
  - SDL2.dll
  - libpng12.dll / libpng16.dll
  - zlib1.dll

**Subfolder: Graphics**

**Files = 16**

  - Char.png
  - Cursor.png
  - Effect.png
  - Enner.png
  - Font.png
  - FOW.png
  - GLTerra.png
  - Humanoid.png
  - Icon.bmp
  - Item.png
  - IVlad.png
  - Menu.png
  - OLTerra.png
  - Smiley.png
  - Symbol.png
  - WTerra.png

**Subfolder: Music**

**Files = 11**

  - Cathedral.mid
  - defeat.mid
  - Dungeon.mid
  - Dungeon2.mid
  - Dungeon3.mid
  - Empty.mid
  - Japan.mid
  - mainmenu.mid
  - newgame.mid
  - victory.mid
  - world.mid

**Subfolder: Script**

**Files = 9**
**Subfolders = 1**

  - char.dat
  - define.dat
  - dungeon.dat
  - glterra.dat
  - gwterra.dat
  - item.dat
  - material.dat
  - olterra.dat
  - owterra.dat

**Subfolder: Script\dungeon**

**Leave only dungeon data files in here**

### Then commit changes, with the release tag v051
Tag can be appended to latest commit

### Push changes
