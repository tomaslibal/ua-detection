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
CND_CONF=Release
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

# Test Directory
TESTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tests

# Test Files
TESTFILES= \
	${TESTDIR}/TestFiles/f1

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
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/FileInputReader/src/FileInputReader.o FileInputReader/src/FileInputReader.cpp

${OBJECTDIR}/NaiveBayessClassifier/src/NaiveBayessClassifier.o: NaiveBayessClassifier/src/NaiveBayessClassifier.cpp 
	${MKDIR} -p ${OBJECTDIR}/NaiveBayessClassifier/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/NaiveBayessClassifier/src/NaiveBayessClassifier.o NaiveBayessClassifier/src/NaiveBayessClassifier.cpp

${OBJECTDIR}/NgramBuilder/src/NgramBuilder.o: NgramBuilder/src/NgramBuilder.cpp 
	${MKDIR} -p ${OBJECTDIR}/NgramBuilder/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/NgramBuilder/src/NgramBuilder.o NgramBuilder/src/NgramBuilder.cpp

${OBJECTDIR}/UATokenizer/src/UATokenizer.o: UATokenizer/src/UATokenizer.cpp 
	${MKDIR} -p ${OBJECTDIR}/UATokenizer/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/UATokenizer/src/UATokenizer.o UATokenizer/src/UATokenizer.cpp

${OBJECTDIR}/common/src/uadet2.o: common/src/uadet2.cpp 
	${MKDIR} -p ${OBJECTDIR}/common/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/common/src/uadet2.o common/src/uadet2.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Build Test Targets
.build-tests-conf: .build-conf ${TESTFILES}
${TESTDIR}/TestFiles/f1: ${TESTDIR}/FileInputReader/test/fileInputReaderTest.o ${TESTDIR}/FileInputReader/test/fileInputReaderTestRunner.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f1 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   


${TESTDIR}/FileInputReader/test/fileInputReaderTest.o: FileInputReader/test/fileInputReaderTest.cpp 
	${MKDIR} -p ${TESTDIR}/FileInputReader/test
	${RM} "$@.d"
	$(COMPILE.cc) -O2 `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/FileInputReader/test/fileInputReaderTest.o FileInputReader/test/fileInputReaderTest.cpp


${TESTDIR}/FileInputReader/test/fileInputReaderTestRunner.o: FileInputReader/test/fileInputReaderTestRunner.cpp 
	${MKDIR} -p ${TESTDIR}/FileInputReader/test
	${RM} "$@.d"
	$(COMPILE.cc) -O2 `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/FileInputReader/test/fileInputReaderTestRunner.o FileInputReader/test/fileInputReaderTestRunner.cpp


${OBJECTDIR}/FileInputReader/src/FileInputReader_nomain.o: ${OBJECTDIR}/FileInputReader/src/FileInputReader.o FileInputReader/src/FileInputReader.cpp 
	${MKDIR} -p ${OBJECTDIR}/FileInputReader/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/FileInputReader/src/FileInputReader.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/FileInputReader/src/FileInputReader_nomain.o FileInputReader/src/FileInputReader.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/FileInputReader/src/FileInputReader.o ${OBJECTDIR}/FileInputReader/src/FileInputReader_nomain.o;\
	fi

${OBJECTDIR}/NaiveBayessClassifier/src/NaiveBayessClassifier_nomain.o: ${OBJECTDIR}/NaiveBayessClassifier/src/NaiveBayessClassifier.o NaiveBayessClassifier/src/NaiveBayessClassifier.cpp 
	${MKDIR} -p ${OBJECTDIR}/NaiveBayessClassifier/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/NaiveBayessClassifier/src/NaiveBayessClassifier.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/NaiveBayessClassifier/src/NaiveBayessClassifier_nomain.o NaiveBayessClassifier/src/NaiveBayessClassifier.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/NaiveBayessClassifier/src/NaiveBayessClassifier.o ${OBJECTDIR}/NaiveBayessClassifier/src/NaiveBayessClassifier_nomain.o;\
	fi

${OBJECTDIR}/NgramBuilder/src/NgramBuilder_nomain.o: ${OBJECTDIR}/NgramBuilder/src/NgramBuilder.o NgramBuilder/src/NgramBuilder.cpp 
	${MKDIR} -p ${OBJECTDIR}/NgramBuilder/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/NgramBuilder/src/NgramBuilder.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/NgramBuilder/src/NgramBuilder_nomain.o NgramBuilder/src/NgramBuilder.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/NgramBuilder/src/NgramBuilder.o ${OBJECTDIR}/NgramBuilder/src/NgramBuilder_nomain.o;\
	fi

${OBJECTDIR}/UATokenizer/src/UATokenizer_nomain.o: ${OBJECTDIR}/UATokenizer/src/UATokenizer.o UATokenizer/src/UATokenizer.cpp 
	${MKDIR} -p ${OBJECTDIR}/UATokenizer/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/UATokenizer/src/UATokenizer.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/UATokenizer/src/UATokenizer_nomain.o UATokenizer/src/UATokenizer.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/UATokenizer/src/UATokenizer.o ${OBJECTDIR}/UATokenizer/src/UATokenizer_nomain.o;\
	fi

${OBJECTDIR}/common/src/uadet2_nomain.o: ${OBJECTDIR}/common/src/uadet2.o common/src/uadet2.cpp 
	${MKDIR} -p ${OBJECTDIR}/common/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/common/src/uadet2.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/common/src/uadet2_nomain.o common/src/uadet2.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/common/src/uadet2.o ${OBJECTDIR}/common/src/uadet2_nomain.o;\
	fi

${OBJECTDIR}/main_nomain.o: ${OBJECTDIR}/main.o main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/main.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -O2 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main_nomain.o main.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/main.o ${OBJECTDIR}/main_nomain.o;\
	fi

# Run Test Targets
.test-conf:
	@if [ "${TEST}" = "" ]; \
	then  \
	    ${TESTDIR}/TestFiles/f1 || true; \
	else  \
	    ./${TEST} || true; \
	fi

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/uadet2

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
