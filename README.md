Repository for _Mira and the Mirror Staff_ (MIR), a game project created in session one of an arcade project called SNHU Arcade.

## What is Mira?
_Mira and the Mirror Staff_ is a single-player, top-down game where the player explores a dungeon abandoned by the distant ancestors of modern-day wizards and mages. As the player explores the dungeon, they acquire new abilities with which to conquer the dungeon are reach its deepest room, where an exceptionally rare yet miraculous healing plant—needed to heal to protagonist's older sister—lies. The Mirror Staff—the titular key item—is one such ability. Its powers allow the player to copy enemies' magic and use it as their own, particularly in combat and puzzle-solving. Coupled with mana and discard systems, this gameplay feature encourages experimentation and creativity, as the game frequently encourages the player to copy different magic. With a broad map made up of five distinct areas, _Mira and the Mirror Staff_ strives to be a game that appeals to fans of dungeon crawlers and Metroidvania-style progression.

## What is SNHU Arcade?
SNHU Arcade is an upcoming game which will include up to 12 games—_Mira_ included. The game places the player in an arcade, where they can walk up to arcade cabinets and play the games on them as they please. This project is intended to showcase the game-development skills of alumni from [Southern New Hampshire University]([url](https://www.snhu.edu/)) (SNHU)'s game-related programs and provide them with more material for their portfolios. It is currently projected to release on Steam in the latter half of 2027 as a free download.

## Prominent Artifacts
- **Combat System**
  - TBA
- **Puzzle System**
  - TBA
- **Level-Streaming Implementation**
  - This game utilizes custom Level Streaming volumes with its LevelStreamActor to stream and unstream dungeon rooms as needed. This enables the game to unload unnecessary actors (i.e., those that are distant from and cannot be seen by the player), thus reducing memory usage and allowing for higher frame rates.
- **Camera System**
  - This system utilizes a custom volume called CameraMovePoints (CMP) and a custom camera. The CMP defines details about its room size (and resulting camera restrictions) and a bounds for its overlap volume, which transfers its restrictions to the camera upon player overlap. The camera—which receives those restrictions—can then pan from room to room and stay within each one's panning boundaries. Moreover, the camera is allowed to freely pan—with certain restrictions—upon transitioning from one room to another, allowing the player to be continuously moving without having to wait for the transition to complete. This lends to a dynamic camera system with minimal to no downtime for the player.

## Inspirations
- **_The Legend of Zelda™: Oracle of Seasons_ (GBC) & _The Legend of Zelda™: Oracle of Ages_ (GBC)**
  - Top-down gameplay
  - Gameplay loop
  - Map system
  - Character design
- **_Mega Man™ Zero_ series (GBA); _Mega Man™ ZX_ series (DS)**
  - General movement feel and mobility systems
  - Combat systems
    - Weapon system (Main Attack + Sub-Attack)
    - Energy system (_Zero 4_)
    - Element system (Flame, Ice, Thunder)
The trademarks above belong to their respective owners.

## GitHub Contribution & Version Control
_Mira and the Mirror Staff_ is a nonpublic project that only those who are on the project and have been granted approval are allowed to contribute on. Public viewers may access the source code, but may not make contributions to the game nor participate in version control. The source code is considered to be copyright of the project's owner.