#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <boost/math/distributions/beta.hpp>
#include <fstream>

int N_i = 0;    //Número de especies de plantas
int N_j = 0;    //Número de especies animales

using state = std::vector<double>;  //Vector que guarda todas las variables (estado del sistema)

std::vector<double> ri, rj, ci, cj, ai, wi, Gj;     //Parámetros
std::vector<std::vector<double>> sigmaA, sigmaP; 

int idx_P(int i);   //Funciones que acceden al índice correspondiente de cada variable en el vector de estado
int idx_A(int j);
int idx_F(int i);
int idx_beta(int i, int j);

void parameters(int N_i, int N_j, state &y);    //Inicialización de parámetros
void equations(const state &y, state &dydt, const int &N_modified);    //Función para escribir las ecuaciones de Lotka-Volterra
void RK4(state &y, double dt, double t_max, const int &N_modified);    //Implementación de Runge-Kutta de orden 4
void save_features_to_file(const std::string &filename, const state &y);    //Guarda los promedios en features.txt

std::vector<std::vector<double>> make_visitation_matrix(const int &N_i, const int &N_j, state &y);
                                                //Función que crea la matriz de visitas para calcular specialization, nestedness y modularization
double calculate_H2(const int &N_i, const int &N_j, const std::vector<std::vector<double>> &V);
                                                //Función para calcular specialization
double calculate_total_interactions(const int &N_i, const int &N_j, const std::vector<std::vector<double>> &V);
                                                //Función para calcular numero total de interacciones de la matriz de visita
int print_matrix(std::vector<std::vector<int>> const & matrix, int const L);
                                                // Funcion que imprime la matriz V

int main(int argc, char **argv) {

    N_i = std::atoi(argv[1]);
    N_j = std::atoi(argv[2]);
    int N_modified = std::atoi(argv[3]);

    //Condiciones iniciales
    int total_vars = N_i + N_j + N_i + N_i * N_j;
    state y(total_vars);

    parameters(N_i, N_j, y);

    RK4(y, 0.01, 15000, N_modified);

    std::vector<std::vector<double>> V = make_visitation_matrix(N_i, N_j, y);

    for(int i = 0; i < N_i; i++){
        for(int j = 0; j < N_j; j++)
            if(j == N_j-1)
            std::cout << V[i][j];
            else std::cout << V[i][j] << " ";
        std::cout << "\n";
    }

    save_features_to_file("features.txt", y);

    return 0;
}

int idx_P(int i){ return i; }

int idx_A(int j){ return N_i+j; }

int idx_F(int i){ return N_i + N_j + i; }

int idx_beta(int i, int j){ return N_i + N_j + N_i + i*N_j + j; }

void parameters(int N_i, int N_j, state &y){
    std::random_device rd;
    std::mt19937 gen(rd());

    //Redimensionar vectores
    ri.resize(N_i); ci.resize(N_i); ai.resize(N_i); wi.resize(N_i);
    rj.resize(N_j); cj.resize(N_j); Gj.resize(N_j);
    sigmaP.resize(N_i, std::vector<double>(N_j));
    sigmaA.resize(N_i, std::vector<double>(N_j));

    //Distribuciones
    std::uniform_real_distribution<> dis01(0.0, 0.1);
    std::uniform_real_distribution<> dis1(0.0, 1.0);
    std::uniform_real_distribution<> disG(1.0, 2.0);
    boost::math::beta_distribution<> disbeta(2, 5);

    for (int i = 0; i < N_i; ++i) {
        ri[i] = dis01(gen);
        ci[i] = dis01(gen);
        wi[i] = dis01(gen);

        double randuniform = dis1(gen);
        ai[i] = quantile(disbeta, randuniform);

        y[idx_P(i)] = dis01(gen);  
        y[idx_F(i)] = dis1(gen);    

        for (int j = 0; j < N_j; ++j) {
            sigmaP[i][j] = dis01(gen);
            sigmaA[i][j] = dis01(gen);
            y[idx_beta(i, j)] = 1.0 / N_i;  //Forrajeo uniforme para todos los animales
        }
    }

    for (int j = 0; j < N_j; ++j) {
        rj[j] = dis01(gen);
        cj[j] = dis01(gen);
        Gj[j] = disG(gen);  
        y[idx_A(j)] = dis01(gen); 
    }
}

