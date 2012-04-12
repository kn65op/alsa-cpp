#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/713640644/Recorder.o \
	${OBJECTDIR}/_ext/713640644/Device.o \
	${OBJECTDIR}/_ext/713640644/Data.o \
	${OBJECTDIR}/_ext/713640644/extras.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libalsa-cpp.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libalsa-cpp.so: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -shared -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libalsa-cpp.so -fPIC ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/713640644/Recorder.o: /home/tomko/moje_dziela/alsa-cpp/src/Recorder.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/713640644
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/713640644/Recorder.o /home/tomko/moje_dziela/alsa-cpp/src/Recorder.cpp

${OBJECTDIR}/_ext/713640644/Device.o: /home/tomko/moje_dziela/alsa-cpp/src/Device.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/713640644
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/713640644/Device.o /home/tomko/moje_dziela/alsa-cpp/src/Device.cpp

${OBJECTDIR}/_ext/713640644/Data.o: /home/tomko/moje_dziela/alsa-cpp/src/Data.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/713640644
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/713640644/Data.o /home/tomko/moje_dziela/alsa-cpp/src/Data.cpp

${OBJECTDIR}/_ext/713640644/extras.o: /home/tomko/moje_dziela/alsa-cpp/src/extras.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/713640644
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/713640644/extras.o /home/tomko/moje_dziela/alsa-cpp/src/extras.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libalsa-cpp.so

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
