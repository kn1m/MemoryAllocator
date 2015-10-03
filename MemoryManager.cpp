//
// Created by m3sc4 on 02.10.15.
//

#include "MemoryManager.h"

MemoryManager::MemoryManager(int memory_size)
{
    pool_size = memory_size;
    pool = std::vector<char>((u_long) pool_size);
    pool_adress_table = std::map<int, int>();
    pool_adress_table.emplace(0, pool_size);
    pool_table = std::map<void*, int>();
}

void *MemoryManager::alloc(std::size_t size)
{
    size = cast_size(size);
    for(auto& adress: pool_adress_table)
    {
        if(adress.second - adress.first >= static_cast<int>(size))
        {
            auto block = &pool[adress.first];
            pool_table.emplace(block, adress.first);
            pool_adress_table.erase(adress.first);
            pool_adress_table.emplace(adress.first + static_cast<int>(size), adress.second);
            return block;
        }
    }
    return nullptr;
}

void *MemoryManager::realloc(void *mem_block, std::size_t new_size)
{
    if(mem_block == nullptr)
    {
        return alloc(new_size);
    }
    auto current_b = pool_table.find(mem_block);
    auto next_b = std::next(current_b, 1);
    int block_border;
    if(next_b == pool_table.end())
        block_border = pool_adress_table.rbegin()->first;
    else
        block_border = next_b->second;
    new_size = cast_size(new_size);
    for(auto& adress: pool_adress_table)
    {
        if(adress.second - adress.first >= static_cast<int>(new_size))
        {
            auto j = 0;
            for(auto i = current_b->second; i < block_border;i++ && j++)
            {
                pool[adress.first + j] = pool[i];
            }
            free(mem_block);
            return alloc(new_size);
        }
    }
    return nullptr;
}

void MemoryManager::free(void *mem_block)
{
    if(pool_table.find(mem_block) == pool_table.end())
        return;
    auto current_b = pool_table.find(mem_block);
    auto next_b = std::next(current_b, 1);
    if(next_b == pool_table.end())
        pool_adress_table.emplace(current_b->second, pool_size);
    else
        pool_adress_table.emplace(current_b->second, next_b->second);
    fix_free_space();
    pool_table.erase(mem_block);
}

void MemoryManager::dump() {

    std::cout << "------------------------------------" << '\n';
    std::cout << "Free space:" << '\n';
    for (auto it=pool_adress_table.begin(); it!=pool_adress_table.end(); ++it)
        std::cout << it->first << " => " << it->second << '\n';
    std::cout << "************************************" << '\n';
    std::cout << "Located memory:" << '\n';
    for (auto it=pool_table.begin(); it!=pool_table.end(); ++it)
        std::cout << it->first << " => " << it->second << '\n';

}

std::size_t MemoryManager::cast_size(std::size_t size) {
    if(size % 4 == 0)
    {
        return size;
    }
    return size + (4 - size % 4);
}

void MemoryManager::fix_free_space()
{
    int first_border = -1;
    int second_border = -1;
    for(auto& it: pool_adress_table)
    {
        if (second_border == it.second)
        {
            pool_adress_table.erase(it.first);
            break;
        }
        second_border = it.second;
    }
    second_border = -1;
    for (auto it=pool_adress_table.begin(); it!=pool_adress_table.end(); ++it)
    {
        if (first_border == it->first)
        {
            pool_adress_table[second_border] = it->second;
            pool_adress_table.erase(it);
        }
        second_border = it->first;
        first_border = it->second;
    }
}
