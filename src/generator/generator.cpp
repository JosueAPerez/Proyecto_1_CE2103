#include <iostream>
#include <cstdint>
#include <random>
#include <ctime>
#include <cstdio>
#include <string>
void throw_error_message();
int main(int argc, char *argv[])
{
    // tamanio de archivo
    std::string arc_size;
    // donde es que termina el archivo creado
    std::string output_path;
    // cantidad de enteros a escribir
    long long int_amount;
    // tamanio del buffer
    const int Buffer_size = 1024 * 1024;
    int32_t *buffer = new int32_t[Buffer_size];
    if (argc < 5)
    {
        throw_error_message();
        return 1;
    }
    for (int i = 1; i < argc; i++)
    {
        if (std::string(argv[i]) == "-size")
        {
            arc_size = argv[i + 1];
            i++;
        }
        else if (std::string(argv[i]) == "-output")
        {
            output_path = argv[i + 1];
            i++;
        }
        else
        {
            throw_error_message();
            return 1;
        }
    }
    if (arc_size != "SMALL" && arc_size != "MEDIUM" && arc_size != "LARGE" && arc_size != "TEST")
    {
        throw_error_message();
        return 1;
    }
    if (arc_size == "SMALL")
    {
        int_amount = (128LL * 1024 * 1024) / 4;
    }
    else if (arc_size == "MEDIUM")
    {
        int_amount = (256LL * 1024 * 1024) / 4;
    }
    else if (arc_size == "LARGE")
    {
        int_amount = (512LL * 1024 * 1024) / 4;
    }
    else if (arc_size == "TEST")
    {
        int_amount = (512LL * 1024) / 4;
    }
    FILE *archivo = fopen(output_path.c_str(), "wb");
    if (archivo == nullptr)
    {
        std::cerr << "Error: no se pudo crear el archivo" << output_path << std::endl;
        return 1;
    }
    std::mt19937 rng(time(nullptr));
    std::uniform_int_distribution<int32_t> dist(INT32_MIN, INT32_MAX);
    int j = 0;
    for (long long i = 0; i < int_amount; i++)
    {
        buffer[j] = dist(rng);
        j++;
        if (j == Buffer_size)
        {
            size_t escritos = fwrite(buffer, sizeof(int32_t), Buffer_size, archivo);
            j = 0;
            if (escritos != Buffer_size)
            {
                std::cerr << "Error: no se pudieron escribir todos los datos" << std::endl;
                return 1;
            }
        }
    }
    if (j > 0)
    {
        size_t escritos = fwrite(buffer, sizeof(int32_t), j, archivo);
        if (escritos != (size_t)j)
        {
            std::cerr << "Error: no se pudieron escribir todos los datos" << std::endl;
            return 1;
        }
    }
    fclose(archivo);
    delete[] buffer;
    return 0;
}
// mensaje de error en caso de algun problema
void throw_error_message() { std::cerr << "Se debe utilizar: ./generator -size <SIZE> -output <OUTPUT FILE PATH>" << '\n'; }