#
#  Copyright (c) 2018, The OpenThread Authors.
#  All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions are met:
#  1. Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.
#  2. Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in the
#     documentation and/or other materials provided with the distribution.
#  3. Neither the name of the copyright holder nor the
#     names of its contributors may be used to endorse or promote products
#     derived from this software without specific prior written permission.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
#  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
#  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
#  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
#  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
#  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
#  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
#  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
#  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
#  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
#  POSSIBILITY OF SUCH DAMAGE.
#

PROJECT_NAME     := OpenThread-FreeRTOS
TARGETS          := ot_cli_freertos
OUTPUT_DIRECTORY := _build

AR               ?= arm-none-eabi-ar
CCAS             ?= $(CCPREFIX) arm-none-eabi-gcc
CPP              ?= arm-none-eabi-cpp
CC               ?= $(CCPREFIX) arm-none-eabi-gcc
CXX              ?= $(CCPREFIX) arm-none-eabi-g++
LD               ?= arm-none-eabi-ld
STRIP            ?= arm-none-eabi-strip
NM               ?= arm-none-eabi-nm
RANLIB           ?= arm-none-eabi-ranlib
OBJCOPY          ?= arm-none-eabi-objcopy

TOP_SRCDIR := $(dir $(realpath $(firstword $(MAKEFILE_LIST))))

# nRF52_SDK required
GNU_INSTALL_ROOT =
SDK_ROOT := $(TOP_SRCDIR)/third_party/nRF5_SDK/repo
PROJ_DIR := .
SRC_DIR := ./src/

$(OUTPUT_DIRECTORY)/ot_cli_freertos.out: \
    LINKER_SCRIPT  := $(SRC_DIR)/platforms/nrf52840/gcc_nrf52.ld

MBEDTLS_DIR := $(PROJ_DIR)/third_party/openthread/third_party/mbedtls

OT_DRIVER_DIR := $(PROJ_DIR)/third_party/openthread/third_party/NordicSemiconductor/

