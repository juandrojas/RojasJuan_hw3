#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <math.h>

using namespace std;

//parámetros
const double L = 1.0;
const double delta_x = 0.01;
const double delta_y = delta_x;
const double c = 300.0;

//para garantizar la condición de estabilidad
const double delta_t = (delta_x/c)/10.0;

const double t_f = 0.06;
const int nx = int(L/delta_x);
const int ny = int(L/delta_y);
const int nt = int(t_f/delta_t);

int main() {

  std::vector<std::vector<double> > u_present(nx, std::vector<double>(ny));
  std::vector<std::vector<double> > u_past(nx, std::vector<double>(ny));
  std::vector<std::vector<double> > u_future(nx, std::vector<double>(ny));
  std::vector<std::vector<double> > u_initial;

  //inicializar la condición inicial
  std::fstream file("init.dat");
  std::string line;
  int i = 0;
  while (std::getline(file, line)) {

    stringstream ss(line);
    double value;
    u_initial.push_back(std::vector<double>());
    while (ss >> value) {
      u_initial[i].push_back(value);
    }
    i++;
  }
  file.close();
  //memoria para el vector inicial
  u_initial.resize(nx, vector<double>(ny));

  //Caso 1 - sostenido en la frontera
  //inicializar condición inicial
  for (int i = 0; i < nx; i++){
    for (int j = 0; j < ny; j++) {
      u_present[i][j] = u_initial[i][j];
    }
  }

  // primer paso

  // condición de frontera
  for (int i = 0; i < nx; i++) {
    u_future[i][0] = 0.0;
    u_future[i][ny-1] = 0.0;
  }
  for (int j = 0; j < ny; j++) {
    u_future[0][j] = 0.0;
    u_future[nx-1][j] = 0.0;
  }

  // asumiendo velocidad inicial cero
  for (int i = 1; i < nx-1; i++) {
    for (int j = 1; j < ny-1; j++) {
      u_future[i][j] = 0.5*(pow(delta_t*c,2)*((u_present[i+1][j] - 2*u_present[i][j] + u_present[i-1][j])/pow(delta_x,2) + (u_present[i][j+1] - 2*u_present[i][j] + u_present[i][j-1])/pow(delta_y,2)) + 2*u_present[i][j]);
    }
  }

  //actualizamos
  for (int i = 0; i < nx; i++){
    for (int j = 0; j < ny; j++) {
      double temp = u_present[i][j];
      u_past[i][j] = temp;
      u_present[i][j] = u_future[i][j];
    }
  }

  // el resto de tiempos
  for (int k = 1; k < nt; k++) {

    // condición de frontera
    for (int i = 0; i < nx; i++) {
      u_future[i][0] = 0.0;
      u_future[i][ny-1] = 0.0;
    }
    for (int j = 0; j < ny; j++) {
      u_future[0][j] = 0.0;
      u_future[nx-1][j] = 0.0;
    }

    //actual PDE
    for (int i = 1; i < nx-1; i++){
      for (int j = 1; j < ny-1; j++) {
        u_future[i][j] =  pow(delta_t*c,2)*((u_present[i+1][j] - 2*u_present[i][j] + u_present[i-1][j])/pow(delta_x,2) + (u_present[i][j+1] - 2*u_present[i][j] + u_present[i][j-1])/pow(delta_y,2)) + 2*u_present[i][j] - u_past[i][j];
      }
    }
    //actualizamos
    for (int i = 0; i < nx; i++){
      for (int j = 0; j < ny; j++) {
        u_past[i][j] = u_present[i][j];
        u_present[i][j] = u_future[i][j];
      }
    }
  }

  //guardamos los datos
  std::ofstream file_fixed("data_PDE_fixed.dat");
  for (int i = 0; i < nx; i++) {
    for (int j = 0; j < ny; j++) {
      file_fixed << u_future[i][j] << " ";
    }
    file_fixed << "\n";
  }
  file_fixed.close();
  return 0;
}
