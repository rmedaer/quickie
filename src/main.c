#include <stdlib.h>
#include <stdio.h>
#include <Python.h>

int
main(int argc, char* argv[]) {
    int i;
    wchar_t** decoded_argv;

    decoded_argv = PyMem_Malloc(sizeof(wchar_t*)* argc);
    for (i = 0; i < argc; ++i) {
        decoded_argv[i] = Py_DecodeLocale(argv[i], NULL);
    }

    // program goes here
    extern const char _binary_quickierc_py_start, _binary_quickierc_py_end;
    const char *start = &_binary_quickierc_py_start;
    const char *end = &_binary_quickierc_py_end;

    fwrite(start, end - start, 1, stdout);
    
    for (i = 0; i < argc; ++i) {
        PyMem_RawFree(decoded_argv[i]);
    }

    return 0;
}
