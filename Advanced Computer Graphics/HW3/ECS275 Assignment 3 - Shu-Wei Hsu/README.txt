ECS275A Assignment 3
Shu-Wei Hsu 998569417

For assignment 3:

all are 1024 x 1024 pictures with antialiasing

[image_dironly.jpg]
Direct illumination with soft shadow (area light) without reflection color.
it is based on Phong Shading for BRDF without ambient color.
Sampling times: 64

[image_dironly_no_reflection.jpg]
Direct illumination with soft shadow (area light).
it is based on Phong Shading for BRDF without ambient color.
Sampling times: 64

[image_indironly.jpg]
Indirect illumination with random hemisphere sampling on diffuse surface. Russian Roulette is used for possible absorption of the ray.
Sampling times: 200

[image_all.jpg]
Direct and indirect illumination with soft shadow and color bleeding.
Direct illumination sampling times: 64
Indirect illumination sampling times: 200

[image_all_no_reflection.jpg]
Direct and indirect illumination with soft shadow and color bleeding.
It is without reflection color in direct illumination.
Direct illumination sampling times: 64
Indirect illumination sampling times: 200
