# Copyright (c) 2012, Amir Nathoo <amir.nth@gmail.com>
# All rights reserved.
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#     1.Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     2.Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#     3.Neither the name of the <organization> nor the
#       names of its contributors may be used to endorse or promote products
#       derived from this software without specific prior written permission.
#       
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

# The following build targets are always phony
#
.PHONY: default help build clean purge FORCE
FORCE:

#
# Common commands used throughout the build environment
#
CD     = cd 
CP     = cp
CPTREE = cp -r
CHMOD  = chmod
ECHO   = echo
MKDIR  = mkdir -p
PERL   = perl
RM     = rm -f
RMTREE = rm -rf
SED    = sed
TOUCH  = touch
XARGS  = xargs

ARCH=i386-linux-gnu

CC  = $(CROSS_COMPILE)gcc
CXX = $(CROSS_COMPILE)g++
LD = $(CROSS_COMPILE)ld

CFLAGS   = -Wall
CXXFLAGS = $(CFLAGS)
CPPFLAGS = $(CFLAGS)

LIBGEN := $(CROSS_COMPILE)ar
LIBGEN_OPTS := -cr

SHARED_LIBGEN := $(CC)
SHARED_LIBGEN_OPTS := -shared

EXEGEN := $(CC)

define EXTRACT_TAR_GZ
	tar zxvf $1
endef

define EXTRACT_TAR_BZ2
	tar jxvf $1
endef

define APPLY_PATCH
	patch -p0 < $1
endef

#
# Compile C or CXX source file into an intermediate file
#
# $@ = The name of the object (.o) file to create
# $< = The name of the source (.c or .cpp) file (must be first prerequisite listed in rule)
# $1 = Additional compile options
#
define COMPILE
	@$(ECHO) Compiling $<
	@$(MKDIR) $(dir $@)
	@$(RM) $@
	$(if $(filter .c,$(suffix $<)), $(CC) -c $(CPPFLAGS) $(CFLAGS) $1 $< -o $@,)
	$(if $(filter .cpp,$(suffix $<)), $(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $1 $< -o $@,)
endef

#
# Build a dependency file from a C or CXX source file
#
# $@ = The name of the dependency (.d) file to create
# $< = The name of the source (.c or .cpp) file (must be first prerequisite listed in rule)
# $1 = Additional compile options
#
define BUILD_DEPENDS
	@$(ECHO) Building dependencies for $<
	@$(MKDIR) $(dir $@)
	@$(RM) $@
	@$(if $(filter .c,$(suffix $<)), $(CC) -M $(CPPFLAGS) $(CFLAGS) $1 $< > $@.tmp,)
	@$(if $(filter .cpp,$(suffix $<)), $(CXX) -M $(CPPFLAGS) $(CXXFLAGS) $1 $< > $@.tmp,)
	@$(SED) 's,.*\.o[ :]*,$(@:.d=.o) $@ : ,g' < $@.tmp > $@
	@$(RM) $@.tmp
endef

#
# Build a library from a list of object files
#
# $@ = The name of the library (.a) file to create
# $1 = The list of all object (.o) files to put into the library
#
define BUILD_LIBRARY
	@$(ECHO) Building library $@
	@$(MKDIR) $(dir $@)
	@$(RM) $@
	@$(MKDIR) $(dir $@)
	$(LIBGEN) $(LIBGEN_OPTS) $@ $1
endef

#
# Build a shared library from a list of object files
#
# $@ = The name of the library (.dll) file to create
# $1 = The list of all object (.o) files to put into the library
#
define BUILD_SHARED_LIBRARY
	@$(ECHO) Building shared library $@
	@$(MKDIR) $(dir $@)
	@$(RM) $@
	@$(MKDIR) $(dir $@)
	$(SHARED_LIBGEN) $(SHARED_LIBGEN_OPTS) -o $@ $1
endef

#
# Build an executable from a list of object files
#
# $@ = The name of the executable () file to create
# $1 = The list of all object (.o) files to put into the library
#
define BUILD_EXECUTABLE
	@$(ECHO) Building executable $@
	@$(MKDIR) $(dir $@)
	@$(RM) $@
	@$(MKDIR) $(dir $@)
	$(EXEGEN) $(EXEGEN_OPTS) -o $@ $1
endef

#
# Convert from a platform-dependent path to a unix-style path
#
TO_UNIX_PATH = $1

