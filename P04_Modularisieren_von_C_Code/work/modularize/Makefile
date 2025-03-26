# Before I got to run 'make test' I had to do several things:
# 1. Install CUnit: brew install cunit
# 2. Add the header path via CPPFLAGS for CUnit (see below)
# 3. Add the library path via LDFLAGS for CUnit (see below)
# 4. Find and use the correct directory of shared.mk (see below)
# 5. Link local test library test_utils.c
# 6. Remove `-static` from LDFLAGS in the shared.mk file, since macOS doesn't support static linking
# 7. Manually compile the test_utils.c file into libsnptest.a by running (inside testlib)
#    mkdir -p lib
#    gcc -c -Iinclude -I/opt/homebrew/opt/cunit/include src/test_utils.c -o lib/test_utils.o
#    ar rcs lib/libsnptest.a lib/test_utils.o
# 8. Verify the test suite by finally running `make test`

# After this, I could move on to implementing the tasks.

# Link local test library test_utils.c
SNP_TESTLIB := ../../../testlib

# Use the correct directory of shared.mk.
# God knows why the prof imposed a ~/snp/ directory here while the file
# was in the root directory the whole time...
SNP_SHARED_MAKEFILE := $(if $(SNP_SHARED_MAKEFILE),$(SNP_SHARED_MAKEFILE),../../../shared.mk)

TARGET     := bin/triangle
SOURCES    := src/triangle.c src/read.c src/rectang.c
TSTSOURCES := tests/tests.c
LIBS       := -lm

include $(SNP_SHARED_MAKEFILE)

# Link Homebrew CUnit
CPPFLAGS += -I/opt/homebrew/opt/cunit/include
LDFLAGS += -L/opt/homebrew/opt/cunit/lib