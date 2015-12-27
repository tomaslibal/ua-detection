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
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/FileInputReader/src/FileInputReader.o \
	${OBJECTDIR}/NaiveBayessClassifier/src/NaiveBayessClassifier.o \
	${OBJECTDIR}/NgramBuilder/src/NgramBuilder.o \
	${OBJECTDIR}/UATokenizer/src/UATokenizer.o \
	${OBJECTDIR}/common/src/uadet2.o \
	${OBJECTDIR}/main.o


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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/uadet2

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/uadet2: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/uadet2 ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/FileInputReader/src/FileInputReader.o: FileInputReader/src/FileInputReader.cpp 
	${MKDIR} -p ${OBJECTDIR}/FileInputReader/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/FileInputReader/src/FileInputReader.o FileInputReader/src/FileInputReader.cpp

${OBJECTDIR}/NaiveBayessClassifier/src/NaiveBayessClassifier.o: NaiveBayessClassifier/src/NaiveBayessClassifier.cpp 
	${MKDIR} -p ${OBJECTDIR}/NaiveBayessClassifier/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/NaiveBayessClassifier/src/NaiveBayessClassifier.o NaiveBayessClassifier/src/NaiveBayessClassifier.cpp

${OBJECTDIR}/NgramBuilder/src/NgramBuilder.o: NgramBuilder/src/NgramBuilder.cpp 
	${MKDIR} -p ${OBJECTDIR}/NgramBuilder/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/NgramBuilder/src/NgramBuilder.o NgramBuilder/src/NgramBuilder.cpp

${OBJECTDIR}/UATokenizer/src/UATokenizer.o: UATokenizer/src/UATokenizer.cpp 
	${MKDIR} -p ${OBJECTDIR}/UATokenizer/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/UATokenizer/src/UATokenizer.o UATokenizer/src/UATokenizer.cpp

${OBJECTDIR}/common/src/uadet2.o: common/src/uadet2.cpp 
	${MKDIR} -p ${OBJECTDIR}/common/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/common/src/uadet2.o common/src/uadet2.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/uadet2

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
