
#ifndef Polygon_h
#define Polygon_h

#include "Vector.h"
#include "Primitive.h"
#include <vector>
class HitRecord;
class Point;
class RenderContext;
using namespace std;

class Polygon : public Primitive {
public:
    Polygon(Material* material, const vector<Point>& points);
    virtual ~Polygon();
    
    virtual void getBounds(BoundingBox& bbox) const;
    virtual void intersect(HitRecord& hit, const RenderContext& context, const Ray& ray) const;
    virtual void normal(Vector& normal, const RenderContext& context,
                        const Point & hitpos, const Ray& ray, const HitRecord& hit) const;
    
private:
    Polygon(const Polygon&);
    Polygon& operator=(const Polygon&);
    
    vector<Point> points;
    Vector n;
    double d;
};

#endif