OPENTHREAD_SRC_FILES_ = \
    $(wildcard $(PROJ_DIR)/third_party/openthread/src/cli/*.cpp) \
    $(wildcard $(PROJ_DIR)/third_party/openthread/src/cli/*.c) \
    $(wildcard $(PROJ_DIR)/third_party/openthread/src/diag/*.c) \
    $(wildcard $(PROJ_DIR)/third_party/openthread/src/diag/*.cpp) \
    $(wildcard $(PROJ_DIR)/third_party/openthread/src/core/*.cpp) \
    $(wildcard $(PROJ_DIR)/third_party/openthread/src/core/*.c) \
    $(wildcard $(PROJ_DIR)/third_party/openthread/src/core/api/*.cpp) \
    $(wildcard $(PROJ_DIR)/third_party/openthread/src/core/api/*.c) \
    $(wildcard $(PROJ_DIR)/third_party/openthread/src/core/coap/*.cpp) \
    $(wildcard $(PROJ_DIR)/third_party/openthread/src/core/coap/*.c) \
    $(wildcard $(PROJ_DIR)/third_party/openthread/src/core/common/*.cpp) \
    $(wildcard $(PROJ_DIR)/third_party/openthread/src/core/common/*.c) \
    $(wildcard $(PROJ_DIR)/third_party/openthread/src/core/crypto/*.cpp) \
    $(wildcard $(PROJ_DIR)/third_party/openthread/src/core/crypto/*.c) \
    $(wildcard $(PROJ_DIR)/third_party/openthread/src/core/mac/*.cpp) \
    $(wildcard $(PROJ_DIR)/third_party/openthread/src/core/mac/*.c) \
    $(wildcard $(PROJ_DIR)/third_party/openthread/src/core/meshcop/*.cpp) \
    $(wildcard $(PROJ_DIR)/third_party/openthread/src/core/meshcop/*.c) \
    $(wildcard $(PROJ_DIR)/third_party/openthread/src/core/net/*.cpp) \
    $(wildcard $(PROJ_DIR)/third_party/openthread/src/core/net/*.c) \
    $(wildcard $(PROJ_DIR)/third_party/openthread/src/core/thread/*.cpp) \
    $(wildcard $(PROJ_DIR)/third_party/openthread/src/core/thread/*.c) \
    $(wildcard $(PROJ_DIR)/third_party/openthread/src/core/utils/*.cpp) \
    $(wildcard $(PROJ_DIR)/third_party/openthread/src/core/utils/*.c) \
    $(wildcard $(PROJ_DIR)/third_party/openthread/src/ncp/*.cpp) \
    $(wildcard $(PROJ_DIR)/third_party/openthread/src/ncp/*.c) \
    $(wildcard $(PROJ_DIR)/third_party/openthread/examples/platforms/utils/*.cpp) \
    $(wildcard $(PROJ_DIR)/third_party/openthread/examples/platforms/utils/*.c) \

OPENTHREAD_SRC_FILES = $(filter-out $(PROJ_DIR)/third_party/openthread/src/core/common/extension_example.cpp, $(OPENTHREAD_SRC_FILES_))

OPENTHREAD_INC_FOLDERS = \
    $(PROJ_DIR)/third_party/openthread/examples/platforms \
    $(PROJ_DIR)/third_party/openthread/include \
    $(PROJ_DIR)/third_party/openthread/src/core \
    $(PROJ_DIR)/third_party/openthread/src/ncp \
    $(PROJ_DIR)/third_party/openthread/src/cli \
    $(PROJ_DIR)/third_party/openthread/src/ \

MBEDTLS_SRC_FILES = \
    $(MBEDTLS_DIR)/repo/library/aes.c                          \
    $(MBEDTLS_DIR)/repo/library/asn1parse.c                    \
    $(MBEDTLS_DIR)/repo/library/asn1write.c                    \
    $(MBEDTLS_DIR)/repo/library/base64.c                       \
    $(MBEDTLS_DIR)/repo/library/bignum.c                       \
    $(MBEDTLS_DIR)/repo/library/ccm.c                          \
    $(MBEDTLS_DIR)/repo/library/cipher.c                       \
    $(MBEDTLS_DIR)/repo/library/cipher_wrap.c                  \
    $(MBEDTLS_DIR)/repo/library/cmac.c                         \
    $(MBEDTLS_DIR)/repo/library/ctr_drbg.c                     \
    $(MBEDTLS_DIR)/repo/library/debug.c                        \
    $(MBEDTLS_DIR)/repo/library/ecdh.c                         \
    $(MBEDTLS_DIR)/repo/library/ecdsa.c                        \
    $(MBEDTLS_DIR)/repo/library/ecjpake.c                      \
    $(MBEDTLS_DIR)/repo/library/ecp_curves.c                   \
    $(MBEDTLS_DIR)/repo/library/ecp.c                          \
    $(MBEDTLS_DIR)/repo/library/entropy.c                      \
    $(MBEDTLS_DIR)/repo/library/entropy_poll.c                 \
    $(MBEDTLS_DIR)/repo/library/gcm.c                          \
    $(MBEDTLS_DIR)/repo/library/md.c                           \
    $(MBEDTLS_DIR)/repo/library/md_wrap.c                      \
    $(MBEDTLS_DIR)/repo/library/memory_buffer_alloc.c          \
    $(MBEDTLS_DIR)/repo/library/oid.c                          \
    $(MBEDTLS_DIR)/repo/library/pem.c                          \
    $(MBEDTLS_DIR)/repo/library/pk.c                           \
    $(MBEDTLS_DIR)/repo/library/pk_wrap.c                      \
    $(MBEDTLS_DIR)/repo/library/pkparse.c                      \
    $(MBEDTLS_DIR)/repo/library/platform.c                     \
    $(MBEDTLS_DIR)/repo/library/platform_util.c                \
    $(MBEDTLS_DIR)/repo/library/sha1.c                         \
    $(MBEDTLS_DIR)/repo/library/sha256.c                       \
    $(MBEDTLS_DIR)/repo/library/ssl_cookie.c                   \
    $(MBEDTLS_DIR)/repo/library/ssl_ciphersuites.c             \
    $(MBEDTLS_DIR)/repo/library/ssl_cli.c                      \
    $(MBEDTLS_DIR)/repo/library/ssl_srv.c                      \
    $(MBEDTLS_DIR)/repo/library/ssl_ticket.c                   \
    $(MBEDTLS_DIR)/repo/library/ssl_tls.c                      \
    $(MBEDTLS_DIR)/repo/library/x509.c                         \
    $(MBEDTLS_DIR)/repo/library/x509_crt.c                     \
    $(MBEDTLS_DIR)/repo/library/rsa.c                          \
    $(MBEDTLS_DIR)/repo/library/rsa_internal.c                 \
    $(NULL)

MBEDTLS_INC_FOLDERS = \
    $(MBEDTLS_DIR)/repo/include            \
    $(MBEDTLS_DIR)/repo/include/mbedtls    \

PLATFORM_SRC_FILES = \
    $(SRC_DIR)/platforms/nrf52840/alarm.c \
    $(SRC_DIR)/platforms/nrf52840/diag.c \
    $(SRC_DIR)/platforms/nrf52840/fem.c \
    $(SRC_DIR)/platforms/nrf52840/flash.c \
    $(SRC_DIR)/platforms/nrf52840/logging.c \
    $(SRC_DIR)/platforms/nrf52840/misc.c \
    $(SRC_DIR)/platforms/nrf52840/main.c \
    $(SRC_DIR)/platforms/nrf52840/netif.cpp \
    $(SRC_DIR)/platforms/nrf52840/radio.c \
    $(SRC_DIR)/platforms/nrf52840/random.c \
    $(SRC_DIR)/platforms/nrf52840/temp.c \
    $(SRC_DIR)/platforms/nrf52840/system.c \
    $(SRC_DIR)/platforms/nrf52840/uart.c \
    $(SRC_DIR)/platforms/nrf52840/spi-slave.c \
    $(SRC_DIR)/platforms/nrf52840/flash_nosd.c \
    $(OT_DRIVER_DIR)/drivers/radio/nrf_802154_notification_direct.c \
    $(OT_DRIVER_DIR)/drivers/radio/nrf_802154_priority_drop_direct.c \
    $(OT_DRIVER_DIR)/drivers/radio/nrf_802154_request_direct.c \
    $(OT_DRIVER_DIR)/drivers/radio/nrf_802154.c \
    $(OT_DRIVER_DIR)/drivers/radio/nrf_802154_ack_pending_bit.c \
    $(OT_DRIVER_DIR)/drivers/radio/nrf_802154_core.c \
    $(OT_DRIVER_DIR)/drivers/radio/nrf_802154_core_hooks.c \
    $(OT_DRIVER_DIR)/drivers/radio/nrf_802154_critical_section.c \
    $(OT_DRIVER_DIR)/drivers/radio/nrf_802154_debug.c \
    $(OT_DRIVER_DIR)/drivers/radio/nrf_802154_pib.c \
    $(OT_DRIVER_DIR)/drivers/radio/nrf_802154_revision.c                       \
    $(OT_DRIVER_DIR)/drivers/radio/nrf_802154_rsch.c                           \
    $(OT_DRIVER_DIR)/drivers/radio/nrf_802154_rssi.c                           \
    $(OT_DRIVER_DIR)/drivers/radio/nrf_802154_rx_buffer.c                      \
    $(OT_DRIVER_DIR)/drivers/radio/nrf_802154_timer_coord.c                    \
    $(OT_DRIVER_DIR)/drivers/radio/fem/nrf_fem_control.c                       \
    $(OT_DRIVER_DIR)/drivers/radio/mac_features/nrf_802154_ack_timeout.c       \
    $(OT_DRIVER_DIR)/drivers/radio/mac_features/nrf_802154_csma_ca.c           \
    $(OT_DRIVER_DIR)/drivers/radio/mac_features/nrf_802154_delayed_trx.c       \
    $(OT_DRIVER_DIR)/drivers/radio/mac_features/nrf_802154_filter.c            \
    $(OT_DRIVER_DIR)/drivers/radio/platform/clock/nrf_802154_clock_sdk.c       \
    $(OT_DRIVER_DIR)/drivers/radio/platform/hp_timer/nrf_802154_hp_timer.c     \
    $(OT_DRIVER_DIR)/drivers/radio/timer_scheduler/nrf_802154_timer_sched.c    \
    $(OT_DRIVER_DIR)/drivers/radio/raal/single_phy/single_phy.c \
    $(OT_DRIVER_DIR)/libraries/crypto/aes_alt.c                                \
    $(OT_DRIVER_DIR)/libraries/crypto/aes_alt_cc310.c                          \
    $(OT_DRIVER_DIR)/libraries/crypto/aes_alt_soft.c                           \
    $(OT_DRIVER_DIR)/libraries/crypto/cc310_mbedtls.c                          \
    $(OT_DRIVER_DIR)/libraries/crypto/ecp_alt_cc310.c                          \
    $(OT_DRIVER_DIR)/libraries/crypto/ecp_curves_alt.c                         \
    $(OT_DRIVER_DIR)/libraries/crypto/sha256_alt_cc310.c                       \

PLATFORM_INC_FOLDERS = \
    $(OT_DRIVER_DIR)/drivers/radio \
    $(OT_DRIVER_DIR)/drivers/radio/hal \
    $(OT_DRIVER_DIR)/drivers/radio/raal \
    $(OT_DRIVER_DIR)/libraries/crypto \
    $(PROJ_DIR)/include/platforms/nrf52840 \

LWIP_DIR := $(PROJ_DIR)/third_party/lwip

LWIP_SRC_FILES = \
    $(LWIP_DIR)/repo/src/core/altcp.c \
    $(LWIP_DIR)/repo/src/core/altcp_alloc.c \
    $(LWIP_DIR)/repo/src/core/altcp_tcp.c \
    $(LWIP_DIR)/repo/src/core/mem.c \
    $(LWIP_DIR)/repo/src/core/memp.c \
    $(LWIP_DIR)/repo/src/core/netif.c \
    $(LWIP_DIR)/repo/src/core/pbuf.c \
    $(LWIP_DIR)/repo/src/core/raw.c \
    $(LWIP_DIR)/repo/src/core/stats.c \
    $(LWIP_DIR)/repo/src/core/sys.c \
    $(LWIP_DIR)/repo/src/core/tcp.c \
    $(LWIP_DIR)/repo/src/core/tcp_in.c \
    $(LWIP_DIR)/repo/src/core/tcp_out.c \
    $(LWIP_DIR)/repo/src/core/udp.c \
    $(LWIP_DIR)/repo/src/core/inet_chksum.c \
    $(LWIP_DIR)/repo/src/core/init.c \
    $(LWIP_DIR)/repo/src/core/timeouts.c \
    $(LWIP_DIR)/repo/src/core/def.c \
    $(LWIP_DIR)/repo/src/core/ip.c \
    $(LWIP_DIR)/repo/src/core/dns.c \
    $(LWIP_DIR)/repo/src/netif/ethernet.c \
    $(LWIP_DIR)/repo/src/apps/altcp_tls/altcp_tls_mbedtls_mem.c \
    $(wildcard $(LWIP_DIR)/repo/src/core/ipv4/*.c) \
    $(wildcard $(LWIP_DIR)/repo/src/core/ipv6/*.c) \
    $(LWIP_DIR)/port/altcp_tls_mbedtls.c \
    $(LWIP_DIR)/port/sys_arch.c \
    $(wildcard $(LWIP_DIR)/repo/src/apps/mqtt/*.c) \
    $(wildcard $(LWIP_DIR)/repo/src/api/*.c) \

LWIP_INC_FOLDERS = \
    $(LWIP_DIR)/repo/src \
    $(LWIP_DIR)/repo/src/apps/altcp_tls \
    $(LWIP_DIR)/repo/src/include \
    $(LWIP_DIR)/port \
    $(NULL)

OPENTHREAD_FLAGS = \
    -D_GNU_SOURCE \
    -DOPENTHREAD_FTD=1 \
    -DSPINEL_PLATFORM_HEADER=\"spinel_platform.h\" \
    -Wno-error=non-virtual-dtor \
    -DMBEDTLS_CONFIG_FILE=\"mbedtls-config.h\" \
    -DMBEDTLS_USER_CONFIG_FILE=\"nrf52840-mbedtls-config.h\" \
    -DSPINEL_PLATFORM_HEADER=\"spinel_platform.h\" \
    -DNRF_802154_PROJECT_CONFIG=\"platform-config.h\" \
    -DCONFIG_GPIO_AS_PINRESET \
    -DENABLE_FEM=1 \
    -DOPENTHREAD_CONFIG_LOG_PLATFORM=1 \
    -DRAAL_SINGLE_PHY=1 \
    -DOPENTHREAD_PROJECT_CORE_CONFIG_FILE=\"openthread-core-nrf52840-config.h\" \

# Source files common to all targets
DRIVER_SRC_FILES = \
    $(SDK_ROOT)/modules/nrfx/mdk/gcc_startup_nrf52840.S \
    $(SDK_ROOT)/components/libraries/log/src/nrf_log_frontend.c \
    $(SDK_ROOT)/components/libraries/log/src/nrf_log_str_formatter.c \
    $(SDK_ROOT)/components/boards/boards.c \
    $(SDK_ROOT)/external/freertos/source/croutine.c \
    $(SDK_ROOT)/external/freertos/source/event_groups.c \
    $(SDK_ROOT)/external/freertos/source/portable/MemMang/heap_3.c \
    $(SDK_ROOT)/external/freertos/source/list.c \
    $(SDK_ROOT)/external/freertos/portable/GCC/nrf52/port.c \
    $(SDK_ROOT)/external/freertos/portable/CMSIS/nrf52/port_cmsis.c \
    $(SDK_ROOT)/external/freertos/portable/CMSIS/nrf52/port_cmsis_systick.c \
    $(SDK_ROOT)/external/freertos/source/queue.c \
    $(SDK_ROOT)/external/freertos/source/stream_buffer.c \
    $(SDK_ROOT)/external/freertos/source/tasks.c \
    $(SDK_ROOT)/external/freertos/source/timers.c \
    $(SDK_ROOT)/components/libraries/util/app_error.c \
    $(SDK_ROOT)/components/libraries/util/app_error_handler_gcc.c \
    $(SDK_ROOT)/components/libraries/util/app_error_weak.c \
    $(SDK_ROOT)/components/libraries/timer/app_timer_freertos.c \
    $(SDK_ROOT)/components/libraries/uart/app_uart.c \
    $(SDK_ROOT)/components/libraries/fifo/app_fifo.c \
    $(SDK_ROOT)/components/libraries/util/app_util_platform.c \
    $(SDK_ROOT)/components/libraries/util/nrf_assert.c \
    $(SDK_ROOT)/components/libraries/atomic/nrf_atomic.c \
    $(SDK_ROOT)/components/libraries/balloc/nrf_balloc.c \
    $(SDK_ROOT)/external/fprintf/nrf_fprintf.c \
    $(SDK_ROOT)/external/fprintf/nrf_fprintf_format.c \
    $(SDK_ROOT)/components/libraries/memobj/nrf_memobj.c \
    $(SDK_ROOT)/components/libraries/ringbuf/nrf_ringbuf.c \
    $(SDK_ROOT)/components/libraries/strerror/nrf_strerror.c \
    $(SDK_ROOT)/integration/nrfx/legacy/nrf_drv_uart.c \
    $(SDK_ROOT)/integration/nrfx/legacy/nrf_drv_clock.c \
    $(SDK_ROOT)/integration/nrfx/legacy/nrf_drv_power.c \
    $(SDK_ROOT)/modules/nrfx/drivers/src/nrfx_clock.c \
    $(SDK_ROOT)/modules/nrfx/drivers/src/nrfx_power_clock.c \
    $(SDK_ROOT)/modules/nrfx/drivers/src/prs/nrfx_prs.c \
    $(SDK_ROOT)/modules/nrfx/drivers/src/nrfx_uart.c \
    $(SDK_ROOT)/modules/nrfx/drivers/src/nrfx_uarte.c \
    $(SDK_ROOT)/modules/nrfx/mdk/system_nrf52840.c \
    $(SDK_ROOT)/modules/nrfx/hal/nrf_nvmc.c \
    $(SDK_ROOT)/external/segger_rtt/SEGGER_RTT.c \
    $(SDK_ROOT)/external/segger_rtt/SEGGER_RTT_Syscalls_GCC.c \
    $(SDK_ROOT)/external/segger_rtt/SEGGER_RTT_printf.c \

DRIVER_INC_FOLDERS += \
    $(PROJ_DIR)/common \
    $(SDK_ROOT)/components \
    $(SDK_ROOT)/modules/nrfx/mdk \
    $(SDK_ROOT)/external/freertos/source/include \
    $(SDK_ROOT)/external/freertos/portable/CMSIS/nrf52 \
    $(SDK_ROOT)/external/freertos/portable/GCC/nrf52 \
    $(SDK_ROOT)/components/libraries/fifo \
    $(SDK_ROOT)/components/libraries/strerror \
    $(SDK_ROOT)/components/toolchain/cmsis/include \
    $(SDK_ROOT)/components/libraries/util \
    $(SDK_ROOT)/components/libraries/balloc \
    $(SDK_ROOT)/components/libraries/ringbuf \
    $(SDK_ROOT)/modules/nrfx/hal \
    $(SDK_ROOT)/components/libraries/bsp \
    $(SDK_ROOT)/components/libraries/uart \
    $(SDK_ROOT)/components/libraries/log \
    $(SDK_ROOT)/modules/nrfx \
    $(SDK_ROOT)/components/libraries/experimental_section_vars \
    $(SDK_ROOT)/integration/nrfx/legacy \
    $(SDK_ROOT)/components/libraries/delay \
    $(SDK_ROOT)/integration/nrfx \
    $(SDK_ROOT)/components/drivers_nrf/nrf_soc_nosd \
    $(SDK_ROOT)/components/libraries/atomic \
    $(SDK_ROOT)/components/boards \
    $(SDK_ROOT)/components/libraries/memobj \
    $(SDK_ROOT)/modules/nrfx/drivers/include \
    $(SDK_ROOT)/external/fprintf \
    $(SDK_ROOT)/external/segger_rtt \
    $(SDK_ROOT)/components/libraries/log/src \
    $(SDK_ROOT)/external/nrf_cc310/include \

LIBJWT_ROOT=$(PROJ_DIR)/third_party/libjwt

LIBJWT_SRC_FILES = \
    $(LIBJWT_ROOT)/repo/libjwt/base64.c \
    $(LIBJWT_ROOT)/repo/libjwt/jwt-mbedtls.c \
    $(LIBJWT_ROOT)/repo/libjwt/jwt.c

LIBJWT_INC_FOLERS = \
    $(LIBJWT_ROOT) \
    $(LIBJWT_ROOT)/repo/include \
    $(NULL)

JANSSON_ROOT=$(PROJ_DIR)/third_party/jansson

JANSSON_SRC_FILES = $(wildcard $(JANSSON_ROOT)/repo/src/*.c)

JANSSON_INC_FOLDERS = \
    $(JANSSON_ROOT)/ \
    $(JANSSON_ROOT)/repo/src \
    $(NULL)

APP_SRC_FILES = \
    $(SRC_DIR)/common/default_uart.c \
    $(SRC_DIR)/common/entropy_utils.c \
    $(SRC_DIR)/common/uart_io.c \
    $(SRC_DIR)/apps/misc/nat64_utils.c \
    $(SRC_DIR)/apps/misc/time_ntp.cpp \
    $(SRC_DIR)/apps/google_cloud_iot/mqtt_client.cpp \

SRC_FILES += \
    $(SRC_DIR)/examples/ot_cli/main.c \
    $(OPENTHREAD_SRC_FILES) \
    $(DRIVER_SRC_FILES) \
    $(PLATFORM_SRC_FILES) \
    $(LWIP_SRC_FILES) \
    $(MBEDTLS_SRC_FILES) \
    $(LIBJWT_SRC_FILES) \
    $(JANSSON_SRC_FILES) \
    $(APP_SRC_FILES) \

# Include folders common to all targets
INC_FOLDERS += \
    $(PROJ_DIR)/include \
    $(PROJ_DIR)/include/cfgs \
    $(PLATFORM_INC_FOLDERS) \
    $(OPENTHREAD_INC_FOLDERS) \
    $(MBEDTLS_INC_FOLDERS) \
    $(LWIP_INC_FOLDERS) \
    $(DRIVER_INC_FOLDERS) \
    $(LIBJWT_INC_FOLERS) \
    $(JANSSON_INC_FOLDERS) \
    $(CLOUD_IOT_INC_FOLDERS) \

# Libraries common to all targets
LIB_FILES += \
    $(SDK_ROOT)/external/nrf_cc310/lib/libnrf_cc310_0.9.10.a \
#   $(SDK_ROOT)/external/openthread/lib/gcc/libmbedcrypto.a \
#   $(SDK_ROOT)/external/openthread/lib/gcc/libopenthread-nrf52840-sdk.a \

# Optimization flags
OPT = -O3 -g3
# Uncomment the line below to enable link time optimization
#OPT += -flto

# C flags common to all targets
CFLAGS += $(OPT)
CFLAGS += -DBOARD_PCA10056
CFLAGS += -DBSP_DEFINES_ONLY
CFLAGS += -DCONFIG_GPIO_AS_PINRESET
CFLAGS += -DFLOAT_ABI_HARD
CFLAGS += -DNRF52840_XXAA
CFLAGS += -mcpu=cortex-m4
CFLAGS += -mthumb -mabi=aapcs
CFLAGS += -Wall
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
# keep every function in a separate section, this allows linker to discard unused ones
CFLAGS += -ffunction-sections -fdata-sections -fno-strict-aliasing
CFLAGS += -fno-builtin -fshort-enums
CFLAGS += -DFREERTOS
CFLAGS += -DIPv4
CFLAGS += -DLWIP_DEBUG
CFLAGS += -DHAVE_CONFIG_H
CFLAGS += $(OPENTHREAD_FLAGS)

# C++ flags common to all targets
CXXFLAGS += $(OPT)
CXXFLAGS += -DBOARD_PCA10056
CXXFLAGS += -DBSP_DEFINES_ONLY
CXXFLAGS += -DCONFIG_GPIO_AS_PINRESET
CXXFLAGS += -DFLOAT_ABI_HARD
CXXFLAGS += -DNRF52840_XXAA
CXXFLAGS += -mcpu=cortex-m4
CXXFLAGS += -mthumb -mabi=aapcs
CXXFLAGS += -Wall
CXXFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
CXXFLAGS += -fno-builtin -fshort-enums
CXXFLAGS += -DFREERTOS
CFLAGS += -DIPv4
CFLAGS += -DLWIP_DEBUG
CFLAGS += -DHAVE_CONFIG_H
CXXFLAGS += $(OPENTHREAD_FLAGS)

# Assembler flags common to all targets
ASMFLAGS += -g3
ASMFLAGS += -mcpu=cortex-m4
ASMFLAGS += -mthumb -mabi=aapcs
ASMFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
ASMFLAGS += -DBOARD_PCA10056
ASMFLAGS += -DBSP_DEFINES_ONLY
ASMFLAGS += -DCONFIG_GPIO_AS_PINRESET
ASMFLAGS += -DFLOAT_ABI_HARD
ASMFLAGS += -DNRF52840_XXAA
ASMFLAGS += -DFREERTOS
ASMFLAGS += -DIPv4
ASMFLAGS += -DLWIP_DEBUG
ASMFLAGS += $(OPENTHREAD_FLAGS)

# Linker flags
LDFLAGS += $(OPT)
LDFLAGS += -mthumb -mabi=aapcs -L$(SDK_ROOT)/modules/nrfx/mdk -T$(LINKER_SCRIPT)
LDFLAGS += -mcpu=cortex-m4
LDFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
# let linker dump unused sections
LDFLAGS += -Wl,--gc-sections
## use newlib in nano version
#LDFLAGS += --specs=nano.specs

ot_cli_freertos: CFLAGS += -D__HEAP_SIZE=8192
ot_cli_freertos: CFLAGS += -D__STACK_SIZE=8192
ot_cli_freertos: CXXFLAGS += -D__HEAP_SIZE=8192
ot_cli_freertos: CXXFLAGS += -D__STACK_SIZE=8192
ot_cli_freertos: ASMFLAGS += -D__HEAP_SIZE=8192
ot_cli_freertos: ASMFLAGS += -D__STACK_SIZE=8192

# Add standard libraries at the very end of the linker input, after all objects
# that may need symbols provided by these libraries.
LIB_FILES += -lc -lnosys -lm -lstdc++


.PHONY: default help

# Default target - first one defined
default: ot_cli_freertos

# Print all targets that can be built
help:
	@echo following targets are available:
	@echo		ot_cli_freertos
	@echo		sdk_config - starting external tool for editing sdk_config.h
	@echo		flash      - flashing binary

TEMPLATE_PATH := $(SDK_ROOT)/components/toolchain/gcc


include $(TEMPLATE_PATH)/Makefile.common

$(foreach target, $(TARGETS), $(call define_target, $(target)))

.PHONY: flash erase

# Flash the program
flash: default
	@echo Flashing: $(OUTPUT_DIRECTORY)/ot_cli_freertos.hex
	nrfjprog -f nrf52 --program $(OUTPUT_DIRECTORY)/ot_cli_freertos.hex --chiperase
	nrfjprog -f nrf52 --reset

erase:
	nrfjprog -f nrf52 --eraseall

SDK_CONFIG_FILE := $(PROJ_DIR)/sdk_config.h
CMSIS_CONFIG_TOOL := $(SDK_ROOT)/external_tools/cmsisconfig/CMSIS_Configuration_Wizard.jar
sdk_config:
	java -jar $(CMSIS_CONFIG_TOOL) $(SDK_CONFIG_FILE)
