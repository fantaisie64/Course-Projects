
#include "PinholeCamera.h"
#include "Ray.h"
#include "Math.h"
#include <stdlib.h>
#include <math.h>

PinholeCamera::PinholeCamera(const Point& eye, const Point& lookat, const Vector& up,
                             double hfov, double aperture, double focal)
  : eye(eye), lookat(lookat), up(up), hfov(hfov), aperture(aperture), focal(focal)
{
}

PinholeCamera::~PinholeCamera()
{
}

void PinholeCamera::preprocess(double aspect_ratio)
{
  lookdir = lookat-eye;
  lookdir.normalize();
  u = Cross(lookdir, up);
  v = Cross(u, lookdir);
  double ulen = tan(hfov/2.*M_PI/180.);
  u.normalize();
  u *= ulen;
  double vlen = ulen/aspect_ratio;
  v.normalize(); 
  v *= vlen;
}

void PinholeCamera::makeRay(Ray& ray, const RenderContext& context, double x, double y) const
{
  Vector direction = lookdir+u*x+v*y;
  direction.normalize();
  ray = Ray(eye, direction);
}

void PinholeCamera::makeLensRay(Ray& ray, const RenderContext& context, double x, double y) const
{
    
    // point on the image plane
    Point imagePoint = eye + lookdir + u*x + v*y;
    
    // define distance bwteen camera/image plane and image plane/lens
    double D = 1.0;
    
    // center of lens
    Point lensCenter = eye + lookdir * (2.0 * D);
    
    // generate a random point on lens
    double sqRadius = pow(aperture, 2);
    double randomSqRadius = drand48() * sqRadius;
    double randomRadius = sqrt(randomSqRadius);
    double randomTheta = drand48() * 360.0;
    double rX = randomRadius * cos(randomTheta);
    double rY = randomRadius * sin(randomTheta);
    Point randomPoint = lensCenter+u*rX+v*rY;
    
    // compute focal point on focal plane
    double distImagPointToLensCenter = (lensCenter - imagePoint).length();
    double distLensCenterTofocalPoint = ( distImagPointToLensCenter * focal)/D;
    double distImagPointTofocalPoint = distImagPointToLensCenter + distLensCenterTofocalPoint;
    Vector dirImagPointToLensCenter = lensCenter - imagePoint;
    dirImagPointToLensCenter.normalize();
    Point focalPoint = imagePoint + (dirImagPointToLensCenter * distImagPointTofocalPoint);
    
    // make ray
    Vector randomDirection = focalPoint - randomPoint;
    randomDirection.normalize();
    ray = Ray(randomPoint, randomDirection);
}
