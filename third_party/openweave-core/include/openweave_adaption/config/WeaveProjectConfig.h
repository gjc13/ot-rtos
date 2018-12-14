/*
 *
 *    Copyright (c) 2018 Nest Labs, Inc.
 *    All rights reserved.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

/**
 *    @file
 *      Weave project configuration for the ESP32 platform.
 *
 */
#ifndef WEAVEPROJECTCONFIG_H
#define WEAVEPROJECTCONFIG_H

#define WEAVE_CONFIG_SUPPORT_PASE_CONFIG1 0

#define WEAVE_CONFIG_MAX_INTERFACES 4
#define WEAVE_CONFIG_MAX_LOCAL_ADDR_UDP_ENDPOINTS 4
#define WEAVE_CONFIG_MAX_TUNNELS 0
#define WEAVE_CONFIG_ENABLE_TUNNELING 1

#define WEAVE_CONFIG_ERROR_TYPE int32_t
#define WEAVE_CONFIG_NO_ERROR 0
#define WEAVE_CONFIG_ERROR_MIN 4000000
#define WEAVE_CONFIG_ERROR_MAX 4000999

#define ASN1_CONFIG_ERROR_TYPE int
#define ASN1_CONFIG_NO_ERROR 0
#define ASN1_CONFIG_ERROR_MIN 5000000
#define ASN1_CONFIG_ERROR_MAX 5000999

#define WeaveDie() abort()

// ==================== Security Adaptations ====================

#define WEAVE_CONFIG_USE_OPENSSL_ECC 0
#define WEAVE_CONFIG_USE_MICRO_ECC 1
#define WEAVE_CONFIG_HASH_IMPLEMENTATION_OPENSSL 0
#define WEAVE_CONFIG_HASH_IMPLEMENTATION_MINCRYPT 1
#define WEAVE_CONFIG_RNG_IMPLEMENTATION_OPENSSL 0
#define WEAVE_CONFIG_RNG_IMPLEMENTATION_NESTDRBG 1
#define WEAVE_CONFIG_AES_IMPLEMENTATION_OPENSSL 0
#define WEAVE_CONFIG_AES_IMPLEMENTATION_AESNI 0
#define WEAVE_CONFIG_AES_IMPLEMENTATION_PLATFORM 1
#define WEAVE_CONFIG_ENABLE_PROVISIONING_BUNDLE_SUPPORT 0


#endif /* WEAVEPROJECTCONFIG_H */
