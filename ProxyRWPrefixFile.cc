#include "ProxyRWPrefixFile.hh"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <cctype>
#include "XrdCl/XrdClLog.hh"
#include <sys/socket.h>
#include <netdb.h>

namespace xrdcl_proxy
{
//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
ProxyRWPrefixFile::ProxyRWPrefixFile():
  mIsOpen(false),
  pFile(0)
{}

//------------------------------------------------------------------------------
// Destructor
//------------------------------------------------------------------------------
ProxyRWPrefixFile::~ProxyRWPrefixFile()
{
  if (pFile) {
    delete pFile;
  }
}

//------------------------------------------------------------------------------
// Open
//------------------------------------------------------------------------------
XRootDStatus
ProxyRWPrefixFile::Open(const std::string& url,
                      OpenFlags::Flags flags,
                      Access::Mode mode,
                      ResponseHandler* handler,
                      uint16_t timeout)
{
  XRootDStatus st;

  if (mIsOpen) {
    st = XRootDStatus(stError, errInvalidOp);
    return st;
  }

  pFile = new XrdCl::File(false);
  std::string open_url = ConstructFinalUrl(url);
  st = pFile->Open(open_url, flags, mode, handler, timeout);

  if (st.IsOK()) {
    mIsOpen = true;
  }

  return st;
}

//------------------------------------------------------------------------------
// Get proxy prefix Url
//------------------------------------------------------------------------------
std::string
ProxyRWPrefixFile::GetPrefixUrl() const
{
  std::string url_prefix = (getenv("XROOT_PROXY") ? getenv("XROOT_PROXY") : "");

  // Try out also the lower-case one
  if (url_prefix.empty()) {
    url_prefix = (getenv("xroot_proxy") ? getenv("xroot_proxy") : "");
  }

  return url_prefix;
}

//------------------------------------------------------------------------------
// Trim whitespaces from both ends for a string
//------------------------------------------------------------------------------
std::string
ProxyRWPrefixFile::trim(const std::string& in) const
{
  std::string::const_iterator wsfront, wsback;
  std::string::const_reverse_iterator rwsback;
  wsfront = in.begin();
  rwsback = in.rbegin();

  while (*wsfront == ' ') {
    ++wsfront;
  }

  while (*rwsback == ' ') {
    ++rwsback;
  }

  wsback = rwsback.base();
  return (wsback <= wsfront ? std::string() : std::string(wsfront, wsback));
  /* TODO: To be used when C++11 is available
     auto wsfront = std::find_if_not(in.begin(), in.end(),
     [](int c) -> bool {return std::isspace(c);});
     auto wsback = std::find_if_not(in.rbegin(), in.rend(),
     [](int c) -> bool {return std::isspace(c);}).base();
     return (wsback <= wsfront ? std::string() : std::string(wsfront, wsback));
  */
}

//------------------------------------------------------------------------------
// Get list of domains which are NOT to be prefixed
//------------------------------------------------------------------------------
std::list<std::string>
ProxyRWPrefixFile::GetExclDomains() const
{
  std::string excl_domains = (getenv("XROOT_PROXY_EXCL_DOMAINS") ?
                              getenv("XROOT_PROXY_EXCL_DOMAINS") : "");

  if (excl_domains.empty()) {
    return std::list<std::string>();
  }

  char delim = ',';
  std::string item;
  std::list<std::string> lst;
  std::stringstream ss(excl_domains);

  while (getline(ss, item, delim)) {
    lst.push_back(trim(item));
  }

  return lst;
}

//------------------------------------------------------------------------------
// Construct final Url
//------------------------------------------------------------------------------
std::string
ProxyRWPrefixFile::ConstructFinalUrl(const std::string& orig_surl) const
{
  std::string final_surl = orig_surl;
  std::string url_prefix = GetPrefixUrl();
  XrdCl::Log* log = DefaultEnv::GetLog();
  log->Debug(1, "url=%s, prefix_url=%s", orig_surl.c_str(), url_prefix.c_str());

  if (!url_prefix.empty()) {
    bool exclude = false;
    std::list<std::string> lst_excl = GetExclDomains();
    XrdCl::URL orig_url(orig_surl);
    std::string orig_host = orig_url.GetHostId();
    // Remove port if present
    size_t pos = orig_host.find(':');

    if (pos != std::string::npos) {
      orig_host = orig_host.substr(0, pos);
    }

    orig_host = GetFqdn(orig_host);

    for (std::list<std::string>::iterator it_excl = lst_excl.begin();
         it_excl != lst_excl.end(); ++it_excl) {
      if (url_prefix.size() < it_excl->size()) {
        continue;
      }

      if (std::equal(it_excl->rbegin(), it_excl->rend(), orig_host.rbegin())) {
        exclude = true;
        break;
      }
    }

    if (!exclude) {
      final_surl.insert(0, url_prefix);
    }
  }

  log->Debug(1, "final_url=%s", final_surl.c_str());
  return final_surl;
}

//------------------------------------------------------------------------------
// Get FQDN for specified host
//------------------------------------------------------------------------------
std::string
ProxyRWPrefixFile::GetFqdn(const std::string& hostname) const
{
  XrdCl::Log* log = DefaultEnv::GetLog();
  std::string fqdn = hostname;
  struct addrinfo hints, *info;
  int gai_result;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC; /*either IPV4 or IPV6*/
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_CANONNAME;

  if ((gai_result = getaddrinfo(hostname.c_str(), NULL, &hints, &info)) != 0) {
    log->Error(1, "getaddrinfo: %s", gai_strerror(gai_result));
    return fqdn;
  }

  if (info) {
    fqdn = info->ai_canonname;
  }

  freeaddrinfo(info);
  return fqdn;
}

} // namespace xrdcl_proxy
