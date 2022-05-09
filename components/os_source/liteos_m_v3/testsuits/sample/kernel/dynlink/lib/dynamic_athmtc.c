/*
 * Copyright (c) 2021-2021 Huawei Device Co., Ltd. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 *    conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 *    of conditions and the following disclaimer in the documentation and/or other materials
 *    provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 *    to endorse or promote products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#define OK          0
#define NOK         1

void It_dynlink_dowhile(int cnt, int *out)
{
    int outParam = 0;
    cnt += 1;

    do {
        ++outParam;
        --cnt;
    } while (cnt);

    *out = outParam;
}

void It_dynlink_while(int cnt, int *out)
{
    int index = 0;
    int outParam = 0;
    cnt += 1;

    while ((index++) < cnt) {
        outParam += 1;
    }

    *out = outParam;
}

void It_dynlink_for(int cnt, int *out)
{
    int index = 0;
    int outParam = 0;
    cnt += 1;

    for (; index < cnt; ++index) {
        ++outParam;
    }

    *out = outParam;
}

void It_dynlink_ifelse(int inVal, int *outVal)
{
    int outParam;

    if (inVal <= 101) {
        outParam = 101;
    } else if (inVal == 102) {
        outParam = inVal;
    } else if (inVal == 103) {
        outParam = inVal;
    } else if (inVal == 104) {
        outParam = inVal;
    } else if (inVal == 105) {
        outParam = inVal;
    } else if (inVal == 106) {
        outParam = inVal;
    } else if (inVal == 107) {
        outParam = inVal;
    } else if (inVal == 108) {
        outParam = inVal + 1;
    } else if (inVal == 109) {
        outParam = 45;
    } else {
        outParam = 45;
    }

    *outVal = outParam;
}

void It_dynlink_continue(int cnt, int *out)
{
    int index = 0;
    int outParam = 0;
    cnt += 2;

    for (; index < cnt; ++index) {
        if (index % 2) {
            continue;
        } else {
            ++outParam;
        }
    }

    *out = outParam;
}

void It_dynlink_switch(char *inVal, char *outVal)
{
    int i;
    char outParam;

    for (i = 0; i < 3; ++i) {
        switch (inVal[i]) {
            case 'A':
            case 'a':
                outParam = 'b';
                break;
            case 'B':
            case 'b':
                outParam = 'c';
                break;
            case 'C':
            case 'c':
                outParam = 'd';
                break;
            case 'D':
            case 'd':
                outParam = 'e';
                break;
            case 'E':
            case 'e':
                outParam = 'f';
                break;
            case 'F':
            case 'f':
                outParam = 'g';
                break;
            case 'G':
            case 'g':
                outParam = 'h';
                break;
            case 'H':
            case 'h':
                outParam = 'i';
                break;
            default:
                outParam = 'z';
                break;
        }
        outVal[i] = outParam;
    }
}
