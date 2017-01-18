# build is the DDK command to build the project; it depends on the sources and the dirs file 
BUILD=build 

.SILENT :

#############################################################################

SPO_BUILD=$(COMMON_BUILD_ROOT)\BuildOutput

DRIVER_BASE=.
INF_FILE   =MFUSB_DualInterface_PortingKitSample
DRIVER_DIR =$(DRIVER_BASE)\sys
INF_SRC    =$(DRIVER_BASE)\inf

DDKOUTPUT_DIR=obj$(BUILD_ALT_DIR)\$(OUTPUTDIR)

DRIVER_DDKOUTPUT_DIR=$(DRIVER_DIR)\$(DDKOUTPUT_DIR)

DST_DIR=$(SPO_BUILD)\Windows\$(FLAVOR_WIN)\MFUSB_DualInterface_PortingKitSample
OBJ_DIR=$(DST_DIR)\obj\$(PLATFORM)
INF_DIR=$(DST_DIR)\bin
BIN_DIR=$(DST_DIR)\bin\$(PLATFORM)
SYM_DIR=$(DST_DIR)\pdb\$(PLATFORM)

##############################################################################

All: $(OBJ_DIR) $(LIB_DIR) $(BIN_DIR) $(SYM_DIR) $(SUBDIRS) build binplace_obj binplace_sys binplace_pdb copy_inf

##############################################################################

# build the obj/lib/bin dirs if needed

$(OBJ_DIR):
    echo Creating output directory $(OBJ_DIR)
    -md $(OBJ_DIR)


$(BIN_DIR):
    echo Creating output directory $(BIN_DIR)
    -md $(BIN_DIR)

$(SYM_DIR):
    echo Creating output directory $(SYM_DIR)
    -md $(SYM_DIR)

################################################################################

binplace_obj: 
    -copy /y /v $(DRIVER_DDKOUTPUT_DIR)\*.obj $(OBJ_DIR)

binplace_sys: 
    -copy /y /v $(DRIVER_DDKOUTPUT_DIR)\*.sys $(BIN_DIR)

binplace_pdb: 
    -copy /y /v $(DRIVER_DDKOUTPUT_DIR)\*.pdb $(SYM_DIR)

copy_inf:
    -copy $(INF_SRC)\$(INF_FILE).inf $(INF_DIR)\.

clean:
    echo Erasing all targets...
    -rd /s /q $(DST_DIR)
    echo done 

build: 
	$(BUILD) /c /b /g /w /F








