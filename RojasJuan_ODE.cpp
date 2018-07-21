#include <iostream>
#include <math.h>
#include <valarray>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

//parámetros
const double m = 2.5;
const double q = 1.5;
const double B_x = 0.0;
const double B_y = 0.0;
const double B_z = 3.0;
std::valarray<double> B(3);

//longitud de la simulación
const double t_f = 10.0;
const double h = 0.001;
const int N = int(t_f/h);

//variables a usar, vectores para tiempo, posición y velocidad.
double t[N];
std::valarray<std::valarray<double> > r(std::valarray<double>(3), N);
std::valarray<std::valarray<double> > v(std::valarray<double>(3), N);

//condiciones iniciales
const double x_0 = 1.0;
const double y_0 = 0.0;
const double z_0 = 0.0;

const double x_dot_0 = 0.0;
const double y_dot_0 = 1.0;
const double z_dot_0 = 2.0;

//funciones
std::valarray<double> dot(double time, std::valarray<double> pos, std::valarray<double> vel);
std::valarray<double> double_dot(double time, std::valarray<double> pos, std::valarray<double> vel);

int main() {

  //inicializar campo magnético
  B[0] = B_x;
  B[1] = B_y;
  B[2] = B_z;

  //condición inicial
  r[0][0] = x_0;
  r[0][1] = y_0;
  r[0][2] = z_0;

  v[0][0] = x_dot_0;
  v[0][1] = y_dot_0;
  v[0][2] = z_dot_0;

  //Runge_Kutta
  for (int n = 1; n < N; n++) {

    std::valarray<double> k1_1 = h*dot(t[n-1],r[n-1],v[n-1]);
		std::valarray<double> k1_2 = h*double_dot(t[n-1],r[n-1],v[n-1]);

		std::valarray<double> k2_1 = h*dot(t[n-1] + h/2.0, r[n-1] + k1_1/2.0, v[n-1] + k1_2/2.0);
		std::valarray<double> k2_2 = h*double_dot(t[n-1] + h/2.0, r[n-1] + k1_1/2.0, v[n-1] + k1_2/2.0);

		std::valarray<double> k3_1 = h*dot(t[n-1] + h/2.0, r[n-1] + k2_1/2.0, v[n-1] + k2_2/2.0);
		std::valarray<double> k3_2 = h*double_dot(t[n-1] + h/2.0, r[n-1] + k2_1/2.0, v[n-1] + k2_2/2.0);

		std::valarray<double> k4_1 = h*dot(t[n-1] + h, r[n-1] + k3_1/2.0, v[n-1] + k3_2/2.0);
		std::valarray<double> k4_2 = h*double_dot(t[n-1] + h, r[n-1] + k3_1/2.0, v[n-1] + k3_2/2.0);

    //actualización de datos
		t[n] = t[n-1] + h;
		r[n] = r[n-1] + 1.0/6.0*(k1_1 + 2.0*k2_1 + 2.0*k3_1 + k4_1);
		v[n] = v[n-1] + 1.0/6.0*(k1_2 + 2.0*k2_2 + 2.0*k3_2 + k4_2);

  }
  //impresión de datos
  std::ofstream fileODE("data_ODE.dat");
  for (int n = 0; n < N; n++) {
    fileODE << t[n] << " " << r[n][0] << " " << r[n][1] << " " << r[n][2] << "\n";
  }
  fileODE.close();

  return 0;

}

std::valarray<double> dot(double t, std::valarray<double> pos, std::valarray<double> vel){
    return vel;
}

std::valarray<double> double_dot(double t, std::valarray<double> pos, std::valarray<double> vel){
    std::valarray<double> ans(3);
    ans[0] = q/m*(vel[1]*B[2] - vel[2]*B[1]);
    ans[1] = q/m*(vel[2]*B[0] - vel[0]*B[2]);
    ans[2] = q/m*(vel[0]*B[1] - vel[1]*B[0]);
    return ans;
}
