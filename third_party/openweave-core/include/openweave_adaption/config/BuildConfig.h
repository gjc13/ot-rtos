/* Define to 1 if you want to enable WoBle over bluez. */
#define CONFIG_BLE_PLATFORM_BLUEZ 0

/* Define to 1 if you want to use the OpenWeave Device Layer. */
#define CONFIG_DEVICE_LAYER 0

/* Define to 1 if you want to use Weave with a system that supports
   callback-based vcbprintf */
#define CONFIG_HAVE_VCBPRINTF 0

/* Define to 1 if you want to use Weave with a system that supports
   callback-based vcbprintf */
#define CONFIG_HAVE_VSNPRINTF_EX 0

/* Define to 1 if you want to use Weave with the Bluetooth Low Energy network
   stack. */
#define CONFIG_NETWORK_LAYER_BLE 0

/* Define to 1 if you want to use Weave with the Internet Protocol network
   stack. */
#define CONFIG_NETWORK_LAYER_INET 1

/* Weave target network stack(s) */
#define CONFIG_TARGET_NETWORKS "sockets"

/* Define to 1 if your C++ compiler doesn't accept -c and -o together. */
/* #undef CXX_NO_MINUS_C_MINUS_O */

/* Define to 1 if you have the <BleProjectConfig.h> header file. */
#define HAVE_BLEPROJECTCONFIG_H 1

/* Define to 1 if the system has the type `clockid_t'. */
#define HAVE_CLOCKID_T 0

/* Define to 1 if you have the `clock_gettime' function. */
#define HAVE_CLOCK_GETTIME 0

/* Define to 1 if you have the `clock_settime' function. */
#define HAVE_CLOCK_SETTIME 0

/* Define to 1 if you have the <ctype.h> header file. */
#define HAVE_CTYPE_H 1

/* Define to 1 if you have the <curl/curl.h> header file. */
/* #undef HAVE_CURL_CURL_H */

/* Define to 1 if you have the <curl/easy.h> header file. */
/* #undef HAVE_CURL_EASY_H */

/* define if the compiler supports basic C++11 syntax */
#define HAVE_CXX11 1

/* Define to 1 if you have the declaration of `CLOCK_BOOTTIME', and to 0 if
   you don't. */
#define HAVE_DECL_CLOCK_BOOTTIME 1

/* Define to 1 if you have the declaration of `CLOCK_MONOTONIC', and to 0 if
   you don't. */
#define HAVE_DECL_CLOCK_MONOTONIC 1

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define to 1 if you have the <Foundation/Foundation.h> header file. */
/* #undef HAVE_FOUNDATION_FOUNDATION_H */

/* Define to 1 if you have the `free' function. */
#define HAVE_FREE 1

/* Define to 1 if you have the `freeifaddrs' function. */
#define HAVE_FREEIFADDRS 0

/* Define to 1 if you have the `getifaddrs' function. */
#define HAVE_GETIFADDRS 0

/* Define to 1 if you have the `gettimeofday' function. */
#define HAVE_GETTIMEOFDAY 0

/* Define to 1 if your <netinet/icmp6.h> header file defines the ICMP6_FILTER
   socket option. */
#define HAVE_ICMP6_FILTER 0

/* Define to 1 if you have the <ifaddrs.h> header file. */
#define HAVE_IFADDRS_H 0

/* Define to 1 if you have the <InetProjectConfig.h> header file. */
/* #undef HAVE_INETPROJECTCONFIG_H */

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the <linux/if_tun.h> header file. */
#define HAVE_LINUX_IF_TUN_H 0

/* Define to 1 if you have the <linux/ipv6_route.h> header file. */
/* #undef HAVE_LINUX_IPV6_ROUTE_H */

/* Define to 1 if you have the <linux/route.h> header file. */
/* #undef HAVE_LINUX_ROUTE_H */

/* Define to 1 if you have the `localtime' function. */
#define HAVE_LOCALTIME 0

/* Define to 1 if you have the <lwip/debug.h> header file. */
#define HAVE_LWIP_DEBUG_H  1

/* Define to 1 if you have the <lwip/dns.h> header file. */
#define HAVE_LWIP_DNS_H 1

/* Define to 1 if you have the <lwip/err.h> header file. */
#define HAVE_LWIP_ERR_H 1
/* #undef HAVE_LWIP_ERR_H */

/* Define to 1 if you have the <lwip/ethip6.h> header file. */
#define HAVE_LWIP_ETHIP6_H 1
/* #undef HAVE_LWIP_ETHIP6_H */

/* Define to 1 if you have the <lwip/init.h> header file. */
#define HAVE_LWIP_INIT_H 1
/* #undef HAVE_LWIP_INIT_H */

