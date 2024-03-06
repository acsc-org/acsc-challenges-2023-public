/*
   A C-program for MT19937-64 (2004/9/29 version).
   Coded by Takuji Nishimura and Makoto Matsumoto.

   This is a 64-bit version of Mersenne Twister pseudorandom number
   generator.

   Before using, initialize the state by using init_genrand64(seed)
   or init_by_array64(init_key, key_length).

   Copyright (C) 2004, Makoto Matsumoto and Takuji Nishimura,
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

     3. The names of its contributors may not be used to endorse or promote
        products derived from this software without specific prior written
        permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

   References:
   T. Nishimura, ``Tables of 64-bit Mersenne Twisters''
     ACM Transactions on Modeling and
     Computer Simulation 10. (2000) 348--357.
   M. Matsumoto and T. Nishimura,
     ``Mersenne Twister: a 623-dimensionally equidistributed
       uniform pseudorandom number generator''
     ACM Transactions on Modeling and
     Computer Simulation 8. (Jan. 1998) 3--30.

   Any feedback is very welcome.
   http://www.math.hiroshima-u.ac.jp/~m-mat/MT/emt.html
   email: m-mat @ math.sci.hiroshima-u.ac.jp (remove spaces)
*/

#include <stdio.h>
#include "rng.h"

#define C_OBF(R)                           \
 __asm__ __volatile__(  ".quad 0xea57ca34da91ba36\n" :: "a"(R)  )
#define C_DOBF(R, addr_key, noop)                           \
 __asm__ __volatile__(  ".byte 0x00,0x00\n" :: "a"(R), "b"(addr_key), "c"(noop)  )

#define NN 312
#define MM 156
#define MATRIX_A 0xB5026F5AA96619E9ULL
#define UM 0xFFFFFFFF80000000ULL /* Most significant 33 bits */
#define LM 0x7FFFFFFFULL         /* Least significant 31 bits */

/* The array for the state vector */
static unsigned long long mt[NN];
/* mti==NN+1 means mt[NN] is not initialized */
static int mti = NN + 1;

/* initializes mt[NN] with a seed */
void init_genrand64(unsigned long long seed)
{
    C_OBF(0x1013);
    C_DOBF(0x203e, 0x8ce027, 0xf0);
    mt[0] = seed;
    C_DOBF(0x20ba, 0xf0e9e0, 0xaf);
    C_OBF(0x100d);
    C_DOBF(0x2068, 0xc29293, 0xbf);
    C_OBF(0x1004);
    for (mti = 1; mti < NN; mti++)
    {
        mt[mti] = (9834136284391279305ULL * (mt[mti - 1] ^ (mt[mti - 1] >> 62)) + mti);
    }
}

/* generates a random number on [0, 2^64-1]-interval */
unsigned long long genrand64_int64(void)
{
    int i;
    unsigned long long x;
    static unsigned long long mag01[2] = {0ULL, MATRIX_A};

    C_OBF(0x103d);
    C_DOBF(0x200a, 0x429059, 0x97);
    if (mti >= NN)
    { /* generate NN words at one time */

        C_OBF(0x1011);
        C_DOBF(0x2095, 0xe45f45, 0xe6);
        /* if init_genrand64() has not been called, */
        /* a default initial seed is used     */
        if (mti == NN + 1)
            init_genrand64(5489ULL);

        C_OBF(0x1020);
        C_DOBF(0x2066, 0x7cd873, 0x3e);
        C_OBF(0x1027);
        for (i = 0; i < NN - MM; i++)
        {
            x = (mt[i] & UM) | (mt[i + 1] & LM);
            mt[i] = mt[i + MM] ^ (x >> 1) ^ mag01[(int)(x & 1ULL)];
        }
        C_OBF(0x1022);
        C_DOBF(0x20b6, 0x91eab2, 0x25);
        C_OBF(0x1005);
        for (; i < NN - 1; i++)
        {
            x = (mt[i] & UM) | (mt[i + 1] & LM);
            mt[i] = mt[i + (MM - NN)] ^ (x >> 1) ^ mag01[(int)(x & 1ULL)];
        }
        C_OBF(0x1016);
        C_DOBF(0x206b, 0x9aa30d, 0x80);
        C_OBF(0x1019);
        x = (mt[NN - 1] & UM) | (mt[0] & LM);
        mt[NN - 1] = mt[MM - 1] ^ (x >> 1) ^ mag01[(int)(x & 1ULL)];

        mti = 0;
    }

    C_OBF(0x1012);
    C_DOBF(0x20b3, 0xe236a0, 0xb);
    x = mt[mti++];

    C_OBF(0x100a);
    C_DOBF(0x20ff, 0x47c7b5, 0xde);
    x ^= (x >> 26) & 0xBBBBBBBBBBBBBBBBULL;
    x ^= (x << 19) & 0xA82C9FFFEDA60000ULL;
    C_OBF(0x101c);
    C_DOBF(0x20c6, 0x9aca75, 0x29);
    x ^= (x << 37) & 0xFFFABCD000000000ULL;
    x ^= (x >> 45);

    return x;
}