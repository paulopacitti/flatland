# flatland

![flatland logo](docs/logo.png)

a 2D Game Engine made with C++. Made along with [pikuma](https://pikuma.com/courses/cpp-2d-game-engine-development). *Work in progress*.


## Pre-requisites
- SDL2 (`sdl2`, `sdl2_image`, `sdl2_tff`, `sdl2_mixer`)
- Lua 5.4.7

All other libs are statically linked and present along with other header files in `libs/`.

## Architecture

- **Entity**: represents a general-purpose object. Every game object is represented as an entity. Usually, it only consists of a unique id, typically use a plain integer for this.
- **Component**: an entity as possessing a particular aspect, and holds the data needed to model that aspect. For example, every game object that can take damage might have a Health component associated with its entity. Implementations typically use structs, classes, or associative arrays.
  - **Signature**: `std::bitset` to represent which components an entity has or which entities a system is insterested in.
- **System**: a process which acts on all entities with the desired components. For example, a physics system may query for entities having mass, velocity and position components, and iterate over the results doing physics calculations on the set of components for each entity. 
- **Registry**: holds all the entities, components and entities of the game engine. As singleton, one instance per execution. Holds the API to add entities, components and systems to the running game.
  - **Component Pools**: data structure that holds component pools, where the component ID is the index, and a pool of components (another vector) hold the entites that hold this component.



## Contribution Guidelides

### Code style
- Private variables have the `m_` syntax.
- `this` keyword must be used whenever a struct or class is accessing or modifying a public attribute.
- If a data structure is just holding public data and it will not be used for inheritance, use structs. Otherwise, use classes.

