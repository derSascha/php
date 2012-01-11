/*
   +----------------------------------------------------------------------+
   | PHP Version 5                                                        |
   +----------------------------------------------------------------------+
   | Copyright (c) 1997-2012 The PHP Group                                |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Author: Kirill Maximov <kir@rus.net>                                 |
   +----------------------------------------------------------------------+
 */

/* $Id: cyr_convert.c 321634 2012-01-01 13:15:04Z felipe $ */

#include <stdlib.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <string.h>
#include <errno.h>

#include "php.h"
#include "cyr_convert.h"

#include <stdio.h>

/*****************************************************************************
* This is codetables for different Cyrillic charsets (relative to koi8-r). 
* Each table contains data for 128-255 symbols from ASCII table.
* First 256 symbols are for conversion from koi8-r to corresponding charset,
* second 256 symbols are for reverse conversion, from charset to koi8-r.
*
* Here we have the following tables:
* _cyr_win1251   - for windows-1251 charset
* _cyr_iso88595  - for iso8859-5 charset
* _cyr_cp866     - for x-cp866 charset
* _cyr_mac       - for x-mac-cyrillic charset
*
*****************************************************************************/

typedef unsigned char _cyr_charset_table[512];

/* {{{ static const _cyr_charset_table _cyr_win1251
 */
static const _cyr_charset_table _cyr_win1251 = {
0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,
48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,
64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,
80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,
96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,
112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,
46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,
46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,
154,174,190,46,159,189,46,46,179,191,180,157,46,46,156,183,
46,46,182,166,173,46,46,158,163,152,164,155,46,46,46,167,
225,226,247,231,228,229,246,250,233,234,235,236,237,238,239,240,
242,243,244,245,230,232,227,254,251,253,255,249,248,252,224,241,
193,194,215,199,196,197,214,218,201,202,203,204,205,206,207,208,
210,211,212,213,198,200,195,222,219,221,223,217,216,220,192,209,
0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,
48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,
64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,
80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,
96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,
112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,184,186,32,179,191,32,32,32,32,32,180,162,32,
32,32,32,168,170,32,178,175,32,32,32,32,32,165,161,169,
254,224,225,246,228,229,244,227,245,232,233,234,235,236,237,238,
239,255,240,241,242,243,230,226,252,251,231,248,253,249,247,250,
222,192,193,214,196,197,212,195,213,200,201,202,203,204,205,206,
207,223,208,209,210,211,198,194,220,219,199,216,221,217,215,218,
},
_cyr_cp866 = { 
0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,
48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,
64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,
80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,
96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,
112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,
225,226,247,231,228,229,246,250,233,234,235,236,237,238,239,240,
242,243,244,245,230,232,227,254,251,253,255,249,248,252,224,241,
193,194,215,199,196,197,214,218,201,202,203,204,205,206,207,208,
35,35,35,124,124,124,124,43,43,124,124,43,43,43,43,43,
43,45,45,124,45,43,124,124,43,43,45,45,124,45,43,45,
45,45,45,43,43,43,43,43,43,43,43,35,35,124,124,35,
210,211,212,213,198,200,195,222,219,221,223,217,216,220,192,209,
179,163,180,164,183,167,190,174,32,149,158,32,152,159,148,154,
0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,
48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,
64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,
80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,
96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,
112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
205,186,213,241,243,201,32,245,187,212,211,200,190,32,247,198,
199,204,181,240,242,185,32,244,203,207,208,202,216,32,246,32,
238,160,161,230,164,165,228,163,229,168,169,170,171,172,173,174,
175,239,224,225,226,227,166,162,236,235,167,232,237,233,231,234,
158,128,129,150,132,133,148,131,149,136,137,138,139,140,141,142,
143,159,144,145,146,147,134,130,156,155,135,152,157,153,151,154,
},
_cyr_iso88595 = {
0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,
48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,
64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,
80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,
96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,
112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,179,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
225,226,247,231,228,229,246,250,233,234,235,236,237,238,239,240,
242,243,244,245,230,232,227,254,251,253,255,249,248,252,224,241,
193,194,215,199,196,197,214,218,201,202,203,204,205,206,207,208,
210,211,212,213,198,200,195,222,219,221,223,217,216,220,192,209,
32,163,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,
48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,
64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,
80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,
96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,
112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,241,32,32,32,32,32,32,32,32,32,32,32,32,
32,32,32,161,32,32,32,32,32,32,32,32,32,32,32,32,
238,208,209,230,212,213,228,211,229,216,217,218,219,220,221,222,
223,239,224,225,226,227,214,210,236,235,215,232,237,233,231,234,
206,176,177,198,180,181,196,179,197,184,185,186,187,188,189,190,
191,207,192,193,194,195,182,178,204,203,183,200,205,201,199,202,
},
_cyr_mac = {
0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,
48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,
64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,
80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,
96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,
112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,
225,226,247,231,228,229,246,250,233,234,235,236,237,238,239,240,
242,243,244,245,230,232,227,254,251,253,255,249,248,252,224,241,
160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,
176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,
128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,
144,145,146,147,148,149,150,151,152,153,154,155,156,179,163,209,
193,194,215,199,196,197,214,218,201,202,203,204,205,206,207,208,
210,211,212,213,198,200,195,222,219,221,223,217,216,220,192,255,
0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,
48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,
64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,
80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,
96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,
112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,
192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,
208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,
160,161,162,222,164,165,166,167,168,169,170,171,172,173,174,175,
176,177,178,221,180,181,182,183,184,185,186,187,188,189,190,191,
254,224,225,246,228,229,244,227,245,232,233,234,235,236,237,238,
239,223,240,241,242,243,230,226,252,251,231,248,253,249,247,250,
158,128,129,150,132,133,148,131,149,136,137,138,139,140,141,142,
143,159,144,145,146,147,134,130,156,155,135,152,157,153,151,154,
};
/* }}} */

