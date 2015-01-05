
#include "Scene.h"
#include "Background.h"
#include "Camera.h"
#include "HitRecord.h"
#include "Image.h"
#include "Light.h"
#include "Material.h"
#include "LambertianMaterial.h"
#include "Primitive.h"
#include "Object.h"
#include "Ray.h"
#include "RenderContext.h"
#include <float.h>
#include <iostream>
#include <stdlib.h>
using namespace std;

Scene::Scene()
{
  object = 0;
  background = 0;
  camera = 0;
  ambient = Color(0, 0, 0);
  image = 0;
  minAttenuation = 0;
}

Scene::~Scene()
{
  delete object;
  delete background;
  delete camera;
  delete image;
  for(int i=0;i<static_cast<int>(lights.size());i++){
    Light* light = lights[i];
    delete light;
  }
}

void Scene::preprocess()
{
  background->preprocess();
  for(int i=0;i<static_cast<int>(lights.size());i++){
    Light* light = lights[i];
    light->preprocess();
  }
  double aspect_ratio = image->aspect_ratio();
  camera->preprocess(aspect_ratio);
  object->preprocess();
}

void Scene::render()
{
  if(!object || !background || !camera || !image){
    cerr << "Incomplete scene, cannot render!\n";
    exit(1);
  }
  int xres = image->getXresolution();
  int yres = image->getYresolution();
  RenderContext context(this);
  double dx = 2./xres;
  double xmin = -1. + dx/2.;
  double dy = 2./yres;
  double ymin = -1. + dy/2.;
  Color atten(1,1,1);
  for(int i=0;i<yres;i++){
    //cerr << "y=" << i << '\n';
    double y = ymin + i*dy;
    for(int j=0;j<xres;j++){
      double x = xmin + j*dx;
      //cerr << "x=" << j << ", y=" << i << '\n';
      Ray ray;
      camera->makeRay(ray, context, x, y);
      HitRecord hit(DBL_MAX);
      object->intersect(hit, context, ray);
      Color result;
      if(hit.getPrimitive()){
        // Ray hit something...
        const Material* matl = hit.getMaterial();
        matl->shade(result, context, ray, hit, atten, 0);
      } else {
        background->getBackgroundColor(result, context, ray);
      }
      image->set(j, i, result);
    }
  }
}

void Scene::newRender()
{
    if(!object || !background || !camera || !image){
        cerr << "Incomplete scene, cannot render!\n";
        exit(1);
    }
    int xres = image->getXresolution();
    int yres = image->getYresolution();
    RenderContext context(this);
    double dx = 2./xres;
    double xmin = -1. + dx/2.;
    double dy = 2./yres;
    double ymin = -1. + dy/2.;
    Color atten(1,1,1);
    setMaxRayDepth(10);
    setMinAttenuation(0.01);
    for(int i=0;i<yres;i++){
        //cerr << "y=" << i << '\n';
        double y = ymin + i*dy;
        for(int j=0;j<xres;j++){
            double x = xmin + j*dx;
            //cerr << "x=" << j << ", y=" << i << '\n';
            Ray ray;
            camera->makeRay(ray, context, x, y);
            //HitRecord hit(DBL_MAX);
            //object->intersect(hit, context, ray);
            Color result;
            result = newTraceRay(context, object, ray, atten, 1);
            image->set(j, i, result);
        }
    }
}

double Scene::traceRay(Color& result, const RenderContext& context, const Ray& ray, const Color& atten, int depth) const
{
  if(depth >= maxRayDepth || atten.maxComponent() < minAttenuation){
    result = Color(0, 0, 0);
    return 0;
  } else {
    HitRecord hit(DBL_MAX);
    object->intersect(hit, context, ray);
    if(hit.getPrimitive()){
      // Ray hit something...
      const Material* matl = hit.getMaterial();
      matl->shade(result, context, ray, hit, atten, depth);
      return hit.minT();
    } else {
      background->getBackgroundColor(result, context, ray);
      return DBL_MAX;
    }
  }
}

