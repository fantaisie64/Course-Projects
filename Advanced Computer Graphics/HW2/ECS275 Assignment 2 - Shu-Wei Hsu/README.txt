ECS275A Assignment 2
Shu-Wei Hsu 998569417

For assignment 2, I added:

[image_none.jpg]
regular ray tracing result

[image_antialiasing.jpg]
antialiasing with 16 Stratified sampling times (grid sampling)

[image_depthoffield.jpg]
depth of field with the blue sphere focused:
16 sampling times
aperture 10.0
focal length 31.5

[image_motionblur.jpg]
motion blur with tetrahedron speed [ -0.5, -4.5, -1.5 ] with the equation:
newPoint0(points[0].x()+speed.x()*time, points[0].y()+speed.y()*time, points[0].z()+speed.z()*time);
and green sphere with speed [ -1.0, 6.0, 0.2 ] (nonlinear with the z axis) with the equation:
newCenter(center.x()+speed.x()*time*0.01, center.y()+speed.y()*time*0.01, center.z()+pow(speed.z()*time,2)*0.01*time);
16 sampling times

[image_ad.jpg]
with anti-aliasing & depth of field
16 sampling times

[image_am.jpg]
with anti-aliasing & motion blur
16 sampling times

[image_dm.jpg]
with depth of field & motion blur
16 sampling times

[image_adm.jpg]
all - with anti-aliasing & depth of field & motion blur
16 sampling times


test.scn:
 aperture 10.0 : aperture for depth of field
 focal 31.5 : focal length for depth of field
 antialiasingSampleTimes 16 : sample times of antialiasing
 depthOfFieldSampleTimes 16 : sample times of depth of field
 motionBlurSampleTimes 16 : sample times of motion blur
 speed [ x, y, z ] : speed of objects for motion blur

Parser.cc:
 more code for new parameters in test.scn

Scene.cc:
 new argument time in newTraceRay()
 code snippets in newRender() for:
 with antialiasing : with 16 sampling time in the sample pixel
 with antialiasing (Stratified sampling) : grid sampling used in the following effects
 with depth of field : sampling r by sqrt(drand48(r^2))
 with motion blur : randomly sampled within (time slot-0.5, time slot+0.5)
 with anti-aliasing & depth of field
 with anti-aliasing & motion blur
 with depth of field & motion blur
 all - with anti-aliasing & depth of field & motion blur
 
 All with only 16 sample times. The way I did is to simply choose 1 time slot randomly sampled within (time slot-0.5, time slot+0.5) for motion blur, choose a random point on the lens for depth of field, and to do the random grid sampling by shuffling the numbers from 0 to 15 in the following way:
set a sequence of number from 0 to 15
0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
randomly choose a number 7, put the last number (15) to the location of 7:
0, 1, 2, 3, 4, 5, 6, 15, 8, 9, 10, 11, 12, 13, 14
randomly choose a number 2, put the last number (14) to the location of 2:
0, 1, 14, 3, 4, 5, 6, 15, 8, 9, 10, 11, 12, 13
and so on
Otherwise it takes hours to calculate all three effects

Polygon.cc:
simply calculate all points by:
newPoint0(points[0].x()+speed.x()*time, points[0].y()+speed.y()*time, points[0].z()+speed.z()*time);

Sphere.cc:
I tried to make the trajectory of spheres nonlinear by setting the z center.z()+pow(speed.z()*time,2)*0.01*time as:
newCenter(center.x()+speed.x()*time*0.01, center.y()+speed.y()*time*0.01, center.z()+pow(speed.z()*time,2)*0.01*time);

PinholeCamera.cc:
I made a new function called makeLensRay() to get the random points on lens and random direction from lens to the points on focal plane. It is done by proportionality of triangles
