bin_PROGRAMS = quickie

quickierc.o: quickierc.py
	$(AM_V_GEN) $(LD) -r -b binary -o $@ $<

nodist_EXTRA_quickie_SOURCES = quickierc.o
BUILT_SOURCES = quickierc.o
CLEANFILES = quickierc.o

quickie_SOURCES = main.c
quickie_CFLAGS = -fPIC $(PYTHON_CFLAGS) $(CAPNG_CFLAGS)
quickie_LDFLAGS = $(PYTHON_LDFLAGS)
quickie_LDADD = $(CAPNG_LIBS) quickierc.o
