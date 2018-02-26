find_package(XRootD REQUIRED)

include_directories ( ${XROOTD_INCLUDE_DIR} ./)

add_library(
  XrdClProxyRW-${PLUGIN_VERSION}
  SHARED
  ProxyRWPrefixFile.cc  ProxyRWPrefixFile.hh  ProxyRWPrefixPlugin.cc  ProxyRWPrefixPlugin.hh
)

target_link_libraries(
  XrdClProxyRW-${PLUGIN_VERSION}
  XrdCl
  XrdUtils
)

set_target_properties(
  XrdClProxyRW-${PLUGIN_VERSION}
  PROPERTIES
  INTERFACE_LINK_LIBRARIES ""
  LINK_INTERFACE_LIBRARIES ""
)

install(
  TARGETS XrdClProxyRW-${PLUGIN_VERSION}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} )
