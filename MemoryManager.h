//
// Created by m3sc4 on 02.10.15.
//

#ifndef MEMORYALLOCATOR_MEMORYMANAGER_H
#define MEMORYALLOCATOR_MEMORYMANAGER_H


#include <vector>
#include <unordered_map>
#include <map>
#include <iostream>
#include <iterator>

class MemoryManager {
public:
    MemoryManager(int memory_size);
    ~MemoryManager() {};

    void* alloc(std::size_t size);
    void* realloc(void* mem_block, std::size_t new_size);
    void free(void* mem_block);

    void dump();
private:
    std::vector<char> pool;
    int pool_size;

    std::map<int, int> pool_adress_table;
    std::map<void*, int> pool_table;

    std::size_t cast_size(std::size_t size);
    void fix_free_space();

};


#endif //MEMORYALLOCATOR_MEMORYMANAGER_H
