#ifndef BMBMI1__H__INCLUDED__
#define BMBMI1__H__INCLUDED__
/*
Copyright(c) 2002-2017 Anatoliy Kuznetsov(anatoliy_kuznetsov at yahoo.com)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

For more information please visit:  http://bitmagic.io
*/

/*! \file bmbmi1,h
    \brief Intel BMI1 Bit manipulation primitives (internal)
*/

#include "bmdef.h"
#include "bmconst.h"

#define BMBMI1OPT

namespace bm
{

/**
    \brief word find index of the rank-th bit set by bit-testing
    \param val - 64-bit work to search
    \param rank - rank to select (should be > 0)
 
    \return selected value (inxed of bit set)
*/
inline
unsigned bmi1_select64_lz(bm::id64_t val, unsigned rank)
{
    unsigned bc, lz;
    unsigned val_lo32 = val & 0xFFFFFFFFull;
    
    unsigned bc_lo32 = _mm_popcnt_u64(val_lo32);
    if (rank <= bc_lo32)
    {
        unsigned val_lo16 = val & 0xFFFFull;
        unsigned bc_lo16 = _mm_popcnt_u64(val_lo16);
        val = (rank <= bc_lo16) ? val_lo16 : val_lo32;
    }
    bc = _mm_popcnt_u64(val);

    unsigned diff = bc - rank;
    for (; diff; --diff)
    {
        lz = _lzcnt_u64(val);
        val &= ~(1ull << (63 - lz));
    }
    BM_ASSERT(_mm_popcnt_u64(val) == rank);
    lz = _lzcnt_u64(val);
    lz = 63 - lz;
    return lz;
}



} // bm

#endif
