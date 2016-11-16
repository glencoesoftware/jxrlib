##//*@@@+++@@@@******************************************************************
##//
##// Copyright � Microsoft Corp.
##// All rights reserved.
##//
##// Redistribution and use in source and binary forms, with or without
##// modification, are permitted provided that the following conditions are met:
##//
##// � Redistributions of source code must retain the above copyright notice,
##//   this list of conditions and the following disclaimer.
##// � Redistributions in binary form must reproduce the above copyright notice,
##//   this list of conditions and the following disclaimer in the documentation
##//   and/or other materials provided with the distribution.
##//
##// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
##// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
##// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
##// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
##// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
##// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
##// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
##// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
##// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
##// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
##// POSSIBILITY OF SUCH DAMAGE.
##//
##//*@@@---@@@@******************************************************************
## Makefile for building JPEG XR Porting Kit
##
build: all

SWIG?=swig

JXR_VERSION=1.1

DIR_SRC=$(CURDIR)
DIR_SYS=image/sys
DIR_DEC=image/decode
DIR_ENC=image/encode

DIR_GLUE=jxrgluelib
DIR_CXX=cpp
DIR_JAVA=java/target/swig
DIR_TEST=jxrtestlib
DIR_EXEC=jxrencoderdecoder

JAVA_PKG=ome/jxrlib

## Are we building shared?
ifneq ($(SHARED),)
PICFLAG=-fPIC
else
PICFLAG=
endif

ifneq ($(BIG_ENDIAN),)
ENDIANFLAG=-D_BIG__ENDIAN_
else
ENDIANFLAG=
endif

ifndef JAVA_HOME
JAVA_HOME:=$(shell jrunscript -e 'java.lang.System.out.println(new java.io.File(java.lang.System.getProperty("java.home")).getParent());')
endif

ifndef JAVA_INCLUDE
JAVA_INCLUDE=$(JAVA_HOME)/include
endif

ifndef DIR_BUILD
DIR_BUILD=$(CURDIR)/build
endif

ifndef PREFIX
PREFIX=/usr/local
endif

ifndef DIR_INSTALL
DIR_INSTALL=$(PREFIX)/lib/jxrlib-$(JXR_VERSION)
endif

CD=cd
MK_DIR=mkdir -p
CFLAGS=-I. -Icommon/include -I$(DIR_SYS) $(ENDIANFLAG) -D__ANSI__ -DDISABLE_PERF_MEASUREMENT -w -Os -fPIC
CXXFLAGS=-I. -Icommon/include -I$(DIR_SYS) -I$(DIR_GLUE) -I$(DIR_TEST) -Wno-self-assign-field -Wno-unsequenced -Os -fPIC

ifdef DEBUG
CFLAGS:=$(CFLAGS) -g -O0 -DDEBUG
CXXFLAGS:= $(CXXFLAGS) -g -O0 -DDEBUG
endif

ifeq ($(strip $(shell uname)), Darwin)
  CC?=clang
  CXX?=clang++
  LIBSUFFIX = dylib
  PLATFORM = darwin
else
  CC?=cc
  CXX?=g++
  LIBSUFFIX = so
  PLATFORM = linux
endif

STATIC_LIBRARIES=$(DIR_BUILD)/libjxrglue.a $(DIR_BUILD)/libjpegxr.a
SHARED_LIBRARIES=$(DIR_BUILD)/libjxrglue.$(LIBSUFFIX) $(DIR_BUILD)/libjpegxr.$(LIBSUFFIX)
CXX_STATIC_LIBRARIES=$(DIR_BUILD)/libjxr++.a
CXX_SHARED_LIBRARIES=$(DIR_BUILD)/libjxr++.$(LIBSUFFIX)

ifneq ($(SHARED),)
LIBRARIES=$(SHARED_LIBRARIES)
CXX_LIBRARIES=$(CXX_SHARED_LIBRARIES)
else
LIBRARIES=$(STATIC_LIBRARIES)
CXX_LIBRARIES=$(CXX_STATIC_LIBRARIES)
endif

