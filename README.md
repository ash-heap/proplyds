proplyds
========

A procedural planet generator.


Linux
=====

- UBUNTU: sudo apt-get install libsdl2-2.0-0 libsdl2-image-2.0-0 libsdl2-net-dev libsdl2-dev libsdl2-image-dev libsdl2-ttf-2.0-0 libsdl2-gfx-1.0-0 libsdl2-mixer-2.0-0 libsdl2-ttf-dev libsdl2-gfx-dev libsdl2-mixer-dev libsdl2-net-2.0-0 libbulletfileloader2.81 libbulletcollision2.81 libbulletml0d2 libbullet-dev libbulletml-dev libbulletsoftbody2.81 libbulletdynamics2.81 libbulletworldimporter2.81 libbullet-extras-dev libbulletxmlworldimporter2.81 libassimp3 libassimp-dev libsoil1 libsoil-dev
- make
- ./main


Windows
=======

- acquire and untar the media and win3pdeps packages into the source directory
- run Visual Studio and load the .sln file
- build and run

NOTE: 64-bit systems may fail to run the 32-bit version because of missing 32-bit .dlls (in this case, the operating system will attempt to load the 64-bit .dlls, and the game will crash at startup).

