/*
  Copyright (c) 2009 Dave Gamble

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/
#include <common/bk_include.h>
#include "cJsontest.h"

#if CJSON_TEST_DEMO
#include <stdio.h>
#include "cJSON.h"
#include <os/mem.h>
#include "uart_pub.h"
#include "stdarg.h"
#include <common/bk_include.h>
#include <os/os.h>
#include "Error.h"
#include "portmacro.h"

/* a bunch of json: */
const char text1[] = "{\n\"name\": \"beken\", \n\"format\": {\"type\":\"rect\",\n\"interlace\": false,\"frame rate\": 24\n}\n}";
const char text2[] = "[\"Sunday\", \"Monday\", \"Tuesday\", \"Wednesday\", \"Thursday\", \"Friday\", \"Saturday\"]";

void json_item_show(cJSON *item)
{
    char *out = NULL;

    bk_printf("name:%s,", item->string);
    switch(item->type)
    {
    case cJSON_False:
        bk_printf("%s\n", "false");
        break;
    case cJSON_True:
        bk_printf("%s\n", "true");
        break;
    case cJSON_NULL:
        bk_printf("%s\n", "JSON_NULL");
        break;
    case cJSON_Number:
        bk_printf("%d\n", item->valueint);
        break;
    case cJSON_String:
        bk_printf("%s\n", item->valuestring);
        break;
    case cJSON_Array:
        bk_printf("%s\n", item->string);
        break;
    case cJSON_Object:
        out = cJSON_Print(item);
        bk_printf("%s\n", out);
        if(out)
            os_free(out);
        break;
    default:
        bk_printf("error\r\n");
        break;
    }
}

/* Parse text to JSON, then render back to text, and print! */
void doit(char *text)
{
    char *out;
    cJSON *json;

    json = cJSON_Parse(text);
    if (!json)
    {
        bk_printf("Error before: [%s]\n", cJSON_GetErrorPtr());
    }
    else
    {
        out = cJSON_Print(json);
        cJSON_Delete(json);
        bk_printf("%s\n", out);
        if(out)
            os_free(out);
    }
}

/* Used by some code below as an example datatype. */
struct record
{
    const char *precision;
    double lat, lon;
    const char *address, *city, *state, *zip, *country;
};

/* Create a bunch of objects as demonstration. */
void create_objects()
{
    cJSON *root, *fmt;
    char *out;
    int i = 0;
    cJSON *item;
    int array_size;
    cJSON *cmp;

    root = cJSON_CreateObject();

    cJSON_AddItemToObject(root, "Cmp", cJSON_CreateString("Beken"));
    cJSON_AddItemToObject(root, "fmt", fmt = cJSON_CreateObject());
    cJSON_AddStringToObject(fmt, "type", "rect");
    cJSON_AddNumberToObject(fmt, "wdth", 1920);
    cJSON_AddNumberToObject(fmt, "hght", 1080);
    cJSON_AddFalseToObject (fmt, "interlace");
    cJSON_AddNumberToObject(fmt, "rate",	24);

    json_item_show(root);

    cmp = cJSON_GetObjectItem(root, "fmt");
    if(!cmp)
    {
        bk_printf("no fmt\r\n");
        return;
    }

    array_size = cJSON_GetArraySize(cmp);
    bk_printf("array size is %d\r\n", array_size);

    for(i = 0; i < array_size; i++)
    {
        item = cJSON_GetArrayItem(cmp, i);
        json_item_show(item);
    }

    if(root)
        cJSON_Delete(root);
}


void cjson_test_main ( beken_thread_arg_t arg )
{
    bk_printf("\r\n\r\n cjson testing..................\r\n ");
    rtos_delay_milliseconds( 1500 );

    /* Process each json textblock by parsing, then rebuilding: */
    doit((char *)text1);

    /* Now some samplecode for building objects concisely: */
    create_objects();

    bk_printf(" cjson test over..................\r\n\r\n ");

    rtos_delete_thread( NULL );
}

int demo_start( void )
{
    bk_err_t err = kNoErr;

    err = rtos_create_thread( NULL, BEKEN_APPLICATION_PRIORITY,
                              "json_test",
                              (beken_thread_function_t)cjson_test_main,
                              0x800,
                              (beken_thread_arg_t)0 );
    return err;
}

#endif

