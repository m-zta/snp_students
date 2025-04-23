SNP_SHARED_MAKEFILE := $(if $(SNP_SHARED_MAKEFILE),$(SNP_SHARED_MAKEFILE),$(HOME)/zhaw_code/snp_students/shared.mk)
SNP_TESTLIB := $(HOME)/zhaw_code/snp_students/testlib

TARGET     := bin/personen-verwaltung
MODULES    := src/person.c src/list.c
SOURCES    := src/main.c $(MODULES)
TSTSOURCES := tests/tests.c $(MODULES)

# LDFLAGS für CUnit Library Pfad (kann wahrscheinlich hier bleiben)
LDFLAGS += -L/opt/homebrew/lib

# shared.mk einbinden
include $(SNP_SHARED_MAKEFILE)

# Füge CUnit Include Pfad HIER hinzu (NACH dem include)!
CPPFLAGS += -I/opt/homebrew/Cellar/cunit/2.1-3/include

# CFLAGS  += -Werror