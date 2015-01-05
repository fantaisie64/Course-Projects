
#include "AreaLight.h"

AreaLight::AreaLight(const Point& position, const Vector& normal, double length, double width, const Color& color)
: position(position), normal(normal), length(length), width(width), color(color)
{
    this->normal.normalize();
    d = Dot(normal, position);
}

AreaLight::~AreaLight()
{
}

void AreaLight::preprocess()
{
}

double AreaLight::getLight(Color& light_color, Vector& light_direction,
                            const RenderContext&, const Point& hitpos) const
{
    light_color = color;
    Point randomPoint = Point( position.x() + (drand48()-0.5)*length, position.y()+ (drand48()-0.5)*width, position.z() );
    Vector dir = randomPoint-hitpos;
    double len = dir.normalize();
    light_direction = dir;
    return len;
}

bool AreaLight::hitLight(const Ray& ray) const
{
    double denom = Dot(normal, ray.direction());
    if(denom > 0.0 && Abs(denom) > 0.0001f){
        double t = (d-Dot(normal, ray.origin()))/denom;
        Point intersect = ray.origin() + t * ray.direction();
        if( (intersect.x()-position.x())<length/2 && (intersect.x()-position.x())>-(length/2)
           &&(intersect.y()-position.y())<width/2 && (intersect.y()-position.y())>-(width/2) )
            return true;
        
    }
    return false;
}
