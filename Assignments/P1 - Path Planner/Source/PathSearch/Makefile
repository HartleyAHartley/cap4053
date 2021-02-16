MSVC_INCLUDE = $(HOME)/msincludes/msvc/14.28.29333/include/
UCRT_INCLUDE = $(HOME)/msincludes/10sdk/Include/10.0.18362.0/ucrt/

MSVC_LIB = $(HOME)/msincludes/msvc/14.28.29333/lib/x64/
UCRT_LIB = $(HOME)/msincludes/10sdk/Lib/10.0.18362.0/ucrt/x64/
UM_LIB = $(HOME)/msincludes/10sdk/Lib/10.0.18362.0/um/x64/

OBJ = PathSearch.obj ../Framework/TileSystem/TileMap.obj ../Framework/TileSystem/Tile.obj
DEPENDS = PathSearch.h
OUT = PathSearch

$(OUT).dll $(OUT).lib: $(OBJ)
	lld-link $^ /DLL /OUT:$(OUT).dll /LIBPATH:$(MSVC_LIB) /LIBPATH:$(UCRT_LIB) /LIBPATH:$(UM_LIB)

%.obj: %.cpp $(DEPENDS)
	clang-cl $< /w /TP /EHsc /Ox /c /I $(MSVC_INCLUDE) /I $(UCRT_INCLUDE) 

clean:
	rm -f $(OBJ) $(OUT).dll $(OUT).lib
