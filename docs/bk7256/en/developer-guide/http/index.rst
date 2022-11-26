HTTP Component WebClient
===================================

WebClient is ported from open source component of RT-Thread

Source Code Path
--------------------------

components/webclient


Process
--------------------------

For the process of HTTP GET/POST, please refer to test code ``test_http(void)`` , the detailed process is as follows

To send a GET/POST request using the WebClient software, the following basic processes are required

- 1. **Create the client session structure**

 ::

   struct  webclient_header
   {
       char *buffer;                       //header data added or retrieved
       size_t length;                      //current length of header data

       size_t size;                        //maximum length of supported header data
   };

   struct webclient_session
   {
       struct webclient_header *header;    //save the header structure
       int socket;                         //current connection socket
       int resp_status;                    //response status code

       char *host;                         //connection server address
       char *req_url;                      //request address for the connection

       int chunk_sz;                       //size of a chunk in chunk mode
       int chunk_offset;                   //remaining data size in chunk mode

       int content_length;                 //Length of currently received data (non-chunk mode)
       size_t content_remainder;           //current remaining received data length

       rt_bool_t is_tls;                   //current connection is HTTPS or not
   #ifdef WEBCLIENT_USING_MBED_TLS
       MbedTLSSession *tls_session;        //HTTPS protocol related session structure
   #endif
   };


The WebClient session structure stores part of information about currently established HTTP connection and can be used to interact with HTTP data. Before setting up a HTTP connection, user needs to create and initialize this structure. The following is an example of how to create the strucure
::

      struct webclient_session *session = RT_NULL;

      /* create webclient session and set header response size */
      session = webclient_session_create(1024);
      if (session == RT_NULL)
      {
          ret = -RT_ENOMEM;
          goto __exit;
      }


- 2. **splicing header data**

The WebClient package provides two ways to send request headers

 - Default header data

    If user wants to use the default header information, you don't need to concatenate any header data and can simply call the GET sending command. The default header data is generally only used for GET requests

 - Customize header data

    Custom header data is added using the `webclient_header_fields_add` function. The added header information is located in the client session structure and sent by GET/POST request

 Example code is as follows
 ::

   /* splicing Header information */
   webclient_header_fields_add(session, "Content-Length: %d\r\n", strlen(post_data));

   webclient_header_fields_add(session, "Content-Type: application/octet-stream\r\n");


- 3. **send GET/POST request**

After the header information is added, user can call the `webclient_get` function or `webclient_post` function to send the GET/POST request command. The main operations of the function are as follows

  - the incoming URL is used to obtain information and established a TCP connection

  - send default or concatenated header information

  - receives and parses the header of the response data

  - returns an error or response status code

Example code for sending a GET request
::

   int resp_status = 0;

   /* send GET request by default header */
   if ((resp_status = webclient_get(session, URI)) != 200)
   {
       LOG_E("webclient GET request failed, response(%d) error.", resp_status);
       ret = -RT_ERROR;
       goto __exit;
   }


- 4. **receive the response data**

After sending the GET/POST request, user can use the `webclient_read` function to receive the actual data for response. Because the actual data of response may be long, we often need to receive the response data in a loop to guide the data receiving completion. The following is a loop to receive and print the response data
::

   int content_pos = 0;
   /* gets the lenght of received response data */
   int content_length = webclient_content_length_get(session);

   /* Receive response data until completed in the loop */
   do
   {
       bytes_read = webclient_read(session, buffer, 1024);
       if (bytes_read <= 0)
       {
           break;
       }

       /* print response data */
       for (index = 0; index < bytes_read; index++)
       {
           rt_kprintf("%c", buffer[index]);
       }

       content_pos += bytes_read;
   } while (content_pos < content_length);


- 5. **close and release the client session structure**

After the request is sent and received, user needs to close and release the client session structure with `webclient_close` function to complete the HTTP data exchange process

the usage is as follows
::

   if (session)
   {
       webclient_close(session);
   }


 - usage

 The WebClient software provides several different ways to use GET/POST requests for different situations

 - GET request mode

