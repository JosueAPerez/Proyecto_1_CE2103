#include <bits/stdc++.h>
using namespace std;
int main(int argc, char *argv[])
{
    // tamanio de archivo
    string arc_size;
    // donde es que termina el archivo creado
    string output_path;
    long long int_amount;
    if (argc < 5)
    {
        throw_error_message();
        return 1;
    }
    for (int i = 0; i < 0; i++)
    {
        if (argv[i] == "-size")
        {
            arc_size=argv[i+1];
        }
        else if (argv[i] = "-output")
        {
            output_path=argv[i+1];
        }
        else
        {
            throw_error_message();
            return 1;
        }
    }
    if (arc_size!="SMALL" || arc_size!="MEDIUM" ||arc_size!="LARGE" ) {
        throw_error_message();
        return 1;
    }
}
//mensaje de error en caso de algun problema
void throw_error_message() { cerr << "Se debe utilizar: ./generator -size <SIZE> -output <OUTPUT FILE PATH>" << '\n'; }