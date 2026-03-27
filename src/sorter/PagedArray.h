#ifndef PAGEDARRAY_H
#define PAGEDARRAY_H

#include <cstdint>
#include <cstdio>
#include "Page.h"

class PagedArray {
private:
    Page** pages;        // arreglo de punteros a páginas
    int pageSize;        // cuántos enteros caben en cada página
    int pageCount;       // cuántas páginas pueden estar en memoria
    FILE* archivo;       // archivo sobre el que se trabaja
    long long totalInts; // total de enteros en el archivo
    long long pageFaults;      // contador de page faults
    long long pageHits;        // contador de page hits
    int nextChange;
    int findPage(int pageNumber);       // busca si una página está cargada, retorna su índice o -1
    int findFreePage();                 // busca un espacio libre, retorna su índice o -1
    int selectVictim();                 // algoritmo de reemplazo, retorna índice de la víctima
    void loadPage(int slot, int pageNumber);   // carga una página del disco al espacio
    void savePage(int slot);                   // escribe una página del espacio al disco

public:
    PagedArray(const char* filePath, int pageSize, int pageCount, long long totalInts);
    ~PagedArray();

    int32_t& operator[](long long index); 

    long long getPageFaults();
    long long getPageHits();
};

#endif