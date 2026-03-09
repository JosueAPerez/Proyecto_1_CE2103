#include <iostream>
#include <cstdint>
#include <random>
#include <ctime>
#include <cstdio>
#include <string>
using namespace std;
void throw_error_message();
int main(int argc, char *argv[])
{
    // tamanio de archivo
    string arc_size;
    // donde es que termina el archivo creado
    string output_path;
    // cantidad de enteros a escribir
    long long int_amount;
    // tamanio del buffer
    const int Buffer_size = 1024 * 1024;
    int32_t* buffer= new int32_t[Buffer_size];
    if (argc < 5)
    {
        throw_error_message();
        return 1;
    }
    for (int i = 1; i < argc; i++)
    {
        if (string(argv[i]) == "-size")
        {
            arc_size = argv[i + 1];
            i++;
        }
        else if (string(argv[i]) == "-output")
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
        int_amount = (512LL * 1024 * 1024) / 4;
    }
    else if (arc_size == "MEDIUM")
    {
        int_amount = (1LL * 1024 * 1024 * 1024) / 4;
    }
    else if (arc_size == "LARGE")
    {
        int_amount = (2LL * 1024 * 1024 * 1024) / 4;
    }
    else if (arc_size=="TEST"){
        int_amount = 100;
    }
    FILE *archivo = fopen(output_path.c_str(), "wb");
    if (archivo == nullptr)
    {
        cerr << "Error: no se pudo crear el archivo" << output_path << endl;
        return 1;
    }
    mt19937 rng(time(nullptr));
    uniform_int_distribution<int32_t> dist(INT32_MIN, INT32_MAX);
    int j = 0;
    for (long long i = 0; i < int_amount; i++)
    {
        buffer[j] = dist(rng);
      j++;
        if(j==Buffer_size){
            size_t escritos = fwrite(buffer,sizeof(int32_t),Buffer_size,archivo);
            j=0;
            if (escritos!=Buffer_size){
                cerr<< "Error: no se pudieron escribir todos los datos"<<endl;
                return 1;
            }
        }
    }
    fclose(archivo);
    delete[] buffer;
    return 0;
}
// mensaje de error en caso de algun problema
void throw_error_message() { cerr << "Se debe utilizar: ./generator -size <SIZE> -output <OUTPUT FILE PATH>" << '\n'; }