/* Define to 1 if you have the <lwip/ip_addr.h> header file. */
#define HAVE_LWIP_IP_ADDR_H 1
/* #undef HAVE_LWIP_IP_ADDR_H */

/* Define to 1 if you have the <lwip/ip.h> header file. */
#define HAVE_LWIP_IP_H 1
/* #undef HAVE_LWIP_IP_H */

/* Define to 1 if you have the <lwip/mem.h> header file. */
#define HAVE_LWIP_MEM_H 1
/* #undef HAVE_LWIP_MEM_H */

/* Define to 1 if you have the <lwip/netif.h> header file. */
#define HAVE_LWIP_NETIF_H 1
/* #undef HAVE_LWIP_NETIF_H */

/* Define to 1 if you have the <lwip/opt.h> header file. */
#define HAVE_LWIP_OPT_H 1
/* #undef HAVE_LWIP_OPT_H */

/* Define to 1 if you have the <lwip/pbuf.h> header file. */
#define HAVE_LWIP_PBUF_H 1
/* #undef HAVE_LWIP_PBUF_H */

/* Define to 1 if you have the <lwip/raw.h> header file. */
#define HAVE_LWIP_RAW_H 1
/* #undef HAVE_LWIP_RAW_H */

/* Define to 1 if you have the <lwip/snmp.h> header file. */
#define HAVE_LWIP_SNMP_H 1
/* #undef HAVE_LWIP_SNMP_H */

/* Define to 1 if you have the <lwip/stats.h> header file. */
#define HAVE_LWIP_STATS_H 1
/* #undef HAVE_LWIP_STATS_H */

/* Define to 1 if you have the <lwip/sys.h> header file. */
#define HAVE_LWIP_SYS_H 1
/* #undef HAVE_LWIP_SYS_H */

/* Define to 1 if you have the <lwip/tcpip.h> header file. */
#define HAVE_LWIP_TCPIP_H 1
/* #undef HAVE_LWIP_TCPIP_H */

/* Define to 1 if you have the <lwip/tcp.h> header file. */
#define HAVE_LWIP_TCP_H 1
/* #undef HAVE_LWIP_TCP_H */

/* Define to 1 if you have the <lwip/udp.h> header file. */
#define HAVE_LWIP_UDP_H 1
/* #undef HAVE_LWIP_UDP_H */

/* Define to 1 if you have the `malloc' function. */
#define HAVE_MALLOC 1

/* Define to 1 if you have the `memchr' function. */
#define HAVE_MEMCHR 1

/* Define to 1 if you have the `memcmp' function. */
#define HAVE_MEMCMP 1

/* Define to 1 if you have the `memmove' function. */
#define HAVE_MEMMOVE 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the `memset' function. */
#define HAVE_MEMSET 1

/* Define to 1 if you have the <netinet/icmp6.h> header file. */
#define HAVE_NETINET_ICMP6_H 0

/* Define to 1 if you have the <netinet/in.h> header file. */
#define HAVE_NETINET_IN_H 0

/* Define to 1 if you have the <new> header file. */
#define HAVE_NEW 1

/* Define to 1 if you have the <nlassert.h> header file. */
/* #undef HAVE_NLASSERT_H */

/* Define to 1 if you have the <nlbyteorder.h> header file. */
/* #undef HAVE_NLBYTEORDER_H */

/* Define to 1 if you have the <nlfaultinjection.hpp> header file. */
/* #undef HAVE_NLFAULTINJECTION_HPP */

/* Define to 1 if you have the <nlunit-test.h> header file. */
/* #undef HAVE_NLUNIT_TEST_H */

/* Define to 1 if you have the <openssl/aes.h> header file. */
#define HAVE_OPENSSL_AES_H 0

/* Define to 1 if you have the <openssl/bn.h> header file. */
#define HAVE_OPENSSL_BN_H 0

/* Define to 1 if you have the <openssl/ec.h> header file. */
#define HAVE_OPENSSL_EC_H 0

/* Define to 1 if you have the <openssl/err.h> header file. */
#define HAVE_OPENSSL_ERR_H 0

/* Define to 1 if you have the <openssl/evp.h> header file. */
#define HAVE_OPENSSL_EVP_H 1

/* Define to 1 if you have the <openssl/sha.h> header file. */
#define HAVE_OPENSSL_SHA_H 0

/* Define if you have POSIX threads libraries and header files. */
#define HAVE_PTHREAD 0

/* Have PTHREAD_PRIO_INHERIT. */
#define HAVE_PTHREAD_PRIO_INHERIT 1

/* Define to 1 if you have the `realloc' function. */
#define HAVE_REALLOC 0

/* Define to 1 if you have the `snprintf' function. */
#define HAVE_SNPRINTF 1

