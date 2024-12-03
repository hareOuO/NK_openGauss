#-------------------------------------------------------------------------
#
# Makefile--
#    Makefile for libpq subsystem (backend half of libpq interface)
#
# IDENTIFICATION
#    src/common/backend/libpq/Makefile
#
#-------------------------------------------------------------------------

subdir = src/common/backend/libpq
top_builddir = ../../../..
include $(top_builddir)/src/Makefile.global

LIBS := $(filter-out -lssl -lcrypto, $(LIBS))


LDFLAGS += -L/usr/lib64
# $(info LDFLAGS = $(LDFLAGS))
# 追加libsodium的库路径到LIBS
LIBS += -lsodium

# $(info LIBS = $(LIBS))  # This will output the value of LIBS

# be-fsstubs is here for historical reasons, probably belongs elsewhere
ifneq "$(MAKECMDGOALS)" "clean"
  ifneq "$(MAKECMDGOALS)" "distclean"
    ifneq "$(shell which g++ |grep hutaf_llt |wc -l)" "1"
      -include $(DEPEND)
    endif
  endif
endif

# 添加libsodium的头文件路径
CPPFLAGS += -I/usr/include/sodium


OBJS = be-fsstubs.o be-secure.o auth.o crypt.o hba.o ip.o md5.o sha2.o pqcomm.o \
       pqformat.o pqsignal.o


include $(top_srcdir)/src/gausskernel/common.mk
