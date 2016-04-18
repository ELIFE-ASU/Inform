// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <ctest.h>

#include <inform/state_encoding.h>
#include <math.h>

CTEST(StateEncoding, EncodeNULL)
{
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(0), inform_encode_base(NULL,0,2));
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(0), inform_encode_base(NULL,1,2));
}

CTEST(StateEncoding, EncodeInvalidBase)
{
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(1), inform_encode_base((int[]){0},1,-1));
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(1), inform_encode_base((int[]){0},1,0));
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(1), inform_encode_base((int[]){0},1,1));
}

CTEST(StateEncoding, EncodeTooLong_Base2)
{
    int *ones = calloc(63, sizeof(int));
    for (size_t i = 0; i < 63; ++i) ones[i] = 1;
    ASSERT_EQUAL_U(0x7fffffffffffffff, inform_encode_base(ones,63,2));
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(2), inform_encode_base(ones,64,2));
    free(ones);
}

CTEST(StateEncoding, EncodeTooLong_Base3)
{
    int *twos = calloc(40, sizeof(int));
    for (size_t i = 0; i < 40; ++i) twos[i] = 2;
    ASSERT_EQUAL_U(0xa8b8b452291fe820, inform_encode_base(twos,40,3));
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(2), inform_encode_base(twos,41,3));
    free(twos);
}

CTEST(StateEncoding, EncodeTooLong_Base4)
{
    int *threes = calloc(31, sizeof(int));
    for (size_t i = 0; i < 31; ++i) threes[i] = 3;
    ASSERT_EQUAL_U(0x3fffffffffffffff, inform_encode_base(threes,31,4));
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(2), inform_encode_base(threes,32,4));
    free(threes);
}

CTEST(StateEncoding, EncodeTooLong_Base5)
{
    int *fours = calloc(27, sizeof(int));
    for (size_t i = 0; i < 27; ++i) fours[i] = 4;
    ASSERT_EQUAL_U(0x6765c793fa10079c, inform_encode_base(fours,27,5));
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(2), inform_encode_base(fours,28,5));
    free(fours);
}

CTEST(StateEncoding, EncodeLengthOne_Base2)
{
   ASSERT_EQUAL_U(0, inform_encode_base((int[]){0},1,2));
   ASSERT_EQUAL_U(1, inform_encode_base((int[]){1},1,2));

   ASSERT_EQUAL_U(0, inform_encode_base((int[]){0,0},1,2));
   ASSERT_EQUAL_U(1, inform_encode_base((int[]){1,0},1,2));
   ASSERT_EQUAL_U(0, inform_encode_base((int[]){0,1},1,2));
   ASSERT_EQUAL_U(1, inform_encode_base((int[]){1,1},1,2));
}

CTEST(StateEncoding, EncodeLengthOne_Base5)
{
   ASSERT_EQUAL_U(0, inform_encode_base((int[]){0},1,5));
   ASSERT_EQUAL_U(1, inform_encode_base((int[]){1},1,5));
   ASSERT_EQUAL_U(2, inform_encode_base((int[]){2},1,5));
   ASSERT_EQUAL_U(3, inform_encode_base((int[]){3},1,5));
   ASSERT_EQUAL_U(4, inform_encode_base((int[]){4},1,5));

   ASSERT_EQUAL_U(0, inform_encode_base((int[]){0,1},1,5));
   ASSERT_EQUAL_U(1, inform_encode_base((int[]){1,1},1,5));
   ASSERT_EQUAL_U(2, inform_encode_base((int[]){2,1},1,5));
   ASSERT_EQUAL_U(3, inform_encode_base((int[]){3,1},1,5));
   ASSERT_EQUAL_U(4, inform_encode_base((int[]){4,1},1,5));
}

