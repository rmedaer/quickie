#include <stdlib.h>
#include <stdio.h>
#include <Python.h>

extern char _binary_quickierc_py_start, _binary_quickierc_py_end;

int
main(int argc, char* argv[]) {
    int i;
    wchar_t** decoded_argv;
    PyObject *module;
    PyObject *code;
    PyObject *fn;
    PyObject *tuple;

    PyObject *target_module;
    PyObject *target_paths;

    decoded_argv = PyMem_Malloc(sizeof(wchar_t*)* argc);
    for (i = 0; i < argc; ++i) {
        decoded_argv[i] = Py_DecodeLocale(argv[i], NULL);
    }

    Py_SetProgramName(decoded_argv[0]);
    Py_Initialize();

    PySys_SetArgv(argc, decoded_argv);

    // TODO explain why I'm doing this.. (and why this is bad)
    // Need to check if `ld -b binary` ends with \0
    *(&_binary_quickierc_py_end - 1) = 0;

    code = Py_CompileString(&_binary_quickierc_py_start, "", Py_file_input);
    module = PyImport_ExecCodeModule("quickierc", code);

    fn = PyObject_GetAttrString(module, "read_quickierc");
    tuple = PyObject_CallObject(fn, NULL);

    target_module = PyTuple_GetItem(tuple, 0);
    // TODO check target_module is a string

    target_paths = PyTuple_GetItem(tuple, 1);
    // TODO check target_paths is a list

    // Append paths from quickierc in Python sys.path
    PyObject* sys_string = Py_BuildValue("s", "sys");
    PyObject* sys_module = PyImport_Import(sys_string);
    PyObject* sys_path_array = PyObject_GetAttrString(sys_module, "path");
    for (i = 0; i < PyList_Size(target_paths); ++i) {
        PyList_Append(sys_path_array, PyList_GetItem(target_paths, i));
    }

    // Load `runpy` module
    PyObject* runpy_string = Py_BuildValue("s", "runpy");
    PyObject* runpy_module = PyImport_Import(runpy_string);
    if (runpy_module == NULL) {
        fprintf(stderr, "Cannot import `runpy` module\n");
        return 1;
    }

    // Call run_module function
    PyObject* run_module_function = PyObject_GetAttrString(runpy_module, "run_module");
    if (run_module_function == NULL) {
        fprintf(stderr, "Cannot get `runpy.run_module` function\n");
        return 1;
    }

    PyObject* run_name_argument = Py_BuildValue("s", "__main__");
    PyObject* run_module_arguments = PyTuple_Pack(3, target_module, Py_None, run_name_argument);
    PyObject* result = PyObject_CallObject(run_module_function, run_module_arguments);
    if (result == NULL) {
        if (PyErr_Occurred()) {
            PyErr_Print();
        }
        return 1;
    }
    
    for (i = 0; i < argc; ++i) {
        PyMem_RawFree(decoded_argv[i]);
    }

    return 0;
}
