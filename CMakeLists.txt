cmake_minimum_required(VERSION 3.1)
project(examples)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(vtkbool)

add_subdirectory(vtkbool)

find_package(VTK REQUIRED COMPONENTS
    vtkCommonComputationalGeometry
    vtkCommonCore
    vtkFiltersGeneral
    vtkFiltersHybrid
    vtkFiltersModeling
    vtkFiltersSources
    vtkInteractionStyle
    vtkImagingOpenGL2
    vtkIOGeometry
    vtkIOImage
    vtkIOPLY
    vtkIOXML
    vtkRenderingCore
    vtkRenderingFreeType
    vtkRenderingOpenGL2
    vtkViewsInfovis
    NO_MODULE)
include(${VTK_USE_FILE})

include_directories(vtkbool/libs/vp)

find_package(GLEW REQUIRED)

add_executable(vtkbool_union vtkbool_union.cxx)
target_link_libraries(vtkbool_union ${GLEW_LIBRARIES} ${VTK_LIBRARIES} vtkbool)

add_executable(translate translate.cxx)
target_link_libraries(translate ${GLEW_LIBRARIES} ${VTK_LIBRARIES} vtkbool)

add_executable(native_union native_union.cxx)
target_link_libraries(native_union ${GLEW_LIBRARIES} ${VTK_LIBRARIES})

add_executable(torus torus.cxx)
target_link_libraries(torus ${GLEW_LIBRARIES} ${VTK_LIBRARIES})

add_executable(append_for_union append_for_union.cxx)
target_link_libraries(append_for_union ${GLEW_LIBRARIES} ${VTK_LIBRARIES})

add_executable(3dhull 3dhull.cxx)
target_link_libraries(3dhull ${GLEW_LIBRARIES} ${VTK_LIBRARIES})

add_executable(extrusion extrusion.cxx)
target_link_libraries(extrusion ${GLEW_LIBRARIES} ${VTK_LIBRARIES})

add_executable(spring spring.cxx)
target_link_libraries(spring ${GLEW_LIBRARIES} ${VTK_LIBRARIES})

add_executable(colors colors.cxx)
target_link_libraries(colors ${GLEW_LIBRARIES} ${VTK_LIBRARIES} vtkbool)
