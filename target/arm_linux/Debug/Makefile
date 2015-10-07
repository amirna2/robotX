
include $(PROJECT_TARGET_DIR)/target.mk

#
# Project Directory locations
#
BINDIR	=	$(PROJECT_TARGET_DIR)/bin
OBJDIR	=	$(PROJECT_TARGET_DIR)/objs
#LIBDIR	=	$(PROJECT_TARGET_DIR)/lib


#
# Target definition (use this if you are generating a shared library)
#
TARGET_PREFIX = lib
TARGET_POSTFIX = .so

#
# Compile options
#
CPPFLAGS +=-g -O0


#
# Source and output
#
SOURCES = $(PROJECT_SRC_DIR)/utils/Mutex.cpp \
					$(PROJECT_SRC_DIR)/utils/Condition.cpp \
					$(PROJECT_SRC_DIR)/utils/Serial.cpp \
					$(PROJECT_SRC_DIR)/utils/Thread.cpp \
					$(PROJECT_SRC_DIR)/subsumption/BehaviorModule.cpp \
					$(PROJECT_SRC_DIR)/behaviors/BehaviorAvoid.cpp \
					$(PROJECT_SRC_DIR)/behaviors/BehaviorEmergencyStop.cpp \
					$(PROJECT_SRC_DIR)/behaviors/BehaviorUserControl.cpp \
					$(PROJECT_SRC_DIR)/behaviors/BehaviorExplore.cpp \
					$(PROJECT_SRC_DIR)/subsumption/Arbitrator.cpp \
					$(PROJECT_SRC_DIR)/subsumption/Monitor.cpp \
					$(PROJECT_SRC_DIR)/logger/LogMessage.cpp \
					$(PROJECT_SRC_DIR)/MainBot.cpp

OBJS    =	$(OBJDIR)/Mutex.o \
					$(OBJDIR)/Condition.o \
					$(OBJDIR)/Serial.o \
					$(OBJDIR)/Thread.o \
					$(OBJDIR)/BehaviorAvoid.o \
					$(OBJDIR)/BehaviorEmergencyStop.o \
					$(OBJDIR)/BehaviorUserControl.o \
					$(OBJDIR)/BehaviorExplore.o \
					$(OBJDIR)/Arbitrator.o \
					$(OBJDIR)/Monitor.o \
					$(OBJDIR)/LogMessage.o \
					$(OBJDIR)/BehaviorModule.o \
					$(OBJDIR)/MainBot.o

OUT     = $(BINDIR)/MainBot

INCLUDE = -I$(TARGET_TOOLS_DIR)/$(ARCH)/libc/usr/include \
          -I/opt/arm-toolchain/arm-linux-gnueabihf/libc/usr/include \
          -I$(PROJECT_SRC_DIR)/include \
          -I$(PROJECT_SRC_DIR)/utils/include \
          -I$(PROJECT_SRC_DIR)/logger \
          -I$(PROJECT_SRC_DIR)/subsumption/include \
          -I$(PROJECT_SRC_DIR)/behaviors 
          
LDFLAGS = -L$(TOOLCHAIN_LIBS)/lib/$(ARCH) -lpthread -ldl -lm

all:	$(OUT)

$(OUT):	$(SOURCES)
	@echo "Building $(OUT)"
	@$(CXX) $(CPPFLAGS) $(INCLUDE) -o $(OUT) $(SOURCES) $(LDFLAGS)
	
clean:
	rm $(OUT)
  
