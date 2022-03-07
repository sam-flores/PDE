#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "FPToolkit.c"

int width = 600;
int height = 600;
double origin = 300;
double map_x = 600/20;
double map_y = 600/8;
double min = -10;
double max = 10;
double M = 10000;
double N = 200;
int init = 1;
double t = 0;

double prod_u(double u, double v){ // production function for u
  return 100*u*(1 - v);
}

double prod_v(double u, double v){ // production function for v
  return -100*v*(1 - u);
}

double init_u(double x){ // initial conditions for u over x
    return 1/(x*x+1);

}
double init_v(double x){ // initial conditions for v over x
  return 1 - 1/(x*x+1);
}

double diffuse_u(double u){ // diffusion function for u
  return 0;
}

double diffuse_v(double u){ // diffusion function for v, function of u
  return 10*u;
}

void draw_points(double x1, double y1, double x2, double y2,
                  double r, double g, double b){

  x1 *= map_x;
  y1 = y1*map_y + origin;
  x2 *= map_x;
  y2 = y2*map_y + origin;
  G_rgb(r, g, b);
  G_line(x1, y1, x2, y2);

}

void axes(){

  double y_zero = height/2, y_max = height;
  double x_zero = width/2, x_max = width;

  G_rgb(1, 1, 1);
  G_line(0, origin, width, origin);
  G_line(origin, 0,origin, height);
  G_rgb(1, 1, 0);
  G_line(0, origin + map_y, width, origin + map_y);


}

double secondderivative(double u[3], double deltaX){

    return ((u[2]) - (2 * u[1]) + (u[0])) / (deltaX * deltaX);

}

void plot(){

  double prev_u[10000], prev_v[10000],
          u[10000], v[10000];

  double dX = (max - min)/(N-1);
  double dT = 1/(M-1); // 10 time steps
  int size = N-1;

  double a[3];        //holds the values around u[i],v[i] for second derivative
  int i = 0;
  double x = 0;
  double su, sv, diffusion;

  if(init == 1){                   //set up initial conditions
    for(double x = min; x <= max; x += dX){
      u[i] = init_u(x);
      v[i] = init_v(x);
      prev_u[i] = u[i]; // state zero
      prev_v[i] = v[i];
      if(i > 1 && i < size - 1){
      draw_points(x, x + dX, u[i], u[i + 1], 0, 1, 1);  // x1, y1, x2, y2, rgb
      draw_points(x, x + dX, v[i], v[i + 1], 0, 1, 0);
    }
      i++;
    }
    init = 0; // every other run is not initial conditions
  }else if(init == 0){
    for(i = 0; i < size; i++){ //evolve u,v
      if(i >= 1 && i < size - 1){ // evolve the second derivative around i of previous state
        a[0] = prev_u[i - 1];
        a[1] = prev_u[i];
        a[2] = prev_u[i + 1];
        su = secondderivative(a, dX);
        a[0] = prev_v[i - 1];
        a[1] = prev_v[i];
        a[2] = prev_v[i + 1];
        sv = secondderivative(a, dX);
      }else if(i == size - 1 || i == 0){
         su = 1;  // partial u partial x is 0 at the boundary
         sv = 1;
       }

       diffusion = diffuse_u(u[i]); // evaluate diffusion which depends on u at x
       u[i] = u[i] + diffusion*su*dT + prod_u(u[i],v[i])*dT; // euler formula

       diffusion = diffuse_v(u[i]); // evaluate diffusion which depends on u at x
       v[i] = v[i] + diffusion*sv*dT + prod_v(u[i], v[i])*dT; //diffusion constant 1/10
       // euler method^^^^
       if(i > 1 && i < size - 1){
       draw_points(x, u[i], x + dX, u[i + 1], 0, 1, 1);
       draw_points(x, v[i], x + dX, v[i + 1], 0, 1, 0);
     }
       x += dX;
     }
   }
   for(i = 0; i < size; i++){ // copy in current state into previous state
     prev_u[i] = u[i];
     prev_v[i] = v[i];
   }
}

int main() {

    G_init_graphics(width, height);
    int q = '\0';

    while(q != 'q'){
      G_rgb(0, 0, 0);
      G_clear();
      axes();
      plot();
      G_display_image();
      usleep(10000);
      q = G_no_wait_key();
  }
}
