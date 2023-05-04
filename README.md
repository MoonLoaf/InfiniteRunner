# InfiniteRunner
Assignment for my C++ game programming course

The game system is based primarily on a spawner that contains a TArray of TSubclasses of WorldChunks (Actors). These worldchunks are handmade in the sense that there are four visually different chunks.

The chunks each contain a C-style array of Fvectors representing locations in worldspace. This only works when the spawner is at world-origin, which is not optimal. But since im not planning on moving my spawner I'll keep it this way for now. The worldchunks iterate through the spawnpoints in beginplay and return a random bool deciding whether or not to create an obstacle at this point. If an obstacle is created it is attached to the worldchunk itself as a child object, with FAttachmentrules to keep the transform of the obstacle relative to thr parent. There are three different obstacles that have different requirements to overcome, this can currently result in a softlock where three non-jumpable obstacles all spawn in the same row. This needs to be fixed. 

The spawner keeps all spawned Chunks in an array, and is iteration backwards in the array to constantly check the last chunk if it should be removed. Then creating a new one if the latest chunk's further edge is too close to the player.

My player is static on the X axis, and has access to an array of floats that represent the players position on the Y axis. The player can move between these based on a variable called lane switch speed, which determines how fast you can perform a laneswitch. Im trying to use timers to accomplish this, along with the VInterpTo function to get a smooth transition. But currently the lanedwitching is a bit jittery. This will require further work.

I have also setup two different classes for the obstacles, since one of the types of obstacles is not supposed to deal damage on collision. In the characters's onCapsuleHit im checking whether the other actor is a damaging obstacle to determine this.

I've also set up a super simple UI using the UUserWidget class and a widget blueprint.


Part 2 Update

Bugs fixed:
Since multiplayer was added I decoupled the chunkspawner from any reference to my player
[File in Commit](https://github.com/MoonLoaf/InfiniteRunner/commit/cfe727edc8354fd4d14aadaef3d26ace55060e7c#diff-5daa2f216710b0b30ac0439e30c18e78bad3fc13f97a34915b392ea668cd849f)

Fixed bug where playercharacter never got to run OnMoveComplete() resulting in the players position being locked.
[File in Commit](https://github.com/MoonLoaf/InfiniteRunner/commit/cfe727edc8354fd4d14aadaef3d26ace55060e7c#diff-fc58bd985b663937e3a7af901b8e6820cf4ea1973c398b4caa2254d576afb58a)

Added High score that reads and writes to/from Json between game sessions [File in Commit](https://github.com/MoonLoaf/InfiniteRunner/commit/1c6e5bb465ed6f8ed07df50104e24320c9da18c0#diff-1be5d583375af947f663de90b7c570ea7dd26fee1f8f7ff7524d00de3ed789b4)

Added Multiplayer by creating another instance of playercontroller and player blueprint (with another input mapping)[File in Commit](https://github.com/MoonLoaf/InfiniteRunner/commit/1c6e5bb465ed6f8ed07df50104e24320c9da18c0#diff-0d3ff877872baaeef17a710953927f6e9372f0e06d0cc08fc7ddf5aa0a038134)

I want to point out that the enhanced input system is currently broken for this sort of implementation of multiplayer in Unreal 5.1[Link to reported bug](https://issues.unrealengine.com/issue/UE-169979)

This bug first appeared to me quite late in my project and I want to emphasize that I have spent a lot of time trying to solve and debug this before realizing this is a known issue, which is why I have left most of my debugging in my code. The work around I have found for this is plugging in Keyboard+moouse and TWO separate gamepads, one of which will control player 2. Sometimes this works with just one gamepad.

Moved all chunk and obstacle generation logic into ChunkSpawner to easier implement the despawning of obstacles by having a single array of all obstacles[File in Commit](https://github.com/MoonLoaf/InfiniteRunner/commit/925b6893c07ac504ae477c92b5836baa9821d3b6#diff-5daa2f216710b0b30ac0439e30c18e78bad3fc13f97a34915b392ea668cd849f)

Added "Empty Obstacles" which checks for other obstacles around it to confirm that a player has dodged an obstacle. This uses the players capsule collider component, and if a random number between 0 and 1 is less than .25, it calls a function in chunkSpawner to remove a random obstacle further down [File in Commit](https://github.com/MoonLoaf/InfiniteRunner/commit/925b6893c07ac504ae477c92b5836baa9821d3b6#diff-7c80e1579b613422ebc81d8ff64f681df8adb2fe4c7fbfa64c447f25d979d8d5)
