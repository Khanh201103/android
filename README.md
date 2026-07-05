# Android Automotive for Raspberry Pi 4

This repository contains the complete Raspberry Pi 4 device tree used by this
Android 15 Automotive build, including its media, audio, camera, car, Bluetooth,
graphics, and SELinux configuration. It also adds the Navis Calculator Binder
service.

The upstream AOSP source tree and generated build output are intentionally
excluded.

## Sync the source tree

```sh
mkdir android
cd android
repo init -u https://android.googlesource.com/platform/manifest \
  -b android-15.0.0_r32

mkdir -p .repo/local_manifests
cp /path/to/this-repo/local_manifests/*.xml .repo/local_manifests/
repo sync -c
```

## Apply the project files

From this repository, copy the Raspberry Pi overlay into the synced AOSP tree:

```sh
cp -a device/brcm/rpi4/. /path/to/android/device/brcm/rpi4/
```

## Build

```sh
cd /path/to/android
source build/envsetup.sh
lunch aosp_rpi4_car-ap3a-userdebug
m navis_calculator_service
```

The product configuration includes the service in `system_ext` and enables ADB
over TCP on port 5555 for `userdebug` and `eng` builds.

After installing the image, the Binder service can be checked with:

```sh
adb shell dumpsys navis.calculator.ICalculator/default
adb shell dumpsys navis.calculator.ICalculator/default add 2 3
```
