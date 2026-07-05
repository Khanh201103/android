#
# Copyright (C) 2021-2023 KonstaKANG
#
# SPDX-License-Identifier: Apache-2.0
#

PRODUCT_MAKEFILES := \
    $(LOCAL_DIR)/aosp_rpi4.mk \
    $(LOCAL_DIR)/aosp_rpi4_car.mk \
    $(LOCAL_DIR)/aosp_rpi4_tv.mk

COMMON_LUNCH_CHOICES := \
    aosp_rpi4-trunk_staging-userdebug \
    aosp_rpi4_car-trunk_staging-userdebug \
    aosp_rpi4_tv-trunk_staging-userdebug
