To do for each release
----------------------

1) Pull from master
2) Update version numbers in `CMakeLists.txt`
3) Update savefile version, bonefile version or highscore version, as appropriate
4) Push a git tag to `master` in the format `v053` (use the same number as the new version number entered in `CMakeLists.txt`)

A zip file should appear later, on the release page. The binary is compiled with static linking, therefore, no dynamic link libraries are shipped.


Old instructions for posterity
------------------------------

### Pull from master

### Things to change around in the code:

 - Update the IVAN version number in the root CMakeLists.txt from 0.52 to 0.53, depending on what the next version is
 - Search for "increment" in the project files and increment the number associated with the bone files, savefiles and high score versions etc
 - Update this file (releasing.md) to the _next_ release number with the instructions for the next release

### Things to change around on the web:

 - Announce the release on the RogueBasin main page at www.roguebasin.com.
 - Update the [IVAN wiki page at RogueBasin](http://www.roguebasin.com/index.php?title=IVAN) with the new version number and release date.

### Compile the game

### Include the following files in a zip folder for each release:

**Folder: IvanWin053**

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

**Files = 19**

  - Char.png
  - Char-outlined.png
  - Cursor.png
  - Effect.png
  - Enner.png
  - Font.png
  - FOW.png
  - GLTerra.png
  - Humanoid.png
  - Humanoid-outlined.png
  - Icon.bmp
  - Item.png
  - Item-outlined.png
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

**Subfolder: Script\sound**

### Then commit changes, with the release tag v053
Tag can be appended to latest commit

### Push changes
