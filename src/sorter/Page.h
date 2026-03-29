#ifndef PAGE_H
#define PAGE_H
#include <cstdint>

class Page {
    public:
        int32_t* data;
        int pageNumber;
        bool isDirty;
        int pageSize;
        long long lastUsed;
    Page(int pageSize){
        this->pageSize=pageSize;
        this->pageNumber = -1;
        this->isDirty=false;
        this->data = new int32_t[pageSize];
        this->lastUsed=0;
    }
    ~Page(){
        delete[] data;
    }
};

#endif