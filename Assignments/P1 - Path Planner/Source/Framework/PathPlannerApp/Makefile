MSVC_INCLUDE = $(HOME)/msincludes/msvc/14.28.29333/include/
UCRT_INCLUDE = $(HOME)/msincludes/10sdk/Include/10.0.18362.0/ucrt/
UM_INCLUDE = $(HOME)/msincludes/10sdk/Include/10.0.18362.0/um/
SHARED_INCLUDE = $(HOME)/msincludes/10sdk/Include/10.0.18362.0/shared/

MSVC_LIB = $(HOME)/msincludes/msvc/14.28.29333/lib/x64/
UCRT_LIB = $(HOME)/msincludes/10sdk/Lib/10.0.18362.0/ucrt/x64/
UM_LIB = $(HOME)/msincludes/10sdk/Lib/10.0.18362.0/um/x64/

OBJ = PathPlannerLab.obj
DEPENDS = PathPlannerLab.h PathPlannerUtility.h stdafx.h Resource/resource.h
OUT = PathPlannerApp

$(OUT).exe: $(OBJ) Resource/PathPlanner.res ../TileSystem/TileSystem.lib ../../PathSearch/PathSearch.lib $(UM_LIB)/gdi32.lib $(UM_LIB)/user32.lib $(UM_LIB)/comctl32.lib $(UM_LIB)/comdlg32.lib
	lld-link $^ /SUBSYSTEM:WINDOWS /OUT:$(OUT).exe /LIBPATH:$(MSVC_LIB) /LIBPATH:$(UCRT_LIB) /LIBPATH:$(UM_LIB)

%.obj: %.cpp $(DEPENDS)
	clang-cl $< /w /TP /EHsc /Ox /c /MD /I $(MSVC_INCLUDE) /I $(UCRT_INCLUDE) /I $(UM_INCLUDE) /I $(SHARED_INCLUDE)

%.res: %.rc
	wrc $< -o $@

clean:
	rm -f $(OBJ) $(OUT).exe Resource/PathPlanner.res