CTEST(StateEncoding, EncodeLengthTwo_Base2)
{
   ASSERT_EQUAL_U(0, inform_encode_base((int[]){0,0},2,2));
   ASSERT_EQUAL_U(1, inform_encode_base((int[]){1,0},2,2));
   ASSERT_EQUAL_U(2, inform_encode_base((int[]){0,1},2,2));
   ASSERT_EQUAL_U(3, inform_encode_base((int[]){1,1},2,2));

   ASSERT_EQUAL_U(0, inform_encode_base((int[]){0,0,0},2,2));
   ASSERT_EQUAL_U(1, inform_encode_base((int[]){1,0,0},2,2));
   ASSERT_EQUAL_U(2, inform_encode_base((int[]){0,1,0},2,2));
   ASSERT_EQUAL_U(3, inform_encode_base((int[]){1,1,0},2,2));
   ASSERT_EQUAL_U(0, inform_encode_base((int[]){0,0,1},2,2));
   ASSERT_EQUAL_U(1, inform_encode_base((int[]){1,0,1},2,2));
   ASSERT_EQUAL_U(2, inform_encode_base((int[]){0,1,1},2,2));
   ASSERT_EQUAL_U(3, inform_encode_base((int[]){1,1,1},2,2));
}

CTEST(StateEncoding, EncodeLengthTwo_Base5)
{
   ASSERT_EQUAL_U(0, inform_encode_base((int[]){0,0},2,5));
   ASSERT_EQUAL_U(1, inform_encode_base((int[]){1,0},2,5));
   ASSERT_EQUAL_U(2, inform_encode_base((int[]){2,0},2,5));
   ASSERT_EQUAL_U(3, inform_encode_base((int[]){3,0},2,5));
   ASSERT_EQUAL_U(4, inform_encode_base((int[]){4,0},2,5));
   ASSERT_EQUAL_U(5, inform_encode_base((int[]){0,1},2,5));
   ASSERT_EQUAL_U(6, inform_encode_base((int[]){1,1},2,5));
   ASSERT_EQUAL_U(7, inform_encode_base((int[]){2,1},2,5));
   ASSERT_EQUAL_U(8, inform_encode_base((int[]){3,1},2,5));
   ASSERT_EQUAL_U(9, inform_encode_base((int[]){4,1},2,5));
   ASSERT_EQUAL_U(10, inform_encode_base((int[]){0,2},2,5));
   ASSERT_EQUAL_U(11, inform_encode_base((int[]){1,2},2,5));
   ASSERT_EQUAL_U(12, inform_encode_base((int[]){2,2},2,5));
   ASSERT_EQUAL_U(13, inform_encode_base((int[]){3,2},2,5));
   ASSERT_EQUAL_U(14, inform_encode_base((int[]){4,2},2,5));
   ASSERT_EQUAL_U(15, inform_encode_base((int[]){0,3},2,5));
   ASSERT_EQUAL_U(16, inform_encode_base((int[]){1,3},2,5));
   ASSERT_EQUAL_U(17, inform_encode_base((int[]){2,3},2,5));
   ASSERT_EQUAL_U(18, inform_encode_base((int[]){3,3},2,5));
   ASSERT_EQUAL_U(19, inform_encode_base((int[]){4,3},2,5));
   ASSERT_EQUAL_U(20, inform_encode_base((int[]){0,4},2,5));
   ASSERT_EQUAL_U(21, inform_encode_base((int[]){1,4},2,5));
   ASSERT_EQUAL_U(22, inform_encode_base((int[]){2,4},2,5));
   ASSERT_EQUAL_U(23, inform_encode_base((int[]){3,4},2,5));
   ASSERT_EQUAL_U(24, inform_encode_base((int[]){4,4},2,5));
}

CTEST(StateEncoding, EncodeLengthThree_Base2)
{
   ASSERT_EQUAL_U(0, inform_encode_base((int[]){0,0,0},3,2));
   ASSERT_EQUAL_U(1, inform_encode_base((int[]){1,0,0},3,2));
   ASSERT_EQUAL_U(2, inform_encode_base((int[]){0,1,0},3,2));
   ASSERT_EQUAL_U(3, inform_encode_base((int[]){1,1,0},3,2));
   ASSERT_EQUAL_U(4, inform_encode_base((int[]){0,0,1},3,2));
   ASSERT_EQUAL_U(5, inform_encode_base((int[]){1,0,1},3,2));
   ASSERT_EQUAL_U(6, inform_encode_base((int[]){0,1,1},3,2));
   ASSERT_EQUAL_U(7, inform_encode_base((int[]){1,1,1},3,2));
}

