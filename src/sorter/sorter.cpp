#include <iostream>
#include <cstdint>
#include <random>
#include <ctime>
#include <cstdio>
#include <string>
#include <chrono>
#include "PagedArray.h"
#include "algorithms/insertion_sort.h"
#include "algorithms/quick_sort_m3.h"
#include "algorithms/bubble_sort.h"
#include "algorithms/quick_sort.h"
#include "algorithms/merge_sort.h"


void throw_error_message();
bool copyFile(const std::string &inputPath, const std::string &outputPath);
void generateTextFile(const std::string &outputBinPath, const std::string &outputTxtPath, long long totalInts);
int main(int argc, char *argv[])
{
    std::string inputPath;
    std::string outputPath;
    std::string sortingAlgorithm;
    int pageSize;
    int pageCount;
    if (argc < 11)
    {
        throw_error_message();
        return 1;
    }
    for (int i = 1; i < argc; i++)
    {
        if (std::string(argv[i]) == "-input")
        {
            inputPath = argv[i + 1];
            i++;
        }
        else if (std::string(argv[i]) == "-output")
        {
            outputPath = argv[i + 1];
            i++;
        }
        else if (std::string(argv[i]) == "-alg")
        {
            sortingAlgorithm = argv[i + 1];
            i++;
        }
        else if (std::string(argv[i]) == "-pageSize")
        {
            pageSize = std::stoi(argv[i + 1]);
            i++;
        }
        else if (std::string(argv[i]) == "-pageCount")
        {
            pageCount = std::stoi(argv[i + 1]);
            i++;
        }
        else
        {
            throw_error_message();
            return 1;
        }
    }
    std::string outputBinPath = outputPath + ".bin";
    std::string outputTxtPath = outputPath + ".txt";
    FILE *test = fopen(inputPath.c_str(), "rb");
    if (test == nullptr)
    {
        std::cerr << "Error: el archivo " << inputPath << " no existe" << std::endl;
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
    if (sortingAlgorithm != "quick" && sortingAlgorithm != "merge" &&
        sortingAlgorithm != "bubble" &&
        sortingAlgorithm != "quickm3" && sortingAlgorithm != "insertion")
    {
        std::cerr << "Error: algoritmo no reconocido" << std::endl;
        return 1;
    }
    PagedArray *arr = new PagedArray(outputBinPath.c_str(), pageSize, pageCount, totalInts);
    auto start = std::chrono::high_resolution_clock::now();
    if (sortingAlgorithm == "insertion")
    {
        insertionSort(*arr, totalInts);
    }
    else if (sortingAlgorithm == "bubble")
    {
        bubbleSort(*arr, totalInts);
    }
    else if (sortingAlgorithm == "quickm3")
    {
        quickSortM3(*arr, 0, totalInts - 1);
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
        std::cerr << "Error: algoritmo no reconocido" << std::endl;
        return 1;
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Tiempo: " << elapsed.count() << " segundos" << std::endl;
    std::cout << "Algoritmo: " << sortingAlgorithm << std::endl;
    std::cout << "Page faults: " << arr->getPageFaults() << std::endl;
    std::cout << "Page hits: " << arr->getPageHits() <<std::endl;
    delete arr;
    generateTextFile(outputBinPath, outputTxtPath, totalInts);
    return 0;
}
void throw_error_message()
{
    std::cerr << "Se debe utilizar: -input /ruta/entrada.bin -output /ruta/salida -alg quicksort -pageSize 1024 -pageCount 4" << std::endl;
}
bool copyFile(const std::string &inputPath, const std::string &outputPath)
{
    FILE *inputFile = fopen(inputPath.c_str(), "rb");
    if (inputFile == nullptr)
    {
        std::cerr << "Error: no se pudo abrir el archivo de entrada" << std::endl;
        return false;
    }
    FILE *outputFile = fopen(outputPath.c_str(), "wb");
    if (outputFile == nullptr)
    {
        std::cerr << "Error: no se pudo crear el archivo de salida" << std::endl;
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
void generateTextFile(const std::string &outputBinPath, const std::string &outputTxtPath, long long totalInts)
{
    FILE *outputBinFile = fopen(outputBinPath.c_str(), "rb");
    if (outputBinFile == nullptr)
    {
        std::cerr << "Error: no se pudo crear el archivo de texto" << std::endl;
        return;
    }
    FILE *outputTxtFile = fopen(outputTxtPath.c_str(), "w");
    if (outputTxtFile == nullptr)
    {
        std::cerr << "Error no se pudo crear el archivo de texto" << std::endl;
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