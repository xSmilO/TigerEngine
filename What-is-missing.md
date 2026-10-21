What Else is TigerEngine Missing?

You have built a fantastic foundation (Rendering, Input, Scene Graph, Math, and basic Collisions). But if we look at professional 2D engines like Unity, Godot, or GameMaker, here are the core systems TigerEngine currently lacks, ranked by importance:

1. Rotation (Critical Missing Math)
Right now, your GameObjects have Position and Scale, but they cannot rotate! A true 2D engine uses a complete "Transform" system (Position, Rotation, Scale). We need to add an angle (in degrees or radians) and update the vertex shader to rotate the vertices before drawing them.

2. Spritesheet Animation
Currently, a GameObject can only show one static Texture. 2D games require animations (like a character walking). This is done by taking a single large image (a spritesheet) and telling the shader to only draw a small fraction of the image, shifting those coordinates every few frames.

3. Text and UI Rendering
You have no way to draw a "Score: 100" or a "Game Over" screen. Drawing text in OpenGL is notoriously difficult because you have to load a .ttf font file, generate a texture for every letter of the alphabet, and render them as tiny textured quads side-by-side.

4. Parent / Child Hierarchy
Right now, the Scene is just a flat list of objects. If you want a Player to hold a Sword, you have to manually code the Sword to follow the Player. In a real engine, you make the Sword a "Child" of the Player, and it automatically inherits the Player's position and rotation.

5. Audio System
An engine needs a way to play .wav or .ogg files. This is usually done by integrating a third-party library like miniaudio or OpenAL so you can write Audio::PlaySound("jump.wav").

6. Rigid Collision Resolution
Your AABB system detects if a collision happens (turning the player red), but it doesn't push the player back. A physics system needs "resolution"—calculating exactly how far inside the wall the player is, and pushing them out so they slide smoothly along the edge.