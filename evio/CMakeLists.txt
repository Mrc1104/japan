cmake_minimum_required(VERSION 3.5)

#----------------------------------------------------------------------------
# Names of the main items built here
set(LIBNAME ev)
set(ALTLIBNAME wrEvio)
set(SUBPROJECT_NAME Decode)

#----------------------------------------------------------------------------
# Required dependencies
find_package(EVIO CONFIG QUIET)
if(NOT EVIO_FOUND)
  find_package(EVIO MODULE)
  if(NOT EVIO_FOUND)
    include(FetchBuildEVIO)
  endif()
endif()

if(ONLINE_ET)
  find_package(ET)
endif()

#----------------------------------------------------------------------------
# Sources and headers
set(src
    src/THaCodaData.cxx
    src/THaCodaFile.cxx
  )
set(headers
    include/CustomAlloc.h
    include/Decoder.h
    include/Helper.h
    include/THaCodaData.h
    include/THaCodaFile.h
)

#----------------------------------------------------------------------------
# libevio
add_library(${LIBNAME} SHARED ${src} ${headers} lib${LIBNAME}Dict.cxx)
add_library(${PROJECT_NAME}::${SUBPROJECT_NAME} ALIAS ${LIBNAME})

target_compile_options(${LIBNAME}
  PUBLIC
    ${${MAIN_PROJECT_NAME_UC}_CXX_FLAGS_LIST}
  PRIVATE
    ${${MAIN_PROJECT_NAME_UC}_DIAG_FLAGS_LIST}
  )

target_include_directories(${LIBNAME}
  PUBLIC
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  )

target_link_libraries(${LIBNAME}
  PUBLIC
    ROOT::Core
    #${ONLIBS}    # et.h is in the THaEtClient.h public interface
  PRIVATE
    EVIO::EVIO
  )


install(TARGETS ${LIBNAME}
  EXPORT ${MAIN_PROJECT_NAME_LC}-exports
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  )
install(FILES ${headers} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

#----------------------------------------------------------------------------
# ROOT dictionary
build_root_dictionary(lib${LIBNAME} ${headers}
  TARGETS ${LIBNAME}
  PCMNAME ${LIBNAME}
  LINKDEF ${ALTLIBNAME}_LinkDef.h
  )
