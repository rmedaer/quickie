bin_PROGRAMS = quickie

quickieconfig.o: quickieconfig.py
	$(AM_V_GEN) $(LD) -r -b binary -o $@ $<

nodist_EXTRA_quickie_SOURCES = quickieconfig.o
BUILT_SOURCES = quickieconfig.o
CLEANFILES = quickieconfig.o

quickie_SOURCES = main.c
quickie_CFLAGS = -fPIC $(PYTHON_CFLAGS) $(CAPNG_CFLAGS)
quickie_LDADD = quickieconfig.o $(PYTHON_LDFLAGS) $(CAPNG_LIBS)