/* Define to 1 if your <sys/socket.h> header file defines the SO_BINDTODEVICE
   socket option. */
#define HAVE_SO_BINDTODEVICE 0

/* Define to 1 if stdbool.h conforms to C99. */
#define HAVE_STDBOOL_H 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdio.h> header file. */
#define HAVE_STDIO_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the `strchr' function. */
#define HAVE_STRCHR 1

/* Define to 1 if you have the `strcmp' function. */
#define HAVE_STRCMP 1

/* Define to 1 if you have the `strdup' function. */
#define HAVE_STRDUP 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the `strlen' function. */
#define HAVE_STRLEN 1

/* Define to 1 if the system has the type `struct in6_rtmsg'. */
#define HAVE_STRUCT_IN6_RTMSG 1

/* Define to 1 if you have the <SystemProjectConfig.h> header file. */
#define HAVE_SYSTEMPROJECTCONFIG_H 1

/* Define to 1 if you have the <sys/socket.h> header file. */
#define HAVE_SYS_SOCKET_H 0

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 0

/* Define to 1 if you have the <sys/time.h> header file. */
#define HAVE_SYS_TIME_H 0

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 0

/* Define to 1 if you have the <time.h> header file. */
#define HAVE_TIME_H 0

/* Define to 1 if <linux/if_tun.h> header file defines the TUNGETIFF ioctl
   command. */
#define HAVE_TUNGETIFF 0

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 0

/* Define to 1 if your libc exports vcbprintf. */
#define HAVE_VCBPRINTF 0

/* Define to 1 if your libc exports vnsprintf_ex. */
#define HAVE_VSNPRINTF_EX 0

/* Define to 1 if you have the <WarmProjectConfig.h> header file. */
/* #undef HAVE_WARMPROJECTCONFIG_H */

/* Define to 1 if you have the <WeaveProjectConfig.h> header file. */
#define HAVE_WEAVEPROJECTCONFIG_H 1

/* Define to 1 if the system has the type `_Bool'. */
#define HAVE__BOOL 1

/* Define to 0 for disabling ASYNC DNS */
#define INET_CONFIG_ENABLE_ASYNC_DNS_SOCKETS 1

/* Define to 1 for enabling DNS Resolver */
#define INET_CONFIG_ENABLE_DNS_RESOLVER 0

/* Define to 1 for enabling IPv4 */
#define INET_CONFIG_ENABLE_IPV4 0

/* Define to 1 for enabling Raw Endpoint */
//#define INET_CONFIG_ENABLE_RAW_ENDPOINT 0

/* Define to 1 for enabling TCP Endpoint */
#define INET_CONFIG_ENABLE_TCP_ENDPOINT 1

/* Define to 1 for enabling TUN Endpoint (only for LWIP and Linux w/ support)
   */
#define INET_CONFIG_ENABLE_TUN_ENDPOINT 1

/* Define to 1 for enabling UDP Endpoint */
#define INET_CONFIG_ENABLE_UDP_ENDPOINT 1

/* Weave target inet network endpoint(s) */
#define INET_ENDPOINTS "all"

/* Define to 1 if you want to use Weave with the inet dns resolver. */
#define INET_WANT_ENDPOINT_DNS 0

/* Define to 1 if you want to use Weave with the inet raw endpoint. */
#define INET_WANT_ENDPOINT_RAW 1

/* Define to 1 if you want to use Weave with the inet tcp endpoint. */
#define INET_WANT_ENDPOINT_TCP 1

/* Define to 1 if you want to use Weave with the inet tun endpoint. */
#define INET_WANT_ENDPOINT_TUN 1

/* Define to 1 if you want to use Weave with the inet udp endpoint. */
#define INET_WANT_ENDPOINT_UDP 1

/* Define to 1 if you want to use Weave with IPv4. */
#define INET_WANT_IPV4 1

/* Define to the sub-directory in which libtool stores uninstalled libraries.
   */
#define LT_OBJDIR ".libs/"

/* Define to 1 if your C compiler doesn't accept -c and -o together. */
/* #undef NO_MINUS_C_MINUS_O */

/* Name of package */
//#define PACKAGE "weave"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "weave.support@nestlabs.com"

/* Define to the full name of this package. */
#define PACKAGE_NAME "Weave"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "Weave gd06513c1-dirty"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "weave"

/* Define to the home page for this package. */
#define PACKAGE_URL "https://developer.nest.com/"

/* Define to the version of this package. */
#define PACKAGE_VERSION "gd06513c1-dirty"

/* Define to necessary symbol if this constant uses a non-standard name on
   your system. */
/* #undef PTHREAD_CREATE_JOINABLE */

/* Approximation of PTHREAD_NULL since pthread.h does not define one */
#define PTHREAD_NULL 0

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Version number of package */
#define VERSION "gd06513c1-dirty"

