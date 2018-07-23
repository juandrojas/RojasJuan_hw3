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

  //inicializamos vectores que contienen estados temporales de la membrana
  std::vector<std::vector<double> > u_present(ny, std::vector<double>(nx));
  std::vector<std::vector<double> > u_past(ny, std::vector<double>(nx));
  std::vector<std::vector<double> > u_future(ny, std::vector<double>(nx));
  std::vector<std::vector<double> > u_initial;

  //inicializar la condición inicial del archivo init.dat
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
  u_initial.resize(ny, vector<double>(nx));

  //Caso 1 - sostenido en la frontera

  //inicializar condición inicial
  for (int i = 0; i < ny; i++){
    for (int j = 0; j < nx; j++) {
      u_present[i][j] = u_initial[i][j];
    }
  }

  // primer paso

  // condición de frontera
  for (int i = 0; i < ny; i++) {
    u_future[i][0] = 0.0;
    u_future[i][ny-1] = 0.0;
  }
  for (int j = 0; j < nx; j++) {
    u_future[0][j] = 0.0;
    u_future[nx-1][j] = 0.0;
  }

  //asumiendo velocidad inicial cero
  for (int i = 1; i < ny-1; i++) {
    for (int j = 1; j < nx-1; j++) {
      u_future[i][j] = 0.5*(pow(delta_t*c,2)*((u_present[i+1][j] - 2*u_present[i][j] + u_present[i-1][j])/pow(delta_y,2) + (u_present[i][j+1] - 2*u_present[i][j] + u_present[i][j-1])/pow(delta_x,2)) + 2*u_present[i][j]);
    }
  }

  //actualizamos
  for (int i = 0; i < ny; i++){
    for (int j = 0; j < nx; j++) {
      double temp = u_present[i][j];
      u_past[i][j] = temp;
      u_present[i][j] = u_future[i][j];
    }
  }
  int aux_sum = 1;
  std::ofstream trans_cut_fixed("data_PDE_fixed_cut.dat");

  for (int i = 0; i < ny; i++) {
    trans_cut_fixed << u_past[i][50] << " ";
  }
  trans_cut_fixed << "\n";

  // el resto de tiempos
  for (int k = 1; k < nt; k++) {

    // condición de frontera
    for (int i = 0; i < ny; i++) {
      u_future[i][0] = 0.0;
      u_future[i][nx-1] = 0.0;
    }
    for (int j = 0; j < nx; j++) {
      u_future[0][j] = 0.0;
      u_future[ny-1][j] = 0.0;
    }

    //actual PDE
    for (int i = 1; i < ny-1; i++){
      for (int j = 1; j < nx-1; j++) {
        u_future[i][j] =  pow(delta_t*c,2)*((u_present[i+1][j] - 2*u_present[i][j] + u_present[i-1][j])/pow(delta_y,2) + (u_present[i][j+1] - 2*u_present[i][j] + u_present[i][j-1])/pow(delta_x,2)) + 2*u_present[i][j] - u_past[i][j];
      }
    }
    //actualizamos
    for (int i = 0; i < ny; i++){
      for (int j = 0; j < nx; j++) {
        u_past[i][j] = u_present[i][j];
        u_present[i][j] = u_future[i][j];
      }
    }

    //cortes transversales
    if (aux_sum < 900) {
      aux_sum++;
    }
    else
    {
      for (int i = 0; i < ny; i++) {
        trans_cut_fixed << u_present[i][50] << " ";
      }
      trans_cut_fixed << "\n";
      aux_sum = 1;
    }
  }
  trans_cut_fixed.close();

  //guardamos los datos
  std::ofstream file_fixed("data_PDE_fixed.dat");
  for (int i = 0; i < ny; i++) {
    for (int j = 0; j < nx; j++) {
      file_fixed << u_future[i][j] << " ";
    }
    file_fixed << "\n";
  }
  file_fixed.close();

  //Caso 2: frontera libre
  //inciializar condición inicial
  for (int i = 0; i < ny; i++){
    for (int j = 0; j < nx; j++) {
      u_present[i][j] = u_initial[i][j];
    }
  }

  //paso k=0; asumimos velocidad inicial igual a cero
  //condición de frontera

  //esquinas
  u_future[0][0] = 0.5*(pow(delta_t*c,2)*((2*u_present[1][0] - 2*u_present[0][0])/pow(delta_y,2) + (2*u_present[0][1] - 2*u_present[0][0])/pow(delta_x,2)) + 2*u_present[0][0]);

  u_future[ny-1][0] = 0.5*(pow(delta_t*c,2)*((2*u_present[ny-2][0] - 2*u_present[ny-1][0])/pow(delta_y,2) + (2*u_present[ny-1][1] - 2*u_present[ny-1][0])/pow(delta_x,2)) + 2*u_present[ny-1][0]);

  u_future[ny-1][nx-1] = 0.5*(pow(delta_t*c,2)*((u_present[ny-2][nx-1] - 2*u_present[ny-1][nx-1])/pow(delta_y,2) + (u_present[ny-1][nx-2] - 2*u_present[ny-1][nx-1])/pow(delta_x,2)) + 2*u_present[ny-1][nx-1]);

  u_future[0][nx-1] = 0.5*(pow(delta_t*c,2)*((u_present[1][nx-1] - 2*u_present[0][nx-1])/pow(delta_y,2) + (u_present[0][nx-2] - 2*u_present[0][nx-1])/pow(delta_x,2)) + 2*u_present[0][nx-1]);

  //resto de bordes
  for (int i = 1; i < ny-1; i++) {
    u_future[i][0] = 0.5*(pow(delta_t*c,2)*((u_present[i+1][0] - 2*u_present[i][0] + u_present[i-1][0])/pow(delta_y,2) + (2*u_present[i][1] - 2*u_present[i][0])/pow(delta_x,2)) + 2*u_present[i][0]);

    u_future[i][nx-1] = 0.5*(pow(delta_t*c,2)*((u_present[i+1][nx-1] - 2*u_present[i][nx-1] + u_present[i-1][nx-1])/pow(delta_y,2) + (2*u_present[i][nx-2] - 2*u_present[i][nx-1])/pow(delta_x,2)) + 2*u_present[i][nx-1]);
  }
  for (int j = 0; j < ny-1; j++) {
    u_future[0][j] = 0.5*(pow(delta_t*c,2)*((2*u_present[1][j] - 2*u_present[0][j])/pow(delta_y,2) + (u_present[0][j+1] - 2*u_present[0][j] + u_present[0][j-1])/pow(delta_x,2)) + 2*u_present[0][j]);

    u_future[ny-1][j] = 0.5*(pow(delta_t*c,2)*((2*u_present[ny-2][j] - 2*u_present[ny-1][j])/pow(delta_y,2) + (u_present[ny-1][j+1] - 2*u_present[ny-1][j] + u_present[ny-1][j-1])/pow(delta_x,2)) + 2*u_present[ny-1][j]);
  }



  //el resto de la membrana
    for (int i = 1; i < ny-1; i++) {
      for (int j = 1; j < nx-1; j++) {
        u_future[i][j] = 0.5*(pow(delta_t*c,2)*((u_present[i+1][j] - 2*u_present[i][j] + u_present[i-1][j])/pow(delta_y,2) + (u_present[i][j+1] - 2*u_present[i][j] + u_present[i][j-1])/pow(delta_x,2)) + 2*u_present[i][j]);
      }
    }

    //cortes transversales
    aux_sum = 1;
    std::ofstream trans_cut_free("data_PDE_free_cut.dat");

    for (int i = 0; i < ny; i++) {
      trans_cut_free<< u_past[i][50] << " ";
    }
    trans_cut_free << "\n";

    //actualizamos
    for (int i = 0; i < ny; i++){
      for (int j = 0; j < nx; j++) {
        double temp = u_present[i][j];
        u_past[i][j] = temp;
        u_present[i][j] = u_future[i][j];
      }
    }

    //el resto de tiempos
    for (int k = 1; k < nt; k++) {

      //condición de frontera
      //esquinas
      u_future[0][0] = 0.5*(pow(delta_t*c,2)*((2*u_present[1][0] - 2*u_present[0][0])/pow(delta_y,2) + (2*u_present[0][1] - 2*u_present[0][0])/pow(delta_x,2)) + 2*u_present[0][0]);

      u_future[ny-1][0] = 0.5*(pow(delta_t*c,2)*((2*u_present[ny-2][0] - 2*u_present[ny-1][0])/pow(delta_y,2) + (2*u_present[ny-1][1] - 2*u_present[ny-1][0])/pow(delta_x,2)) + 2*u_present[ny-1][0]);

      u_future[ny-1][nx-1] = 0.5*(pow(delta_t*c,2)*((u_present[ny-2][nx-1] - 2*u_present[ny-1][nx-1])/pow(delta_y,2) + (u_present[ny-1][nx-2] - 2*u_present[ny-1][nx-1])/pow(delta_x,2)) + 2*u_present[ny-1][nx-1]);

      u_future[0][nx-1] = 0.5*(pow(delta_t*c,2)*((u_present[1][nx-1] - 2*u_present[0][nx-1])/pow(delta_y,2) + (u_present[0][nx-2] - 2*u_present[0][nx-1])/pow(delta_x,2)) + 2*u_present[0][nx-1]);

      //resto de bordes
      for (int i = 1; i < ny-1; i++) {
        u_future[i][0] = 0.5*(pow(delta_t*c,2)*((u_present[i+1][0] - 2*u_present[i][0] + u_present[i-1][0])/pow(delta_y,2) + (2*u_present[i][1] - 2*u_present[i][0])/pow(delta_x,2)) + 2*u_present[i][0]);

        u_future[i][nx-1] = 0.5*(pow(delta_t*c,2)*((u_present[i+1][nx-1] - 2*u_present[i][nx-1] + u_present[i-1][nx-1])/pow(delta_y,2) + (2*u_present[i][nx-2] - 2*u_present[i][nx-1])/pow(delta_x,2)) + 2*u_present[i][nx-1]);
      }
      for (int j = 0; j < ny-1; j++) {
        u_future[0][j] = 0.5*(pow(delta_t*c,2)*((2*u_present[1][j] - 2*u_present[0][j])/pow(delta_y,2) + (u_present[0][j+1] - 2*u_present[0][j] + u_present[0][j-1])/pow(delta_x,2)) + 2*u_present[0][j]);

        u_future[ny-1][j] = 0.5*(pow(delta_t*c,2)*((2*u_present[ny-2][j] - 2*u_present[ny-1][j])/pow(delta_y,2) + (u_present[ny-1][j+1] - 2*u_present[ny-1][j] + u_present[ny-1][j-1])/pow(delta_x,2)) + 2*u_present[ny-1][j]);
      }



      //el resto de la membrana
      for (int i = 1; i < ny-1; i++){
        for (int j = 1; j < nx-1; j++) {
          u_future[i][j] =  pow(delta_t*c,2)*((u_present[i+1][j] - 2*u_present[i][j] + u_present[i-1][j])/pow(delta_y,2) + (u_present[i][j+1] - 2*u_present[i][j] + u_present[i][j-1])/pow(delta_x,2)) + 2*u_present[i][j] - u_past[i][j];
        }
      }

      //actualizamos
      for (int i = 0; i < ny; i++){
        for (int j = 0; j < nx; j++) {
          u_past[i][j] = u_present[i][j];
          u_present[i][j] = u_future[i][j];
        }
      }

      //cortes transversales
      if (aux_sum < 900) {
        aux_sum++;
      }
      else
      {
        for (int i = 0; i < ny; i++) {
          trans_cut_free << u_present[i][50] << " ";
        }
        trans_cut_free << "\n";
        aux_sum = 1;
      }
    }
    trans_cut_free.close();

    //guardamos los datos
    std::ofstream file_free("data_PDE_free.dat");
    for (int i = 0; i < ny; i++) {
      for (int j = 0; j < nx; j++) {
        file_free << u_future[i][j] << " ";
      }
      file_free << "\n";
    }
    file_free.close();

  return 0;
}