LIBS=-L$(DIR_BUILD) $(shell echo $(LIBRARIES) | sed -E 's%$(DIR_BUILD)/lib([^ ]*)\.(a|$(LIBSUFFIX))%-l\1%g') -lm
CXXLIBS=-L$(DIR_BUILD) -ljxr++ -lcrypto

ifdef HOMEBREW
CFLAGS:=$(CFLAGS) -I$(shell brew --prefix openssl)/include
CXXFLAGS:=$(CXXFLAGS) -I$(shell brew --prefix openssl)/include
LIBS:=$(LIBS) -L$(shell brew --prefix openssl)/lib
endif

##--------------------------------
##
## Common files
##

SRC_SYS=adapthuff.c image.c strcodec.c strPredQuant.c strTransform.c perfTimerANSI.c
OBJ_SYS=$(patsubst %.c, $(DIR_BUILD)/$(DIR_SYS)/%.o, $(SRC_SYS))

$(DIR_BUILD)/$(DIR_SYS)/%.o: $(DIR_SRC)/$(DIR_SYS)/%.c
	@echo "Building C common files"
	$(MK_DIR) $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

##--------------------------------
##
## Decode files
##

SRC_DEC=decode.c postprocess.c segdec.c strdec.c strInvTransform.c strPredQuantDec.c JXRTranscode.c
OBJ_DEC=$(patsubst %.c, $(DIR_BUILD)/$(DIR_DEC)/%.o, $(SRC_DEC))
$(DIR_BUILD)/$(DIR_DEC)/%.o: $(DIR_SRC)/$(DIR_DEC)/%.c
	@echo "Building C decode files"
	$(MK_DIR) $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

##--------------------------------
##
## Encode files
##

SRC_ENC=encode.c segenc.c strenc.c strFwdTransform.c strPredQuantEnc.c
OBJ_ENC=$(patsubst %.c, $(DIR_BUILD)/$(DIR_ENC)/%.o, $(SRC_ENC))

$(DIR_BUILD)/$(DIR_ENC)/%.o: $(DIR_SRC)/$(DIR_ENC)/%.c
	@echo "Building C encode files"
	$(MK_DIR) $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

##--------------------------------
##
## JPEG XR library
##

$(DIR_BUILD)/libjpegxr.a: $(OBJ_ENC) $(OBJ_DEC) $(OBJ_SYS)
	@echo "Building JPEG XR static lib"
	$(MK_DIR) $(@D)
	ar rvu $@ $(OBJ_ENC) $(OBJ_DEC) $(OBJ_SYS)
	ranlib $@

$(DIR_BUILD)/libjpegxr.$(LIBSUFFIX): $(OBJ_ENC) $(OBJ_DEC) $(OBJ_SYS)
	@echo "Building JPEG XR shared lib"
	$(MK_DIR) $(@D)
	$(CC) -shared $? -o $@

##--------------------------------
##
## Glue files
##

SRC_GLUE=JXRGlue.c JXRMeta.c JXRGluePFC.c JXRGlueJxr.c
OBJ_GLUE=$(patsubst %.c, $(DIR_BUILD)/$(DIR_GLUE)/%.o, $(SRC_GLUE))

$(DIR_BUILD)/$(DIR_GLUE)/%.o: $(DIR_SRC)/$(DIR_GLUE)/%.c
	@echo "Building C glue files"
	$(MK_DIR) $(@D)
	$(CC) $(CFLAGS) -I$(DIR_GLUE) -c $< -o $@

##--------------------------------
##
## C++ files
##

