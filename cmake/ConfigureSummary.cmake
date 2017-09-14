# cmake/ConfigureSummary.cmake
#
# This file is part of NEST.
#
# Copyright (C) 2004 The NEST Initiative
#
# NEST is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) any later version.
#
# NEST is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with NEST.  If not, see <http://www.gnu.org/licenses/>.

function( NEST_PRINT_CONFIG_SUMMARY )
  message( "" )
  message( "--------------------------------------------------------------------------------" )
  message( "NEST Configuration Summary" )
  message( "--------------------------------------------------------------------------------" )
  message( "" )
  message( "Build type          : ${CMAKE_BUILD_TYPE}" )
  message( "Target System       : ${CMAKE_SYSTEM_NAME}" )
  message( "Cross Compiling     : ${CMAKE_CROSSCOMPILING}" )
  message( "C compiler          : ${CMAKE_C_COMPILER}" )
  message( "C compiler flags    : ${ALL_CFLAGS}" )
  message( "C++ compiler        : ${CMAKE_CXX_COMPILER}" )
  message( "C++ compiler flags  : ${ALL_CXXFLAGS}" )
  message( "Build dynamic       : ${BUILD_SHARED_LIBS}" )
  message( "" )
  message( "Built-in modules    : ${SLI_MODULES}" )
  if ( external-modules )
    message( "User modules        : ${external-modules}" )
    foreach ( mod ${external-modules} )
      message( "  ${mod}:" )
      message( "    Header          : ${${mod}_EXT_MOD_INCLUDE}" )
      message( "    Library         : ${${mod}_EXT_MOD_LIBRARY}" )
      message( "" )
    endforeach ()
  else ()
    message( "User modules        : None" )
  endif ()

  if ( HAVE_PYTHON )
    message( "Python bindings     : Yes (Python ${PYTHON_VERSION}: ${PYTHON})" )
    message( "       Includes     : ${PYTHON_INCLUDE_DIRS}" )
    message( "       Libraries    : ${PYTHON_LIBRARIES}" )
    message( "" )
    if ( CYTHON_FOUND )
      message( "Cython bindings     : Yes (Cython ${CYTHON_VERSION}: ${CYTHON_EXECUTABLE})" )
    else ()
      message( "Cython bindings     : No. Make sure to cythonize `pynestkernel.pyx` before." )
    endif ()
  else ()
    message( "Python bindings     : No" )
  endif ()

  if ( OPENMP_FOUND )
    message( "Use threading       : Yes (OpenMP: ${OpenMP_CXX_FLAGS})" )
  else ()
    message( "Use threading       : No" )
  endif ()

  if ( HAVE_GSL )
    message( "Use GSL             : Yes (GSL ${GSL_VERSION})" )
    message( "    Includes        : ${GSL_INCLUDE_DIRS}" )
    message( "    Libraries       : ${GSL_LIBRARIES}" )
    message( "" )
  else ()
    message( "Use GSL             : No" )
  endif ()

  if ( HAVE_READLINE )
    message( "Use Readline        : Yes (GNU Readline ${READLINE_VERSION})" )
    message( "    Includes        : ${READLINE_INCLUDE_DIRS}" )
    message( "    Libraries       : ${READLINE_LIBRARIES}" )
    message( "" )
  else ()
    message( "Use Readline        : No" )
  endif ()

  if ( HAVE_LIBLTDL )
    message( "Use libltdl         : Yes (LTDL ${LTDL_VERSION})" )
    message( "    Includes        : ${LTDL_INCLUDE_DIRS}" )
    message( "    Libraries       : ${LTDL_LIBRARIES}" )
    message( "" )
  else ()
    message( "Use libltdl         : No" )
  endif ()

  if ( DOXYGEN_FOUND )
    message( "Use doxygen         : Yes (${DOXYGEN_EXECUTABLE})" )
    message( "                    : target `doc` available" )
    if ( DOXYGEN_DOT_FOUND )
      message( "    `dot` available : Yes (${DOXYGEN_DOT_EXECUTABLE})" )
      message( "                    : target `fulldoc` available" )
    endif ()
  else ()
    message( "Use doxygen         : No" )
  endif ()

  if ( HAVE_MPI )
    message( "Use MPI             : Yes (MPI: ${MPI_CXX_COMPILER})" )
    message( "    FLAGS           : ${MPI_CXX_COMPILE_FLAGS}" )
    message( "    Includes        : ${MPI_CXX_INCLUDE_PATH}" )
    message( "    Link Flags      : ${MPI_CXX_LINK_FLAGS}" )
    message( "    Libraries       : ${MPI_CXX_LIBRARIES}" )
    message( "" )
  else ()
    message( "Use MPI             : No" )
  endif ()

  if ( HAVE_MUSIC )
    message( "Use MUSIC           : Yes (MUSIC ${MUSIC_VERSION})" )
    message( "    Includes        : ${MUSIC_INCLUDE_DIRS}" )
    message( "    Libraries       : ${MUSIC_LIBRARIES}" )
    message( "" )
  else ()
    message( "Use MUSIC           : No" )
  endif ()

  if ( HAVE_LIBNEUROSIM )
    message( "Use libneurosim     : Yes (LibNeurosim ${LIBNEUROSIM_VERSION})" )
    message( "    Includes        : ${LIBNEUROSIM_INCLUDE_DIRS}" )
    message( "    Libraries       : ${LIBNEUROSIM_LIBRARIES}" )
    message( "" )
  else ()
    message( "Use libneurosim     : No" )
  endif ()

  if ( HAVE_BOOST )
    message( "Use Boost           : Yes (Boost ${BOOST_VERSION})" )
    message( "    Includes        : ${BOOST_INCLUDE_DIR}" )
    message( "    Libraries       : ${BOOST_LIBRARIES}" )
    message( "" )
  else ()
    message( "Use Boost           : No" )
  endif ()

  if ( with-libraries )
    message( "Additional libraries:" )
    foreach ( lib ${with-libraries} )
      message( "                     ${lib}" )
    endforeach ()
  endif ()

  if ( with-includes )
    message( "Additional includes:" )
    foreach ( inc ${with-includes} )
      message( "                     -I${inc}" )
    endforeach ()
  endif ()

  message( "" )
  message( "--------------------------------------------------------------------------------" )
  message( "" )
  message( "The NEST executable will be installed to:" )
  message( "  ${CMAKE_INSTALL_FULL_BINDIR}/" )
  message( "" )
  message( "Documentation and examples will be installed to:" )
  message( "  ${CMAKE_INSTALL_FULL_DOCDIR}/" )
  message( "" )
  if ( HAVE_PYTHON )
    message( "PyNEST will be installed to:" )
    message( "    ${CMAKE_INSTALL_PREFIX}/${PYEXECDIR}" )
    message( "" )
  endif ()
  message( "To set necessary environment variables, add the following line" )
  message( "to your ~/.bashrc :" )
  message( "  source ${CMAKE_INSTALL_FULL_BINDIR}/nest_vars.sh" )
  message( "" )
  message( "--------------------------------------------------------------------------------" )
  message( "" )

  if ( NOT HAVE_GSL )
    message( "" )
    message( "ATTENTION!" )
    message( "You are about to compile NEST without the GNU Scientific" )
    message( "Library or your GSL is to old (before v1.11). This means" )
    message( "that conductance-based neuron models and some random number" )
    message( "generators will not be available." )
    message( "" )
    message( "--------------------------------------------------------------------------------" )
    message( "" )
  endif ()

  message( "You can now build and install NEST with" )
  message( "  make" )
  message( "  make install" )
  message( "  make installcheck" )
  message( "" )

  if ( HAVE_MPI )
    message( "You have configured NEST with support for distributed computing." )
    message( "After running make install, please tell NEST how to start a" )
    message( "distributed job on your system before running make installcheck." )
    message( "You can do this by modifying the template for command /mpirun in" )
    message( "~/.nestrc, which is created by the first start of NEST." )
    message( "" )
  endif ()

  message( "If you experience problems with the installation or the use of NEST," )
  message( "please see http://www.nest-simulator.org/frequently_asked_questions" )
  message( "or go to http://www.nest-simulator.org/community to find out how to" )
  message( "join the user mailing list." )
  message( "" )
endfunction()
