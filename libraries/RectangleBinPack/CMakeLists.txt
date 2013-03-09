# CMakeLists.txt for RectangleBinPack

include_directories(
   ${CMAKE_SOURCE_DIR}/libraries/RectangleBinPack/RectangleBinPack
   # ${CMAKE_SOURCE_DIR}/libraries/renderStack_toolkit/include
)

add_library(
   RectangleBinPack STATIC
   RectangleBinPack/GuillotineBinPack.cpp
   RectangleBinPack/GuillotineBinPack.h
   RectangleBinPack/MaxRectsBinPack.cpp
   RectangleBinPack/MaxRectsBinPack.h
   RectangleBinPack/Rect.cpp
   RectangleBinPack/Rect.h
   RectangleBinPack/ShelfBinPack.cpp
   RectangleBinPack/ShelfBinPack.h
   RectangleBinPack/ShelfNextFitBinPack.cpp
   RectangleBinPack/ShelfNextFitBinPack.h
   RectangleBinPack/SkylineBinPack.cpp
   RectangleBinPack/SkylineBinPack.h
)

install(TARGETS RectangleBinPack DESTINATION lib)

