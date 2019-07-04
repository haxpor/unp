cmake_minimum_required(VERSION 3.0.2)
project (UNP)

include(CheckFunctionExists)
include(CheckSymbolExists)
include(CheckIncludeFileCXX)
include(CheckCXXSourceCompiles)
include(CheckStructHasMember)
include(CheckTypeSize)

include_directories("${PROJECT_BINARY_DIR}/include")

cmake_host_system_information(RESULT CPU_NAME_ QUERY PROCESSOR_NAME)
cmake_host_system_information(RESULT OS_NAME_ QUERY OS_NAME)

set(CPU_VENDOR_OS "\"${CPU_NAME_}_${CMAKE_SYSTEM_PROCESSOR}_${OS_NAME_}\"")

# check for following cases
# - header file exists
# - struct definition exists
# - function definition exists in header file
# - function symbol exists in linking state
# - local file system file

# macro for checking for library
macro (check_lib libname)
  string(TOUPPER ${libname} UPPER_LIBNAME)

  find_library(HAVE_LIB${UPPER_LIBNAME} ${libname})
  if (NOT HAVE_LIB${UPPER_LIBNAME})
    message(WARNING "Not found ${libname} library")
  endif()
endmacro (check_lib)

# macro checking for function symbol
macro (check_fs fname)
  string(TOUPPER ${fname} UPPER_STR)
  check_function_exists(${fname} HAVE_${UPPER_STR})
endmacro (check_fs)

# macro checking for function prototype
macro (check_fp fname header)
  string(TOUPPER ${fname} UPPER_STR)
  check_symbol_exists(${fname} "${header}" HAVE_${UPPER_STR}_PROTO)
endmacro (check_fp)

# macro checking for struct availability from within header file
macro (check_structh stname header)
  string(TOUPPER ${stname} UPPER_STR)
  check_cxx_source_compiles("#include <${header}>\nstruct ${stname} t;\nint main(void){return 0;}" HAVE_${UPPER_STR}_STRUCT)
endmacro (check_structh)

# macro checking for struct availability without a need of knowing header file
#macro (check_struct stname)
#  string(TOUPPER ${stname} UPPER_STR)
#  string(CONCAT VAR_STR "STRUCT_" ${UPPER_STR})
#  check_type_size(${stname}
#endmacro (check_struct)

# macro checking if a struct has a target element
macro (check_struct_elem stname elemname header)
  string(TOUPPER ${stname} UPPER_STRUCT)
  string(TOUPPER ${elemname} UPPER_ELEM)
  check_struct_has_member("struct ${stname}" ${elemname} ${header} HAVE_${UPPER_STRUCT}_${UPPER_ELEM} LANGUAGE CXX)
endmacro (check_struct_elem)

# macro checking header file availability
macro (check_header header)
  string(TOUPPER ${header} UPPER_STR)
  string(REPLACE "." "_" UPPER_STR2 ${UPPER_STR})
  string(REPLACE "/" "_" UPPER_STR ${UPPER_STR2})

  check_include_file_cxx(${header} HAVE_${UPPER_STR})
endmacro (check_header)

# macro checking if file exists
macro (check_exists file)
  if (EXISTS ${file})
    string(TOUPPER ${file} UPPER_STR)
    string(REPLACE "/" "_" UPPER_STR2 ${UPPER_STR})

    # check to replace leading "/" with empty string
    string(REGEX MATCH "^_.*" HAVE_UNDERSCORE_LEADING ${UPPER_STR2})
    if (HAVE_UNDERSCORE_LEADING)
      string(LENGTH ${UPPER_STR2} STR_LENGTH)
      string(SUBSTRING ${UPPER_STR2} 1 ${STR_LENGTH}-1 RESULT_STR)
    else ()
      set(RESULT_STR ${UPPER_STR2})
    endif (HAVE_UNDERSCORE_LEADING)

    set(HAVE_${RESULT_STR} 1)
    message(STATUS "Found ${file}")
  else()
    message(WARNING "Not found file ${file}")
  endif()
endmacro (check_exists)

check_structh("addrinfo" "netdb.h")
check_header("arpa/inet.h")
check_fs(bzero)

check_exists(/dev/streams/xtiso/tcp)
check_exists(/dev/tcp)
check_exists(/dev/xti/tcp)

