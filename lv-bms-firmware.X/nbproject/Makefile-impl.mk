#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a pre- and a post- target defined where you can add customization code.
#
# This makefile implements macros and targets common to all configurations.
#
# NOCDDL


# Building and Cleaning subprojects are done by default, but can be controlled with the SUB
# macro. If SUB=no, subprojects will not be built or cleaned. The following macro
# statements set BUILD_SUB-CONF and CLEAN_SUB-CONF to .build-reqprojects-conf
# and .clean-reqprojects-conf unless SUB has the value 'no'
SUB_no=NO
SUBPROJECTS=${SUB_${SUB}}
BUILD_SUBPROJECTS_=.build-subprojects
BUILD_SUBPROJECTS_NO=
BUILD_SUBPROJECTS=${BUILD_SUBPROJECTS_${SUBPROJECTS}}
CLEAN_SUBPROJECTS_=.clean-subprojects
CLEAN_SUBPROJECTS_NO=
CLEAN_SUBPROJECTS=${CLEAN_SUBPROJECTS_${SUBPROJECTS}}


# Project Name
PROJECTNAME=lv-bms-firmware.X

# Active Configuration
DEFAULTCONF=Full_firmware
CONF=${DEFAULTCONF}

# All Configurations
ALLCONFS=Full_firmware indicator_test CAN_test BQ79600_test error_load_store_test 


# build
.build-impl: .build-pre
	${MAKE} -f nbproject/Makefile-${CONF}.mk SUBPROJECTS=${SUBPROJECTS} .build-conf


# clean
.clean-impl: .clean-pre
	${MAKE} -f nbproject/Makefile-${CONF}.mk SUBPROJECTS=${SUBPROJECTS} .clean-conf

# clobber
.clobber-impl: .clobber-pre .depcheck-impl
	    ${MAKE} SUBPROJECTS=${SUBPROJECTS} CONF=Full_firmware clean
	    ${MAKE} SUBPROJECTS=${SUBPROJECTS} CONF=indicator_test clean
	    ${MAKE} SUBPROJECTS=${SUBPROJECTS} CONF=CAN_test clean
	    ${MAKE} SUBPROJECTS=${SUBPROJECTS} CONF=BQ79600_test clean
	    ${MAKE} SUBPROJECTS=${SUBPROJECTS} CONF=error_load_store_test clean



# all
.all-impl: .all-pre .depcheck-impl
	    ${MAKE} SUBPROJECTS=${SUBPROJECTS} CONF=Full_firmware build
	    ${MAKE} SUBPROJECTS=${SUBPROJECTS} CONF=indicator_test build
	    ${MAKE} SUBPROJECTS=${SUBPROJECTS} CONF=CAN_test build
	    ${MAKE} SUBPROJECTS=${SUBPROJECTS} CONF=BQ79600_test build
	    ${MAKE} SUBPROJECTS=${SUBPROJECTS} CONF=error_load_store_test build



# dependency checking support
.depcheck-impl:
#	@echo "# This code depends on make tool being used" >.dep.inc
#	@if [ -n "${MAKE_VERSION}" ]; then \
#	    echo "DEPFILES=\$$(wildcard \$$(addsuffix .d, \$${OBJECTFILES}))" >>.dep.inc; \
#	    echo "ifneq (\$${DEPFILES},)" >>.dep.inc; \
#	    echo "include \$${DEPFILES}" >>.dep.inc; \
#	    echo "endif" >>.dep.inc; \
#	else \
#	    echo ".KEEP_STATE:" >>.dep.inc; \
#	    echo ".KEEP_STATE_FILE:.make.state.\$${CONF}" >>.dep.inc; \
#	fi
