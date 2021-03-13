//CSCI 5607 HW3 - Rays & Files
//This HW has three steps:
// 1. Compile and run the program (the program takes a single command line argument)
// 2. Understand the code in this file (rayTrace_vec3.cpp), in particular be sure to understand:
//     -How ray-sphere intersection works
//     -How the rays are being generated
//     -The pipeline from rays, to intersection, to pixel color
//    After you finish this step, and understand the math, take the HW quiz on canvas
// 3. Update the file parse_vec3.h so that the function parseSceneFile() reads the passed in file
//     and sets the relevant global variables for the rest of the code to product to correct image

//To Compile: g++ -fsanitize=address -std=c++11 rayTrace_vec3.cpp

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

//Images Lib includes:
#define STB_IMAGE_IMPLEMENTATION //only place once in one .cpp file
#define STB_IMAGE_WRITE_IMPLEMENTATION //only place once in one .cpp files
#include "image_lib.h" //Defines an image class and a color class
#include <iostream>
//#Vec3 Library
#include "vec3.h"

//High resolution timer
#include <chrono>

//Scene file parser
#include "parse_vec3.h"
vec3 intPoint;
bool intersect = false;

//Tests is the ray intersects the sphere
float raySphereIntersect(vec3 start, vec3 dir, vec3 center, float radius){
  float a = dot(dir,dir); //TODO: What do we know about "a" if "dir" is normalized on creation?
  vec3 toStart = (start - center);
  float b = 2 * dot(dir,toStart);
  float c = dot(toStart,toStart) - radius*radius;
  float discr = b*b - 4*a*c;
  //intPoint = toStart + (c*dir);
  if (discr < 0) return 0;
  else{
    float t0 = (-b + sqrt(discr))/(2*a);
    float t1 = (-b - sqrt(discr))/(2*a);
    if (t0 > 0 || t1 > 0){
      float tPosMin = std::fmin(t0,t1);
      tPosMin = std::fmax(0.0,tPosMin);
      intPoint = toStart + (tPosMin*dir);
      return tPosMin;
    }
  }
  return 0;
}
bool raySphereIntersectBool(vec3 start, vec3 dir, vec3 center, float radius){
  float a = dot(dir,dir); //TODO: What do we know about "a" if "dir" is normalized on creation?
  vec3 toStart = (start - center);
  float b = 2 * dot(dir,toStart);
  float c = dot(toStart,toStart) - radius*radius;
  float discr = b*b - 4*a*c;
  //intPoint = toStart + (c*dir);
  if (discr < 0) return false;
  else{
    float t0 = (-b + sqrt(discr))/(2*a);
    float t1 = (-b - sqrt(discr))/(2*a);
    if (t0 > 0 || t1 > 0){
      float tPosMin = std::fmin(t0,t1);
      //tPosMin = std::fmax(0.0,tPosMin);
      //intPoint = toStart + (tPosMin*dir);
      return true;
    }
  }
  return false;
}
int closestSphere(vec3 start, vec3 dir, vec3 centers[], float radii[]){
  float min = 10000;
  int indexToUse = 0;
  for(int q = 0; q < i;q++){
    float hit = raySphereIntersect(start,dir,centers[q],radii[q]);
    if(hit>0 && hit<min){

      indexToUse = q;
      min = hit;
    }
  }
  return indexToUse;
}

int main(int argc, char** argv){

  //Read command line paramaters to get scene file
  if (argc != 2){
     std::cout << "Usage: ./a.out scenefile\n";
     return(0);
  }
  std::string secenFileName = argv[1];

  //Parse Scene File
  parseSceneFile(secenFileName);

  float imgW = img_width, imgH = img_height;
  float halfW = imgW/2, halfH = imgH/2;
  float d = halfH / tanf(halfAngleVFOV * (M_PI / 180.0f));

  Image outputImg = Image(img_width,img_height);
  auto t_start = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < img_width; i++){
    for (int j = 0; j < img_height; j++){
      Color color;
      //TODO: In what way does this assumes the basis is orthonormal?
      //for(int q = 0; q < 10; q++){
        float u = (halfW - (imgW)*(i/imgW));
        float v = (halfH - (imgH)*(j/imgH));
        vec3 p = eye - d*forward + u*right + v*up;
        vec3 rayDir = (p - eye).normalized();  //Normalizing here is optional
        int index = closestSphere(eye,rayDir,spheres,radii);
        bool hit = raySphereIntersectBool(eye,rayDir,spheres[index],radii[index]);
        vec3 rayToLight = lightPosP - intPoint;
        bool hit2 = raySphereIntersectBool(intPoint,rayToLight,spheres[index],radii[index]);
        //std::cout<<hit2;

        if (hit){
          //lighting equation, diffuse + specular + ambient
          vec3 normal = ( intPoint-spheres[index] ).normalized();
          vec3 reflect = 2 * (dot(normal,rayToLight.normalized())) * (normal - rayToLight.normalized());
          float r = (materials[index][0] * ra) + (materials[index][3]  * std::fmax(0.0,dot(normal,(intPoint-lightPosP).normalized()))) + (materials[index][6]*ioR * pow(std::fmax(0,dot((intPoint-eye).normalized(),reflect)), highlight));
          float g = (materials[index][1] * ga) + (materials[index][4]  * std::fmax(0.0,dot(normal,(intPoint-lightPosP).normalized()))) + (materials[index][7]*ioR * pow(std::fmax(0,dot((intPoint-eye).normalized(),reflect)), highlight));
          float b = (materials[index][2] * ba) + (materials[index][5]  *std::fmax(0.0,dot(normal,(intPoint-lightPosP).normalized()))) + (materials[index][8]*ioR * pow(std::fmax(0,dot((intPoint-eye).normalized(),reflect)), highlight));
          //std::cout<<"HHEELLOO"<<normal.x<<"  "<<normal.y<<"  "<<normal.z;
          //std::cout<<"HHEELLOO"<<dot(normal,(intPoint-lightPosP).normalized());
          color = Color(r,g,b);


        }else if(hit2){
          color = Color(0,0,0);
        }
        else {
          color = Color(red,green,blue);
        }

        //outputImg.setPixel(i,j, Color(fabs(i/imgW),fabs(j/imgH),fabs(0))); //TODO: Try this, what is it visualizing?
    //  }
      outputImg.setPixel(i,j, color);

    }
  }
  auto t_end = std::chrono::high_resolution_clock::now();
  printf("Rendering took %.2f ms\n",std::chrono::duration<double, std::milli>(t_end-t_start).count());

  outputImg.write(imgName.c_str());
  return 0;
}
