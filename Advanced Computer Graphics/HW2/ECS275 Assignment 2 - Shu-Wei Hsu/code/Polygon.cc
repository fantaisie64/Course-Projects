
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

Polygon::Polygon(Material* material, const vector<Point>& points, const Vector& speed)
: Primitive(material), points(points), speed(speed)
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

void Polygon::intersect(HitRecord& hit, const RenderContext& context, const Ray& ray, double time) const
{
    static Vector newN(0.0, 0.0, 0.0);
    static double newD = 0.0;
    time = 0.01 * time;
    
    if(points.size()>=3){
        
        // set new normal n and d
        Point newPoint0(points[0].x()+speed.x()*time, points[0].y()+speed.y()*time, points[0].z()+speed.z()*time);
        Point newPoint1(points[1].x()+speed.x()*time, points[1].y()+speed.y()*time, points[1].z()+speed.z()*time);
        Point newPoint2(points[2].x()+speed.x()*time, points[2].y()+speed.y()*time, points[2].z()+speed.z()*time);
        Vector sideVector1(newPoint1 - newPoint0);
        Vector sideVector2(newPoint2 - newPoint1);
        newN = Cross(sideVector1, sideVector2);
        newN.normalize();
        newD = Dot(newN, newPoint0);
        
        int vectorDirection, newVectorDirection;
        vectorDirection = newVectorDirection = -1;
        // find ray-plane intersection
        double denom = Dot(newN, ray.direction());
        double t = (newD-Dot(newN, ray.origin()))/denom;
        Point intersectionPoint = ray.origin() + ( t * ray.direction() );
        
        // check if there is a intersection
        int j = 0;
        for (int i=0; i<points.size(); i++) {
            if(i+1>=points.size())
                j = 0;
            else
                j = i+1;
            // pluker coordinates
            Point newPointI(points[i].x()+speed.x()*time, points[i].y()+speed.y()*time, points[i].z()+speed.z()*time);
            Point newPointJ(points[j].x()+speed.x()*time, points[j].y()+speed.y()*time, points[j].z()+speed.z()*time);
            Vector p_d1 = newPointJ - newPointI;
            Vector p_m1 = Cross(newPointI-Point(0,0,0), newPointJ-Point(0,0,0));
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
    
    /*int vectorDirection, newVectorDirection;
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
     }*/
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

