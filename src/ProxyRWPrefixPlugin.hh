#pragma once
#include "XrdCl/XrdClPlugInInterface.hh"

namespace xrdcl_proxy
{
//------------------------------------------------------------------------------
//! XrdCl proxy prefix plugin factory
//------------------------------------------------------------------------------
class ProxyRWFactory: public XrdCl::PlugInFactory
{
public:
  //----------------------------------------------------------------------------
  //! Construtor
  //!
  //! @param config map containing configuration parameters
  //----------------------------------------------------------------------------
  ProxyRWFactory(const std::map<std::string, std::string>* config);

  //----------------------------------------------------------------------------
  //! Destructor
  //----------------------------------------------------------------------------
  virtual ~ProxyRWFactory();

  //----------------------------------------------------------------------------
  //! Create a file plug-in for the given URL
  //----------------------------------------------------------------------------
  virtual XrdCl::FilePlugIn* CreateFile(const std::string& url);

  //----------------------------------------------------------------------------
  //! Create a file system plug-in for the given URL
  //----------------------------------------------------------------------------
  virtual XrdCl::FileSystemPlugIn* CreateFileSystem(const std::string& url);
};

} // namespace xrdcl_proxy
