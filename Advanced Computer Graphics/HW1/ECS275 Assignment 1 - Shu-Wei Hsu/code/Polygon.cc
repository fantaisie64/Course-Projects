
#include "Polygon.h"
#include "HitRecord.h"
#include "Math.h"
#include "Ray.h"
#include <iostream>
#include <stdlib.h>
using namespace std;

enum VectorDirection{
    Clockwise,
    CounterClockwise
};

Polygon::Polygon(Material* material, const vector<Point>& points)
: Primitive(material), points(points)
{
    Vector sideVector1(points[1] - points[0]);
    Vector sideVector2(points[2] - points[1]);
    n = Cross(sideVector1, sideVector2);
    n.normalize();
    d = Dot(n, points[0]);
}

Polygon::~Polygon()
{
}

void Polygon::intersect(HitRecord& hit, const RenderContext& context, const Ray& ray) const
{
    int vectorDirection, newVectorDirection;
    vectorDirection = newVectorDirection = -1;
    if(points.size()>=3){
        // find ray-plane intersection
        double denom = Dot(n, ray.direction());
        double t = (d-Dot(n, ray.origin()))/denom;
        Point intersectionPoint = ray.origin() + ( t * ray.direction() );
        
        // check if there is a intersection
        int j = 0;
        for (int i=0; i<points.size(); i++) {
            if(i+1>=points.size())
                j = 0;
            else
                j = i+1;
            // pluker coordinates
            Vector p_d1 = points[j] - points[i];
            Vector p_m1 = Cross(points[i]-Point(0,0,0), points[j]-Point(0,0,0));
            Vector p_d2 = intersectionPoint - ray.origin();
            Vector p_m2 = Cross(ray.origin()-Point(0,0,0), intersectionPoint-Point(0,0,0));
            double dotProduct = Dot(p_d1, p_m2) + Dot(p_d2, p_m1);
            
            if(dotProduct==0.0){
                break;
            }else if(dotProduct>0.0){
                newVectorDirection = CounterClockwise;
            }else if(dotProduct<0.0){
                newVectorDirection = Clockwise;
            }
            
            // set first value
            if(i==0){
                vectorDirection = newVectorDirection;
                continue;
            }
            
            if(vectorDirection != newVectorDirection){
                return;
            }
        }
        
        // find hit
        hit.hit(t, this, matl);
    }
}

void Polygon::normal(Vector& normal, const RenderContext&,
                   const Point&, const Ray&, const HitRecord&) const
{
    normal = n;
}

void Polygon::getBounds(BoundingBox& bbox) const
{
    cerr <<  "Plane::getBounds() called!" << endl;
    exit( 1 );
}

