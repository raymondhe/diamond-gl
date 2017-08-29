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

## BUT...

We thinking about rewrite wrapper. 

Need resolve these issues: 
- We want move most actions to runtime. 
- Remove tuple semantic. 
- Remove code bloat. 
- Clean STL semantic.
- Make VAO binding with template type.
- By default make void buffer, but make typed proxy (which will converted to void buffer). 
