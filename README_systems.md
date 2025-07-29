# WIL_Dungeon

Developed with Unreal Engine 5

## Table of Contents

- [Overview](#overview)
- [Systems](#systems)
  - [Dungeon Generation](#dungeon-generation)
  - [Character & Stats](#character--stats)
  - [Combat & Weapons](#combat--weapons)
  - [Inventory System](#inventory-system)
  - [Crafting System](#crafting-system)
  - [Enemies & AI](#enemies--ai)
  - [Audio & Footsteps](#audio--footsteps)
  - [Input System](#input-system)
  - [Game Modes](#game-modes)
  - [User Interface](#user-interface)
- [Setup](#setup)

---

## Overview

WIL_Dungeon is a dungeon-crawling game built in Unreal Engine 5, featuring procedural dungeon generation, a modular character system, inventory and crafting, enemy AI, and a robust UI.

---

## Systems

### Dungeon Generation

- **C++:** `D_Generator`, `TileMatrixComponent`, `DungeonGenerator`, `DungeonArea`, `DungeonRoom`, `DungeonHallway`, `DungeonCave`
- **Blueprints:** `BP_DungeonGenerator`, `BP_DungeonRoom`, `BP_DungeonHallway`, `BP_DungeonExit`, `BP_DungeonCave`
- **How it works:**  
  The dungeon is generated procedurally using a grid-based system. `D_Generator` and `TileMatrixComponent` handle the creation of rooms, hallways, and placement of objects. The system supports spawning central rooms, exits, enemies, and loot. Room and hallway Blueprints define the visual and collision aspects.

### Character & Stats

- **C++:** `DungeonCharacter`, `CharacterStatsComp`
- **Blueprints:** `BP_DungeonCharacter`
- **How it works:**  
  The player character is based on `DungeonCharacter`, which includes movement, camera, and a `CharacterStatsComp` for health, stamina, system exposure, speed, and attack stats. Stats are editable and can be modified by gameplay events.

### Combat & Weapons

- **C++:** `CombatComponent`, `WeaponBase`
- **Blueprints:** `BP_Weapon_Sword`, `BP_Weapon_Spear`, `BP_Weapon_Dagger`, etc.
- **How it works:**  
  The `CombatComponent` manages weapon equipping, attack input, and hit detection. Weapons are modular actors with their own meshes, damage values, and attack animations. The system supports multiple weapon types (dagger, sword, spear).

### Inventory System

- **Blueprints:** `BP_InventoryComponent`, `BP_Item`, `BP_Item_WDorp`, `S_ItemStructure`, UI Blueprints (`InventoryUI`, `InventorySlotWidget`)
- **How it works:**  
  The inventory is a grid-based system managed by `BP_InventoryComponent`. Items are defined by Blueprints and structures. The UI allows players to view, equip, and use items. Carry capacity is determined by the character’s stats.

### Crafting System

- **Blueprints:** `BP_CraftingComponent`, `BP_CraftingBench`, `CraftingMenu_WBP`, `Craft_WBP`, `RecipeSlot_WBP`, `RecipeCost_WBP`, `CraftingRecipesStruct`, `CraftingCategoriesEnum`
- **How it works:**  
  Players can craft items using resources found in the dungeon. The crafting system is managed by `BP_CraftingComponent` and uses data structures for recipes and categories. The UI provides a menu for selecting recipes and viewing required materials.

### Enemies & AI

- **C++:** `Enemy_Character`
- **Blueprints:** `BP_Enemy`, `BP_Enemy_Wolf`, `BP_Enemy_Boar`, `BP_Enemy_Pheasant`, AI Controllers, Behavior Trees, Blackboards
- **How it works:**  
  Enemies are based on `Enemy_Character` and have their own health, damage, and drop logic. AI is implemented using Unreal’s Behavior Trees and Blackboards, with custom controllers for each enemy type (Wolf, Boar, Pheasant).

### Audio & Footsteps

- **Blueprints:** `BP_SFXManager`, `BP_FootstepAnimNotify`
- **How it works:**  
  Audio cues for footsteps and other SFX are managed by `BP_SFXManager`. Animation notifies trigger footstep sounds during character movement.

### Input System

- **Blueprints:** `IMC_Player`, `IA_Move`, `IA_Look`, `IA_Jump`
- **How it works:**  
  Input actions and mappings are set up using Unreal’s Enhanced Input system, allowing for flexible and customizable controls.

### Game Modes

- **Blueprints:** `BP_D_Gen_GMB`, `BP_Start_GMB`, `MainMenu_GMB`, `GMB_TEST`
- **How it works:**  
  Game modes define the rules and flow for different parts of the game, such as the main menu, dungeon generation, and test environments.

### User Interface

- **Blueprints:** `PlayerHUD`, `InventoryUI`, `InventorySlotWidget`, `CraftingMenu_WBP`, `MainMenuUI`, `PauseMenu`, `PlayerMenuUI`
- **How it works:**  
  The UI is built with UMG Blueprints, providing menus for inventory, crafting, player stats, and game navigation.

---

## Setup

1. **Requirements:**  
   - Unreal Engine 5.x
   - Windows 10 or later

2. **How to Run:**  
   - Clone or download the repository.
   - Open `WIL_Dungeon.uproject` in Unreal Engine.
   - Build the project (C++ code will be compiled automatically).
   - Press Play to start the game.

3. **Project Structure:**  
   - `Source/` contains all C++ code for core systems.
   - `Content/` contains all Blueprints, assets, and UI.
   - `Config/` contains project configuration files.

--- 