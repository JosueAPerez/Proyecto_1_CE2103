#include <iostream>
#include <cstdint>
#include <random>
#include <ctime>
#include <cstdio>
#include <string>
#include <chrono>
#include "PagedArray.h"
#include "algorithms/bubble_sort.h"
#include "algorithms/selection_sort.h"
#include "algorithms/insertion_sort.h"
#include "algorithms/quick_sort.h"
#include "algorithms/merge_sort.h"

using namespace std;
void throw_error_message();
bool copyFile(const string &inputPath, const string &outputPath);
void generateTextFile(const string &outputBinPath, const string &outputTxtPath, long long totalInts);
int main(int argc, char *argv[])
{
    string inputPath;
    string outputPath;
    string sortingAlgorithm;
    int pageSize;
    int pageCount;
    if (argc < 11)
    {
        throw_error_message();
        return 1;
    }
    for (int i = 1; i < argc; i++)
    {
        if (string(argv[i]) == "-input")
        {
            inputPath = argv[i + 1];
            i++;
        }
        else if (string(argv[i]) == "-output")
        {
            outputPath = argv[i + 1];
            i++;
        }
        else if (string(argv[i]) == "-alg")
        {
            sortingAlgorithm = argv[i + 1];
            i++;
        }
        else if (string(argv[i]) == "-pageSize")
        {
            pageSize = stoi(argv[i + 1]);
            i++;
        }
        else if (string(argv[i]) == "-pageCount")
        {
            pageCount = stoi(argv[i + 1]);
            i++;
        }
        else
        {
            throw_error_message();
            return 1;
        }
    }
    string outputBinPath = outputPath + ".bin";
    string outputTxtPath = outputPath + ".txt";
    FILE *test = fopen(inputPath.c_str(), "rb");
    if (test == nullptr)
    {
        cerr << "Error: el archivo " << inputPath << " no existe" << std::endl;
        return 1;
    }
    fclose(test);
    FILE *f = fopen(inputPath.c_str(), "rb");
    fseek(f, 0, SEEK_END);
    long long totalInts = ftell(f) / sizeof(int32_t);
    fclose(f);
    if (!copyFile(inputPath, outputBinPath))
    {
        return 1;
    }
    if ((sortingAlgorithm == "bubble" || sortingAlgorithm == "selection" || sortingAlgorithm == "insertion") && totalInts > 33554432)
    {
        cerr << "Error: ese algoritmo no es viable para archivos MEDIUM o LARGE" << endl;
        return 1;
    }
    PagedArray *arr = new PagedArray(outputBinPath.c_str(), pageSize, pageCount, totalInts);
    auto start = chrono::high_resolution_clock::now();
    if (sortingAlgorithm == "bubble")
    {
        bubbleSort(*arr, totalInts);
    }
    else if (sortingAlgorithm == "selection")
    {
        selectionSort(*arr, totalInts);
    }
    else if (sortingAlgorithm == "insertion")
    {
        insertionSort(*arr, totalInts);
    }
    else if (sortingAlgorithm == "quick")
    {
        quickSort(*arr, 0, totalInts - 1);
    }
    else if (sortingAlgorithm == "merge")
    {
        mergeSort(*arr, 0, totalInts - 1);
    }
    else
    {
        cerr << "Error: algoritmo no reconocido" << endl;
        return 1;
    }
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    generateTextFile(outputBinPath, outputTxtPath, totalInts);
    cout << "Tiempo: " << elapsed.count() << " segundos" << endl;
    cout << "Algoritmo: " << sortingAlgorithm << endl;
    cout << "Page faults: " << arr->getPageFaults() << endl;
    cout << "Page hits: " << arr->getPageHits() << endl;
    delete arr;
    return 0;
}
void throw_error_message()
{
    cerr << "Se debe utilizar: -input /ruta/entrada.bin -output /ruta/salida -alg quicksort -pageSize 1024 -pageCount 4" << endl;
}
bool copyFile(const string &inputPath, const string &outputPath)
{
    FILE *inputFile = fopen(inputPath.c_str(), "rb");
    if (inputFile == nullptr)
    {
        cerr << "Error: no se pudo abrir el archivo de entrada" << endl;
        return false;
    }
    FILE *outputFile = fopen(outputPath.c_str(), "wb");
    if (outputFile == nullptr)
    {
        cerr << "Error: no se pudo crear el archivo de salida" << endl;
        fclose(inputFile);
        return false;
    }
    const int Buffer_size = 1024 * 1024;
    int32_t *buffer = new int32_t[Buffer_size];
    size_t leidos;
    while ((leidos = fread(buffer, sizeof(int32_t), Buffer_size, inputFile)) > 0)
    {
        fwrite(buffer, sizeof(int32_t), leidos, outputFile);
    }
    delete[] buffer;
    fclose(inputFile);
    fclose(outputFile);
    return true;
}
void generateTextFile(const string &outputBinPath, const string &outputTxtPath, long long totalInts)
{
    FILE *outputBinFile = fopen(outputBinPath.c_str(), "rb");
    if (outputBinFile == nullptr)
    {
        cerr << "Error: no se pudo crear el archivo de texto" << endl;
        return;
    }
    FILE *outputTxtFile = fopen(outputTxtPath.c_str(), "w");
    if (outputTxtFile == nullptr)
    {
        cerr << "Error no se pudo crear el archivo de texto" << endl;
        fclose(outputBinFile);
        return;
    }
    const int Read_buffer = 1024 * 1024;
    int32_t *intBuffer = new int32_t[Read_buffer];
    const int Text_buffer = 32 * 1024 * 1024;
    char *textBuffer = new char[Text_buffer];
    int textPos = 0;
    long long processed = 0;
    while (processed < totalInts)
    {
        long long remaining = totalInts - processed;
        int toRead = (remaining < Read_buffer) ? remaining : Read_buffer;
        fread(intBuffer, sizeof(int32_t), toRead, outputBinFile);
        for (int i = 0; i < toRead; i++)
        {
            char numStr[16];
            int len = sprintf(numStr, "%d", intBuffer[i]);
            bool isLast = (processed + i == totalInts - 1);
            if (!isLast)
            {
                numStr[len] = ',';
                len++;
            }
            if (textPos + len >= Text_buffer)
            {
                fwrite(textBuffer, 1, textPos, outputTxtFile);
                textPos = 0;
            }
            for (int k = 0; k < len; k++)
            {
                textBuffer[textPos++] = numStr[k];
            }
        }
        processed += toRead;
    }
    if (textPos > 0)
    {
        fwrite(textBuffer, 1, textPos, outputTxtFile);
    }
    delete[] intBuffer;
    delete[] textBuffer;
    fclose(outputBinFile);
    fclose(outputTxtFile);
}