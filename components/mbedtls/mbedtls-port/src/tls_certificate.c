/*
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of mbed TLS (https://tls.mbed.org)
 */

#include "certs.h"

#if 0
#define ENTRUST_ROOT_CA                                                 \
"-----BEGIN CERTIFICATE-----\r\n"                                       \
"MIIEPjCCAyagAwIBAgIESlOMKDANBgkqhkiG9w0BAQsFADCBvjELMAkGA1UEBhMC\r\n"  \
"VVMxFjAUBgNVBAoTDUVudHJ1c3QsIEluYy4xKDAmBgNVBAsTH1NlZSB3d3cuZW50\r\n"  \
"cnVzdC5uZXQvbGVnYWwtdGVybXMxOTA3BgNVBAsTMChjKSAyMDA5IEVudHJ1c3Qs\r\n"  \
"IEluYy4gLSBmb3IgYXV0aG9yaXplZCB1c2Ugb25seTEyMDAGA1UEAxMpRW50cnVz\r\n"  \
"dCBSb290IENlcnRpZmljYXRpb24gQXV0aG9yaXR5IC0gRzIwHhcNMDkwNzA3MTcy\r\n"  \
"NTU0WhcNMzAxMjA3MTc1NTU0WjCBvjELMAkGA1UEBhMCVVMxFjAUBgNVBAoTDUVu\r\n"  \
"dHJ1c3QsIEluYy4xKDAmBgNVBAsTH1NlZSB3d3cuZW50cnVzdC5uZXQvbGVnYWwt\r\n"  \
"dGVybXMxOTA3BgNVBAsTMChjKSAyMDA5IEVudHJ1c3QsIEluYy4gLSBmb3IgYXV0\r\n"  \
"aG9yaXplZCB1c2Ugb25seTEyMDAGA1UEAxMpRW50cnVzdCBSb290IENlcnRpZmlj\r\n"  \
"YXRpb24gQXV0aG9yaXR5IC0gRzIwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEK\r\n"  \
"AoIBAQC6hLZy254Ma+KZ6TABp3bqMriVQRrJ2mFOWHLP/vaCeb9zYQYKpSfYs1/T\r\n"  \
"RU4cctZOMvJyig/3gxnQaoCAAEUesMfnmr8SVycco2gvCoe9amsOXmXzHHfV1IWN\r\n"  \
"cCG0szLni6LVhjkCsbjSR87kyUnEO6fe+1R9V77w6G7CebI6C1XiUJgWMhNcL3hW\r\n"  \
"wcKUs/Ja5CeanyTXxuzQmyWC48zCxEXFjJd6BmsqEZ+pCm5IO2/b1BEZQvePB7/1\r\n"  \
"U1+cPvQXLOZprE4yTGJ36rfo5bs0vBmLrpxR57d+tVOxMyLlbc9wPBr64ptntoP0\r\n"  \
"jaWvYkxN4FisZDQSA/i2jZRjJKRxAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAP\r\n"  \
"BgNVHRMBAf8EBTADAQH/MB0GA1UdDgQWBBRqciZ60B7vfec7aVHUbI2fkBJmqzAN\r\n"  \
"BgkqhkiG9w0BAQsFAAOCAQEAeZ8dlsa2eT8ijYfThwMEYGprmi5ZiXMRrEPR9RP/\r\n"  \
"jTkrwPK9T3CMqS/qF8QLVJ7UG5aYMzyorWKiAHarWWluBh1+xLlEjZivEtRh2woZ\r\n"  \
"Rkfz6/djwUAFQKXSt/S1mja/qYh2iARVBCuch38aNzx+LaUa2NSJXsq9rD1s2G2v\r\n"  \
"1fN2D807iDginWyTmsQ9v4IbZT+mD12q/OWyFcq1rca8PdCE6OoGcrBNOTJ4vz4R\r\n"  \
"nAuknZoh8/CbCzB428Hch0P+vGOaysXCHMnHjf87ElgI5rY97HosTvuDls4MPGmH\r\n"  \
"VHOkc8KT/1EQrBVUAdj8BbGJoX90g5pJ19xOe4pIb4tF9g==\r\n"                  \
"-----END CERTIFICATE-----\r\n"


const char mbedtls_root_certificate[] = 
	ENTRUST_ROOT_CA
	;
