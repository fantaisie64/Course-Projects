#ifndef Cube_h
#define Cube_h

#include "Primitive.h"
#include "Point.h"
class Ray;

class Cube : public Primitive {
public:
    Cube(Material* material, const Point& center, double sideLength);
    virtual ~Cube();
    
    virtual void getBounds(BoundingBox& bbox) const;
    virtual void intersect(HitRecord& hit, const RenderContext& context, const Ray& ray) const;
    virtual void normal(Vector& normal, const RenderContext& context,
                        const Point& hitpos, const Ray& ray, const HitRecord& hit) const;
protected:
    Point center;
    double sideLength;
};

#endif
