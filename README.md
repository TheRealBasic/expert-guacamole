# Shadow Lantern

Shadow Lantern is a single-player, text-based survival puzzle written in modern C++17. You wake up inside a moonlit mansion with
nothing but a temperamental lantern. Three tasks stand between you and freedom:

1. Restore every emergency generator.
2. Find the key that unlocks the estate's final gate.
3. Slip past the monster that haunts the corridors and reach the exit alive.

The entire experience runs in a terminal window—no external assets or third-party libraries are required. Every turn challenges
you to balance light, stealth, and resource management while the monster relentlessly prowls the map.

## Building

The project uses CMake and has no dependencies beyond a C++17-capable compiler. Build and run the executable like so:

```bash
cmake -S . -B build
cmake --build build
./build/shadow_lantern
```

On Windows, generate Visual Studio or Ninja files with the same commands from a "x64 Native Tools" command prompt and run the
resulting binary.

## Playing the Game

Shadow Lantern plays out on a handcrafted mansion layout represented with ASCII characters. The legend is:

| Symbol | Meaning                            |
| ------ | ----------------------------------- |
| `P`    | Your current position               |
| `M`    | The monster (only visible when lit or detected) |
| `G`    | Offline generator                   |
| `B`    | Spare battery                       |
| `K`    | Gate key                            |
| `D`/`d`| Locked/unlocked gate                |
| `E`    | Exit gate                           |
| `#`    | Impassable rubble                   |
| `.`/`:`| Explored floor (lit/faded memory)   |

Each turn you may enter exactly one command. Most commands consume a turn, after which:

1. Your lantern drains power (faster when switched on).
2. The monster takes a step. It will chase aggressively when it senses you within six tiles, otherwise it roams randomly.
3. Fear rises if you linger in the dark or the monster corners you.

You lose when your fear reaches 100% or the monster moves onto your tile. You win by reaching the exit tile after activating every
generator and unlocking the gate.

### Core Resources

- **Battery (0–100%)** – Drains every turn. Keeping the lantern on improves visibility but chews through power. When the meter hits
  zero the lantern shuts off automatically and your fear spikes.
- **Spare Batteries** – Collected from `B` tiles. Use `charge` to swap one in (+35% battery) and instantly re-light the lantern.
- **Keys** – Pick up a key from the `K` tile to unlock the gate (`D`). Keys are consumed automatically when you step into the gate.
- **Fear (0–100%)** – Represents your grip on sanity. Darkness, close calls, and direct contact raise the value. Hitting 100% ends
the run.
- **Generators** – Activate each `G` tile with the `take` command to restore power and grant a 20% battery boost.

### Command Reference

| Command                             | Description |
| ----------------------------------- | ----------- |
| `move <n|s|e|w>` or `n/s/e/w`       | Walk one tile in the chosen direction. Attempts blocked by walls or the locked gate do not consume a turn. |
| `take` / `grab` / `activate`        | Pick up every item on the current tile and power a generator if present. |
| `toggle`                            | Switch the lantern on or off. Light grants vision; darkness hides you but raises fear faster near the monster. |
| `charge`                            | Spend one spare battery to restore 35% power and automatically turn the lantern back on. |
| `scan`                              | Spend 4% battery to ping the environment. Reveals how far the nearest generator is and exposes the monster for three turns. |
| `wait` / `rest`                     | Hold position while the world advances. Useful for baiting the monster or conserving stamina. |
| `inventory` / `inv`                 | Show current keys and spare batteries (does not consume a turn). |
| `status`                            | Recap your fear level (no turn). |
| `help`                              | Remind yourself of the objectives (no turn). |
| `quit`                              | Abandon the run immediately. |

Commands are case-insensitive and can contain extra whitespace—for example, `MOVE North` works exactly like `n`.

### Tips for Survival

- **Light reveals, darkness conceals.** Keep the lantern on while exploring unknown rooms, then consider dousing it once you know a
  safe loop to kite the monster.
- **Generators are free heals.** Activating one immediately restores 20% battery—plan your route so you arrive on fumes to maximise
the refill.
- **Use scans to break line-of-sight.** When you hear the monster but cannot see it, trigger `scan` to learn whether it is closing
  in. The temporary reveal also keeps it rendered on the map even in darkness.
- **Unlock the gate proactively.** Once you have the key, open the gate on your way past so you have a clear sprint line to the exit
  when every generator is humming.
- **Manage fear.** Spending multiple turns in the dark near the monster will rapidly spiral. If your lantern fails, either recharge
  immediately or lure the monster toward lit hallways.

### Ending the Run

- **Victory:** After all generators glow green (`G` tiles disappear) and the gate shows `d`, step onto the `E` tile to escape.
- **Defeat:** Fear reaches 100%, or the monster ends its movement on your tile.

Every run is deterministic—the mansion layout, monster behaviour, and generator placement remain the same—so mastery comes from
optimising your route and resource usage.

Good luck, stalker of shadows.
