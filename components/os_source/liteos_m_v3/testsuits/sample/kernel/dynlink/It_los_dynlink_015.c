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
#include "los_dynlink.h"
#include "It_los_dynlink.h"

/* Test that interfaces in dynamic_athmtc.so */
STATIC UINT32 TestCase(VOID)
{
    VOID *handle = NULL;
    VOID (*func0)(INT32, INT32 *) = NULL;
    VOID (*func1)(CHAR *, CHAR *) = NULL;
    CHAR *symbolName1 = "It_dynlink_dowhile";
    CHAR *symbolName2 = "It_dynlink_while";
    CHAR *symbolName3 = "It_dynlink_for";
    CHAR *symbolName4 = "It_dynlink_ifelse";
    CHAR *symbolName5 = "It_dynlink_continue";
    CHAR *symbolName6 = "It_dynlink_switch";
    CHAR *dsoName = DSO_FULL_PATH("dynamic_athmtc.so");
    INT32 out = 0;
    CHAR inChar[3];
    CHAR outChar[3] = { 0 };
    INT32 ret;

    handle = (VOID *)LOS_SoLoad(dsoName, NULL);
    ICUNIT_ASSERT_NOT_EQUAL(handle, NULL, handle);

    func0 = (VOID (*)(INT32, INT32 *))LOS_FindSym(handle, symbolName1);
    ICUNIT_GOTO_NOT_EQUAL(func0, NULL, func0, EXIT);
    func0(1, &out);
    ICUNIT_GOTO_EQUAL(out, 2, out, EXIT);

    func0 = (VOID (*)(INT32, INT32 *))LOS_FindSym(handle, symbolName2);
    ICUNIT_GOTO_NOT_EQUAL(func0, NULL, func0, EXIT);
    func0(2, &out);
    ICUNIT_GOTO_EQUAL(out, 3, out, EXIT);

    func0 = (VOID (*)(INT32, INT32 *))LOS_FindSym(handle, symbolName3);
    ICUNIT_GOTO_NOT_EQUAL(func0, NULL, func0, EXIT);
    func0(3, &out);
    ICUNIT_GOTO_EQUAL(out, 4, out, EXIT);

    func0 = (VOID (*)(INT32, INT32 *))LOS_FindSym(handle, symbolName4);
    ICUNIT_GOTO_NOT_EQUAL(func0, NULL, func0, EXIT);
    func0(105, &out);
    ICUNIT_GOTO_EQUAL(out, 105, out, EXIT);

    func0 = (VOID (*)(INT32, INT32 *))LOS_FindSym(handle, symbolName5);
    ICUNIT_GOTO_NOT_EQUAL(func0, NULL, func0, EXIT);
    func0(10, &out);
    ICUNIT_GOTO_EQUAL(out, 6, out, EXIT);

    func1 = (VOID (*)(CHAR *, CHAR *))LOS_FindSym(handle, symbolName6);
    ICUNIT_GOTO_NOT_EQUAL(func1, NULL, func1, EXIT);
    inChar[0] = 'a';
    inChar[1] = 'c';
    inChar[2] = 'q';
    func1(inChar, outChar);
    ICUNIT_GOTO_EQUAL(outChar[0], 'b', outChar[0], EXIT);
    ICUNIT_GOTO_EQUAL(outChar[1], 'd', outChar[1], EXIT);
    ICUNIT_GOTO_EQUAL(outChar[2], 'z', outChar[2], EXIT);

EXIT:
    ret = LOS_SoUnload(handle);
    ICUNIT_ASSERT_EQUAL(ret, 0, ret);

    return LOS_OK;
}

VOID ItLosDynlink015(VOID)
{
    TEST_ADD_CASE("ItLosDynlink015", TestCase, TEST_LOS, TEST_DYNLINK, TEST_LEVEL1, TEST_FUNCTION);
}
