# InfiniteRunner
Assignment for my C++ game programming course

The game system is based primarily on a spawner that contains a TArray of TSubclasses of WorldChunks (Actors). These worldchunks are handmade in the sense that there are four visually different chunks.

The chunks each contain a C-style array of Fvectors representing locations in worldspace. This only works when the spawner is at world-origin, which is not optimal. But since im not planning on moving my spawner I'll keep it this way for now. The worldchunks iterate through the spawnpoints in beginplay and return a random bool deciding whether or not to create an obstacle at this point. If an obstacle is created it is attached to the worldchunk itself as a child object, with FAttachmentrules to keep the transform of the obstacle relative to thr parent. There are three different obstacles that have different requirements to overcome, this can currently result in a softlock where three non-jumpable obstacles all spawn in the same row. This needs to be fixed. 

The spawner keeps all spawned Chunks in an array, and is iteration backwards in the array to constantly check the last chunk if it should be removed. Then creating a new one if the latest chunk's further edge is too close to the player.

My player is static on the X axis, and has access to an array of floats that represent the players position on the Y axis. The player can move between these based on a variable called lane switch speed, which determines how fast you can perform a laneswitch. Im trying to use timers to accomplish this, along with the VInterpTo function to get a smooth transition. But currently the lanedwitching is a bit jittery. This will require further work.

I have also setup two different classes for the obstacles, since one of the types of obstacles is not supposed to deal damage on collision. In the characters's onCapsuleHit im checking whether the other actor is a damaging obstacle to determine this.

I've also set up a super simple UI using the UUserWidget class and a widget blueprint.