void equations(const state &y, state &dydt, const int &N_modified){
    dydt.assign(y.size(), 0.0);
    
    for(int i = 0; i < N_i; i++){
        double P = y[idx_P(i)];
        double sum = 0.0; 
        
        for(int j = 0; j < N_j; j++){
            double F = y[idx_F(i)];
            double A = y[idx_A(j)];
            double beta = y[idx_beta(i, j)];
            sum += sigmaP[i][j] * beta * F * A; 
        }
        dydt[idx_P(i)] = ri[i] * P - ci[i] * P * P + sum;   //Ecuación de plantas
    }
    
    for(int j = 0; j < N_j; j++){
        double A = y[idx_A(j)];
        double sum = 0.0;
        
        for(int i = 0; i < N_i; i++){
            double F = y[idx_F(i)];
            double beta = y[idx_beta(i, j)];
            sum += sigmaA[i][j] * beta * F * A;
        }
        dydt[idx_A(j)] = rj[j] * A - cj[j] * A * A + sum;   //Ecuación de animales
    }
    
    for(int i = 0; i < N_i; i++){
        double P = y[idx_P(i)];
        double F = y[idx_F(i)];
        double sum = 0.0;
        
        for(int j = 0; j < N_j; j++){
            double A = y[idx_A(j)];
            double beta = y[idx_beta(i, j)];
            sum += beta * F * A;
        }
        dydt[idx_F(i)] = ai[i] * P - wi[i] * F - sum;   //Ecuación de recursos florales
    }
    
    for(int i = 0; i < N_i; i++){
        for(int j = N_modified; j < N_j; j++){
            double beta = y[idx_beta(i, j)];
            double F = y[idx_F(i)];
            double sum = 0.0;
            
            for(int k = 0; k < N_i; k++){
                double beta_kj = y[idx_beta(k, j)];
                double F_k = y[idx_F(k)];
                sum += sigmaA[k][j] * beta_kj * F_k;
            }
            dydt[idx_beta(i, j)] = Gj[j] * beta * (sigmaA[i][j] * F - sum); //Ecuación del forrajeo normal
        }
    }
    for(int i = 0; i < N_i; i++){
        for(int j = 0; j < N_modified; j++){
            double beta = y[idx_beta(i, j)];
            double F = y[idx_F(i)];
            double sum = 0.0;
            
            for(int k = 0; k < N_i; k++){
                double beta_kj = y[idx_beta(k, j)];
                double F_k = y[idx_F(k)];
                sum += sigmaA[k][j] * beta_kj * F_k;
            }
            dydt[idx_beta(i, j)] = Gj[j] * beta * (sum - sigmaA[i][j] * F); //Ecuación del forrajeo modificado
        }
    }
}

void RK4(state &y, double dt, double t_max, const int &N_modified){
    state k1, k2, k3, k4, y_temp(y.size());
    
    for(double t = 0.0; t < t_max; t += dt){

        equations(y, k1, N_modified);
        
        for(int i = 0; i < y.size(); i++){
            y_temp[i] = y[i] + 0.5 * dt * k1[i];
        }
        equations(y_temp, k2, N_modified);
        
        for(int i = 0; i < y.size(); i++){
            y_temp[i] = y[i] + 0.5 * dt * k2[i];
        }
        equations(y_temp, k3, N_modified);
        
        for(int i = 0; i < y.size(); i++){
            y_temp[i] = y[i] + dt * k3[i];
        }
        equations(y_temp, k4, N_modified);
        
        for(int i = 0; i < y.size(); i++){
            y[i] += (dt/6.0) * (k1[i] + 2*k2[i] + 2*k3[i] + k4[i]); 
        }
    }
}

std::vector<std::vector<double>> make_visitation_matrix(const int &N_i, const int &N_j, state &y){
    std::vector<std::vector<double>> c;
    c.resize(N_i, std::vector<double>(N_j));
    for (int i = 0; i < N_i; ++i) {
        for (int j = 0; j < N_j; ++j) {
            c[i][j] = y[idx_beta(i, j)] * y[idx_F(i)] * y[idx_A(j)];
        } 
    }
    return c;
}

double calculate_H2(const int &N_i, const int &N_j, const std::vector<std::vector<double>> &V){
    double m = calculate_total_interactions(N_i, N_j, V);
    std::vector<std::vector<double>> p;
    p.resize(N_i, std::vector<double>(N_j));
    for (int i = 0; i < N_i; ++i) {
        for (int j = 0; j < N_j; ++j) {
            p[i][j] = V[i][j]/m;
        } 
    }
    double H2 = 0.0;
    for (int i = 0; i < N_i; ++i) {
        for (int j = 0; j < N_j; ++j) {
            H2 -= p[i][j]*std::log(p[i][j]);
        }
    }
    return H2;
}

double calculate_total_interactions(const int &N_i, const int &N_j, const std::vector<std::vector<double>> &V){
    double m = 0.0;
    for (int i = 0; i < N_i; ++i) {
        for (int j = 0; j < N_j; ++j) {
            m += V[i][j]; 
        } 
    }
    return m;
}

int print_matrix(const int &N_i, const int &N_j,std::vector<std::vector<int>> const &matrix){
    for(int i = 0; i < N_i; ++i) {
        for(int j = 0; j < N_j; ++j) {
            std::cout << matrix[i][j]   << "\t";
        }
        std::cout << std::endl;
    }
    return 1;
}

void save_features_to_file(const std::string &filename, const state &y) {
    double A_sum = 0.0;
    double F_sum = 0.0;

    for (int j = 0; j < N_j; ++j)
        A_sum += y[idx_A(j)];
    for (int i = 0; i < N_i; ++i)
        F_sum += y[idx_F(i)];

    double A_mean = A_sum / N_j;
    double F_mean = F_sum / N_i;

    std::ofstream fout(filename);
    fout << A_mean << " " << F_mean << "\n";
    fout.close();
}
