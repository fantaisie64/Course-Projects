
#ifndef AreaLight_h
#define AreaLight_h

#include "Light.h"
#include "Point.h"
#include "Color.h"
#include "Ray.h"
#include <stdlib.h>

class AreaLight : public Light {
public:
    AreaLight(const Point& position, const Vector& normal, double length, double width, const Color& color);
    virtual ~AreaLight();
    
    virtual void preprocess();
    virtual double getLight(Color& light_color, Vector& light_direction,
                            const RenderContext& context, const Point& pos) const;
    virtual bool hitLight(const Ray& ray) const;
    
private:
    AreaLight(const AreaLight&);
    AreaLight& operator=(const AreaLight&);
    
    Point position;
    Vector normal;
    double d;
    Color color;
    double length;
    double width;
};

#endif