SRC_CXX=$(wildcard $(DIR_SRC)/$(DIR_CXX)/lib/*.cpp)
OBJ_CXX=$(patsubst $(DIR_SRC)/$(DIR_CXX)/lib/%.cpp, $(DIR_BUILD)/$(DIR_CXX)/%.o, $(SRC_CXX))

$(DIR_BUILD)/$(DIR_CXX)/%.o: $(DIR_SRC)/$(DIR_CXX)/lib/%.cpp
	@echo "Building C++ files"
	$(MK_DIR) $(@D)
	$(CXX) $(CXXFLAGS) -I$(DIR_CXX)/lib/ -c $< -o $@

##--------------------------------
##
## Test files
##

SRC_TEST=JXRTest.c JXRTestBmp.c JXRTestHdr.c JXRTestPnm.c JXRTestTif.c JXRTestYUV.c
OBJ_TEST=$(patsubst %.c, $(DIR_BUILD)/$(DIR_TEST)/%.o, $(SRC_TEST))

$(DIR_BUILD)/$(DIR_TEST)/%.o: $(DIR_SRC)/$(DIR_TEST)/%.c
	@echo "Building C test files"
	$(MK_DIR) $(@D)
	$(CC) $(CFLAGS) -I$(DIR_GLUE) -I$(DIR_TEST) -c $< -o $@

##--------------------------------
##
## JPEG XR Glue library
##

$(DIR_BUILD)/libjxrglue.a: $(OBJ_GLUE) $(OBJ_TEST)
	@echo "Building glue static lib"
	$(MK_DIR) $(@D)
	ar rvu $@ $(OBJ_GLUE) $(OBJ_TEST)
	ranlib $@

$(DIR_BUILD)/libjxrglue.$(LIBSUFFIX): $(OBJ_GLUE) $(OBJ_TEST)
	@echo "Building glue shared lib"
	$(MK_DIR) $(@D)
	$(CC) -shared $? -o $@

##--------------------------------
##
## C++ Wrapper library
##

$(DIR_BUILD)/libjxr++.a: $(OBJ_CXX) | $(LIBRARIES)
	@echo "Building C++ static wrapper lib"
	$(MK_DIR) $(@D)
	ar rvu $@ $(OBJ_CXX) $(LIBRARIES)
	ranlib $@


$(DIR_BUILD)/libjxr++.$(LIBSUFFIX): $(OBJ_CXX) | $(LIBRARIES)
	@echo "Building C++ wrapper lib"
	$(MK_DIR) $(@D)
	$(CXX) -shared $(OBJ_CXX) -fPIC $(LIBS) -o $@


##--------------------------------
##
## Java Wrapper library
##

SRC_JAVA=$(wildcard $(DIR_SRC)/$(DIR_JAVA)/$(JAVA_PKG)/*.java)

swig:
	mkdir -p $(DIR_SRC)/$(DIR_JAVA)/$(JAVA_PKG)
	$(SWIG) -java -c++ -package ome.jxrlib -outdir $(DIR_SRC)/$(DIR_JAVA)/$(JAVA_PKG) -o $(DIR_SRC)/$(DIR_JAVA)/JXR_wrap.cxx java/JXR.i

$(DIR_BUILD)/libjxrjava.$(LIBSUFFIX): $(LIBRARIES) $(CXX_LIBRARIES)
	@echo "Building JNI"
	@echo "JAVA_INCLUDE=$(JAVA_INCLUDE)"
	$(CXX) -o $(DIR_BUILD)/libjxrjava.$(LIBSUFFIX) -shared -I$(JAVA_INCLUDE) -I$(JAVA_INCLUDE)/$(PLATFORM) -I$(DIR_CXX)/lib $(CXXFLAGS) $(OBJ_SYS) $(OBJ_ENC) $(OBJ_DEC) $(OBJ_GLUE) $(OBJ_TEST) $(OBJ_CXX) $(DIR_JAVA)/JXR_wrap.cxx

##--------------------------------
##
## Enc app files
##
ENCAPP=JxrEncApp

$(DIR_BUILD)/$(ENCAPP): $(DIR_SRC)/$(DIR_EXEC)/$(ENCAPP).c $(LIBRARIES)
	@echo "Building JxrEncApp"
	$(MK_DIR) $(@D)
	$(CC) $< -o $@ $(CFLAGS) -I$(DIR_GLUE) -I$(DIR_TEST) $(LIBS)

##--------------------------------
##
## Dec app files
##

DECAPP=JxrDecApp

$(DIR_BUILD)/$(DECAPP): $(DIR_SRC)/$(DIR_EXEC)/$(DECAPP).c $(LIBRARIES)
	@echo "Building JxrDecApp"
	$(MK_DIR) $(@D)
	$(CC) $< -o $@ $(CFLAGS) -I$(DIR_GLUE) -I$(DIR_TEST) $(LIBS)

##--------------------------------
##
## C++ Decompression app
##

CXXDECAPP=jxrdecode

$(DIR_BUILD)/$(CXXDECAPP): $(DIR_SRC)/$(DIR_CXX)/$(CXXDECAPP).cpp $(LIBRARIES) $(CXX_LIBRARIES)
	echo "Building C++ jxrdecode"
	$(MK_DIR) $(@D)
	$(CXX) $<  $(OBJ_CXX) -o $@ -I$(DIR_CXX)/lib $(CXXFLAGS) $(LIBS) $(CXXLIBS)

##--------------------------------
##
## JPEG XR library
##
all: $(DIR_BUILD)/$(ENCAPP) $(DIR_BUILD)/$(DECAPP) $(DIR_BUILD)/$(CXXDECAPP) $(LIBRARIES) $(CXX_LIBRARIES) $(DIR_BUILD)/libjxrjava.$(LIBSUFFIX)

clean:
	rm -rf $(DIR_BUILD)/*App $(DIR_BUILD)/**/*.o $(DIR_BUILD)/**/*.class $(DIR_BUILD)/libj*.a $(DIR_BUILD)/libj*.$(LIBSUFFIX) $(DIR_BUILD)/libjxr.pc $(DIR_BUILD)/$(CXXDECAPP) $(DIR_BUILD)/$(JAR)

$(DIR_BUILD)/libjxr.pc: $(DIR_SRC)/libjxr.pc.in
	@python -c 'import os; d = { "DIR_INSTALL": "$(DIR_INSTALL)", "JXR_VERSION": "$(JXR_VERSION)", "JXR_ENDIAN": "$(ENDIANFLAG)" }; fin = open("$<", "r"); fout = open("$@", "w+"); fout.writelines( [ l % d for l in fin.readlines()])'

install: all $(DIR_BUILD)/libjxr.pc
	install -d $(DIR_INSTALL)/lib/pkgconfig $(DIR_INSTALL)/bin $(DIR_INSTALL)/include/libjxr/common  $(DIR_INSTALL)/include/libjxr/image/x86 $(DIR_INSTALL)/include/libjxr/glue $(DIR_INSTALL)/include/libjxr/test $(DIR_INSTALL)/share/doc/jxr-$(JXR_VERSION)
	install $(LIBRARIES) $(DIR_INSTALL)/lib
	install -m 644 $(DIR_BUILD)/libjxr.pc $(DIR_INSTALL)/lib/pkgconfig
	install $(DIR_BUILD)/$(ENCAPP) $(DIR_BUILD)/$(DECAPP) $(DIR_INSTALL)/bin
	install -m 644 $(DIR_SRC)/common/include/*.h $(DIR_INSTALL)/include/libjxr/common
	install -m 644 $(DIR_SRC)/image/x86/*.h $(DIR_INSTALL)/include/libjxr/image/x86
	install -m 644 $(DIR_SRC)/$(DIR_SYS)/*.h $(DIR_INSTALL)/include/libjxr/image
	install -m 644 $(DIR_SRC)/$(DIR_ENC)/*.h $(DIR_INSTALL)/include/libjxr/image
	install -m 644 $(DIR_SRC)/$(DIR_DEC)/*.h $(DIR_INSTALL)/include/libjxr/image
	install -m 644 $(DIR_SRC)/$(DIR_GLUE)/*.h $(DIR_INSTALL)/include/libjxr/glue
	install -m 644 $(DIR_SRC)/$(DIR_TEST)/*.h $(DIR_INSTALL)/include/libjxr/test
	install -m 644 doc/* $(DIR_INSTALL)/share/doc/jxr-$(JXR_VERSION)

##