/* {{{ static char * php_convert_cyr_string(unsigned char *str, int length, char from, char to TSRMLS_DC)
* This is the function that performs real in-place conversion of the string 
* between charsets. 
* Parameters:
*    str - string to be converted
*    from,to - one-symbol label of source and destination charset
* The following symbols are used as labels:
*    k - koi8-r
*    w - windows-1251
*    i - iso8859-5
*    a - x-cp866
*    d - x-cp866
*    m - x-mac-cyrillic
*****************************************************************************/
static char * php_convert_cyr_string(unsigned char *str, int length, char from, char to TSRMLS_DC)
{
	const unsigned char *from_table, *to_table;
	unsigned char tmp;
	int i;

	from_table = NULL;
	to_table   = NULL;
	
	switch (toupper((int)(unsigned char)from))
	{
		case 'W':
			from_table = _cyr_win1251;
			break;
		case 'A':
		case 'D':
			from_table = _cyr_cp866;
			break;
		case 'I':
			from_table = _cyr_iso88595;
			break;
		case 'M':
			from_table = _cyr_mac;
			break;
		case 'K':
			break;
		default:
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Unknown source charset: %c", from);
			break;
	}

	switch (toupper((int)(unsigned char)to))
	{
		case 'W':
			to_table = _cyr_win1251;
			break;
		case 'A':
		case 'D':
			to_table = _cyr_cp866;
			break;
		case 'I':
			to_table = _cyr_iso88595;
			break;
		case 'M':
			to_table = _cyr_mac;
			break;
		case 'K':
			break;
		default:
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Unknown destination charset: %c", to);
			break;
	}


	if (!str)
		return (char *)str;
	
	for( i = 0; i<length; i++)
	{
		tmp = (from_table == NULL)? str[i] : from_table[ str[i] ];
		str[i] = (to_table == NULL) ? tmp : to_table[tmp + 256];
	}
	return (char *)str;
}
/* }}} */

/* {{{ proto string convert_cyr_string(string str, string from, string to)
   Convert from one Cyrillic character set to another */
PHP_FUNCTION(convert_cyr_string)
{
	char *input, *fr_cs, *to_cs;
	int input_len, fr_cs_len, to_cs_len;
	unsigned char *str;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sss", &input, &input_len, &fr_cs, &fr_cs_len, &to_cs, &to_cs_len) == FAILURE) {
		return;
	}

	str = (unsigned char*) estrndup(input, input_len);

	php_convert_cyr_string(str, input_len, fr_cs[0], to_cs[0] TSRMLS_CC);
	RETVAL_STRING((char *)str, 0)
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 fdm=marker
 * vim<600: sw=4 ts=4
 */