check_header(errno.h)
check_header(fcntl.h)
check_fs(getaddrinfo)
check_fp(getaddrinfo "netdb.h")
check_fs(gethostbyname2)
check_fs(gethostbyname_r)
check_fs(gethostname)
check_fp(gethostname "unistd.h")
check_fs(getnameinfo)
check_fp(getnameinfo "netdb.h")
check_fp(getrusage "sys/resource.h")
check_fs(hstrerror)
check_fp(hstrerror "netdb.h")
check_structh("if_nameindex" "net/if.h")
check_fs(if_nametoindex)
check_fp(if_nametoindex "net/if.h")
check_fs(inet_aton)
check_fp(inet_aton "arpa/inet.h")
check_fs(inet_pton)
check_fp(inet_pton "arpa/inet.h")
check_fs(kevent)
check_fs(kqueue)
check_lib(nsl)
check_lib(pthread)
check_lib(pthreads)
check_lib(resolv)
check_lib(xti)
check_fs(mkstemp)
check_struct_elem(msghdr msg_control "sys/socket.h") # FIXME: if we can perform without specifying header file, same for other FIXME
check_header("netconfig.h")
check_header("netdb.h")
check_header("netdir.h")
check_header("netinet/in.h")
check_header("net/if_dl.h")
check_fs(poll)
check_header("poll.h")
check_fs(pselect)
check_fp(pselect "sys/stat.h")
check_header("pthread.h")
check_header("signal.h")
check_fs(snprintf)
check_fp(snprintf "stdio.h")
check_structh("sockaddr_dl" "net/if_dl.h")
check_struct_elem(sockaddr sa_len "sys/socket.h") # FIXME
check_fs(sockatmark)
check_fp(sockatmark "sys/socket.h")
check_header("stdio.h")
check_header("stdlib.h")
check_header("strings.h")
check_header("string.h")
check_header("stropts.h")
check_struct_elem(ifreq ifr_mtu "net/if.h") # FIXME
check_structh("sockaddr_storage" "sys/socket.h") # FIXME
check_header("sys/event.h")
check_header("sys/filio.h")
check_header("sys/ioctl.h")
check_header("sys/select.h")
check_header("sys/socket.h")
check_header("sys/sockio.h")
check_header("sys/stat.h")
check_header("sys/sysctl.h")
check_header("sys/time.h")
check_header("sys/types.h")
check_header("sys/uio.h")
check_header("sys/un.h")
check_header("sys/wait.h")
check_structh("timespec" "time.h")
check_header("time.h")
check_header("unistd.h")
check_fs(vsnprintf)
check_header("xti.h")
check_header("xti_inet.h")

# check if system support IPV4
execute_process(COMMAND ifconfig
                COMMAND grep inet
                COMMAND grep -v inet6
                OUTPUT_VARIABLE IPV4_RESULT ERROR_VARIABLE IPV4_RESULT)
if ("${IPV4_RESULT}" STREQUAL "")
  message(WARNING "System doesn't support IPV4")
  unset(IPV4)
else()
  message(STATUS "System supports IPV4")
  set(IPV4 1)
endif()

# check if system supports IPV6
execute_process(COMMAND ifconfig 
                COMMAND grep inet6 
                OUTPUT_VARIABLE IPV6_RESULT ERROR_VARIABLE IPV6_RESULT)
if ("${IPV6_RESULT}" STREQUAL "")
  message(WARNING "System doesn't support IPV6")
  unset(IPV6)
else()
  message(STATUS "System supports IPV6")
  set(IPV6 1)
endif()

# check if system supports MULTICAST
execute_process(COMMAND ifconfig
                COMMAND grep MULTICAST
                OUTPUT_VARIABLE MULTICAST_RESULT ERROR_VARIABLE MULTICAST_RESULT)
if ("${MULTICAST_RESULT}" STREQUAL "")
  message(WARNING "System doesn't support MULTICAST")
  unset(MCAST)
else()
  message(STATUS "System supports MULTICAST")
  set(MCAST 1)
endif()

# get size of sa_family field in sockaddr structure
# if not exist, then undefine it
set(CMAKE_EXTRA_INCLUDE_FILES "sys/socket.h")
check_type_size("((struct sockaddr*)0)->sa_family" SA_FAMILY_SIZE)
if (${SA_FAMILY_SIZE} STREQUAL "" AND ${SA_FAMILY_SIZE} STREQUAL "0")
  message(WARNING "Not found sa_family field in struct sockaddr")
  unset(SA_FAMILY_T)
else()
  message(STATUS "Size of sa_family in sockaddr is ${SA_FAMILY_SIZE}")
  set(SA_FAMILY_T "${SA_FAMILY_SIZE}")
endif()

# check if all ANSI C header files are available
# from https://www.gnu.org/software/autoconf/manual/autoconf-2.67/html_node/Particular-Headers.html,
# we will check for stdlib.h, stdarg.h, string.h, and float.h
check_header("stdlib.h")
check_header("stdarg.h")
check_header("string.h")
check_header("float.h")
if (HAVE_STDLIB_H AND HAVE_STDARG_H AND HAVE_STRING_H AND HAVE_FLOAT_H)
  message(STATUS "Found all ANSI C header files")
  set(STDC_HEADERS 1)
else()
  message(WARNING "Not found all ANSI C header files")
  unset(STDC_HEADERS)
endif()

# try to include both header files safely
check_cxx_source_compiles("#include <sys/time.h>\n#include <time.h>\nint main(void){return 0;}" TIME_WITH_SYS_TIME)

# check if system supports unix domain
# check if struct sockaddr_un is there, and has socketpair() function
check_structh("sockaddr_un" "sys/un.h") # FIXME
check_fs(socketpair)
if (HAVE_SOCKADDR_UN_STRUCT AND HAVE_SOCKETPAIR)
  message(STATUS "System supports UNIX DOMAIN")
  set(UNIXDOMAIN 1)
  set(UNIXdomain 1)
else()
  message(WARNING "System doesn't support UNIX DOMAIN")
  unset(UNIXDOMAIN)
  unset(UNIXdomain)
endif()

configure_file (
  "${PROJECT_SOURCE_DIR}/include/config.h.in"
  "${PROJECT_BINARY_DIR}/include/config.h"
  )

message(STATUS "CPU_VENDOR_OS = '${CPU_VENDOR_OS}'")
