#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <boost/math/distributions/beta.hpp>

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
void equations(const state &y, state &dydt);    //Función para escribir las ecuaciones de Lotka-Volterra
void RK4(state &y, double dt, double t_max);    //Implementación de Runge-Kutta de orden 4

std::vector<std::vector<double>> make_visitation_matrix(const int &N_i, const int &N_j, const std::vector<double> &FiF, const std::vector<double> &AjF, const std::vector<std::vector<double>> &betaF);
                                                //Función que crea la matriz de visitas para calcular specialization, nestedness y modularization

int main(int argc, char **argv) {

    N_i = std::atoi(argv[1]);
    N_j = std::atoi(argv[2]);

    //Condiciones iniciales
    int total_vars = N_i + N_j + N_i + N_i * N_j;
    state y(total_vars);

    parameters(N_i, N_j, y);

    std::cout << "# t\t";
for (int i = 1; i <= N_i; ++i) std::cout << "P" << i << "\t";
    for (int j = 1; j <= N_j; ++j) std::cout << "A" << j << "\t";
    for (int i = 1; i <= N_i; ++i) std::cout << "F" << i << "\t";
    for (int i = 1; i <= N_i; ++i)
        for (int j = 1; j <= N_j; ++j)
            std::cout << "beta" << i << j << "\t";
    std::cout << "\n";

    RK4(y, 0.015, 1500);  

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
    std::gamma_distribution<> gamma_2(2.0);
    std::gamma_distribution<> gamma_5(5.0);

    for (int i = 0; i < N_i; ++i) {
        ri[i] = dis01(gen);
        ci[i] = dis01(gen);
        wi[i] = dis01(gen);

        //beta(2.0, 5.0) para ai = gamma_2 / (gamma_2 + gamma_5)
        double p = gamma_2(gen);
        double q = gamma_5(gen);
        ai[i] = p / (p + q); //beta(2.0, 5.0) = [0.0, 1.0]

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

void equations(const state &y, state &dydt){
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
        for(int j = 0; j < N_j; j++){
            double beta = y[idx_beta(i, j)];
            double F = y[idx_F(i)];
            double sum = 0.0;
            
            for(int k = 0; k < N_i; k++){
                double beta_kj = y[idx_beta(k, j)];
                double F_k = y[idx_F(k)];
                sum += sigmaA[k][j] * beta_kj * F_k;
            }
            dydt[idx_beta(i, j)] = Gj[j] * beta * (sigmaA[i][j] * F - sum); //Ecuación del forrajeo
        }
    }
}

void RK4(state &y, double dt, double t_max){
    state k1, k2, k3, k4, y_temp(y.size());
    
    for(double t = 0.0; t < t_max; t += dt){
        std::cout << t; 
        
        for(double values : y){
            std::cout << "\t" << values;
        }
        std::cout << "\n";
        equations(y, k1);
        
        for(int i = 0; i < y.size(); i++){
            y_temp[i] = y[i] + 0.5 * dt * k1[i];
        }
        equations(y_temp, k2);
        
        for(int i = 0; i < y.size(); i++){
            y_temp[i] = y[i] + 0.5 * dt * k2[i];
        }
        equations(y_temp, k3);
        
        for(int i = 0; i < y.size(); i++){
            y_temp[i] = y[i] + dt * k3[i];
        }
        equations(y_temp, k4);
        
        for(int i = 0; i < y.size(); i++){
            y[i] += (dt/6.0) * (k1[i] + 2*k2[i] + 2*k3[i] + k4[i]); 
        }
    }
}

std::vector<std::vector<double>> make_visitation_matrix(const int &N_i, const int &N_j, const std::vector<double> &FiF, const std::vector<double> &AjF, const std::vector<std::vector<double>> &betaF){
    std::vector<std::vector<double>> c;
    c.resize(N_i, std::vector<double>(N_j));
    for (int i = 0; i < N_i; ++i) {
        for (int j = 0; j < N_j; ++j) {
            c[i][j] = betaF[i][j]*FiF[i]*AjF[j];
        } 
    }
    return c;
}