# Zeus

Zeus is a small game engine. The primary purpose of building this game engine is to learn new things. It consists of graphics programming (OpenGL, Vulkan), audio systems, physics, animation models, networking, and more.

# Sample Screenshots
## Level Editor Overview
![Screenshot_20221109_111343](https://user-images.githubusercontent.com/58256720/200748809-fca45a6b-7bfe-4bf7-8fcc-8840af606999.png)

# System
## Overcomes
* Zeus Engine reduced the use of video memory using spritesheet concept

## Limitations
* When extracting sub-textures, the extracted sub-texture is stored in computer main memory for later use. In this case Zeus Engine uses an id system which is a 0-based id system. For a large project the specified number may be not enough.
