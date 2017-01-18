SUBDIR=.

#############################################################################

# directories built for: all platforms, all memory targets, all build flavors go here

SUBDIRS=MFUSB_PortingKitSample.dir MFUSB_DualInterface_PortingKitSample.dir

#############################################################################

.SILENT :

#############################################################################

!IF !(defined(DDK_BASEDIR) && defined(DDK_FLAVOR))
!ERROR DDK environment was not initialized; install DDK and run setenv.cmd script in this \
$(MAKEDIR) directory
!ENDIF

#############################################################################
# we expect the following to be defined elsewhere: SPOCLIENT

!IF !(defined(SPOCLIENT))
!ERROR Environment was not initialized; environment variable SPOCLIENT is not defined
!ENDIF

##############################################################################

!IF (defined(SUBDIRS) && "$(SUBDIRS)"!="")

$(SUBDIRS):
    cd $*
    cd
    $(MAKE) /nologo /f $*.mk 
    cd $(MAKEDIR)

!ENDIF  #defined(SUBDIRS) && $(SUBDIRS)!=""
