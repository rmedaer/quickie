#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <cap-ng.h>
#include <sys/prctl.h>
#include <linux/capability.h>
#include <Python.h>

#define PR_CAP_AMBIENT 47
#define PR_CAP_AMBIENT_IS_SET 1
#define PR_CAP_AMBIENT_RAISE 2
#define PR_CAP_AMBIENT_LOWER 3
#define PR_CAP_AMBIENT_CLEAR_ALL 4

extern char _binary_quickierc_py_start, _binary_quickierc_py_end;

int
main(int argc, char* argv[]) {
    int i;
    int rc;
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
    if (tuple == NULL) {
        if (PyErr_Occurred()) {
            PyErr_Print();
        }
        return 1;
    }

    // Set capabilities
    capng_get_caps_process();
    rc = capng_update(CAPNG_ADD, CAPNG_INHERITABLE, CAP_NET_BIND_SERVICE);
    if (rc) {
        fprintf(stderr, "cannot add inheritable cap\n");
        return 1;
    }
    capng_apply(CAPNG_SELECT_CAPS);

    /* Note the two 0s at the end. Kernel checks for these */
    if (prctl(PR_CAP_AMBIENT, PR_CAP_AMBIENT_RAISE, CAP_NET_BIND_SERVICE, 0, 0)) {
        perror("cannot set cap");
        fprintf(stderr,
            "Please add CAP_NET_BIND_SERVICE capability with following command line:\n\n"
            "    sudo setcap \"cap_net_bind_service+ep\" %s\n\n",
            argv[0]
        );
        return 1;
    }

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
    PyObject* init_globals_argument = Py_BuildValue("{s:S}", "__quickierc__", PyTuple_GetItem(tuple, 2));
    PyObject* run_module_arguments = PyTuple_Pack(3, target_module, init_globals_argument, run_name_argument);
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
