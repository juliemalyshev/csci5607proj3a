
//Set the global scene parameter variables
//TODO: Set the scene parameters based on the values in the scene file

#ifndef PARSE_VEC3_H
#define PARSE_VEC3_H

#include <cstdio>
#include <iostream>
#include <fstream>
#include <cstring>

//Camera & Scene Parmaters (Global Variables)
//Here we set default values, override them in parseSceneFile()

//Image Parmaters
int img_width = 800, img_height = 600;
std::string imgName = "raytraced.png";

//Camera Parmaters
vec3 eye = vec3(0,0,0);
vec3 forward = vec3(0,0,-1).normalized();
vec3 up = vec3(0,1,0).normalized();
vec3 right = vec3(-1,0,0).normalized();
float halfAngleVFOV = 35;


//Scene (Sphere) Parmaters
vec3 spheres[20];
float radii[20];
float materials[20][20];
vec3 spherePos = vec3(0,0,2);
float sphereRadius = 1;
int i = 0;
int f = 0;
int y = 0;
int g = 0;

//Color
float red,blue,green;
float ambientr;
float ambientg;
float ambientb;
float diffuser, diffuseg, diffuseb;
float specr,specg,specb;
float transr,transg,transb;
float highlight;
float ioR;

//ambient light array
float amlight[20][20];
float plight[20][20];
//Point light
Color intensityP;
vec3 lightPosP;

float rs;
float bs;
float gs;

//Ambient Light
float ra;
float ba;
float ga;

