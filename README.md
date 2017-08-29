# diamond-gl

Next-gen OpenGL 4.6 C++ low-level wrapper. 

## Features

All features are WIP. 
- Support of GLM, GLI (second is planned)
- Integration with STL
- Context binding system and managment as is in OpenGL concept
- Object oriented as possible (C++ maner)
- Casting classes to GL indexes
- Support of latest OpenGL 4.6
- Full support of Direct State Access
- (Not yet done) Support of most OpenGL loaders

### Update at 30.08.2017

- Rewrite semantics
- Removed weird tuple semantics
- Buffers always untyped (unitype) by default
- Mostly, actions moved to runtine
- VBO binding now can have type
- Replaced pointers to inline variable

## Nearly planned

- Buffer typed proxy