# RDR-Style Advanced Combat System

This document describes the enhanced combat system added to WIL_Dungeon, inspired by Red Dead Redemption's combat mechanics. The system provides advanced combat features including combos, blocking, dodging, weapon recoil, and enhanced hit detection.

## Overview

The RDR-style combat system consists of several key components:

1. **AdvancedCombatComponent** - Core combat state management and mechanics
2. **AdvancedWeaponBase** - Enhanced weapon system with recoil and spread
3. **AdvancedSword** - Example melee weapon implementation
4. **Enhanced DungeonCharacter** - Updated character with new combat inputs

## Key Features

### Combat States
- **Idle** - Default state when not in combat
- **Attacking** - Performing an attack
- **Blocking** - Defensive stance that reduces damage
- **Dodging** - Evasive movement
- **Stunned** - Temporarily unable to act
- **Reloading** - Weapon reload state

### Attack Types
- **Light Attack** - Fast, low damage attacks
- **Heavy Attack** - Slow, high damage attacks
- **Combo Attack** - Chained attacks with damage multipliers
- **Special Attack** - Unique weapon abilities

### Combo System
- Chain attacks within a time window to create combos
- Different combo sequences provide different damage multipliers
- Combo timer automatically resets if no attacks are performed
- Visual feedback for successful combos

### Blocking and Dodging
- **Blocking**: Hold block button to reduce incoming damage by 80%
- **Dodging**: Press dodge to quickly move in a direction, avoiding attacks
- Both actions consume stamina
- Blocking can be held, dodging is instant

### Enhanced Hit Detection
- Cone-shaped attack areas for melee weapons
- Precise hit detection with visual debug lines
- Different damage zones (head, torso, arms, legs, weak points)
- Hit feedback with screen shake and particle effects

### Weapon System Features
- **Recoil Patterns**: Realistic weapon recoil that affects accuracy
- **Spread System**: Accuracy affected by movement, jumping, and aiming
- **Fire Modes**: Single shot, burst fire, automatic, and melee
- **Aiming System**: Hip fire vs. aimed shots with different accuracy
- **Reload System**: Magazine-based ammunition with reload animations

## Input Mapping

The following input actions need to be set up in your project's Input settings:

### Combat Actions
- `LightAttack` - Perform light attack
- `HeavyAttack` - Perform heavy attack
- `Block` - Hold to block (Press/Release)
- `Dodge` - Perform dodge roll
- `Aim` - Hold to aim weapon (Press/Release)
- `Fire` - Fire weapon or special attack
- `Reload` - Reload weapon
- `SwitchFireMode` - Switch between fire modes

## Implementation Details

### AdvancedCombatComponent

The core combat component manages:
- Combat state transitions
- Combo system with timing windows
- Stamina consumption for actions
- Hit detection and damage application
- Camera adjustments during combat

```cpp
// Example usage
UAdvancedCombatComponent* Combat = GetComponentByClass<UAdvancedCombatComponent>();
Combat->PerformAttack(EAttackType::Light);
Combat->StartBlock();
Combat->PerformDodge(FVector(1, 0, 0));
```

### AdvancedWeaponBase

Base class for all weapons with features:
- Recoil patterns and recovery
- Spread calculation based on movement
- Fire mode switching
- Aiming system
- Reload mechanics

```cpp
// Example weapon setup
AAdvancedWeaponBase* Weapon = GetWorld()->SpawnActor<AAdvancedWeaponBase>();
Weapon->FireMode = EWeaponFireMode::Automatic;
Weapon->StartAiming();
Weapon->Fire();
```

### AdvancedSword

Example melee weapon implementation:
- Cone-shaped slash attacks
- Visual debug lines for attack area
- Slash-specific animations and effects
- Melee-optimized recoil and spread

## Integration with Existing Systems

### Character Stats
The combat system integrates with the existing `CharacterStatsComp`:
- Stamina consumption for combat actions
- Health and damage modification
- Movement speed affects combat accuracy

### Inventory System
Weapons can be integrated with the existing inventory:
- Equip weapons through inventory
- Different weapon types for different situations
- Ammunition management

### Enemy AI
The enhanced hit detection works with existing enemies:
- Precise damage application
- Hit feedback for AI reactions
- Different damage zones for strategic combat

## Setup Instructions

1. **Compile the Project**
   - Build the project to compile the new C++ classes
   - Ensure all new files are included in the build

2. **Set Up Input Mappings**
   - Open Project Settings > Input
   - Add the new input actions listed above
   - Bind them to appropriate keys/buttons

3. **Configure the Character**
   - Open your character Blueprint
   - Add the `AdvancedCombatComponent` to the character
   - Set up the component properties in the editor

4. **Create Weapon Blueprints**
   - Create Blueprint classes based on `AdvancedWeaponBase`
   - Configure weapon-specific properties
   - Set up animations and effects

5. **Test the System**
   - Play the game and test all combat actions
   - Verify combo system works correctly
   - Check that blocking and dodging function properly

## Customization

### Adding New Weapon Types
1. Create a new class inheriting from `AdvancedWeaponBase`
2. Override the `Fire()` function for weapon-specific behavior
3. Configure weapon properties in the constructor
4. Add weapon-specific animations and effects

### Modifying Combat Mechanics
1. Edit `AdvancedCombatComponent` parameters
2. Adjust combo timing windows
3. Modify stamina costs for actions
4. Change damage multipliers and hit detection

### Creating New Combo Sequences
1. Add new `FComboData` entries to the `AvailableCombos` array
2. Define the attack sequence
3. Set damage multiplier and timing window
4. Add corresponding animation montages

## Performance Considerations

- The combat system uses efficient collision detection
- Debug visualization can be disabled in shipping builds
- Particle effects should be optimized for performance
- Animation montages should be properly configured

## Troubleshooting

### Common Issues
1. **Input not working**: Check input mappings in project settings
2. **Combat component not found**: Ensure it's added to the character Blueprint
3. **Weapons not firing**: Check ammunition and reload state
4. **Combo system not working**: Verify combo timing windows and sequences

### Debug Features
- Visual debug lines show attack areas
- On-screen messages display hit targets
- Console logs show combat state changes
- Enable debug visualization in development builds

## Future Enhancements

Potential additions to the combat system:
- **Parrying System**: Perfect timing blocks that counter-attack
- **Weapon Switching**: Quick weapon selection during combat
- **Environmental Combat**: Using cover and destructible objects
- **Advanced AI**: Enemies that use the same combat system
- **Multiplayer Combat**: Synchronized combat actions
- **Weapon Modifications**: Attachments and upgrades
- **Combat Styles**: Different fighting techniques and stances

## Conclusion

The RDR-style combat system provides a solid foundation for advanced combat mechanics. It's designed to be modular and extensible, allowing for easy customization and enhancement. The system integrates well with existing game systems while providing the depth and responsiveness expected from modern action games. 