CTEST(StateEncoding, EncodeLengthThree_Base3)
{
   ASSERT_EQUAL_U(0, inform_encode_base((int[]){0,0,0},3,3));
   ASSERT_EQUAL_U(1, inform_encode_base((int[]){1,0,0},3,3));
   ASSERT_EQUAL_U(2, inform_encode_base((int[]){2,0,0},3,3));
   ASSERT_EQUAL_U(3, inform_encode_base((int[]){0,1,0},3,3));
   ASSERT_EQUAL_U(4, inform_encode_base((int[]){1,1,0},3,3));
   ASSERT_EQUAL_U(5, inform_encode_base((int[]){2,1,0},3,3));
   ASSERT_EQUAL_U(6, inform_encode_base((int[]){0,2,0},3,3));
   ASSERT_EQUAL_U(7, inform_encode_base((int[]){1,2,0},3,3));
   ASSERT_EQUAL_U(8, inform_encode_base((int[]){2,2,0},3,3));
   ASSERT_EQUAL_U(9, inform_encode_base((int[]){0,0,1},3,3));
   ASSERT_EQUAL_U(10, inform_encode_base((int[]){1,0,1},3,3));
   ASSERT_EQUAL_U(11, inform_encode_base((int[]){2,0,1},3,3));
   ASSERT_EQUAL_U(12, inform_encode_base((int[]){0,1,1},3,3));
   ASSERT_EQUAL_U(13, inform_encode_base((int[]){1,1,1},3,3));
   ASSERT_EQUAL_U(14, inform_encode_base((int[]){2,1,1},3,3));
   ASSERT_EQUAL_U(15, inform_encode_base((int[]){0,2,1},3,3));
   ASSERT_EQUAL_U(16, inform_encode_base((int[]){1,2,1},3,3));
   ASSERT_EQUAL_U(17, inform_encode_base((int[]){2,2,1},3,3));
   ASSERT_EQUAL_U(18, inform_encode_base((int[]){0,0,2},3,3));
   ASSERT_EQUAL_U(19, inform_encode_base((int[]){1,0,2},3,3));
   ASSERT_EQUAL_U(20, inform_encode_base((int[]){2,0,2},3,3));
   ASSERT_EQUAL_U(21, inform_encode_base((int[]){0,1,2},3,3));
   ASSERT_EQUAL_U(22, inform_encode_base((int[]){1,1,2},3,3));
   ASSERT_EQUAL_U(23, inform_encode_base((int[]){2,1,2},3,3));
   ASSERT_EQUAL_U(24, inform_encode_base((int[]){0,2,2},3,3));
   ASSERT_EQUAL_U(25, inform_encode_base((int[]){1,2,2},3,3));
   ASSERT_EQUAL_U(26, inform_encode_base((int[]){2,2,2},3,3));
}

CTEST(StateEncoding, EncodingInvalidState_Base2)
{
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(3), inform_encode_base((int[]){-1},1,2));
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(3), inform_encode_base((int[]){2},1,2));
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(3), inform_encode_base((int[]){3},1,2));
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(4), inform_encode_base((int[]){0,2},2,2));
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(4), inform_encode_base((int[]){0,3},2,2));
}

CTEST(StateEncoding, EncodingInvalidState_Base3)
{
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(3), inform_encode_base((int[]){-1},1,3));
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(3), inform_encode_base((int[]){3},1,3));
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(3), inform_encode_base((int[]){4},1,3));
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(4), inform_encode_base((int[]){0,3},2,3));
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(4), inform_encode_base((int[]){0,4},2,3));
}