/* Define to 1 to use the legacy WDM profile. */
#define WEAVE_CONFIG_LEGACY_WDM 1

/* Define to 1 for enabling Weave Tunnel failover */
/* #undef WEAVE_CONFIG_TUNNEL_FAILOVER_SUPPORTED */

/* Target platform name for OpenWeave Device Layer. */
#define WEAVE_DEVICE_LAYER_TARGET NONE

/* Define to 1 if you want to build the OpenWeave Device Layer for the
   Espressif ESP32. */
#define WEAVE_DEVICE_LAYER_TARGET_ESP32 0

/* Define to 1 if you want to enable WoBle Control Path and Throughput Test.
   */
#define WEAVE_ENABLE_WOBLE_TEST 0

/* Define to 1 if support for fuzzing enabled */
/* #undef WEAVE_FUZZING_ENABLED */

/* Weave logging style */
#define WEAVE_LOGGING_STYLE stdio

/* Define to 1 if you want to use Weave with the Android logging system */
#define WEAVE_LOGGING_STYLE_ANDROID 0

/* Define to 1 if you want to use Weave with an external (i.e. platform- and
   integrator-defined) logging system */
#define WEAVE_LOGGING_STYLE_EXTERNAL 0

/* Define to 1 if you want to use Weave with a C Standard I/O Library-based
   logging system */
#define WEAVE_LOGGING_STYLE_STDIO 1

/* Define to 1 if you want to use Weave with a C Standard I/O Library-based
   logging system,but override, at link-time an external (i.e. platform- and
   integrator-defined) logging system */
#define WEAVE_LOGGING_STYLE_STDIO_WEAK 0

/* Define to 1 if you want to use Weave with a C Standard I/O Library-based
   logging system with timestamped log entries */
#define WEAVE_LOGGING_STYLE_STDIO_WITH_TIMESTAMPS 0

/* Define to 1 if you want to use LwIP with Weave System Layer. */
#define WEAVE_SYSTEM_CONFIG_USE_LWIP 1

/* Define to 1 if you want to use BSD sockets with Weave System Layer. */
#define WEAVE_SYSTEM_CONFIG_USE_SOCKETS 0

/* Weave target style */
#define WEAVE_TARGET_STYLE embedded

/* Define to 1 if you want to use Weave with an embedded (i.e. non-Unix-based)
   system */
#define WEAVE_TARGET_STYLE_EMBEDDED 1

/* Define to 1 if you want to use Weave with a Unix-based system */
#define WEAVE_TARGET_STYLE_UNIX 0

/* Define to 1 to build Weave with nlFaultInjection features */
#define WEAVE_WITH_NLFAULTINJECTION 0

/* Define to 1 to build Weave with OpenSSL features */
#define WEAVE_WITH_OPENSSL 0

/* Define for Solaris 2.5.1 so the uint32_t typedef from <sys/synch.h>,
   <pthread.h>, or <semaphore.h> is not used. If the typedef were allowed, the
   #define below would cause a syntax error. */
/* #undef _UINT32_T */

/* Define for Solaris 2.5.1 so the uint64_t typedef from <sys/synch.h>,
   <pthread.h>, or <semaphore.h> is not used. If the typedef were allowed, the
   #define below would cause a syntax error. */
/* #undef _UINT64_T */

/* Define for Solaris 2.5.1 so the uint8_t typedef from <sys/synch.h>,
   <pthread.h>, or <semaphore.h> is not used. If the typedef were allowed, the
   #define below would cause a syntax error. */
/* #undef _UINT8_T */

/* Define to the type of a signed integer type of width exactly 16 bits if
   such a type exists and the standard includes do not define it. */
/* #undef int16_t */

/* Define to the type of a signed integer type of width exactly 32 bits if
   such a type exists and the standard includes do not define it. */
/* #undef int32_t */

/* Define to the type of a signed integer type of width exactly 64 bits if
   such a type exists and the standard includes do not define it. */
/* #undef int64_t */

/* Define to the type of a signed integer type of width exactly 8 bits if such
   a type exists and the standard includes do not define it. */
/* #undef int8_t */

/* Define to the type of an unsigned integer type of width exactly 16 bits if
   such a type exists and the standard includes do not define it. */
/* #undef uint16_t */

/* Define to the type of an unsigned integer type of width exactly 32 bits if
   such a type exists and the standard includes do not define it. */
/* #undef uint32_t */

/* Define to the type of an unsigned integer type of width exactly 64 bits if
   such a type exists and the standard includes do not define it. */
/* #undef uint64_t */

/* Define to the type of an unsigned integer type of width exactly 8 bits if
   such a type exists and the standard includes do not define it. */
/* #undef uint8_t */
