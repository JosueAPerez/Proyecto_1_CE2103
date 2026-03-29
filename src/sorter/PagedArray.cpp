#include "PagedArray.h"
#include <iostream>
using namespace std;
PagedArray::PagedArray(const char *filePath, int pageSize, int pageCount, long long totalInts)
{
    this->pageSize = pageSize;
    this->pageCount = pageCount;
    this->totalInts = totalInts;
    this->pageFaults = 0;
    this->pageHits = 0;
    this->nextChange = 0;
    this->lruCounter=0;

    // abrir el archivo en modo lectura/escritura
    this->archivo = fopen(filePath, "r+b");
    if (this->archivo == nullptr)
    {
        cerr << "Error: no se pudo abrir el archivo" << endl;
        exit(1);
    }

    // crear los n espacios para páginas
    this->pages = new Page *[pageCount];
    for (int i = 0; i < pageCount; i++)
    {
        this->pages[i] = new Page(pageSize); // cada espacio empieza vacío
    }
}

PagedArray::~PagedArray()
{
    // guardar todas las páginas sucias antes de cerrar
    for (int i = 0; i < pageCount; i++)
    {
        if (pages[i]->isDirty)
        {
            savePage(i);
        }
        delete pages[i];
    }
    delete[] pages;
    fclose(archivo);
}

int PagedArray::findPage(int pageNumber)
{
    for (int i = 0; i < pageCount; i++)
    {
        if (pages[i]->pageNumber == pageNumber)
        {
            return i;
        }
    }
    return -1;
}

int PagedArray::findFreePage()
{
    for (int i = 0; i < pageCount; i++)
    {
        if (pages[i]->pageNumber == -1)
        {
            return i;
        }
    }
    return -1;
}

void PagedArray::savePage(int slot)
{
    if (!pages[slot]->isDirty)
        return;
    long long offset = (long long)pages[slot]->pageNumber * pageSize * sizeof(int32_t);
    fseek(archivo, offset, SEEK_SET);
    long long start = (long long)pages[slot]->pageNumber * pageSize;
    int cantidadEnteros = pageSize;
    if (start + pageSize > totalInts)
    {
        cantidadEnteros = totalInts - start;
    }
    fwrite(pages[slot]->data, sizeof(int32_t), cantidadEnteros, archivo);
    pages[slot]->isDirty = false;
}

void PagedArray::loadPage(int slot, int pageNumber)
{
    long long offset = (long long)pageNumber * pageSize * sizeof(int32_t);
    fseek(archivo, offset, SEEK_SET);
    long long start = (long long)pageNumber * pageSize;
    int cantidadEnteros = pageSize;
    if (start + pageSize > totalInts)
    {
        cantidadEnteros = totalInts - start;
    }
    fread(pages[slot]->data, sizeof(int32_t), cantidadEnteros, archivo);
    pages[slot]->pageNumber = pageNumber;
    pages[slot]->isDirty = false;
}

int32_t &PagedArray::operator[](long long index)
{
    int pageNumber = index / pageSize;
    int pageOffset = index % pageSize;
    int slot = findPage(pageNumber);
    if (slot != -1)
    {
        pageHits++;
    }
    else
    {
        pageFaults++;
        slot = findFreePage();
        if (slot == -1)
        {
            slot = selectVictim();
            if (pages[slot]->isDirty)
            {
                savePage(slot);
            }
        }
        loadPage(slot, pageNumber);
    }
    lruCounter++;
    pages[slot]->lastUsed = lruCounter;
    pages[slot]->isDirty = true;
    return pages[slot]->data[pageOffset];
}

int PagedArray::selectVictim() {
    int victim = 0;
    long long oldest = pages[0]->lastUsed;

    for (int i = 1; i < pageCount; i++) {
        if (pages[i]->lastUsed < oldest) {
            oldest = pages[i]->lastUsed;
            victim = i;
        }
    }
    return victim;
}
long long PagedArray::getPageFaults() {
    return pageFaults;
}

long long PagedArray::getPageHits() {
    return pageHits;
}