void parseSceneFile(std::string fileName){
  //TODO: Override the default values with new data from the file "fileName"
  FILE *fp;
  long length;
  char line[1024]; //Assumes no line is longer than 1024 characters!

  std::string str = fileName;
  const char *cstr = str.c_str();

  // open the file containing the scene description
  fp = fopen(cstr, "r");

  // check for errors in opening the file
  if (fp == NULL) {
    printf("Can't open file '%s'\n", fileName.c_str());
    return;
  }



  //Loop through reading each line
  while( fgets(line,1024,fp) ) { //Assumes no line is longer than 1024 characters!
    if (line[0] == '#'){
      printf("Skipping comment: %s", line);
      continue;
    }

    char command[100];
    char lineComp[100];
    int fieldsRead = sscanf(line,"%s ",command); //Read first word in the line (i.e., the command type)
    std::string commandStr = command;

    if (fieldsRead < 1){ //No command read
     //Blank line
     continue;
    }

    if (commandStr == "sphere:"){ //If the command is a sphere command
       float x,y,z;
       float r;
       sscanf(line,"sphere: %f %f %f %f", &x, &y, &z, &r);
       //printf(line);
       //printf("Sphere as position (%i,%i,%i) with radius %f\n",x,y,z,r);
       sphereRadius = r;
       spherePos = vec3(x,y,z);
       spheres[i] = spherePos;
       radii[i] = sphereRadius;
       printf("Sphere as position %f %f %f with radius %f\n",spheres[i].x,spheres[i].y,spheres[i].z,radii[i]);
       i++;


    }
    else if (commandStr == "background:"){ //If the command is a background command
       float r,g,b;
       sscanf(line,"background: %f %f %f", &r, &g, &b);
       red = r;
       green = g;
       blue = b;
       printf("Background color of (%f,%f,%f)\n",r,g,b);
    }
    else if (commandStr == "output_image:"){ //If the command is an output_image command
       char outFile[1024];
       sscanf(line,"output_image: %s", outFile);

       imgName = outFile;
       printf("Render to file named: %s\n", outFile);
    }
    else if (commandStr == "camera_pos:"){ //If the command is an output_image command
       int x,y,z;
       sscanf(line,"camera_pos: %d %d %d", &x, &y, &z);
       vec3 newPos = vec3(x,y,z);

       eye = newPos;
       //printf("Render to file named: %s\n", outFile);
    }
    else if (commandStr == "camera_fwd:"){ //If the command is an output_image command
       float x,y,z;
       sscanf(line,"camera_fwd: %f %f %f", &x, &y, &z);
       printf("camera_fwd: %f %f %f", x, y, z);
       vec3 newFwd = vec3(x,y,z).normalized();
       forward = newFwd;
       //printf("Render to file named: %s\n", outFile);
    }
    else if (commandStr == "camera_up:"){ //If the command is camera_up command
       float x,y,z;
       sscanf(line,"camera_up: %f %f %f", &x, &y, &z);
       printf("camera_up: %f %f %f", x, y, z);
       vec3 newUp = vec3(x,y,z).normalized();
       up = newUp;

       printf("\n");
    }
    else if (commandStr == "camera_fov_ha:"){ //If the command is an output_image command
       float a;
       sscanf(line,"camera_fov_ha: %f", &a);
       halfAngleVFOV = a;
       //printf("Render to file named: %s\n", outFile);
    }
    else if (commandStr == "image_resolution:"){ //If the command is an output_image command
       int x,y;
       sscanf(line,"image_resolution: %d %d", &x, &y);
       img_width = x;
       img_height = y;
       //printf("Render to file named: %s\n", outFile);
    }
    else if (commandStr == "material:"){ //If the command is an output_image command
       float ar,ag,ab,dr,dg,db,sr,sg,sb,ns,tr,tg,tb,ior;
       sscanf(line,"material: %f %f %f %f %f %f %f %f %f %f %f %f %f %f", &ar, &ag, &ab, &dr, &dg, &db, &sr, &sg, &sb, &ns, &tr, &tg, &tb, &ior);
       ambientr = ar;
       ambientg = ag;
       ambientb = ab;
       diffuser = dr;
       diffuseg = dg;
       diffuseb = db;
       specr = sr;
       specg = sg;
       specb = sb;
       highlight = ns;
       transr = tr;
       transg = tg;
       transb = tb;
       ioR = ior;

       materials[f][0] = ambientr;
       materials[f][1] = ambientg;
       materials[f][2] = ambientb;
       materials[f][3] = diffuser;
       materials[f][4] = diffuseg;
       materials[f][5] = diffuseb;
       materials[f][6] = specr;
       materials[f][7] = specg;
       materials[f][8] = specb;
       materials[f][9] = highlight;
       materials[f][10] = transr;
       materials[f][11] = transg;
       materials[f][12] = transb;
       materials[f][13] = ioR;
       f++;
       printf("HELLO: %f \n", materials[0][2]);
       //printf("material: %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n", ambientr,ambientg,ambientb,diffuser,diffuseb,diffuseg,specr,specg,specb,highlight,transr, transg, transb,ioR);
    }
    else if (commandStr == "point_light:"){ //If the command is an output_image command
       int r,g,b,x,y,z;

       sscanf(line,"point_light: %d %d %d %d %d %d", &r, &g, &b, &x, &y, &z);


         rs = r;
         gs = g;
         bs = b;

       lightPosP = vec3(x,y,z);
       //printf("Render to file named: %s\n", outFile);
    }
    else if (commandStr == "spot_light:"){ //If the command is an output_image command
       int r,g,b,px,py,pz,dx,dy,dz,a1,a2;
       sscanf(line,"spot_light: %d %d %d %d %d %d %d %d %d %d %d", &r, &g, &b, &px, &py, &pz, &dx, &dy, &dz, &a1, &a2);
       Color intensity = Color(r,g,b);
       vec3 lightPos = vec3(px,py,pz);
       vec3 lightDir = vec3(dx,dy,dz);

       //printf("Render to file named: %s\n", outFile);
    }
    else if (commandStr == "ambient_light:"){ //If the command is an output_image command
       float r,g,b;

       sscanf(line,"ambient_light: %f %f %f", &r, &g, &b);

       ra = r;
       ga = g;
       ba = b;

       printf("RA: %f\n", ra);
    }
    else {
      printf("WARNING. Unknown command: '%s'\n",command);
    }
  }
  //TODO: Create an orthagonal camera basis, based on the provided up and right vectors
  printf("Orthagonal Camera Basis:\n");
  forward = forward.normalized();
  up = up.normalized();
  right = cross(up,forward).normalized();
  up = cross(forward, right).normalized();
  //up = cross(rightward,forward).normalized();


  //forward = cross(up,right).normalized();
  printf("forward: %f,%f,%f\n",forward.x,forward.y,forward.z);
  printf("right: %f,%f,%f\n",right.x,right.y,right.z);
  printf("up: %f,%f,%f\n",up.x,up.y,up.z);
}

#endif