#elif 0
/////www.baidu.com
#define GLOBALSIGN_ROOT_CA             \
"-----BEGIN CERTIFICATE-----\r\n"                                       \
"MIIDdTCCAl2gAwIBAgILBAAAAAABFUtaw5QwDQYJKoZIhvcNAQEFBQAwVzELMAkG\r\n"  \
"A1UEBhMCQkUxGTAXBgNVBAoTEEdsb2JhbFNpZ24gbnYtc2ExEDAOBgNVBAsTB1Jv\r\n"  \
"b3QgQ0ExGzAZBgNVBAMTEkdsb2JhbFNpZ24gUm9vdCBDQTAeFw05ODA5MDExMjAw\r\n"  \
"MDBaFw0yODAxMjgxMjAwMDBaMFcxCzAJBgNVBAYTAkJFMRkwFwYDVQQKExBHbG9i\r\n"  \
"YWxTaWduIG52LXNhMRAwDgYDVQQLEwdSb290IENBMRswGQYDVQQDExJHbG9iYWxT\r\n"  \
"aWduIFJvb3QgQ0EwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDaDuaZ\r\n"  \
"jc6j40+Kfvvxi4Mla+pIH/EqsLmVEQS98GPR4mdmzxzdzxtIK+6NiY6arymAZavp\r\n"  \
"xy0Sy6scTHAHoT0KMM0VjU/43dSMUBUc71DuxC73/OlS8pF94G3VNTCOXkNz8kHp\r\n"  \
"1Wrjsok6Vjk4bwY8iGlbKk3Fp1S4bInMm/k8yuX9ifUSPJJ4ltbcdG6TRGHRjcdG\r\n"  \
"snUOhugZitVtbNV4FpWi6cgKOOvyJBNPc1STE4U6G7weNLWLBYy5d4ux2x8gkasJ\r\n"  \
"U26Qzns3dLlwR5EiUWMWea6xrkEmCMgZK9FGqkjWZCrXgzT/LCrBbBlDSgeF59N8\r\n"  \
"9iFo7+ryUp9/k5DPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNVHRMBAf8E\r\n"  \
"BTADAQH/MB0GA1UdDgQWBBRge2YaRQ2XyolQL30EzTSo//z9SzANBgkqhkiG9w0B\r\n"  \
"AQUFAAOCAQEA1nPnfE920I2/7LqivjTFKDK1fPxsnCwrvQmeU79rXqoRSLblCKOz\r\n"  \
"yj1hTdNGCbM+w6DjY1Ub8rrvrTnhQ7k4o+YviiY776BQVvnGCv04zcQLcFGUl5gE\r\n"  \
"38NflNUVyRRBnMRddWQVDf9VMOyGj/8N7yy5Y0b2qvzfvGn9LhJIZJrglfCm7ymP\r\n"  \
"AbEVtQwdpf5pLGkkeB6zpxxxYu7KyJesF12KwvhHhm4qxFYxldBniYUr+WymXUad\r\n"  \
"DKqC5JlR3XC321Y9YeRq4VzW9v493kHMB65jUr9TU/Qr6cf9tveCX4XSQRjbgbME\r\n"  \
"HMUfpIBvFSDJ3gyICh3WZlXi/EjJKSZp4A==\r\n"                              \
"-----END CERTIFICATE-----"

const char mbedtls_root_certificate[] = 
	GLOBALSIGN_ROOT_CA
;
#elif 1
////myca for mqtt
#define GLOBALSIGN_ROOT_CA   \
"-----BEGIN CERTIFICATE-----\r\n"  \
"MIID3zCCAsegAwIBAgIJAODxoOUE+Ia8MA0GCSqGSIb3DQEBCwUAMIGFMQswCQYD\r\n"  \
"VQQGEwJDTjEQMA4GA1UECAwHc2hhZ2hhaTERMA8GA1UEBwwIc2hhbmdoYWkxDjAM\r\n"  \
"BgNVBAoMBUJFS0VOMRAwDgYDVQQLDAd3aWZpLWdlMRIwEAYDVQQDDAl3aWZpLXRl\r\n"  \
"YW0xGzAZBgkqhkiG9w0BCQEWDGN3bGluMTYzLmNvbTAeFw0yMDA4MTEwNTQ3Mjha\r\n"  \
"Fw0zMDA4MDkwNTQ3MjhaMIGFMQswCQYDVQQGEwJDTjEQMA4GA1UECAwHc2hhZ2hh\r\n"  \
"aTERMA8GA1UEBwwIc2hhbmdoYWkxDjAMBgNVBAoMBUJFS0VOMRAwDgYDVQQLDAd3\r\n"  \
"aWZpLWdlMRIwEAYDVQQDDAl3aWZpLXRlYW0xGzAZBgkqhkiG9w0BCQEWDGN3bGlu\r\n"  \
"MTYzLmNvbTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALwa6fM/5xuR\r\n"  \
"nAHQZ+pDDOAK6EJf04yO28G9NGPYP2mQKhkBhcYC9G9xXLglvuCfXFKpsXxMMOYI\r\n"  \
"LjQWL8iN0M6tyt+t7kEbSlBwac0IuqjB62LvK623XvX7k7an/E87oCZih6TYzCy7\r\n"  \
"5HGIELhODa/8sHG5HT8gkqSCJscKbpCocwj13PPs6vwloCmuG0LxB5xb3CkWQ8Kv\r\n"  \
"DCIF2jBTNbnYixlLYZ/P0SQMlHGLcOqTPvWEnowA1VqgNBCDVyln26XM87AV+qQB\r\n"  \
"5dBcLOZmnX0cpg+SFG+wrNUFN4WKI8Pl2suUiD4jeIixAWS2pyOsm8SvrM1YipM0\r\n"  \
"6IdqUQIeCLsCAwEAAaNQME4wHQYDVR0OBBYEFG7AMq4Tgm/sPp9Nl81gL1/YOcrQ\r\n"  \
"MB8GA1UdIwQYMBaAFG7AMq4Tgm/sPp9Nl81gL1/YOcrQMAwGA1UdEwQFMAMBAf8w\r\n"  \
"DQYJKoZIhvcNAQELBQADggEBACPRNVhHXi+8o5hSSX6dBqdUljeAvN2lDYFgVWPr\r\n"  \
"E3teVQADS3kd/60ACLc34ZP+OojlPMz3zbDosH5gRntwFGJUGqkJgQt7xUS2wgyo\r\n"  \
"fuxLNrnz2dIkVfDA/q4GpPVBZwlFKjEc1N6P7ENiJ/DJLkvYi78NDneComid08q2\r\n"  \
"OAbAJjTcUSOYbEygS2FPgFRR2O4s9nrphDgQIzWLgeHIhT7feSyQ7d0emMUv1iEQ\r\n"  \
"LXJU9sbgYU2G02D51tdz+tFObmtGxiy99gEunK6S9AeUnh8xM5eTJW2ebe0CdFmo\r\n"  \
"JkLASY80XmUo6WQVq7iN6mO3zsgIZmwmwSVnTcSPcHARoZ4=\r\n"  \
"-----END CERTIFICATE-----\r\n"

