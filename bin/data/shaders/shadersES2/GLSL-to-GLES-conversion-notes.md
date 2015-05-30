## Some notes for converting GLSL to GLSL ES shaders

- __Floats are noted without the `f`__  
`1.0f`	→ `1.0` 
- __Sampler2DRect__  
`__Sampler2DRect__` → `__Sampler2D__`  
- __Remove #version__  
`#version 330` → `//#version 330` 
- __In OF, outputColor becomes gl_FragColor__  
`outputColor` → `gl_FragColor` 
- __Uniform, in & out__  
  - In a vertex shader  
`in float myFloat;` → `varying float myFloat;`  
`out float myFloat` → `attribute float myFloat`  
  - In a fragment shader  
`in float myFloat;` → `varying float myFloat;`  

- __Note__  
Some functions like mod() disapear, you have to write your own or replace them.

- __Hint__  
For other compiler errors, you can paste your shader code into a [shadertoy.com](http://shadertoy.com/) shader, detailed syntax errors will be hightligted.

- __Ressources__:  
  - useful for convertion GLSL to GLSL ES  
  [http://stackoverflow.com/questions/19783173/difference-between-opengl-sl-and-opengl-es-2-0-sl](http://stackoverflow.com/questions/19783173/difference-between-opengl-sl-and-opengl-es-2-0-sl)
<!--
- ____  
`` → ``  
-->