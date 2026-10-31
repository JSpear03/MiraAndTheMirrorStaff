Repository for _Mira and the Mirror Staff_ (MIR), a game project created in session one of an arcade project called SNHU Arcade.

## What is Mira?
_Mira and the Mirror Staff_ is a single-player, top-down game where the player explores a dungeon abandoned by the distant ancestors of modern-day wizards and mages. As the player explores the dungeon, they acquire new abilities with which to conquer the dungeon and reach its deepest room, where an exceptionally rare yet miraculous healing plant—needed to heal the protagonist's older sister—lies. The Mirror Staff—the titular key item—is one such ability. Its powers allow the player to copy enemies' magic and use it as their own, particularly in combat and puzzle-solving. Coupled with mana and discard systems, this gameplay feature encourages experimentation and creativity, as the game frequently encourages the player to copy different magic. With a broad map made up of five distinct areas, _Mira and the Mirror Staff_ strives to be a game that appeals to fans of dungeon crawlers and Metroidvania-style progression.

## What is SNHU Arcade?
[SNHU Arcade](https://career360.snhu.edu/experiences/the-arcade-a-collaborative-game-development-experience/) is an upcoming game which will include up to 12 games—_Mira_ included. The game places the player in an arcade, where they can walk up to arcade cabinets and play the games on them as they please. This project is intended to showcase the game-development skills of alumni from [Southern New Hampshire University](https://www.snhu.edu/) (SNHU)'s game-related programs and provide them with more material for their portfolios. It is currently projected to release on Steam in the latter half of 2027 as a free download.

## Prominent Artifacts
- **Combat System**
  - This system is composed of many smaller systems—a copy system, a weapon system, a health and mana system, and an element system.
    - The copy system allows the player to copy enemies' magic once they have obtained a certain key item early in the game. This system greatly informs the gameplay because each magic has a different usage and element. For example, there are melee attacks, projectile attacks, and explosion-based attacks. These attacks can then have one of four elements—Nothing, Flame, Ice, and Thunder. Each element has its advantages and disadvantages (or lack thereof, for Nothing) against each other. This requires players to think about the element matchup before using their copied magic against an enemy, as it will be ineffective if the enemy resists it. 
    - The weapon system mainly concerns the various attack modes at the player's disposal. The Main Attack utilizes any magic that the player has copied. The Sub-Attack utilizes the player's Dagger, a weak attack that serves as a fallback if the player runs out of mana (which is needed to use the Main Attack). Lastly, the Ranged Copy allows the player to copy magic from a distance (the Main Attack can also be used to copy magic at close range if nothing is currently copied). This system thus allows the player to utilize the unique properties and abilities of each attack to determine their own approach to the game.
    - The health and mana systems are quite simple in execution—a character has a set amount of health and mana. If health runs out, the character dies (in the player's case, a Game Over). If mana runs out, the player's copied magic is automatically discarded and they cannot cast any more magic until the Mana Gauge completely refills. The mana system is especially key to the gameplay and game loop because the automatic discard forces the player to frequently experiment with different types of magic, preventing players from using the same magic throughout the entire game. However, the player's health and mana can be increased via power-ups, which makes it easier for players to be more liberal with their magic usage.
  - Accordingly, their combination leads to tactical gameplay, where the player is encouraged to weigh their available options before approaching a challenge.
- **Puzzle System**
  - This system utilizes two types of puzzle assets—Direct Puzzle Assets (DPAs) and Indirect Puzzle Assets (IPAs)—alongside a Puzzle-Asset Manager (PAM), which manages their states and solutions. DPAs and IPAs represent a cause–effect relationship, where the activation or deactivation of a DPA is routed through the PAM to activate or deactivate an IPA. This is achieved through the [dispatch design pattern](https://codingtechroom.com/question/-dispatch-design-pattern), which allows the PAM to manage its linked assets according to their activation and deactivation states. Finally, the PAM includes a feature called Auto-Timer, which handles an automatic state toggle for all DPAs if they are not activated or deactivated within the timer's duration. This allows for unique puzzles where the player must toggle all DPAs in a set time to achieve the puzzle solution, rather than toggling them at their leisure.
  - Moreover, the copy system and element system are widely used here as well. Certain DPAs can be activated only by certain elements, which requires the player to switch out their copied magic frequently in order to progress through the game and find optional power-ups.
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
    - Energy system and copy system (_Zero 4_)
    - Element system (Flame, Ice, Thunder)

The trademarks above belong to their respective owners.

## GitHub Contribution & Version Control
_Mira and the Mirror Staff_ is a nonpublic project that only those who are on the project and have been granted approval are allowed to contribute on. Public viewers may access the source code, but may not make contributions to the game nor participate in version control. The source code is considered to be copyright of the project's owner.