send GET request with default header
::

   struct webclient_session *session = NULL;

   session = webclient_create(1024);

   if(webclient_get(session, URI) != 200)
   {
       LOG_E("error!");
   }

   while(1)
   {
       webclient_read(session, buffer, bfsz);
       ...
   }

   webclient_close(session);


send GET request with custom header
::

   struct webclient_session *session = NULL;

   session = webclient_create(1024);

   webclient_header_fields_add(session, "User-Agent: RT-Thread HTTP Agent\r\n");

   if(webclient_get(session, URI) != 200)
   {
       LOG_E("error!");
   }

   while(1)
   {
       webclient_read(session, buffer, bfsz);
       ...
   }

   webclient_close(session);


send GET request to get part of data (mostly used for resumable data from breakpoints)
::

   struct webclient_session *session = NULL;

   session = webclient_create(1024);

   if(webclient_get_position(URI, 100) != 206)
   {
       LOG_E("error!");
   }

   while(1)
   {
       webclient_read(session, buffer, bfsz);
       ...
   }

   webclient_close(session)；


`webclient_response` is used to receive GET data, which is mainly used to receive GET requests with less data length
::

   struct webclient_session *session = NULL;
   size_t length = 0;
   char *result;

   session = webclient_create(1024);

   if(webclient_get(session, URI) != 200)
   {
       LOG_E("error!");
   }

   webclient_response(session, &result, &length);

   web_free(result);
   webclient_close(session);


`webclient_request` is used to send and receive a GET request, which is usually for less data length and whose header information has been concatenated
::

   size_t length = 0;
   char *result, *header = RT_NULL;

   /* concatenate custom header data */
   webclient_request_header_add(&header, "User-Agent: RT-Thread HTTP Agent\r\n");

   webclient_request(URI, header, NULL, 0, &result, &length);

   web_free(result);


 -  POST request mode

POST requests for piecewise data are mainly used for POST requests with a large amount of data to be uploaded. For example, uploading files to the server
::

   struct webclient_session *session = NULL;

   session = webclient_create(1024);

   /* Concatenates the necessary header information */
   webclient_header_fields_add(session, "Content-Length: %d\r\n", post_data_sz);
   webclient_header_fields_add(session, "Content-Type: application/octet-stream\r\n");

   /* when the third paramenter of webclient_post is NULL, changing to the following loop to upload data*/
   if( webclient_post(session, URI, NULL, 0) != 200)
   {
       LOG_E("error!");
   }

   while(1)
   {
       webclient_write(session, post_data, 1024);
       ...
   }

   if( webclient_handle_response(session) != 200)
   {
       LOG_E("error!");
   }

   webclient_close(session);


POST requests for whole data, mainly used for POST requests with less amount of uploaded data
::

   char *post_data = "abcdefg";

   session = webclient_create(1024);

   /* Concatenates the necessary header information */
   webclient_header_fields_add(session, "Content-Length: %d\r\n", strlen(post_data));
   webclient_header_fields_add(session, "Content-Type: application/octet-stream\r\n");

   if(webclient_post(session, URI, post_data, rt_strlen(post_data)) != 200);
   {
       LOG_E("error!");
   }
   webclient_close(session);

`webclient_request` function is used to send a POST request which is usually used for uploading a small file and the header information has been concatenated
::

   char *post_data = "abcdefg";
   char *header = RT_NULL;

   /* concatenate custom header data */
   webclient_request_header_add(&header, "Content-Length: %d\r\n", strlen(post_data));
   webclient_request_header_add(&header, "Content-Type: application/octet-stream\r\n");

   webclient_request(URI, header, post_data, rt_strlen(post_data), NULL, NULL);


FAQ
-------------------------------

- The HTTPS address does not support::

   [E/WEB]not support https connect, please enable webclient https configure!


 - Reason: HTTPs address is used but HTTPS is not enabled

 - Solution：In menuconfig of WebClient software, enable `MbedTLS support` or `SAL TLS support` from option "Select TLS mode"


- The header data length exceeds::

   [E/WEB]not enough header buffer size(xxx)!

 - Reason：The lenght of added header data exceeds the maximum supported

 - Solution：When creating the client session structure, increase the maximum supported header data length passed in


