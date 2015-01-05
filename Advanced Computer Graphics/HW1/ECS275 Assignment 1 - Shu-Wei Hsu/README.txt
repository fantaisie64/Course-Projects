ECS275A Assignment 1
Shu-Wei Hsu 998569417

For assignment 1, I added a cube and a tetrahedron model in the scene. The cube is aligned to the axis of our coordinate system with the intersection algorithm below:
http://www.scratchapixel.com/lessons/3d-basic-lessons/lesson-7-intersecting-simple-shapes/ray-box-intersection/
The tetrahedron is modeled by 4 polygons. That is, I implemented a flexible polygon class with all kinds of polygons with different number of sides. The intersection algorithm is here:
http://graphics.di.uoa.gr/Downloads/papers/journals/p19.pdf
With applying Pluker Coordinates:
http://en.wikipedia.org/wiki/Pl%C3%BCcker_coordinates
I wrote a recursive ray tracer that can render scene with shadows. I implemented ambient/diffuse/specular/reflection colors of shading of the models in the scene. The modification of files are listed below:

specter.cc - in the main(), change scene->render() to new function scene->newRender()
Scene.cc - add new function newRender() that can call my ray tracing function newTraceRay()
Cube.cc - my Cube class
Polygon.cc - my customized Polygon class that can fit all kinds of polygons
LambertianMaterial.cc - add new member elements Ks (specular) and Kr (reflection)
test.scn - add Cube and 4 polygons for tetrahedron