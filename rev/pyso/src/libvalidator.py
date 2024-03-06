from numba.pycc import CC
from numba import jit, njit
import numpy as np

cc = CC('libvalidator')
# Uncomment the following line to print out the compilation steps
#cc.verbose = True

@njit('(u1[:],)', inline='never', debug=True)
def _atoi_(S):
    S[0] = 5
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _memmove2_(S):
    S[1] = 196
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _va_end3_(S):
    S[2] = 31
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _wcsrtombs2_(S):
    S[3] = 227
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _setvbuf3_(S):
    S[4] = 223
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _vfwprintf2_(S):
    S[5] = 97
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] *= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _signal_(S):
    S[6] = 237
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _iswupper_(S):
    S[7] = 105
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _j2_(S):
    S[8] = 170
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _nl_langinfo2_(S):
    S[9] = 24
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _ispunct_(S):
    S[10] = 75
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] *= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _wcwidth2_(S):
    S[11] = 99
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] *= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _strstr_(S):
    S[12] = 70
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] *= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _hypot3_(S):
    S[13] = 41
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _rand_r_(S):
    S[14] = 154
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _localtime2_(S):
    S[15] = 165
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _va_end2_(S):
    S[16] = 33
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _ctime2_(S):
    S[17] = 253
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] *= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _atof3_(S):
    S[18] = 174
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _snprintf_(S):
    S[19] = 77
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _fprintf2_(S):
    S[20] = 158
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _snprintf2_(S):
    S[21] = 219
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _freopen_(S):
    S[22] = 187
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _strtold2_(S):
    S[23] = 44
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _asctime_r_(S):
    S[24] = 128
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _tmpfile_(S):
    S[25] = 224
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _longjmp_(S):
    S[26] = 132
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _catgets2_(S):
    S[27] = 167
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] *= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _catclose2_(S):
    S[28] = 110
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _wcscmp3_(S):
    S[29] = 217
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _vsscanf2_(S):
    S[30] = 146
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] *= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _setjmp3_(S):
    S[31] = 178
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] *= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _strfmon_(S):
    S[32] = 198
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _fgetwc3_(S):
    S[33] = 92
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] *= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _sqrt_(S):
    S[34] = 153
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] *= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _localtime_r2_(S):
    S[35] = 255
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _getwchar_(S):
    S[36] = 157
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] *= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _gets_(S):
    S[37] = 59
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] *= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _system3_(S):
    S[38] = 16
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _erf_(S):
    S[39] = 159
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] *= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _atof2_(S):
    S[40] = 34
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _nextafterl2_(S):
    S[41] = 123
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _isupper2_(S):
    S[42] = 79
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _gets2_(S):
    S[43] = 13
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _strxfrm3_(S):
    S[44] = 64
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _iswspace_(S):
    S[45] = 76
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _wcschr_(S):
    S[46] = 226
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _strcoll3_(S):
    S[47] = 252
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] *= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _wctomb2_(S):
    S[48] = 26
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] *= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _fread3_(S):
    S[49] = 135
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _gmtime_r2_(S):
    S[50] = 220
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _asctime2_(S):
    S[51] = 35
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _wcsftime_(S):
    S[52] = 171
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _memset3_(S):
    S[53] = 94
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _isxdigit3_(S):
    S[54] = 121
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _iswalnum3_(S):
    S[55] = 207
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] *= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _fileno2_(S):
    S[56] = 231
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _wprintf_(S):
    S[57] = 122
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _realloc3_(S):
    S[58] = 49
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] *= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _wcsptime2_(S):
    S[59] = 238
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _asin_(S):
    S[60] = 243
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _vfwscanf_(S):
    S[61] = 235
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _memset_(S):
    S[62] = 25
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _ldexp2_(S):
    S[63] = 38
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] *= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _log2_(S):
    S[64] = 20
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _mbrtowc2_(S):
    S[65] = 90
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _fgetws_(S):
    S[66] = 12
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _wcstof2_(S):
    S[67] = 56
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _wcsstr2_(S):
    S[68] = 202
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _asctime3_(S):
    S[69] = 210
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _exp_(S):
    S[70] = 8
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _tanh_(S):
    S[71] = 145
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _strtof2_(S):
    S[72] = 209
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _isupper_(S):
    S[73] = 18
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _fopen_(S):
    S[74] = 222
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _nl_langinfo_(S):
    S[75] = 50
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] *= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _wcsncpy_(S):
    S[76] = 86
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] *= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _wcwidth3_(S):
    S[77] = 81
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _iswctype2_(S):
    S[78] = 233
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _wmemset3_(S):
    S[79] = 234
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _strspn2_(S):
    S[80] = 230
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _strcmp3_(S):
    S[81] = 17
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _getwc2_(S):
    S[82] = 204
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _towupper2_(S):
    S[83] = 228
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _strtok3_(S):
    S[84] = 69
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _wcstof3_(S):
    S[85] = 14
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _remove3_(S):
    S[86] = 130
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] *= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _erf3_(S):
    S[87] = 246
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _iswcntrl2_(S):
    S[88] = 152
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _putwchar2_(S):
    S[89] = 250
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _vswscanf2_(S):
    S[90] = 156
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] *= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _getchar2_(S):
    S[91] = 218
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _mbsrtowcs_(S):
    S[92] = 175
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] *= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _fprintf3_(S):
    S[93] = 58
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _catopen3_(S):
    S[94] = 102
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _wmemcpy3_(S):
    S[95] = 39
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _swscanf2_(S):
    S[96] = 166
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _wprintf3_(S):
    S[97] = 131
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _strcat_(S):
    S[98] = 114
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _putwc2_(S):
    S[99] = 126
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _strftime3_(S):
    S[100] = 21
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _labs_(S):
    S[101] = 168
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _getwchar3_(S):
    S[102] = 189
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _wmemset_(S):
    S[103] = 3
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] *= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _iswdigit2_(S):
    S[104] = 61
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _rename2_(S):
    S[105] = 116
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _strspn3_(S):
    S[106] = 111
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _rand_r3_(S):
    S[107] = 98
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _strtold3_(S):
    S[108] = 80
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] *= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _strtod3_(S):
    S[109] = 141
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _vswscanf3_(S):
    S[110] = 72
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _atol_(S):
    S[111] = 11
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _ctime3_(S):
    S[112] = 74
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _toupper_(S):
    S[113] = 205
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _nexttowardl2_(S):
    S[114] = 241
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _difftime2_(S):
    S[115] = 88
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _fdopen_(S):
    S[116] = 63
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _fsetpos_(S):
    S[117] = 129
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] *= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _putwchar3_(S):
    S[118] = 239
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _wcslen2_(S):
    S[119] = 83
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _strtoul2_(S):
    S[120] = 71
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _strcat3_(S):
    S[121] = 199
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _wcstol_(S):
    S[122] = 249
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _assert_(S):
    S[123] = 19
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _modf_(S):
    S[124] = 147
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _wcstoul3_(S):
    S[125] = 221
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] *= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _fsetpos2_(S):
    S[126] = 107
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] *= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _cos_(S):
    S[127] = 236
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] *= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _wcstombs_(S):
    S[128] = 109
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _strtold_(S):
    S[129] = 15
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _fflush_(S):
    S[130] = 124
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _srand_(S):
    S[131] = 245
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _fsetpos3_(S):
    S[132] = 143
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _strcoll2_(S):
    S[133] = 186
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] *= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _exit_(S):
    S[134] = 150
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _quantexpd_(S):
    S[135] = 212
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _raise2_(S):
    S[136] = 113
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _wmemchr2_(S):
    S[137] = 164
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _floor_(S):
    S[138] = 191
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _ferror2_(S):
    S[139] = 40
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _labs3_(S):
    S[140] = 4
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] *= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _fwscanf2_(S):
    S[141] = 192
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _fclose3_(S):
    S[142] = 193
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _setbuf2_(S):
    S[143] = 139
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _ctime_(S):
    S[144] = 84
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _vwscanf3_(S):
    S[145] = 240
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] *= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _floor2_(S):
    S[146] = 144
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] *= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _mktime3_(S):
    S[147] = 201
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] *= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _wctob2_(S):
    S[148] = 169
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _fgetws2_(S):
    S[149] = 96
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _malloc_(S):
    S[150] = 180
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _erf2_(S):
    S[151] = 151
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _mbrtowc3_(S):
    S[152] = 195
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _iswgraph3_(S):
    S[153] = 68
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _strcpy3_(S):
    S[154] = 2
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] *= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _setjmp2_(S):
    S[155] = 104
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _sqrt2_(S):
    S[156] = 213
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] *= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _putc_(S):
    S[157] = 138
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _free_(S):
    S[158] = 54
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] *= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _wcsrchr_(S):
    S[159] = 118
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _wcsspn_(S):
    S[160] = 161
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _wcsspn3_(S):
    S[161] = 101
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _wctomb3_(S):
    S[162] = 148
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _clearerr_(S):
    S[163] = 27
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _feof_(S):
    S[164] = 163
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _strncasecmp_(S):
    S[165] = 203
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _wcstold2_(S):
    S[166] = 108
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _tmpnam_(S):
    S[167] = 173
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _strncmp2_(S):
    S[168] = 48
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _fwrite3_(S):
    S[169] = 67
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _isascii3_(S):
    S[170] = 172
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _sprintf_(S):
    S[171] = 53
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] *= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _vwprintf3_(S):
    S[172] = 225
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _fopen2_(S):
    S[173] = 1
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _fabs2_(S):
    S[174] = 51
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] *= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _vfwscanf2_(S):
    S[175] = 216
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _ldiv3_(S):
    S[176] = 115
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _mbsinit3_(S):
    S[177] = 119
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _toupper3_(S):
    S[178] = 208
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] *= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _setlocale_(S):
    S[179] = 7
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _fileno3_(S):
    S[180] = 247
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _calloc_(S):
    S[181] = 120
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _srand3_(S):
    S[182] = 55
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _gamma_(S):
    S[183] = 65
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _scanf_(S):
    S[184] = 200
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _putchar2_(S):
    S[185] = 23
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _fputc2_(S):
    S[186] = 106
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] *= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _raise_(S):
    S[187] = 149
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _strncmp3_(S):
    S[188] = 184
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] *= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _getenv_(S):
    S[189] = 215
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] *= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _wcsncpy2_(S):
    S[190] = 47
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _localeconv_(S):
    S[191] = 9
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _strtok_r_(S):
    S[192] = 251
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _wcscmp2_(S):
    S[193] = 78
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] *= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _wcscpy_(S):
    S[194] = 89
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _isupper3_(S):
    S[195] = 185
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _fwide_(S):
    S[196] = 211
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] *= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _rename_(S):
    S[197] = 182
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] *= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _assert3_(S):
    S[198] = 194
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _vswprintf3_(S):
    S[199] = 100
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] *= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _va_start2_(S):
    S[200] = 28
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _iswxdigit_(S):
    S[201] = 52
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _fclose_(S):
    S[202] = 29
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _towlower_(S):
    S[203] = 190
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _fgetpos2_(S):
    S[204] = 62
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _regexec3_(S):
    S[205] = 37
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _strcasecmp3_(S):
    S[206] = 32
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _printf_(S):
    S[207] = 214
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _ftell_(S):
    S[208] = 42
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _realloc2_(S):
    S[209] = 0
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _strptime3_(S):
    S[210] = 127
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _assert2_(S):
    S[211] = 206
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _gamma2_(S):
    S[212] = 91
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _strcspn3_(S):
    S[213] = 73
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] *= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _vsscanf_(S):
    S[214] = 232
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _wcslocaleconv3_(S):
    S[215] = 85
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _localeconv2_(S):
    S[216] = 125
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] *= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _fabs3_(S):
    S[217] = 46
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _wmemcmp2_(S):
    S[218] = 197
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] *= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _acos3_(S):
    S[219] = 60
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _nexttoward2_(S):
    S[220] = 244
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _iswxdigit2_(S):
    S[221] = 103
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _iswgraph2_(S):
    S[222] = 95
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _mblen2_(S):
    S[223] = 10
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] *= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _wctype_(S):
    S[224] = 82
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _calloc2_(S):
    S[225] = 57
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] *= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _clearerr3_(S):
    S[226] = 162
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _remove2_(S):
    S[227] = 177
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _time2_(S):
    S[228] = 117
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _getc_(S):
    S[229] = 242
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _sscanf2_(S):
    S[230] = 176
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _sin_(S):
    S[231] = 45
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _tan3_(S):
    S[232] = 30
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _strncpy3_(S):
    S[233] = 155
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _quantexpd3_(S):
    S[234] = 112
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] *= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _asctime_r3_(S):
    S[235] = 181
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _wcwidth_(S):
    S[236] = 134
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] *= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _regcomp_(S):
    S[237] = 136
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _vsnprintf2_(S):
    S[238] = 140
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _iswlower3_(S):
    S[239] = 93
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] *= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _jn3_(S):
    S[240] = 183
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _isdigit3_(S):
    S[241] = 6
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _islower3_(S):
    S[242] = 179
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _jn2_(S):
    S[243] = 229
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _fopen3_(S):
    S[244] = 43
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _wcscat2_(S):
    S[245] = 142
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _iswdigit_(S):
    S[246] = 160
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _iswctype3_(S):
    S[247] = 87
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] *= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _strtol2_(S):
    S[248] = 254
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] ^= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _iswctype_(S):
    S[249] = 248
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _fputwc3_(S):
    S[250] = 137
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _system_(S):
    S[251] = 22
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] -= S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _pow_(S):
    S[252] = 133
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _cos2_(S):
    S[253] = 66
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _iswalpha2_(S):
    S[254] = 188
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('(u1[:],)', inline='never', debug=True)
def _strcasecmp_(S):
    S[255] = 36
    Z = np.zeros(256, dtype=np.uint8)

    for i in range(256):
        Z[i] += S[i]
    return Z

