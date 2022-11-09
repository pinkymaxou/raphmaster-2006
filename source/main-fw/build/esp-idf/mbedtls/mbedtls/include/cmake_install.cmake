# Install script for directory: K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/raphmaster-2006")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "K:/sdk/.espressif/tools/xtensa-esp32-elf/esp-2021r2-patch3-8.4.0/xtensa-esp32-elf/bin/xtensa-esp32-elf-objdump.exe")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/mbedtls" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ FILES
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/aes.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/aesni.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/arc4.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/aria.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/asn1.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/asn1write.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/base64.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/bignum.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/blowfish.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/bn_mul.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/camellia.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/ccm.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/certs.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/chacha20.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/chachapoly.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/check_config.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/cipher.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/cipher_internal.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/cmac.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/compat-1.3.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/config.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/config_psa.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/constant_time.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/ctr_drbg.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/debug.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/des.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/dhm.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/ecdh.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/ecdsa.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/ecjpake.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/ecp.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/ecp_internal.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/entropy.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/entropy_poll.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/error.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/gcm.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/havege.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/hkdf.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/hmac_drbg.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/md.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/md2.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/md4.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/md5.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/md_internal.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/memory_buffer_alloc.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/net.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/net_sockets.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/nist_kw.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/oid.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/padlock.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/pem.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/pk.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/pk_internal.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/pkcs11.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/pkcs12.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/pkcs5.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/platform.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/platform_time.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/platform_util.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/poly1305.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/psa_util.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/ripemd160.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/rsa.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/rsa_internal.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/sha1.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/sha256.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/sha512.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/ssl.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/ssl_cache.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/ssl_ciphersuites.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/ssl_cookie.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/ssl_internal.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/ssl_ticket.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/threading.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/timing.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/version.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/x509.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/x509_crl.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/x509_crt.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/x509_csr.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/mbedtls/xtea.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/psa" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ FILES
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/psa/crypto.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/psa/crypto_builtin_composites.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/psa/crypto_builtin_primitives.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/psa/crypto_compat.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/psa/crypto_config.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/psa/crypto_driver_common.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/psa/crypto_driver_contexts_composites.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/psa/crypto_driver_contexts_primitives.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/psa/crypto_extra.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/psa/crypto_platform.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/psa/crypto_se_driver.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/psa/crypto_sizes.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/psa/crypto_struct.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/psa/crypto_types.h"
    "K:/sdk/esp-idf-v4.4.1/components/mbedtls/mbedtls/include/psa/crypto_values.h"
    )
endif()

