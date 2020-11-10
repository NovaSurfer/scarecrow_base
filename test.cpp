#include <cassert>
#include <cstdint>
#include <cstdio>
#include "memory.h"


struct NonTrivial
{
    NonTrivial()
    {
	printf("%s\n", "NonTrivial()");
    }
};

struct alignas(8) NonTrivialAligned
{
    NonTrivialAligned()
    {
	printf("%s\n", "NonTrivialAligned()");
    }
};


int main()
{
    using namespace sc;
    mem::set_header(memhead_type::SIZE | memhead_type::ALIGN);
    long* l = (long*)mem::alloc_aligned(sizeof(long) * 5, 16);
    assert(reinterpret_cast<uintptr_t>(l) % 16 == 0);
    printf("%zu\n", mem::allocated_size(l));
    printf("%zu\n", mem::allocated_alignment(l));
    mem::dealloc_aligned(l);

    int* l2 = (int*)mem::alloc(sizeof(int) * 5);
    printf("%zu\n", mem::allocated_size(l2));
    mem::dealloc(l2);
    
    NonTrivial* nont = new NonTrivial();
    printf("%zu\n", mem::allocated_size(nont));
    delete nont;

    NonTrivial* nont_arr = new NonTrivial[8];
    printf("%zu\n", mem::allocated_size(nont_arr));
    delete[] nont_arr;

    NonTrivialAligned* nonta = new NonTrivialAligned();
    printf("%zu\n", mem::allocated_size(nonta));
    delete nonta;

    return 0;
}