@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _isspace_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _sscanf3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _cosh_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _feof3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _nextafter_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _isprint_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _strstr3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _localtime_r_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _wctrans3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _wcschr3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _regerror2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _iswspace2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _isprint2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _ctime_r_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _isdigit_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _fputs2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _toupper2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _fgetws3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _wcscoll3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _exit3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _strxfrm_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _fgetpos_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _tolower_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _wcstok3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _iswprint3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _ldiv2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _towupper3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _strncasecmp3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _isgraph3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _vfscanf_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _strtok_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _wctob3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _catopen2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _mblen_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _va_copy3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _free3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _wcslocaleconv_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _iswblank_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _ferror3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _isblank3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _fabs_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _sin3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _isprint3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _vfwprintf_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _wcstok2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _vswprintf_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _asin3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _scanf3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _fwrite2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _wcschr2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _fgets2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _isspace2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _wcstok_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _fgetc_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _tan_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _frexp2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _abort3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _vswscanf_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _iswcntrl3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _towctrans3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _atexit_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _ispunct3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _remove_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _wcscpy2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _ldexp3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _wmemcmp3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _iswblank2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _isalpha3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _iswblank3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _rewind3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _toascii_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _samequantumd3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _wcslen_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _putenv2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _mbstowcs3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _memmove_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _abort_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _fputwc_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _fdopen3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _abs_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _btowc2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _strfmon3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _memcpy2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _wcsptime3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _strncpy2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _strncmp_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _vscanf2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _sin2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _setvbuf_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _fwscanf_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _ungetwc_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _wcstod2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _tanh2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _nextafter3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _wmemmove2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _strtok_r2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _iswalnum_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _sinh2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _wcstol2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _ungetc3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _mktime_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _quantized3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _getchar_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _srand2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _va_copy2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _fputc3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _strncpy_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _wcscat_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _va_arg3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _calloc3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _vsprintf3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _sinh_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _strtoul3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _memcmp_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _setlocale2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _ceil_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _raise3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _rename3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _nexttoward3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _strerror2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _strcmp2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _strncat3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _wcstombs3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _nextafter2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _isalnum2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _isgraph_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _atoi3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _modf3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _atexit2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _sprintf3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _islower_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _tanh3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _puts2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _wctob_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _towlower2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _j_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _iswspace3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _wcsftime3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _putwc3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _atoi2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _va_arg_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _ldiv_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _vwprintf2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _tmpnam2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _localtime_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _system2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _fputws3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _fputc_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _snprintf3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _wcsstr3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _isgraph2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _abs3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _iscntrl2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _samequantumd2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _atol2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _wcslocaleconv2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _strncat_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _sqrt3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _wcscspn3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _swprintf_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _clock3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _rand3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _memmove3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _fclose2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _perror_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _cos3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _mbsrtowcs3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _wcslen3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _putchar_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _strchr3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _tan2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _nexttowardl_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _exit2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _puts_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _getwc_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _vfscanf3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _wcstombs2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _labs2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _fgetc3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _regfree2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _wcsxfrm2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _regfree3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _putwchar_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _fscanf2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _fgets_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _wcstol3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _tolower2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _putenv3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _tmpfile2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _gmtime_r_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _iswdigit3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _ungetc_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _memcmp2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _regcomp3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _fseek_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _localtime3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _strptime_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _strpbrk_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _gmtime_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _printf2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _mbtowc3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _strtof3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _strncasecmp2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _gets3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _iswupper3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _regerror_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _fputws2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _strtok_r3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _getwc3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _gmtime2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _toascii2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _strlen_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _bsearch3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _perror3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _erfc3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _fflush2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _memset2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _freopen3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _acos_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _wcrtomb2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _getwchar2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _difftime_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _getenv2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _wcspbrk_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _perror2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _cosh3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _int3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _mbstowcs2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _fwide2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _iswpunct2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _setvbuf2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _fgetwc2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _wmemcmp_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _strtod_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _wcstoul_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _catclose_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _wcrtomb_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _va_end_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _setjmp_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _strftime_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _atof_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _isblank2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _islower2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _isalpha_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _ftell3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _nl_langinfo3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _ungetwc3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _abort2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _catclose3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _strcoll_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _getchar3_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _iswpunct_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _strchr_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _abs2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _quantized2_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('u1(u1[:],u1[:],u1[:],u1,u1)', inline='never', debug=True)
def _mbtowc_(S, K, flag, i, j):
    j = (j + S[i] + K[i]) % 256
    if i != j:
        S[i] ^= S[j]
        S[j] ^= S[i]
        S[i] ^= S[j]
    # S[i], S[j] = S[j], S[i]

    if (i // 4) != (j // 4):
        flag[i // 4] ^= flag[j // 4]

    flag[i // 4] ^= i
    flag[j // 4] ^= j
    return j
@njit('(u1[:],)', inline='never', debug=True)
def _hypot2_(S):
    _atoi_(S)
    _memmove2_(S)
    _va_end3_(S)
    _wcsrtombs2_(S)
    _setvbuf3_(S)
    _vfwprintf2_(S)
    _signal_(S)
    _iswupper_(S)
    _j2_(S)
    _nl_langinfo2_(S)
    _ispunct_(S)
    _wcwidth2_(S)
    _strstr_(S)
    _hypot3_(S)
    _rand_r_(S)
    _localtime2_(S)

@njit('(u1[:],)', inline='never', debug=True)
def _regcomp2_(S):
    _va_end2_(S)
    _ctime2_(S)
    _atof3_(S)
    _snprintf_(S)
    _fprintf2_(S)
    _snprintf2_(S)
    _freopen_(S)
    _strtold2_(S)
    _asctime_r_(S)
    _tmpfile_(S)
    _longjmp_(S)
    _catgets2_(S)
    _catclose2_(S)
    _wcscmp3_(S)
    _vsscanf2_(S)
    _setjmp3_(S)

@njit('(u1[:],)', inline='never', debug=True)
def _fflush3_(S):
    _strfmon_(S)
    _fgetwc3_(S)
    _sqrt_(S)
    _localtime_r2_(S)
    _getwchar_(S)
    _gets_(S)
    _system3_(S)
    _erf_(S)
    _atof2_(S)
    _nextafterl2_(S)
    _isupper2_(S)
    _gets2_(S)
    _strxfrm3_(S)
    _iswspace_(S)
    _wcschr_(S)
    _strcoll3_(S)

@njit('(u1[:],)', inline='never', debug=True)
def _mbrlen2_(S):
    _wctomb2_(S)
    _fread3_(S)
    _gmtime_r2_(S)
    _asctime2_(S)
    _wcsftime_(S)
    _memset3_(S)
    _isxdigit3_(S)
    _iswalnum3_(S)
    _fileno2_(S)
    _wprintf_(S)
    _realloc3_(S)
    _wcsptime2_(S)
    _asin_(S)
    _vfwscanf_(S)
    _memset_(S)
    _ldexp2_(S)

@njit('(u1[:],)', inline='never', debug=True)
def _mbrlen_(S):
    _log2_(S)
    _mbrtowc2_(S)
    _fgetws_(S)
    _wcstof2_(S)
    _wcsstr2_(S)
    _asctime3_(S)
    _exp_(S)
    _tanh_(S)
    _strtof2_(S)
    _isupper_(S)
    _fopen_(S)
    _nl_langinfo_(S)
    _wcsncpy_(S)
    _wcwidth3_(S)
    _iswctype2_(S)
    _wmemset3_(S)

@njit('(u1[:],)', inline='never', debug=True)
def _iswcntrl_(S):
    _strspn2_(S)
    _strcmp3_(S)
    _getwc2_(S)
    _towupper2_(S)
    _strtok3_(S)
    _wcstof3_(S)
    _remove3_(S)
    _erf3_(S)
    _iswcntrl2_(S)
    _putwchar2_(S)
    _vswscanf2_(S)
    _getchar2_(S)
    _mbsrtowcs_(S)
    _fprintf3_(S)
    _catopen3_(S)
    _wmemcpy3_(S)

@njit('(u1[:],)', inline='never', debug=True)
def _fgetwc_(S):
    _swscanf2_(S)
    _wprintf3_(S)
    _strcat_(S)
    _putwc2_(S)
    _strftime3_(S)
    _labs_(S)
    _getwchar3_(S)
    _wmemset_(S)
    _iswdigit2_(S)
    _rename2_(S)
    _strspn3_(S)
    _rand_r3_(S)
    _strtold3_(S)
    _strtod3_(S)
    _vswscanf3_(S)
    _atol_(S)

@njit('(u1[:],)', inline='never', debug=True)
def _mbrtowc_(S):
    _ctime3_(S)
    _toupper_(S)
    _nexttowardl2_(S)
    _difftime2_(S)
    _fdopen_(S)
    _fsetpos_(S)
    _putwchar3_(S)
    _wcslen2_(S)
    _strtoul2_(S)
    _strcat3_(S)
    _wcstol_(S)
    _assert_(S)
    _modf_(S)
    _wcstoul3_(S)
    _fsetpos2_(S)
    _cos_(S)

@njit('(u1[:],)', inline='never', debug=True)
def _vwscanf_(S):
    _wcstombs_(S)
    _strtold_(S)
    _fflush_(S)
    _srand_(S)
    _fsetpos3_(S)
    _strcoll2_(S)
    _exit_(S)
    _quantexpd_(S)
    _raise2_(S)
    _wmemchr2_(S)
    _floor_(S)
    _ferror2_(S)
    _labs3_(S)
    _fwscanf2_(S)
    _fclose3_(S)
    _setbuf2_(S)

@njit('(u1[:],)', inline='never', debug=True)
def _fscanf_(S):
    _ctime_(S)
    _vwscanf3_(S)
    _floor2_(S)
    _mktime3_(S)
    _wctob2_(S)
    _fgetws2_(S)
    _malloc_(S)
    _erf2_(S)
    _mbrtowc3_(S)
    _iswgraph3_(S)
    _strcpy3_(S)
    _setjmp2_(S)
    _sqrt2_(S)
    _putc_(S)
    _free_(S)
    _wcsrchr_(S)

@njit('(u1[:],)', inline='never', debug=True)
def _ldexp_(S):
    _wcsspn_(S)
    _wcsspn3_(S)
    _wctomb3_(S)
    _clearerr_(S)
    _feof_(S)
    _strncasecmp_(S)
    _wcstold2_(S)
    _tmpnam_(S)
    _strncmp2_(S)
    _fwrite3_(S)
    _isascii3_(S)
    _sprintf_(S)
    _vwprintf3_(S)
    _fopen2_(S)
    _fabs2_(S)
    _vfwscanf2_(S)

@njit('(u1[:],)', inline='never', debug=True)
def _btowc3_(S):
    _ldiv3_(S)
    _mbsinit3_(S)
    _toupper3_(S)
    _setlocale_(S)
    _fileno3_(S)
    _calloc_(S)
    _srand3_(S)
    _gamma_(S)
    _scanf_(S)
    _putchar2_(S)
    _fputc2_(S)
    _raise_(S)
    _strncmp3_(S)
    _getenv_(S)
    _wcsncpy2_(S)
    _localeconv_(S)

@njit('(u1[:],)', inline='never', debug=True)
def _strcasecmp2_(S):
    _strtok_r_(S)
    _wcscmp2_(S)
    _wcscpy_(S)
    _isupper3_(S)
    _fwide_(S)
    _rename_(S)
    _assert3_(S)
    _vswprintf3_(S)
    _va_start2_(S)
    _iswxdigit_(S)
    _fclose_(S)
    _towlower_(S)
    _fgetpos2_(S)
    _regexec3_(S)
    _strcasecmp3_(S)
    _printf_(S)

@njit('(u1[:],)', inline='never', debug=True)
def _isalnum3_(S):
    _ftell_(S)
    _realloc2_(S)
    _strptime3_(S)
    _assert2_(S)
    _gamma2_(S)
    _strcspn3_(S)
    _vsscanf_(S)
    _wcslocaleconv3_(S)
    _localeconv2_(S)
    _fabs3_(S)
    _wmemcmp2_(S)
    _acos3_(S)
    _nexttoward2_(S)
    _iswxdigit2_(S)
    _iswgraph2_(S)
    _mblen2_(S)

@njit('(u1[:],)', inline='never', debug=True)
def _towctrans_(S):
    _wctype_(S)
    _calloc2_(S)
    _clearerr3_(S)
    _remove2_(S)
    _time2_(S)
    _getc_(S)
    _sscanf2_(S)
    _sin_(S)
    _tan3_(S)
    _strncpy3_(S)
    _quantexpd3_(S)
    _asctime_r3_(S)
    _wcwidth_(S)
    _regcomp_(S)
    _vsnprintf2_(S)
    _iswlower3_(S)

@njit('(u1[:],)', inline='never', debug=True)
def _wctype2_(S):
    _jn3_(S)
    _isdigit3_(S)
    _islower3_(S)
    _jn2_(S)
    _fopen3_(S)
    _wcscat2_(S)
    _iswdigit_(S)
    _iswctype3_(S)
    _strtol2_(S)
    _iswctype_(S)
    _fputwc3_(S)
    _system_(S)
    _pow_(S)
    _cos2_(S)
    _iswalpha2_(S)
    _strcasecmp_(S)

@njit('(u1[:],)', inline='never', debug=True)
def _wcspbrk2_(S):
    _hypot2_(S)
    _regcomp2_(S)
    _fflush3_(S)
    _mbrlen2_(S)
    _mbrlen_(S)
    _iswcntrl_(S)
    _fgetwc_(S)
    _mbrtowc_(S)
    _vwscanf_(S)
    _fscanf_(S)
    _ldexp_(S)
    _btowc3_(S)
    _strcasecmp2_(S)
    _isalnum3_(S)
    _towctrans_(S)
    _wctype2_(S)

@njit('u1(u1[:],u1[:],u1[:],u1)', inline='never', debug=True)
def _atan2_(S, K, flag, j):
    j = _isspace_(S, K, flag, 0, j)
    j = _sscanf3_(S, K, flag, 1, j)
    j = _cosh_(S, K, flag, 2, j)
    j = _feof3_(S, K, flag, 3, j)
    j = _nextafter_(S, K, flag, 4, j)
    j = _isprint_(S, K, flag, 5, j)
    j = _strstr3_(S, K, flag, 6, j)
    j = _localtime_r_(S, K, flag, 7, j)
    j = _wctrans3_(S, K, flag, 8, j)
    j = _wcschr3_(S, K, flag, 9, j)
    j = _regerror2_(S, K, flag, 10, j)
    j = _iswspace2_(S, K, flag, 11, j)
    j = _isprint2_(S, K, flag, 12, j)
    j = _ctime_r_(S, K, flag, 13, j)
    j = _isdigit_(S, K, flag, 14, j)
    j = _fputs2_(S, K, flag, 15, j)
    return j
@njit('u1(u1[:],u1[:],u1[:],u1)', inline='never', debug=True)
def _isascii_(S, K, flag, j):
    j = _toupper2_(S, K, flag, 16, j)
    j = _fgetws3_(S, K, flag, 17, j)
    j = _wcscoll3_(S, K, flag, 18, j)
    j = _exit3_(S, K, flag, 19, j)
    j = _strxfrm_(S, K, flag, 20, j)
    j = _fgetpos_(S, K, flag, 21, j)
    j = _tolower_(S, K, flag, 22, j)
    j = _wcstok3_(S, K, flag, 23, j)
    j = _iswprint3_(S, K, flag, 24, j)
    j = _ldiv2_(S, K, flag, 25, j)
    j = _towupper3_(S, K, flag, 26, j)
    j = _strncasecmp3_(S, K, flag, 27, j)
    j = _isgraph3_(S, K, flag, 28, j)
    j = _vfscanf_(S, K, flag, 29, j)
    j = _strtok_(S, K, flag, 30, j)
    j = _wctob3_(S, K, flag, 31, j)
    return j
@njit('u1(u1[:],u1[:],u1[:],u1)', inline='never', debug=True)
def _wcsrchr3_(S, K, flag, j):
    j = _catopen2_(S, K, flag, 32, j)
    j = _mblen_(S, K, flag, 33, j)
    j = _va_copy3_(S, K, flag, 34, j)
    j = _free3_(S, K, flag, 35, j)
    j = _wcslocaleconv_(S, K, flag, 36, j)
    j = _iswblank_(S, K, flag, 37, j)
    j = _ferror3_(S, K, flag, 38, j)
    j = _isblank3_(S, K, flag, 39, j)
    j = _fabs_(S, K, flag, 40, j)
    j = _sin3_(S, K, flag, 41, j)
    j = _isprint3_(S, K, flag, 42, j)
    j = _vfwprintf_(S, K, flag, 43, j)
    j = _wcstok2_(S, K, flag, 44, j)
    j = _vswprintf_(S, K, flag, 45, j)
    j = _asin3_(S, K, flag, 46, j)
    j = _scanf3_(S, K, flag, 47, j)
    return j
@njit('u1(u1[:],u1[:],u1[:],u1)', inline='never', debug=True)
def _wcsrchr2_(S, K, flag, j):
    j = _fwrite2_(S, K, flag, 48, j)
    j = _wcschr2_(S, K, flag, 49, j)
    j = _fgets2_(S, K, flag, 50, j)
    j = _isspace2_(S, K, flag, 51, j)
    j = _wcstok_(S, K, flag, 52, j)
    j = _fgetc_(S, K, flag, 53, j)
    j = _tan_(S, K, flag, 54, j)
    j = _frexp2_(S, K, flag, 55, j)
    j = _abort3_(S, K, flag, 56, j)
    j = _vswscanf_(S, K, flag, 57, j)
    j = _iswcntrl3_(S, K, flag, 58, j)
    j = _towctrans3_(S, K, flag, 59, j)
    j = _atexit_(S, K, flag, 60, j)
    j = _ispunct3_(S, K, flag, 61, j)
    j = _remove_(S, K, flag, 62, j)
    j = _wcscpy2_(S, K, flag, 63, j)
    return j
@njit('u1(u1[:],u1[:],u1[:],u1)', inline='never', debug=True)
def _wctrans2_(S, K, flag, j):
    j = _ldexp3_(S, K, flag, 64, j)
    j = _wmemcmp3_(S, K, flag, 65, j)
    j = _iswblank2_(S, K, flag, 66, j)
    j = _isalpha3_(S, K, flag, 67, j)
    j = _iswblank3_(S, K, flag, 68, j)
    j = _rewind3_(S, K, flag, 69, j)
    j = _toascii_(S, K, flag, 70, j)
    j = _samequantumd3_(S, K, flag, 71, j)
    j = _wcslen_(S, K, flag, 72, j)
    j = _putenv2_(S, K, flag, 73, j)
    j = _mbstowcs3_(S, K, flag, 74, j)
    j = _memmove_(S, K, flag, 75, j)
    j = _abort_(S, K, flag, 76, j)
    j = _fputwc_(S, K, flag, 77, j)
    j = _fdopen3_(S, K, flag, 78, j)
    j = _abs_(S, K, flag, 79, j)
    return j
@njit('u1(u1[:],u1[:],u1[:],u1)', inline='never', debug=True)
def _fileno_(S, K, flag, j):
    j = _btowc2_(S, K, flag, 80, j)
    j = _strfmon3_(S, K, flag, 81, j)
    j = _memcpy2_(S, K, flag, 82, j)
    j = _wcsptime3_(S, K, flag, 83, j)
    j = _strncpy2_(S, K, flag, 84, j)
    j = _strncmp_(S, K, flag, 85, j)
    j = _vscanf2_(S, K, flag, 86, j)
    j = _sin2_(S, K, flag, 87, j)
    j = _setvbuf_(S, K, flag, 88, j)
    j = _fwscanf_(S, K, flag, 89, j)
    j = _ungetwc_(S, K, flag, 90, j)
    j = _wcstod2_(S, K, flag, 91, j)
    j = _tanh2_(S, K, flag, 92, j)
    j = _nextafter3_(S, K, flag, 93, j)
    j = _wmemmove2_(S, K, flag, 94, j)
    j = _strtok_r2_(S, K, flag, 95, j)
    return j
@njit('u1(u1[:],u1[:],u1[:],u1)', inline='never', debug=True)
def _div_(S, K, flag, j):
    j = _iswalnum_(S, K, flag, 96, j)
    j = _sinh2_(S, K, flag, 97, j)
    j = _wcstol2_(S, K, flag, 98, j)
    j = _ungetc3_(S, K, flag, 99, j)
    j = _mktime_(S, K, flag, 100, j)
    j = _quantized3_(S, K, flag, 101, j)
    j = _getchar_(S, K, flag, 102, j)
    j = _srand2_(S, K, flag, 103, j)
    j = _va_copy2_(S, K, flag, 104, j)
    j = _fputc3_(S, K, flag, 105, j)
    j = _strncpy_(S, K, flag, 106, j)
    j = _wcscat_(S, K, flag, 107, j)
    j = _va_arg3_(S, K, flag, 108, j)
    j = _calloc3_(S, K, flag, 109, j)
    j = _vsprintf3_(S, K, flag, 110, j)
    j = _sinh_(S, K, flag, 111, j)
    return j
@njit('u1(u1[:],u1[:],u1[:],u1)', inline='never', debug=True)
def _vsnprintf3_(S, K, flag, j):
    j = _strtoul3_(S, K, flag, 112, j)
    j = _memcmp_(S, K, flag, 113, j)
    j = _setlocale2_(S, K, flag, 114, j)
    j = _ceil_(S, K, flag, 115, j)
    j = _raise3_(S, K, flag, 116, j)
    j = _rename3_(S, K, flag, 117, j)
    j = _nexttoward3_(S, K, flag, 118, j)
    j = _strerror2_(S, K, flag, 119, j)
    j = _strcmp2_(S, K, flag, 120, j)
    j = _strncat3_(S, K, flag, 121, j)
    j = _wcstombs3_(S, K, flag, 122, j)
    j = _nextafter2_(S, K, flag, 123, j)
    j = _isalnum2_(S, K, flag, 124, j)
    j = _isgraph_(S, K, flag, 125, j)
    j = _atoi3_(S, K, flag, 126, j)
    j = _modf3_(S, K, flag, 127, j)
    return j
@njit('u1(u1[:],u1[:],u1[:],u1)', inline='never', debug=True)
def _towlower3_(S, K, flag, j):
    j = _atexit2_(S, K, flag, 128, j)
    j = _sprintf3_(S, K, flag, 129, j)
    j = _islower_(S, K, flag, 130, j)
    j = _tanh3_(S, K, flag, 131, j)
    j = _puts2_(S, K, flag, 132, j)
    j = _wctob_(S, K, flag, 133, j)
    j = _towlower2_(S, K, flag, 134, j)
    j = _j_(S, K, flag, 135, j)
    j = _iswspace3_(S, K, flag, 136, j)
    j = _wcsftime3_(S, K, flag, 137, j)
    j = _putwc3_(S, K, flag, 138, j)
    j = _atoi2_(S, K, flag, 139, j)
    j = _va_arg_(S, K, flag, 140, j)
    j = _ldiv_(S, K, flag, 141, j)
    j = _vwprintf2_(S, K, flag, 142, j)
    j = _tmpnam2_(S, K, flag, 143, j)
    return j
@njit('u1(u1[:],u1[:],u1[:],u1)', inline='never', debug=True)
def _strerror_(S, K, flag, j):
    j = _localtime_(S, K, flag, 144, j)
    j = _system2_(S, K, flag, 145, j)
    j = _fputws3_(S, K, flag, 146, j)
    j = _fputc_(S, K, flag, 147, j)
    j = _snprintf3_(S, K, flag, 148, j)
    j = _wcsstr3_(S, K, flag, 149, j)
    j = _isgraph2_(S, K, flag, 150, j)
    j = _abs3_(S, K, flag, 151, j)
    j = _iscntrl2_(S, K, flag, 152, j)
    j = _samequantumd2_(S, K, flag, 153, j)
    j = _atol2_(S, K, flag, 154, j)
    j = _wcslocaleconv2_(S, K, flag, 155, j)
    j = _strncat_(S, K, flag, 156, j)
    j = _sqrt3_(S, K, flag, 157, j)
    j = _wcscspn3_(S, K, flag, 158, j)
    j = _swprintf_(S, K, flag, 159, j)
    return j
@njit('u1(u1[:],u1[:],u1[:],u1)', inline='never', debug=True)
def _fputs_(S, K, flag, j):
    j = _clock3_(S, K, flag, 160, j)
    j = _rand3_(S, K, flag, 161, j)
    j = _memmove3_(S, K, flag, 162, j)
    j = _fclose2_(S, K, flag, 163, j)
    j = _perror_(S, K, flag, 164, j)
    j = _cos3_(S, K, flag, 165, j)
    j = _mbsrtowcs3_(S, K, flag, 166, j)
    j = _wcslen3_(S, K, flag, 167, j)
    j = _putchar_(S, K, flag, 168, j)
    j = _strchr3_(S, K, flag, 169, j)
    j = _tan2_(S, K, flag, 170, j)
    j = _nexttowardl_(S, K, flag, 171, j)
    j = _exit2_(S, K, flag, 172, j)
    j = _puts_(S, K, flag, 173, j)
    j = _getwc_(S, K, flag, 174, j)
    j = _vfscanf3_(S, K, flag, 175, j)
    return j
@njit('u1(u1[:],u1[:],u1[:],u1)', inline='never', debug=True)
def _cosh2_(S, K, flag, j):
    j = _wcstombs2_(S, K, flag, 176, j)
    j = _labs2_(S, K, flag, 177, j)
    j = _fgetc3_(S, K, flag, 178, j)
    j = _regfree2_(S, K, flag, 179, j)
    j = _wcsxfrm2_(S, K, flag, 180, j)
    j = _regfree3_(S, K, flag, 181, j)
    j = _putwchar_(S, K, flag, 182, j)
    j = _fscanf2_(S, K, flag, 183, j)
    j = _fgets_(S, K, flag, 184, j)
    j = _wcstol3_(S, K, flag, 185, j)
    j = _tolower2_(S, K, flag, 186, j)
    j = _putenv3_(S, K, flag, 187, j)
    j = _tmpfile2_(S, K, flag, 188, j)
    j = _gmtime_r_(S, K, flag, 189, j)
    j = _iswdigit3_(S, K, flag, 190, j)
    j = _ungetc_(S, K, flag, 191, j)
    return j
@njit('u1(u1[:],u1[:],u1[:],u1)', inline='never', debug=True)
def _wcsxfrm3_(S, K, flag, j):
    j = _memcmp2_(S, K, flag, 192, j)
    j = _regcomp3_(S, K, flag, 193, j)
    j = _fseek_(S, K, flag, 194, j)
    j = _localtime3_(S, K, flag, 195, j)
    j = _strptime_(S, K, flag, 196, j)
    j = _strpbrk_(S, K, flag, 197, j)
    j = _gmtime_(S, K, flag, 198, j)
    j = _printf2_(S, K, flag, 199, j)
    j = _mbtowc3_(S, K, flag, 200, j)
    j = _strtof3_(S, K, flag, 201, j)
    j = _strncasecmp2_(S, K, flag, 202, j)
    j = _gets3_(S, K, flag, 203, j)
    j = _iswupper3_(S, K, flag, 204, j)
    j = _regerror_(S, K, flag, 205, j)
    j = _fputws2_(S, K, flag, 206, j)
    j = _strtok_r3_(S, K, flag, 207, j)
    return j
@njit('u1(u1[:],u1[:],u1[:],u1)', inline='never', debug=True)
def _setbuf3_(S, K, flag, j):
    j = _getwc3_(S, K, flag, 208, j)
    j = _gmtime2_(S, K, flag, 209, j)
    j = _toascii2_(S, K, flag, 210, j)
    j = _strlen_(S, K, flag, 211, j)
    j = _bsearch3_(S, K, flag, 212, j)
    j = _perror3_(S, K, flag, 213, j)
    j = _erfc3_(S, K, flag, 214, j)
    j = _fflush2_(S, K, flag, 215, j)
    j = _memset2_(S, K, flag, 216, j)
    j = _freopen3_(S, K, flag, 217, j)
    j = _acos_(S, K, flag, 218, j)
    j = _wcrtomb2_(S, K, flag, 219, j)
    j = _getwchar2_(S, K, flag, 220, j)
    j = _difftime_(S, K, flag, 221, j)
    j = _getenv2_(S, K, flag, 222, j)
    j = _wcspbrk_(S, K, flag, 223, j)
    return j
@njit('u1(u1[:],u1[:],u1[:],u1)', inline='never', debug=True)
def _vscanf3_(S, K, flag, j):
    j = _perror2_(S, K, flag, 224, j)
    j = _cosh3_(S, K, flag, 225, j)
    j = _int3_(S, K, flag, 226, j)
    j = _mbstowcs2_(S, K, flag, 227, j)
    j = _fwide2_(S, K, flag, 228, j)
    j = _iswpunct2_(S, K, flag, 229, j)
    j = _setvbuf2_(S, K, flag, 230, j)
    j = _fgetwc2_(S, K, flag, 231, j)
    j = _wmemcmp_(S, K, flag, 232, j)
    j = _strtod_(S, K, flag, 233, j)
    j = _wcstoul_(S, K, flag, 234, j)
    j = _catclose_(S, K, flag, 235, j)
    j = _wcrtomb_(S, K, flag, 236, j)
    j = _va_end_(S, K, flag, 237, j)
    j = _setjmp_(S, K, flag, 238, j)
    j = _strftime_(S, K, flag, 239, j)
    return j
@njit('u1(u1[:],u1[:],u1[:],u1)', inline='never', debug=True)
def _qsort_(S, K, flag, j):
    j = _atof_(S, K, flag, 240, j)
    j = _isblank2_(S, K, flag, 241, j)
    j = _islower2_(S, K, flag, 242, j)
    j = _isalpha_(S, K, flag, 243, j)
    j = _ftell3_(S, K, flag, 244, j)
    j = _nl_langinfo3_(S, K, flag, 245, j)
    j = _ungetwc3_(S, K, flag, 246, j)
    j = _abort2_(S, K, flag, 247, j)
    j = _catclose3_(S, K, flag, 248, j)
    j = _strcoll_(S, K, flag, 249, j)
    j = _getchar3_(S, K, flag, 250, j)
    j = _iswpunct_(S, K, flag, 251, j)
    j = _strchr_(S, K, flag, 252, j)
    j = _abs2_(S, K, flag, 253, j)
    j = _quantized2_(S, K, flag, 254, j)
    j = _mbtowc_(S, K, flag, 255, j)
    return j
@njit('(u1[:],u1[:],u1[:])', inline='never', debug=True)
def _jn_(S, K, flag):
    j = 0
    j = _atan2_(S, K, flag, j)
    j = _isascii_(S, K, flag, j)
    j = _wcsrchr3_(S, K, flag, j)
    j = _wcsrchr2_(S, K, flag, j)
    j = _wctrans2_(S, K, flag, j)
    j = _fileno_(S, K, flag, j)
    j = _div_(S, K, flag, j)
    j = _vsnprintf3_(S, K, flag, j)
    j = _towlower3_(S, K, flag, j)
    j = _strerror_(S, K, flag, j)
    j = _fputs_(S, K, flag, j)
    j = _cosh2_(S, K, flag, j)
    j = _wcsxfrm3_(S, K, flag, j)
    j = _setbuf3_(S, K, flag, j)
    j = _vscanf3_(S, K, flag, j)
    j = _qsort_(S, K, flag, j)

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _strncat2_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _mbsinit_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _wmemmove3_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _vprintf2_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _wcstoul2_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _wscanf_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _strptime2_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _tmpfile3_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _hypot_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _setbuf_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _erfc_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _free2_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _fmod_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _clock_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _regerror3_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _fread_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _strcspn2_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _fseek3_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _samequantumd_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _wcstold3_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _wcscat3_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _catgets3_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _regexec_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _ctime_r2_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _wcscspn2_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _exp2_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _puts3_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _gmtime_r3_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _vfprintf2_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _sprintf2_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _btowc_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _bsearch_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _fwide3_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _nexttoward_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _nextafterl_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _strlen3_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _isxdigit2_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _iswlower2_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _strpbrk2_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _wctomb_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _gmtime3_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _putc3_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _tmpnam3_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _fdopen2_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _fwrite_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _ungetc2_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _sscanf_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _wcsncat_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _ctime_r3_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _longjmp3_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _iswupper2_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _clearerr2_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _wcsrtombs3_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _mbtowc2_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _isascii2_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _fscanf3_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _strfmon2_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _fwscanf3_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _mbsrtowcs2_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _signal2_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _wcsncpy3_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _vprintf3_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _wctype3_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _towctrans2_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _fputws_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _fgetc2_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _log3_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _fprintf_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _atan_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _regexec2_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _getenv3_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _putwc_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _rewind_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _vswprintf2_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _ceil2_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _catopen_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _iswalnum2_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _iswprint_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _vsscanf3_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _iswalpha_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _putenv_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _swprintf3_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _strtol3_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _erfc2_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _acos2_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _setlocale3_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _wcsncat3_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _catgets_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _fwprintf3_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _isalnum_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _isxdigit_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _bsearch2_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _putc2_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _sinh3_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _regfree_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _strchr2_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _strspn_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _mbrlen3_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _mktime2_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _va_arg2_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _ceil3_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _modf2_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _wprintf2_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _wctrans_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _vwscanf2_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _exp3_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _memcmp3_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _gamma3_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _printf3_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _nextafterl3_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _strcpy2_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _vprintf_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _wcscoll_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _iswxdigit3_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _iswgraph_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _tolower3_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _ftell2_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _wscanf2_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _realloc_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _vwprintf_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _time_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _wcsptime_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _qsort2_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _quantized_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _isdigit2_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _difftime3_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _isspace3_(S, i, j, k, z):
    i = (i + 5) % 256
    if 0 <= i < 256:
        j = (k + S[(j + S[i]) % 256]) % 256
        k = (k + i + S[j]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k
    else:
        j = (k + S[(i + S[j]) % 256]) % 256
        k = (k + j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        z = S[(j + S[(i + S[(z + k) % 256]) % 256]) % 256]
        return z, i, j, k

@njit('(u1, u1)', inline='never', debug=True)
def _frexp3_(a, b):
    return (((a ^ b) + 2 * (a & b)) * 39 + 23) * 151 + 111

@njit('(u1[:], u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _strpbrk3_(S, inp, i, j, k, z):
    z, i, j, k = _strncat2_(S, i, j, k, z)
    inp[0] = _mbsinit_(inp[0], z)
    z, i, j, k = _wmemmove3_(S, i, j, k, z)
    inp[1] = _vprintf2_(inp[1], z)
    z, i, j, k = _wcstoul2_(S, i, j, k, z)
    inp[2] = _wscanf_(inp[2], z)
    z, i, j, k = _strptime2_(S, i, j, k, z)
    inp[3] = _tmpfile3_(inp[3], z)
    z, i, j, k = _hypot_(S, i, j, k, z)
    inp[4] = _setbuf_(inp[4], z)
    z, i, j, k = _erfc_(S, i, j, k, z)
    inp[5] = _free2_(inp[5], z)
    z, i, j, k = _fmod_(S, i, j, k, z)
    inp[6] = _clock_(inp[6], z)
    z, i, j, k = _regerror3_(S, i, j, k, z)
    inp[7] = _fread_(inp[7], z)
    z, i, j, k = _strcspn2_(S, i, j, k, z)
    inp[8] = _fseek3_(inp[8], z)
    z, i, j, k = _samequantumd_(S, i, j, k, z)
    inp[9] = _wcstold3_(inp[9], z)
    z, i, j, k = _wcscat3_(S, i, j, k, z)
    inp[10] = _catgets3_(inp[10], z)
    z, i, j, k = _regexec_(S, i, j, k, z)
    inp[11] = _ctime_r2_(inp[11], z)
    z, i, j, k = _wcscspn2_(S, i, j, k, z)
    inp[12] = _exp2_(inp[12], z)
    z, i, j, k = _puts3_(S, i, j, k, z)
    inp[13] = _gmtime_r3_(inp[13], z)
    z, i, j, k = _vfprintf2_(S, i, j, k, z)
    inp[14] = _sprintf2_(inp[14], z)
    z, i, j, k = _btowc_(S, i, j, k, z)
    inp[15] = _bsearch_(inp[15], z)
    return z, i, j, k

@njit('(u1[:], u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _strerror3_(S, inp, i, j, k, z):
    z, i, j, k = _fwide3_(S, i, j, k, z)
    inp[16] = _nexttoward_(inp[16], z)
    z, i, j, k = _nextafterl_(S, i, j, k, z)
    inp[17] = _strlen3_(inp[17], z)
    z, i, j, k = _isxdigit2_(S, i, j, k, z)
    inp[18] = _iswlower2_(inp[18], z)
    z, i, j, k = _strpbrk2_(S, i, j, k, z)
    inp[19] = _wctomb_(inp[19], z)
    z, i, j, k = _gmtime3_(S, i, j, k, z)
    inp[20] = _putc3_(inp[20], z)
    z, i, j, k = _tmpnam3_(S, i, j, k, z)
    inp[21] = _fdopen2_(inp[21], z)
    z, i, j, k = _fwrite_(S, i, j, k, z)
    inp[22] = _ungetc2_(inp[22], z)
    z, i, j, k = _sscanf_(S, i, j, k, z)
    inp[23] = _wcsncat_(inp[23], z)
    z, i, j, k = _ctime_r3_(S, i, j, k, z)
    inp[24] = _longjmp3_(inp[24], z)
    z, i, j, k = _iswupper2_(S, i, j, k, z)
    inp[25] = _clearerr2_(inp[25], z)
    z, i, j, k = _wcsrtombs3_(S, i, j, k, z)
    inp[26] = _mbtowc2_(inp[26], z)
    z, i, j, k = _isascii2_(S, i, j, k, z)
    inp[27] = _fscanf3_(inp[27], z)
    z, i, j, k = _strfmon2_(S, i, j, k, z)
    inp[28] = _fwscanf3_(inp[28], z)
    z, i, j, k = _mbsrtowcs2_(S, i, j, k, z)
    inp[29] = _signal2_(inp[29], z)
    z, i, j, k = _wcsncpy3_(S, i, j, k, z)
    inp[30] = _vprintf3_(inp[30], z)
    z, i, j, k = _wctype3_(S, i, j, k, z)
    inp[31] = _towctrans2_(inp[31], z)
    return z, i, j, k

@njit('(u1[:], u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _vfprintf3_(S, inp, i, j, k, z):
    z, i, j, k = _fputws_(S, i, j, k, z)
    inp[32] = _fgetc2_(inp[32], z)
    z, i, j, k = _log3_(S, i, j, k, z)
    inp[33] = _fprintf_(inp[33], z)
    z, i, j, k = _atan_(S, i, j, k, z)
    inp[34] = _regexec2_(inp[34], z)
    z, i, j, k = _getenv3_(S, i, j, k, z)
    inp[35] = _putwc_(inp[35], z)
    z, i, j, k = _rewind_(S, i, j, k, z)
    inp[36] = _vswprintf2_(inp[36], z)
    z, i, j, k = _ceil2_(S, i, j, k, z)
    inp[37] = _catopen_(inp[37], z)
    z, i, j, k = _iswalnum2_(S, i, j, k, z)
    inp[38] = _iswprint_(inp[38], z)
    z, i, j, k = _vsscanf3_(S, i, j, k, z)
    inp[39] = _iswalpha_(inp[39], z)
    z, i, j, k = _putenv_(S, i, j, k, z)
    inp[40] = _swprintf3_(inp[40], z)
    z, i, j, k = _strtol3_(S, i, j, k, z)
    inp[41] = _erfc2_(inp[41], z)
    z, i, j, k = _acos2_(S, i, j, k, z)
    inp[42] = _setlocale3_(inp[42], z)
    z, i, j, k = _wcsncat3_(S, i, j, k, z)
    inp[43] = _catgets_(inp[43], z)
    z, i, j, k = _fwprintf3_(S, i, j, k, z)
    inp[44] = _isalnum_(inp[44], z)
    z, i, j, k = _isxdigit_(S, i, j, k, z)
    inp[45] = _bsearch2_(inp[45], z)
    z, i, j, k = _putc2_(S, i, j, k, z)
    inp[46] = _sinh3_(inp[46], z)
    z, i, j, k = _regfree_(S, i, j, k, z)
    inp[47] = _strchr2_(inp[47], z)
    return z, i, j, k

@njit('(u1[:], u1[:], u1, u1, u1, u1)', inline='never', debug=True)
def _iscntrl_(S, inp, i, j, k, z):
    z, i, j, k = _strspn_(S, i, j, k, z)
    inp[48] = _mbrlen3_(inp[48], z)
    z, i, j, k = _mktime2_(S, i, j, k, z)
    inp[49] = _va_arg2_(inp[49], z)
    z, i, j, k = _ceil3_(S, i, j, k, z)
    inp[50] = _modf2_(inp[50], z)
    z, i, j, k = _wprintf2_(S, i, j, k, z)
    inp[51] = _wctrans_(inp[51], z)
    z, i, j, k = _vwscanf2_(S, i, j, k, z)
    inp[52] = _exp3_(inp[52], z)
    z, i, j, k = _memcmp3_(S, i, j, k, z)
    inp[53] = _gamma3_(inp[53], z)
    z, i, j, k = _printf3_(S, i, j, k, z)
    inp[54] = _nextafterl3_(inp[54], z)
    z, i, j, k = _strcpy2_(S, i, j, k, z)
    inp[55] = _vprintf_(inp[55], z)
    z, i, j, k = _wcscoll_(S, i, j, k, z)
    inp[56] = _iswxdigit3_(inp[56], z)
    z, i, j, k = _iswgraph_(S, i, j, k, z)
    inp[57] = _tolower3_(inp[57], z)
    z, i, j, k = _ftell2_(S, i, j, k, z)
    inp[58] = _wscanf2_(inp[58], z)
    z, i, j, k = _realloc_(S, i, j, k, z)
    inp[59] = _vwprintf_(inp[59], z)
    z, i, j, k = _time_(S, i, j, k, z)
    inp[60] = _wcsptime_(inp[60], z)
    z, i, j, k = _qsort2_(S, i, j, k, z)
    inp[61] = _quantized_(inp[61], z)
    z, i, j, k = _isdigit2_(S, i, j, k, z)
    inp[62] = _difftime3_(inp[62], z)
    z, i, j, k = _isspace3_(S, i, j, k, z)
    inp[63] = _frexp3_(inp[63], z)
    return z, i, j, k

@njit('u1[:](u1[:],u1[:])', inline='never', debug=True)
def _wmemchr3_(K, inp):
    S = np.zeros(256, dtype=np.uint8)
    _wcspbrk2_(S)
    _jn_(S, K, inp)
    i, j, k, z = 0, 0, 0, 0
    z, i, j, k = _strpbrk3_(S, inp, i, j, k, z)
    z, i, j, k = _strerror3_(S, inp, i, j, k, z)
    z, i, j, k = _vfprintf3_(S, inp, i, j, k, z)
    z, i, j, k = _iscntrl_(S, inp, i, j, k, z)
    return inp

@njit('u1(u1,)', inline='never', debug=True)
def _fgets3_(x):
    return x - 141

@njit('u1(u1,)', inline='never', debug=True)
def _strcpy_(x):
    return x - 109

@njit('u1(u1,)', inline='never', debug=True)
def _wcstod3_(x):
    return x - 174

@njit('u1(u1,)', inline='never', debug=True)
def _wcscmp_(x):
    return x ^ 83

@njit('u1(u1,)', inline='never', debug=True)
def _fputs3_(x):
    return ((x | 181) - (x & 181))

@njit('u1(u1,)', inline='never', debug=True)
def _iswpunct3_(x):
    return (x | 217) + 217 - (~x & 217)

@njit('u1(u1,)', inline='never', debug=True)
def _strstr2_(x):
    return ((x | 15) - (x & 15))

@njit('u1(u1,)', inline='never', debug=True)
def _longjmp2_(x):
    return x ^ 49

@njit('u1(u1,)', inline='never', debug=True)
def _strftime2_(x):
    return x ^ 226

@njit('u1(u1,)', inline='never', debug=True)
def _mbstowcs_(x):
    return x ^ 110

@njit('u1(u1,)', inline='never', debug=True)
def _div3_(x):
    return (x | 238) + 238 - (~x & 238)

@njit('u1(u1,)', inline='never', debug=True)
def _wmemcpy_(x):
    return (x | 11) + 11 - (~x & 11)

@njit('u1(u1,)', inline='never', debug=True)
def _va_start3_(x):
    return x - 75

@njit('u1(u1,)', inline='never', debug=True)
def _vfwprintf3_(x):
    return (x | 126) + 126 - (~x & 126)

@njit('u1(u1,)', inline='never', debug=True)
def _atan3_(x):
    return x - 20

@njit('u1(u1,)', inline='never', debug=True)
def _mbsinit2_(x):
    return (x | 188) + 188 - (~x & 188)

@njit('u1(u1,)', inline='never', debug=True)
def _wcspbrk3_(x):
    return x - 232

@njit('u1(u1,)', inline='never', debug=True)
def _fgetpos3_(x):
    return ((x | 199) - (x & 199))

@njit('u1(u1,)', inline='never', debug=True)
def _strtok2_(x):
    return x - 32

@njit('u1(u1,)', inline='never', debug=True)
def _feof2_(x):
    return ((x | 239) - (x & 239))

@njit('u1(u1,)', inline='never', debug=True)
def _swscanf_(x):
    return (x | 223) + 223 - (~x & 223)

@njit('u1(u1,)', inline='never', debug=True)
def _ferror_(x):
    return (x | 244) + 244 - (~x & 244)

@njit('u1(u1,)', inline='never', debug=True)
def _strcat2_(x):
    return x ^ 61

@njit('u1(u1,)', inline='never', debug=True)
def _fmod3_(x):
    return ((x | 27) - (x & 27))

@njit('u1(u1,)', inline='never', debug=True)
def _isalpha2_(x):
    return (x | 234) + 234 - (~x & 234)

@njit('u1(u1,)', inline='never', debug=True)
def _wmemset2_(x):
    return (x | 124) + 124 - (~x & 124)

@njit('u1(u1,)', inline='never', debug=True)
def _ispunct2_(x):
    return ((x | 145) - (x & 145))

@njit('u1(u1,)', inline='never', debug=True)
def _fread2_(x):
    return (x | 11) + 11 - (~x & 11)

@njit('u1(u1,)', inline='never', debug=True)
def _pow3_(x):
    return x ^ 137

@njit('u1(u1,)', inline='never', debug=True)
def _rand_r2_(x):
    return ((x | 209) - (x & 209))

@njit('u1(u1,)', inline='never', debug=True)
def _nexttowardl3_(x):
    return (x | 229) + 229 - (~x & 229)

@njit('u1(u1,)', inline='never', debug=True)
def _asctime_r2_(x):
    return x - 134

@njit('u1(u1,)', inline='never', debug=True)
def _floor3_(x):
    return x ^ 138

@njit('u1(u1,)', inline='never', debug=True)
def _vsnprintf_(x):
    return x ^ 239

@njit('u1(u1,)', inline='never', debug=True)
def _wcsrtombs_(x):
    return x ^ 173

@njit('u1(u1,)', inline='never', debug=True)
def _swscanf3_(x):
    return (x | 14) + 14 - (~x & 14)

@njit('u1(u1,)', inline='never', debug=True)
def _wcstof_(x):
    return x ^ 218

@njit('u1(u1,)', inline='never', debug=True)
def _j3_(x):
    return x - 203

@njit('u1(u1,)', inline='never', debug=True)
def _asctime_(x):
    return (x | 168) + 168 - (~x & 168)

@njit('u1(u1,)', inline='never', debug=True)
def _qsort3_(x):
    return x - 210

@njit('u1(u1,)', inline='never', debug=True)
def _strtof_(x):
    return x ^ 176

@njit('u1(u1,)', inline='never', debug=True)
def _malloc2_(x):
    return x - 81

@njit('u1(u1,)', inline='never', debug=True)
def _wcstod_(x):
    return ((x | 42) - (x & 42))

@njit('u1(u1,)', inline='never', debug=True)
def _wcsxfrm_(x):
    return ((x | 99) - (x & 99))

@njit('u1(u1,)', inline='never', debug=True)
def _memchr_(x):
    return ((x | 147) - (x & 147))

@njit('u1(u1,)', inline='never', debug=True)
def _wcscpy3_(x):
    return (x | 58) + 58 - (~x & 58)

@njit('u1(u1,)', inline='never', debug=True)
def _strrchr3_(x):
    return x ^ 231

@njit('u1(u1,)', inline='never', debug=True)
def _quantexpd2_(x):
    return ((x | 176) - (x & 176))

@njit('u1(u1,)', inline='never', debug=True)
def _wcsncmp2_(x):
    return ((x | 17) - (x & 17))

@njit('u1(u1,)', inline='never', debug=True)
def _div2_(x):
    return x - 153

@njit('u1(u1,)', inline='never', debug=True)
def _vscanf_(x):
    return x - 149

@njit('u1(u1,)', inline='never', debug=True)
def _va_copy_(x):
    return (x | 13) + 13 - (~x & 13)

@njit('u1(u1,)', inline='never', debug=True)
def _fwprintf_(x):
    return x - 53

@njit('u1(u1,)', inline='never', debug=True)
def _vfwscanf3_(x):
    return x ^ 14

@njit('u1(u1,)', inline='never', debug=True)
def _strlen2_(x):
    return x ^ 209

@njit('u1(u1,)', inline='never', debug=True)
def _wcsncat2_(x):
    return ((x | 88) - (x & 88))

@njit('u1(u1,)', inline='never', debug=True)
def _vsprintf_(x):
    return x ^ 132

@njit('u1(u1,)', inline='never', debug=True)
def _scanf2_(x):
    return ((x | 124) - (x & 124))

@njit('u1(u1,)', inline='never', debug=True)
def _toascii3_(x):
    return ((x | 85) - (x & 85))

@njit('u1(u1,)', inline='never', debug=True)
def _memcpy_(x):
    return (x | 175) + 175 - (~x & 175)

@njit('u1(u1,)', inline='never', debug=True)
def _rand_(x):
    return x - 183

@njit('u1(u1,)', inline='never', debug=True)
def _fputwc2_(x):
    return ((x | 207) - (x & 207))

@njit('u1(u1,)', inline='never', debug=True)
def _malloc3_(x):
    return x - 167

@njit('u1(u1,)', inline='never', debug=True)
def _time3_(x):
    return x ^ 87

@njit('(u1[:],)', inline='never', debug=True)
def _atexit3_(P):
    a = 0
    a |= _fgets3_(P[0])
    a |= _strcpy_(P[1])
    a |= _wcstod3_(P[2])
    a |= _wcscmp_(P[3])
    a |= _fputs3_(P[4])
    a |= _iswpunct3_(P[5])
    a |= _strstr2_(P[6])
    a |= _longjmp2_(P[7])
    a |= _strftime2_(P[8])
    a |= _mbstowcs_(P[9])
    a |= _div3_(P[10])
    a |= _wmemcpy_(P[11])
    a |= _va_start3_(P[12])
    a |= _vfwprintf3_(P[13])
    a |= _atan3_(P[14])
    a |= _mbsinit2_(P[15])
    return a
@njit('(u1[:],)', inline='never', debug=True)
def _swprintf2_(P):
    a = 0
    a |= _wcspbrk3_(P[16])
    a |= _fgetpos3_(P[17])
    a |= _strtok2_(P[18])
    a |= _feof2_(P[19])
    a |= _swscanf_(P[20])
    a |= _ferror_(P[21])
    a |= _strcat2_(P[22])
    a |= _fmod3_(P[23])
    a |= _isalpha2_(P[24])
    a |= _wmemset2_(P[25])
    a |= _ispunct2_(P[26])
    a |= _fread2_(P[27])
    a |= _pow3_(P[28])
    a |= _rand_r2_(P[29])
    a |= _nexttowardl3_(P[30])
    a |= _asctime_r2_(P[31])
    return a
@njit('(u1[:],)', inline='never', debug=True)
def _iswprint2_(P):
    a = 0
    a |= _floor3_(P[32])
    a |= _vsnprintf_(P[33])
    a |= _wcsrtombs_(P[34])
    a |= _swscanf3_(P[35])
    a |= _wcstof_(P[36])
    a |= _j3_(P[37])
    a |= _asctime_(P[38])
    a |= _qsort3_(P[39])
    a |= _strtof_(P[40])
    a |= _malloc2_(P[41])
    a |= _wcstod_(P[42])
    a |= _wcsxfrm_(P[43])
    a |= _memchr_(P[44])
    a |= _wcscpy3_(P[45])
    a |= _strrchr3_(P[46])
    a |= _quantexpd2_(P[47])
    return a
@njit('(u1[:],)', inline='never', debug=True)
def _strrchr2_(P):
    a = 0
    a |= _wcsncmp2_(P[48])
    a |= _div2_(P[49])
    a |= _vscanf_(P[50])
    a |= _va_copy_(P[51])
    a |= _fwprintf_(P[52])
    a |= _vfwscanf3_(P[53])
    a |= _strlen2_(P[54])
    a |= _wcsncat2_(P[55])
    a |= _vsprintf_(P[56])
    a |= _scanf2_(P[57])
    a |= _toascii3_(P[58])
    a |= _memcpy_(P[59])
    a |= _rand_(P[60])
    a |= _fputwc2_(P[61])
    a |= _malloc3_(P[62])
    a |= _time3_(P[63])
    return a
@cc.export('aes_encrypt', '(u1[:],u1[:],u1[:])')
def aes_encrypt(inp, key, iv):
    key = np.zeros(256, dtype=np.uint8)
    P = _wmemchr3_(key, inp)
    a = 0
    a |= _atexit3_(P)
    a |= _swprintf2_(P)
    a |= _iswprint2_(P)
    a |= _strrchr2_(P)
    return a == 0

@cc.export('fast_len', '(u1[:],)')
def fast_len(inp):
    return 24

@cc.export('fast_xor', '(u1[:],u1[:])')
def fast_xor(inp, k):
    return inp

@cc.export('fast_rev', '(u1[:],)')
def fast_rev(inp):
    return inp

@cc.export('fast_cmp', '(u1,u1[:])')
def fast_cmp(inp, C):
    return inp

if __name__ == "__main__":
    cc.compile()
    # aes_encrypt(np.array(list(b"acsc{pyth0n_numb4_0n3___s0_m4ny_functi0ns_s0_l1ttl3_us3ful_c0d3}"), dtype=np.uint8), None, None)