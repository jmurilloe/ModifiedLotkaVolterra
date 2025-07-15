#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <iomanip>

const int N_FILES = 15;
const std::string FILE_PREFIX = "metrics";
const std::string FILE_SUFFIX = ".txt";

// Función para dividir línea en partes
std::vector<std::string> split(const std::string& line, char sep = '\t') {
    std::vector<std::string> tokens;
    std::stringstream ss(line);
    std::string item;
    while (std::getline(ss, item, sep)) {
        tokens.push_back(item);
    }
    return tokens;
}

// Promedio
double mean(const std::vector<double>& v) {
    double sum = 0;
    for (double x : v) sum += x;
    return sum / v.size();
}

// Desviación estándar
double stddev(const std::vector<double>& v, double m) {
    double sum_sq = 0;
    for (double x : v) sum_sq += (x - m) * (x - m);
    return std::sqrt(sum_sq / v.size());
}

int main() {
    std::vector<std::ifstream> files(N_FILES);
    for (int i = 0; i < N_FILES; ++i) {
        std::string filename = FILE_PREFIX + std::to_string(i + 1) + FILE_SUFFIX;
        files[i].open(filename);
        if (!files[i].is_open()) {
            std::cerr << "❌ Error abriendo archivo: " << filename << std::endl;
            return 1;
        }
        std::string header;
        std::getline(files[i], header); // ignorar encabezado
    }

    std::ofstream output("metrics_summary.txt");
    output << "t\tH2_avg\tmod_avg\tnest_avg\tconn_avg\tFmean_avg\tH2_sd\tmod_sd\tnest_sd\tconn_sd\tFmean_sd\n";

    std::string line;
    while (std::getline(files[0], line)) {
        std::vector<std::string> tokens = split(line);
        int t = std::stoi(tokens[0]);

        std::vector<double> H2s, mods, nests, conns, Fmeans;

        // Agregar valores del primer archivo
        if (tokens[3] == "NA") continue; // si 'nest' es NA, saltar
        H2s.push_back(std::stod(tokens[1]));
        mods.push_back(std::stod(tokens[2]));
        nests.push_back(std::stod(tokens[3]));
        conns.push_back(std::stod(tokens[4]));
        Fmeans.push_back(std::stod(tokens[5]));

        bool skip_line = false;

        // Leer las mismas líneas de los otros archivos
        for (int i = 1; i < N_FILES; ++i) {
            if (!std::getline(files[i], line)) {
                skip_line = true;
                break;
            }
            tokens = split(line);
            if (tokens[3] == "NA") {
                skip_line = true;
                break;
            }
            H2s.push_back(std::stod(tokens[1]));
            mods.push_back(std::stod(tokens[2]));
            nests.push_back(std::stod(tokens[3]));
            conns.push_back(std::stod(tokens[4]));
            Fmeans.push_back(std::stod(tokens[5]));
        }

        if (skip_line) continue;

        // Calcular estadísticas
        double H2_mean = mean(H2s);
        double mod_mean = mean(mods);
        double nest_mean = mean(nests);
        double conn_mean = mean(conns);
        double Fmean_mean = mean(Fmeans);

        double H2_sd = stddev(H2s, H2_mean);
        double mod_sd = stddev(mods, mod_mean);
        double nest_sd = stddev(nests, nest_mean);
        double conn_sd = stddev(conns, conn_mean);
        double Fmean_sd = stddev(Fmeans, Fmean_mean);

        // Escribir resultado
        output << std::fixed << std::setprecision(6)
               << t << "\t" << H2_mean << "\t" << mod_mean << "\t" << nest_mean << "\t" << conn_mean << "\t" << Fmean_mean << "\t"
               << H2_sd << "\t" << mod_sd << "\t" << nest_sd << "\t" << conn_sd << "\t" << Fmean_sd << "\n";
    }

    for (auto& f : files) f.close();
    output.close();

    std::cout << "✅ Archivo 'metrics_summary.txt' creado con éxito.\n";
    return 0;
}