const char mbedtls_root_certificate[] = 
	GLOBALSIGN_ROOT_CA
;
#endif


const size_t mbedtls_root_certificate_len = sizeof(mbedtls_root_certificate);

#if 0
///client client.crt
#define GLOBALSIGN_Client_crt   \
"-----BEGIN CERTIFICATE-----\r\n"  \
"MIIDkDCCAngCCQCb4RI7B1uRIDANBgkqhkiG9w0BAQsFADCBhTELMAkGA1UEBhMC\r\n"  \
"Q04xEDAOBgNVBAgMB3NoYWdoYWkxETAPBgNVBAcMCHNoYW5naGFpMQ4wDAYDVQQK\r\n"  \
"DAVCRUtFTjEQMA4GA1UECwwHd2lmaS1nZTESMBAGA1UEAwwJd2lmaS10ZWFtMRsw\r\n"  \
"GQYJKoZIhvcNAQkBFgxjd2xpbjE2My5jb20wHhcNMjAwODExMDU1MzI1WhcNMzAw\r\n"  \
"ODA5MDU1MzI1WjCBjTELMAkGA1UEBhMCQ04xETAPBgNVBAgMCHNoYW5naGFpMREw\r\n"  \
"DwYDVQQHDAhzaGFuZ2hhaTEOMAwGA1UECgwFYmVrZW4xEzARBgNVBAsMCmJla2Vu\r\n"  \
"LXdpZmkxFjAUBgNVBAMMDTE5Mi4xNjguMTkuMjQxGzAZBgkqhkiG9w0BCQEWDGN3\r\n"  \
"bGluMTYzLmNvbTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAMSKa+Cn\r\n"  \
"9wVL7H1/zY02jJTHmq3Y6XfMwwkHsCE3Oag62snjNs60LQCYIQvGMXRmz9hIuWRW\r\n"  \
"hAA627TLIAvGhXG5mFkuG3HJJLQLdTGuWobjTL74/hrSAstmJM/eg9M1FHvWO9rq\r\n"  \
"bTNagybd9k2BjdZdah1QFlDagECtAbBKq89riKqvlwaIJP43Xb0dYXZ223iHsNDv\r\n"  \
"wYO9TU9Cl7U49UURUA7hw649cPtVA++uKQJSsqEpHF1/Q0fJcCnFqfPSZsqdhBmO\r\n"  \
"p4S1t+xqpautEiGWyBcdOZ1Og4a5gio4hBt4hDpLWXIF/mrjGMvImm4RlOkAhbil\r\n"  \
"6nZz+ZwXl2RD4ZkCAwEAATANBgkqhkiG9w0BAQsFAAOCAQEAkY7PGZkxn4zoV+/a\r\n"  \
"EBcZ06V9PqCYtn+8/r4ciBW1GHjveOXX5plVkaKpkx1QFOS5RV96vk4N10HnKO9V\r\n"  \
"YjRqPN3maiQ+bn3Yve0zopMPbNiA+oTZqrP8jRlpTujCo7XE6fKXfQYpRXijXDHB\r\n"  \
"Ty4pURq8UkEmyZyFoSUYgib2aG0K656OOG5LSRRda4NSWJilhLB5Aheb7gfVcLut\r\n"  \
"eUq28qFSx0BC+Yf2IaMMnOIO1HD8mgdEgktGlX7eapAive3g/sFmaX9TPzV6hJ5U\r\n"  \
"s4OvpG2D3BzwJH5pmoi1ovOi2m0WauR8Z3W3m7kywLEUtfEbX0W+Q02CPMYw1jmi\r\n"  \
"D/qgUw==\r\n"  \
"-----END CERTIFICATE-----\r\n"


const char mbedtls_client_certificate[] =
	GLOBALSIGN_ROOT_CA
	GLOBALSIGN_Client_crt
;
const size_t mbedtls_client_certificate_len = sizeof(mbedtls_client_certificate);
#endif



