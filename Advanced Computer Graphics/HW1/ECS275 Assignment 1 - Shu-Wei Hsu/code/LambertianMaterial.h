
#ifndef LambertianMaterial_h
#define LambertianMaterial_h

#include "Material.h"
#include "Color.h"
#include "Vector.h"

class LambertianMaterial : public Material {
 public:
  LambertianMaterial(const Color& color, float Kd, float Ka, float Ks, float Kr);
  virtual ~LambertianMaterial();

  virtual void shade(Color& result, const RenderContext& context, const Ray& ray,
                     const HitRecord& hit, const Color& atten, int depth) const;
    
 Color getColor() const{
     return color;
 }
    
 float getKd() const{
     return Kd;
 }
    
 float getKa() const{
     return Ka;
 }
    
 float getKs() const{
     return Ks;
 }
    
 float getKr() const{
     return Kr;
 }
 
 private:
  LambertianMaterial(const LambertianMaterial&);
  LambertianMaterial& operator=(const LambertianMaterial&);

  Color color;
  float Kd;
  float Ka;
  float Ks;
  float Kr;
};

#endif