double Scene::traceRay(Color& result, const RenderContext& context, const Object* obj, const Ray& ray, const Color& atten, int depth) const
{
  if(depth >= maxRayDepth || atten.maxComponent() < minAttenuation){
    result = Color(0, 0, 0);
    return 0;
  } else {
    HitRecord hit(DBL_MAX);
    obj->intersect(hit, context, ray);
    if(hit.getPrimitive()){
      // Ray hit something...
      const Material* matl = hit.getMaterial();
      matl->shade(result, context, ray, hit, atten, depth);
      return hit.minT();
    } else {
      background->getBackgroundColor(result, context, ray);
      return DBL_MAX;
    }
  }
}

Color Scene::newTraceRay(const RenderContext& context, const Object* obj, const Ray& ray, const Color& atten, int depth) const
{
    Color result(0, 0, 0);
    
    if(depth >= maxRayDepth || atten.maxComponent() < minAttenuation){
        result = Color(0, 0, 0);
        return result;
    } else {
        HitRecord hit(DBL_MAX);
        object->intersect(hit, context, ray);
        if(hit.getPrimitive()){
            
            const LambertianMaterial* matl = (LambertianMaterial*)hit.getMaterial();
            vector<Light*> lights = this->getLights();
            
            // get intersection point normal
            Vector normal;
            Point intersectionPoint = ray.origin() + ray.direction() * hit.minT();
            hit.getPrimitive()->normal(normal, context, intersectionPoint, ray, hit);
            normal.normalize();
            
            // compute ambient color
            Color ambientColor = this->getAmbient() * matl->getKa();
            result += matl->getColor() * ambientColor;
            
            for(int i=0;i<lights.size();i++){
                Vector lightDirection;
                Color lightColor;
                double lightDistance = lights[i]->getLight(lightColor, lightDirection, context, intersectionPoint);
                HitRecord shadowHit(lightDistance);
                Ray shadowRay(intersectionPoint, lightDirection);
                obj->intersect(shadowHit, context, shadowRay);
                
                // if no shadow
                if(!shadowHit.getPrimitive()){
                    
                    // compute diffuse color
                    Color diffuseColor(0,0,0);
                    float diffuseFactor = matl->getKd();
                    if (diffuseFactor > 0.0f){
                        float dotProduct = Dot(lightDirection, normal);
                        if(dotProduct > 0.0f)
                            diffuseColor += lightColor * dotProduct * diffuseFactor;
                    }
                    
                    // compute specular color
                    int shininess = 10;
                    Color specularColor(0,0,0);
                    float specularFactor = matl->getKs();
                    if (specularFactor > 0.0f){
                        // compute reflection light
                        Vector eyeDirection = ray.origin() - intersectionPoint;
                        eyeDirection.normalize();
                        Vector reflectedLight = 2.0f * Dot(lightDirection, normal) * normal - lightDirection;
                        reflectedLight.normalize();
                        float dotProduct = Dot(reflectedLight, eyeDirection);
                        if(dotProduct > 0.0f)
                            specularColor += lightColor * pow(dotProduct, shininess) * specularFactor;
                    }
                    result += matl->getColor() * (diffuseColor + specularColor);
                    
                }
            }
            
            // compute reflection ray
            Color reflectionColor(0,0,0);
            float reflectionFactor = matl->getKr();
            if (reflectionFactor > 0.0f){
                Vector reflected = 2.0f * Dot((-ray.direction()), normal) * normal - (-ray.direction());
                Ray reflectedRay = Ray(intersectionPoint+ reflected * 0.0001f, reflected); // avoid reflecting to itself
                reflectionColor = newTraceRay(context, object, reflectedRay, atten*reflectionFactor, depth + 1) * reflectionFactor;
            }            
            return result + reflectionColor;
            
        } else{
            background->getBackgroundColor(result, context, ray);
            return result;
        }
    }
}
