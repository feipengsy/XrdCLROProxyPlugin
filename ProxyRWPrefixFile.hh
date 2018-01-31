#pragma once
#include "XrdCl/XrdClDefaultEnv.hh"
#include "XrdCl/XrdClPlugInInterface.hh"

using namespace XrdCl;

namespace xrdcl_proxy
{

class ProxyRWPrefixFile: public XrdCl::FilePlugIn
{
public:
  //----------------------------------------------------------------------------
  //! Constructor
  //----------------------------------------------------------------------------
  ProxyRWPrefixFile();

  //----------------------------------------------------------------------------
  //! Destructor
  //----------------------------------------------------------------------------
  virtual ~ProxyRWPrefixFile();

  //----------------------------------------------------------------------------
  //! Open
  //----------------------------------------------------------------------------
  virtual XRootDStatus Open(const std::string& url,
                            OpenFlags::Flags flags,
                            Access::Mode mode,
                            ResponseHandler* handler,
                            uint16_t timeout);

  //----------------------------------------------------------------------------
  //! Close
  //----------------------------------------------------------------------------
  virtual XRootDStatus Close(ResponseHandler* handler,
                             uint16_t         timeout)
  {
    return pFile->Close(handler, timeout);
  }

  //----------------------------------------------------------------------------
  //! Stat
  //----------------------------------------------------------------------------
  virtual XRootDStatus Stat(bool             force,
                            ResponseHandler* handler,
                            uint16_t         timeout)
  {
    return pFile->Stat(force, handler, timeout);
  }


  //----------------------------------------------------------------------------
  //! Read
  //----------------------------------------------------------------------------
  virtual XRootDStatus Read(uint64_t         offset,
                            uint32_t         size,
                            void*            buffer,
                            ResponseHandler* handler,
                            uint16_t         timeout)
  {
    return pFile->Read(offset, size, buffer, handler, timeout);
  }

  //----------------------------------------------------------------------------
  //! Write
  //----------------------------------------------------------------------------
  virtual XRootDStatus Write(uint64_t         offset,
                             uint32_t         size,
                             const void*      buffer,
                             ResponseHandler* handler,
                             uint16_t         timeout)
  {
    return pFile->Write(offset, size, buffer, handler, timeout);
  }

  //----------------------------------------------------------------------------
  //! Sync
  //----------------------------------------------------------------------------
  virtual XRootDStatus Sync(ResponseHandler* handler,
                            uint16_t         timeout)
  {
    return pFile->Sync(handler, timeout);
  }

  //----------------------------------------------------------------------------
  //! Truncate
  //----------------------------------------------------------------------------
  virtual XRootDStatus Truncate(uint64_t         size,
                                ResponseHandler* handler,
                                uint16_t         timeout)
  {
    return pFile->Truncate(size, handler, timeout);
  }

  //----------------------------------------------------------------------------
  //! VectorRead
  //----------------------------------------------------------------------------
  virtual XRootDStatus VectorRead(const ChunkList& chunks,
                                  void*            buffer,
                                  ResponseHandler* handler,
                                  uint16_t         timeout)
  {
    return pFile->VectorRead(chunks, buffer, handler, timeout);
  }

  //----------------------------------------------------------------------------
  //! Fcntl
  //----------------------------------------------------------------------------
  virtual XRootDStatus Fcntl(const Buffer&    arg,
                             ResponseHandler* handler,
                             uint16_t         timeout)
  {
    return pFile->Fcntl(arg, handler, timeout);
  }

  //----------------------------------------------------------------------------
  //! Visa
  //----------------------------------------------------------------------------
  virtual XRootDStatus Visa(ResponseHandler* handler,
                            uint16_t         timeout)
  {
    return pFile->Visa(handler, timeout);
  }

  //----------------------------------------------------------------------------
  //! IsOpen
  //----------------------------------------------------------------------------
  virtual bool IsOpen() const
  {
    return pFile->IsOpen();
  }

  //----------------------------------------------------------------------------
  //! SetProperty
  //----------------------------------------------------------------------------
  virtual bool SetProperty(const std::string& name,
                           const std::string& value)
  {
    return pFile->SetProperty(name, value);
  }

  //----------------------------------------------------------------------------
  //! GetProperty
  //----------------------------------------------------------------------------
  virtual bool GetProperty(const std::string& name,
                           std::string& value) const
  {
    return pFile->GetProperty(name, value);
  }

private:

  //----------------------------------------------------------------------------
  //! Trim whitespaces from both ends of a string
  //!
  //! @return trimmed string
  //----------------------------------------------------------------------------
  inline std::string trim(const std::string& in) const;

  //----------------------------------------------------------------------------
  //! Get proxy prefix URL from the environment
  //!
  //! @return proxy prefix RUL
  //----------------------------------------------------------------------------
  inline std::string GetPrefixUrl() const;

  //----------------------------------------------------------------------------
  //! Get list of domains which are NOT to be prefixed
  //!
  //! @return list of excluded domains
  //----------------------------------------------------------------------------
  std::list<std::string> GetExclDomains() const;

  //----------------------------------------------------------------------------
  //! Construct final URL if there is a proxy prefix URL specified and if the
  //! exclusion list is satisfied
  //!
  //! @param orig_url original url
  //!
  //! @return final URL
  //----------------------------------------------------------------------------
  std::string ConstructFinalUrl(const std::string& orig_url) const;

  //----------------------------------------------------------------------------
  //! Get FQDN for specified host
  //!
  //! @param hostname hostname without domain
  //!
  //! @return FQDN
  //----------------------------------------------------------------------------
  std::string GetFqdn(const std::string& hostname) const;

  bool mIsOpen;
  XrdCl::File* pFile;
};

} // namespace xrdcl_proxy
