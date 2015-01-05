
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
#include <math.h> 
using namespace std;

Scene::Scene()
{
  object = 0;
  background = 0;
  camera = 0;
  ambient = Color(0, 0, 0);
  image = 0;
  minAttenuation = 0;
  antialiasingSampleTimes = 1;
  depthOfFieldSampleTimes = 1;
  motionBlurSampleTimes = 1;
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
      object->intersect(hit, context, ray, 0);
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
    double pixelWidth  = 1.0 / xres;
    double pixelHeight = 1.0 / yres;
    
    double sqrtAntialiasingSampleTimes_double = round(sqrt((double)antialiasingSampleTimes));
    int sqrtAntialiasingSampleTimes_int = (int) sqrtAntialiasingSampleTimes_double;
    
    for(int i=0;i<yres;i++){
        //cerr << "y=" << i << '\n';
        double y = ymin + i*dy;
        for(int j=0;j<xres;j++){
            double x = xmin + j*dx;
            //cerr << "x=" << j << ", y=" << i << '\n';
            Ray ray;
            //HitRecord hit(DBL_MAX);
            //object->intersect(hit, context, ray);
            Color result(0,0,0);
            double weight = 0.0;
            
            // regular ray tracing
            /*camera->makeRay(ray, context, x, y);
            result = newTraceRay(context, object, ray, atten, 1, 0);*/
            
            // with anti-aliasing
            /*for(int k = 0; k<antialiasingSampleTimes; k++){
                double offset_x = (drand48() - 0.5) * pixelWidth;
                double offset_y = (drand48() - 0.5) * pixelHeight;
                camera->makeRay(ray, context, x+offset_x, y+offset_y);
                result += newTraceRay(context, object, ray, atten, 1, 0);
                weight += 1.0;
            }*/
            
            // with new anti-aliasing (Stratified sampling)
            /*for(int k = 0; k<antialiasingSampleTimes; k++){
                double new_x = (x - 0.5*pixelWidth)
                + (0.5+(double)(k%sqrtAntialiasingSampleTimes_int))
                * (pixelWidth/sqrtAntialiasingSampleTimes_double);
                double new_y = (y - 0.5*pixelHeight)
                + (0.5+(double)(k/sqrtAntialiasingSampleTimes_int))
                * (pixelHeight/sqrtAntialiasingSampleTimes_double);
                double offset_x = new_x + (drand48() - 0.5) * pixelWidth/sqrtAntialiasingSampleTimes_double;
                double offset_y = new_y + (drand48() - 0.5) * pixelHeight/sqrtAntialiasingSampleTimes_double;
                camera->makeRay(ray, context, offset_x, offset_y);
                result += newTraceRay(context, object, ray, atten, 1, 0);
                weight += 1.0;
             }*/
            
            // with depth of field
            /*for(int l = 0; l<depthOfFieldSampleTimes; l++){
                camera->makeLensRay(ray, context, x, y);
                result += newTraceRay(context, object, ray, atten, 1, 0);
                weight += 1.0;
            }*/
            
            // with motion blur
            /*for(int m = 0 ; m<motionBlurSampleTimes; m++){
                double time = double(m)- 0.5 + drand48();
                camera->makeRay(ray, context, x, y);
                result += newTraceRay(context, object, ray, atten, 1, time);
                weight += 1.0;
            }*/
            
            
            
            // with anti-aliasing & depth of field (compressed sampling)
            // with depth of field
            // with new anti-aliasing (Stratified sampling)
            /*for(int k = 0; k<antialiasingSampleTimes; k++){
                double new_x = (x - 0.5*pixelWidth)
                + (0.5+(double)(k%sqrtAntialiasingSampleTimes_int))
                * (pixelWidth/sqrtAntialiasingSampleTimes_double);
                double new_y = (y - 0.5*pixelHeight)
                + (0.5+(double)(k/sqrtAntialiasingSampleTimes_int))
                * (pixelHeight/sqrtAntialiasingSampleTimes_double);
                double offset_x = new_x + (drand48() - 0.5) * pixelWidth/sqrtAntialiasingSampleTimes_double;
                double offset_y = new_y + (drand48() - 0.5) * pixelHeight/sqrtAntialiasingSampleTimes_double;
                camera->makeLensRay(ray, context, offset_x, offset_y);
                result += newTraceRay(context, object, ray, atten, 1, 0);
                weight += 1.0;
            }*/
            
            
            
            // with anti-aliasing & motion blur (not compressed sampling)
            // with motion blur
            /*for(int m = 0 ; m<motionBlurSampleTimes; m++){
                double time = double(m)- 0.5 + drand48();
                
                // with new anti-aliasing (Stratified sampling sampling)
                for(int k = 0; k<antialiasingSampleTimes; k++){
                    double new_x = (x - 0.5*pixelWidth)
                    + (0.5+(double)(k%sqrtAntialiasingSampleTimes_int))
                    * (pixelWidth/sqrtAntialiasingSampleTimes_double);
                    double new_y = (y - 0.5*pixelHeight)
                    + (0.5+(double)(k/sqrtAntialiasingSampleTimes_int))
                    * (pixelHeight/sqrtAntialiasingSampleTimes_double);
                    double offset_x = new_x + (drand48() - 0.5) * pixelWidth/sqrtAntialiasingSampleTimes_double;
                    double offset_y = new_y + (drand48() - 0.5) * pixelHeight/sqrtAntialiasingSampleTimes_double;
                    camera->makeRay(ray, context, offset_x, offset_y);
                    result += newTraceRay(context, object, ray, atten, 1, time);
                    weight += 1.0;
                }
            }*/
            
            // with anti-aliasing & motion blur (compressed sampling)
            // with motion blur
            // create random array
            /*int randomM [16];
            for(int a=0; a<motionBlurSampleTimes; a++){
                randomM[a] = a;
            }
            for(int m = 0 ; m<motionBlurSampleTimes; m++){
                double time = double(m)- 0.5 + drand48();
                
                // with new anti-aliasing (Stratified sampling)
                int k_index = (int)round(drand48() * (double)(motionBlurSampleTimes-1-m));
                int k = randomM[k_index];
                randomM[k_index] = randomM[motionBlurSampleTimes-1-m];
                double new_x = (x - 0.5*pixelWidth)
                + (0.5+(double)(k%sqrtAntialiasingSampleTimes_int))
                * (pixelWidth/sqrtAntialiasingSampleTimes_double);
                double new_y = (y - 0.5*pixelHeight)
                + (0.5+(double)(k/sqrtAntialiasingSampleTimes_int))
                * (pixelHeight/sqrtAntialiasingSampleTimes_double);
                double offset_x = new_x + (drand48() - 0.5) * pixelWidth/sqrtAntialiasingSampleTimes_double;
                double offset_y = new_y + (drand48() - 0.5) * pixelHeight/sqrtAntialiasingSampleTimes_double;
                camera->makeRay(ray, context, offset_x, offset_y);
                result += newTraceRay(context, object, ray, atten, 1, time);
                weight += 1.0;
            }*/
            
            
            
            // with depth of field & with motion blur (compressed sampling)
            /*for(int m = 0 ; m<motionBlurSampleTimes; m++){
                double time = double(m)- 0.5 + drand48();
                camera->makeLensRay(ray, context, x, y);
                result += newTraceRay(context, object, ray, atten, 1, time);
                weight += 1.0;
            }*/
            
            
            
            // all
            // with motion blur
            int randomM [16];
            for(int a=0; a<motionBlurSampleTimes; a++){
                randomM[a] = a;
            }
            for(int m = 0 ; m<motionBlurSampleTimes; m++){
                double time = double(m)- 0.5 + drand48();
                
                // with new anti-aliasing (Stratified sampling)
                int k_index = (int)round(drand48() * (double)(motionBlurSampleTimes-1-m));
                int k = randomM[k_index];
                randomM[k_index] = randomM[motionBlurSampleTimes-1-m];
                double new_x = (x - 0.5*pixelWidth)
                + (0.5+(double)(k%sqrtAntialiasingSampleTimes_int))
                * (pixelWidth/sqrtAntialiasingSampleTimes_double);
                double new_y = (y - 0.5*pixelHeight)
                + (0.5+(double)(k/sqrtAntialiasingSampleTimes_int))
                * (pixelHeight/sqrtAntialiasingSampleTimes_double);
                double offset_x = new_x + (drand48() - 0.5) * pixelWidth/sqrtAntialiasingSampleTimes_double;
                double offset_y = new_y + (drand48() - 0.5) * pixelHeight/sqrtAntialiasingSampleTimes_double;
                // with depth of field
                camera->makeLensRay(ray, context, offset_x, offset_y);
                result += newTraceRay(context, object, ray, atten, 1, time);
                weight += 1.0;
            }
            
            
            result = result/weight;
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
    object->intersect(hit, context, ray, 0);
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
    obj->intersect(hit, context, ray, 0);
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

Color Scene::newTraceRay(const RenderContext& context, const Object* obj, const Ray& ray, const Color& atten, int depth, double time) const
{
    Color result(0, 0, 0);
    
    if(depth >= maxRayDepth || atten.maxComponent() < minAttenuation){
        result = Color(0, 0, 0);
        return result;
    } else {
        HitRecord hit(DBL_MAX);
        object->intersect(hit, context, ray, time);
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
                obj->intersect(shadowHit, context, shadowRay, time);
                
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
                reflectionColor = newTraceRay(context, object, reflectedRay, atten*reflectionFactor, depth + 1, time) * reflectionFactor;
            }            
            return result + reflectionColor;
            
        } else{
            background->getBackgroundColor(result, context, ray);
            return result;
        }
    }
}