CTEST(StateEncoding, EncodingInvalidState_Base4)
{
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(3), inform_encode_base((int[]){-1},1,4));
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(3), inform_encode_base((int[]){4},1,4));
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(3), inform_encode_base((int[]){5},1,4));
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(4), inform_encode_base((int[]){0,4},2,4));
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(4), inform_encode_base((int[]){0,5},2,4));
}

CTEST(StateEncoding, EncodingInvalidState_Base5)
{
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(3), inform_encode_base((int[]){-1},1,5));
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(3), inform_encode_base((int[]){5},1,5));
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(3), inform_encode_base((int[]){6},1,5));
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(4), inform_encode_base((int[]){0,5},2,5));
    ASSERT_EQUAL_U(INFORM_ENCODING_ERROR(4), inform_encode_base((int[]){0,6},2,5));
}

CTEST(StateEncoding, EncodeInvertsDecode_Base2)
{
    int const base = 2;
    uint64_t const M = 10;
    for (uint64_t x = 0; x < (uint64_t)pow(base,M); ++x)
    {
       int *state = inform_decode_base(x,M,base);
       ASSERT_EQUAL_U(x, inform_encode_base(state,M,base));
       free(state);
    }
}

CTEST(StateEncoding, EncodeInvertsDecode_Base3)
{
    int const base = 3;
    uint64_t const M = 6;
    for (uint64_t x = 0; x < (uint64_t)pow(base,M); ++x)
    {
       int *state = inform_decode_base(x,M,base);
       ASSERT_EQUAL_U(x, inform_encode_base(state,M,base));
       free(state);
    }
}

CTEST(StateEncoding, EncodeInvertsDecode_Base4)
{
    int const base = 4;
    uint64_t const M = 5;
    for (uint64_t x = 0; x < (uint64_t)pow(base,M); ++x)
    {
       int *state = inform_decode_base(x,M,base);
       ASSERT_EQUAL_U(x, inform_encode_base(state,M,base));
       free(state);
    }
}

CTEST(StateEncoding, EncodeInvertsDecode_Base5)
{
    int const base = 5;
    uint64_t const M = 4;
    for (uint64_t x = 0; x < (uint64_t)pow(base,M); ++x)
    {
       int *state = inform_decode_base(x,M,base);
       ASSERT_EQUAL_U(x, inform_encode_base(state,M,base));
       free(state);
    }
}

CTEST(StateEncoding, DecodeErrors)
{
    ASSERT_NULL(inform_decode_base(inform_encoding_error,1,2));
    ASSERT_NULL(inform_decode_base(inform_encoding_error + 1,1,2));
    ASSERT_NULL(inform_decode_base(inform_encoding_error + 2,1,2));

    ASSERT_NULL(inform_decode_base(inform_encoding_error,1,3));
    ASSERT_NULL(inform_decode_base(inform_encoding_error + 1,1,3));
    ASSERT_NULL(inform_decode_base(inform_encoding_error + 2,1,3));
}

CTEST(StateEncoding, DecodeTooManyBits_Base2)
{
    int *state = inform_decode_base(0,63,2);
    ASSERT_NOT_NULL(state);
    free(state);

    ASSERT_NULL(inform_decode_base(0,64,2));
}

CTEST(StateEncoding, DecodeTooManyBits_Base3)
{
    int *state = inform_decode_base(0,40,3);
    ASSERT_NOT_NULL(state);
    free(state);

    ASSERT_NULL(inform_decode_base(0,41,3));
}

CTEST(StateEncoding, DecodeTooManyBits_Base4)
{
    int *state = inform_decode_base(0,31,4);
    ASSERT_NOT_NULL(state);
    free(state);

    ASSERT_NULL(inform_decode_base(0,32,4));
}

CTEST(StateEncoding, DecodeTooManyBits_Base5)
{
    int *state = inform_decode_base(0,27,5);
    ASSERT_NOT_NULL(state);
    free(state);

    ASSERT_NULL(inform_decode_base(0,28,5));
}
