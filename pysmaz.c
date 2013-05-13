/* pysmaz.c: A simple python module to use smaz from python. */
/*
 * Copyright (c) 2009, Benjamin Sergeant
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 *     * Redistributions of source code must retain the above copyright notice,
 *       this list of conditions and the following disclaimer.
 * 
 *     * Redistributions in binary form must reproduce the above copyright notice,
 *       this list of conditions and the following disclaimer in the documentation
 *       and/or other materials provided with the distribution.
 * 
 *     * Neither the name of Smaz nor the names of its contributors may be used to
 *       endorse or promote products derived from this software without specific
 *       prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "Python.h"
#include "smaz.h"

/* Use this to raise an exception then return NULL;
 * PyErr_SetString(PyExc_ValueError, "String too big, cannot compress.");
 */

struct SmazBranch *smaz_trie;

static PyObject *
compress(PyObject *self, PyObject *args)
{
	/* The :compress tells PyArg_ParseTuple what function to use 
	 * in its error message
	 */
	char *pstr;
	if (!PyArg_ParseTuple(args, "s:compress", &pstr)) {
		return NULL;
	}

	int outlen = 4096;
	char* out = (char*) malloc(outlen);
	int o;
	while (outlen + 1 == (o = smaz_compress_trie(smaz_trie, pstr, strlen(pstr), out, outlen))) {
		outlen *= 2;
		out = (char*) realloc(out, outlen);
	}

	PyObject* obj = Py_BuildValue("s#", out, o);
	free(out);
	return obj;
}

static PyObject *
decompress(PyObject *self, PyObject *args)
{
	/* The :decompress tells PyArg_ParseTuple what function to use 
	 * in its error message
	 */
	const char *pstr;
	int len;
	if (!PyArg_ParseTuple(args, "s#:decompress", &pstr, &len)) {
		return NULL;
	}

	int outlen = 4096, o;
	char* out = (char*) malloc(outlen);
	while (outlen + 1 == (o = smaz_decompress(pstr, len, out, outlen))) {
		outlen *= 2;
		out = (char*) realloc(out, outlen);
	}

	PyObject* obj = Py_BuildValue("s#", out, o);
	free(out);
	return obj;
}

/* Module functions table. */

static PyMethodDef
module_functions[] = {
    { "compress", compress, METH_VARARGS, "Compress a byte buffer." },
    { "decompress", decompress, METH_VARARGS, "Decompress a byte buffer." },
    { NULL }
};

/* This function is called to initialize the module.
 * The name of this function must be init<mymodule> (module name from setup.py)
 */

void
initsmaz(void)
{
    Py_InitModule3("smaz", module_functions, "A module to compress small strings using smaz.");
    smaz_trie = smaz_build_trie();
}

