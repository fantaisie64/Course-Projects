#include "Cube.h"
#include "BoundingBox.h"
#include "HitRecord.h"
#include "Point.h"
#include "Ray.h"
#include "Vector.h"
#include <math.h>
#include <iostream>
using namespace std;

Cube::Cube(Material* material, const Point& center, double sideLength)
: Primitive(material), center(center), sideLength(sideLength)
{
}

Cube::~Cube()
{
}

void Cube::getBounds(BoundingBox& bbox) const
{
    double halfDiagonal = sideLength/(sqrt(2.0f));
    Vector diag(halfDiagonal, halfDiagonal, halfDiagonal);
    bbox.extend(center+diag);
    bbox.extend(center-diag);
}

void Cube::intersect(HitRecord& hit, const RenderContext&, const Ray& ray) const
{
    BoundingBox bbox;
    getBounds(bbox);
    //Ray ray2(Point(0,0,0), Vector(1,1,1));
    double txmin, txmax, tymin, tymax, tzmin, tzmax;
    if(ray.direction().x() >= 0.0f) {
        txmin = (bbox.getMin().x()-ray.origin().x())/ray.direction().x();
        txmax = (bbox.getMax().x()-ray.origin().x())/ray.direction().x();
    }else{
        txmin = (bbox.getMax().x()-ray.origin().x())/ray.direction().x();
        txmax = (bbox.getMin().x()-ray.origin().x())/ray.direction().x();
    }
    if(ray.direction().y() >= 0.0f) {
        tymin = (bbox.getMin().y()-ray.origin().y())/ray.direction().y();
        tymax = (bbox.getMax().y()-ray.origin().y())/ray.direction().y();
    }else{
        tymin = (bbox.getMax().y()-ray.origin().y())/ray.direction().y();
        tymax = (bbox.getMin().y()-ray.origin().y())/ray.direction().y();
    }
    if(ray.direction().z() >= 0.0f) {
        tzmin = (bbox.getMin().z()-ray.origin().z())/ray.direction().z();
        tzmax = (bbox.getMax().z()-ray.origin().z())/ray.direction().z();
    }else{
        tzmin = (bbox.getMax().z()-ray.origin().z())/ray.direction().z();
        tzmax = (bbox.getMin().z()-ray.origin().z())/ray.direction().z();
    }
    
    // if ray goes outside of cube
    if ((txmin > tymax) || (tymin > txmax)){
        return;
    }
    if ((tymin > tzmax) || (tzmin > tymax)){
        return;
    }
    if ((tzmin > txmax) || (txmin > tzmax)){
        return;
    }
    
    // find hit
    double minT = 0.0;
    if(tymin <= txmin && tzmin <= txmin){
        minT = txmin;
    }else if(tzmin <= tymin && txmin <= tymin){
        minT = tymin;
    }else if(txmin <= tzmin && tymin <= tzmin){
        minT = tzmin;
    }
    
    hit.hit(minT, this, matl);
}

void Cube::normal(Vector& normal, const RenderContext&, const Point& hitpos,
                    const Ray& ray, const HitRecord& hit) const
{
    BoundingBox bbox;
    getBounds(bbox);
    Point checkMinSide(hitpos-bbox.getMin());
    Point checkMaxSide(hitpos-bbox.getMax());
    double checkValue[6];
    checkValue[0] = Abs(checkMinSide.x());
    checkValue[1] = Abs(checkMinSide.y());
    checkValue[2] = Abs(checkMinSide.z());
    checkValue[3] = Abs(checkMaxSide.x());
    checkValue[4] = Abs(checkMaxSide.y());
    checkValue[5] = Abs(checkMaxSide.z());
    int checkSide = 0;
    for(int i = 0; i<6; i++){
        if(i!=checkSide && checkValue[i]<=checkValue[checkSide])
                checkSide = i;
    }
    switch (checkSide){
        case 0:
            normal = Vector(-1.0f,0.0f,0.0f);
            break;
        case 1:
            normal = Vector(0.0f,-1.0f,0.0f);
            break;
        case 2:
            normal = Vector(0.0f,0.0f,-1.0f);
            break;
        case 3:
            normal = Vector(1.0f,0.0f,0.0f);
            break;
        case 4:
            normal = Vector(0.0f,1.0f,0.0f);
            break;
        case 5:
            normal = Vector(0.0f,0.0f,1.0f);
            break;
    }
}