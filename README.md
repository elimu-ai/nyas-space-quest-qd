# Nya's Space Quest: Quantity Discrimination 🕹

This game is a fork of [Nya's Space Quest](https://github.com/elimu-ai/nyas-space-quest).

---

Corresponding [EGMA skill](https://github.com/elimu-ai/model/blob/master/src/main/java/ai/elimu/model/v2/enums/content/NumeracySkill.java): `QUANTITY_DISCRIMINATION`
> Measures children's ability to make judgments about differences by comparing quantities in object groups. This can be done by using numbers or by using objects such as circles.
    
> ![](https://raw.githubusercontent.com/elimu-ai/webapp/master/src/main/webapp/static/img/admin/EGMA_QUANTITY_DISCRIMINATION.png)

## Screenshots 📸

<img width="480" src="https://user-images.githubusercontent.com/15718174/27411324-edec40ac-56e4-11e7-8207-1a05c770024d.png" />

<img width="480" src="https://user-images.githubusercontent.com/15718174/26973174-6901eabe-4d15-11e7-9634-b37cd6006b4a.png" />

## Demo Video 🎥

<kbd>[![](https://i.ytimg.com/vi/gGK4imJdJZk/hqdefault.jpg)](https://youtu.be/gGK4imJdJZk)</kbd>

## Installation

> [!IMPORTANT]
> Note: This app depends on the [elimu.ai Content Provider](https://github.com/elimu-ai/content-provider) to be installed.

## Build & Test 🛠

> Verified setup: Cocos2d-x 3.14.1 (C++) game, built via `proj.android-studio`
> (Gradle modules `:Nya-qd` = app, `:libcocos2dx` = engine) with JDK 17,
> Gradle 8.10 (wrapper), Android Gradle Plugin 8.3.0, `compileSdk 34`,
> NDK `28.2.13676358`, `minSdk 21`. First build takes ~9 min (compiles the
> Cocos native libs for 2 ABIs); incremental builds are fast.

### Prerequisites

- Android SDK with platform `android-34` and matching build-tools, plus NDK
  `28.2.13676358` (pinned via `ndkVersion` in both `build.gradle` files).
- JDK 17 and `proj.android-studio/local.properties` pointing at your SDK, e.g.
  `sdk.dir=/Users/<you>/Library/Android/sdk`.

### Build

```sh
cd proj.android-studio
./gradlew :Nya-qd:assembleDebug
```

APK output: `proj.android-studio/app/build/outputs/apk/debug/Nya-qd-debug.apk`
(~29 MB, debug-signed automatically).

Release signing uses the existing mechanism: pass
`-PRELEASE_STORE_FILE=... -PRELEASE_STORE_PASSWORD=... -PRELEASE_KEY_ALIAS=... -PRELEASE_KEY_PASSWORD=...`
and build `:Nya-qd:assembleRelease`.

### Install & smoke-test on an emulator

```sh
# Boot an AVD (see GPU note below), then:
adb -s <device> install -r proj.android-studio/app/build/outputs/apk/debug/Nya-qd-debug.apk
adb -s <device> shell am start -n org.literacyapp.nya_qd/org.cocos2dx.cpp.AppActivity
adb -s <device> shell pidof org.literacyapp.nya_qd # process alive = no crash
adb -s <device> exec-out screencap -p > nya-qd.png
adb -s <device> logcat -d | grep -E "FATAL EXCEPTION" # expect no output
```

> Emulator GPU note (macOS, from the sister `nyas-space-quest` repo): with host-GPU
> (`gfxstream`) graphics the game process can run but every GL draw fails with
> `0x0502` and the screen stays black. If so, retest with software GLES:
> `emulator -avd <name> -gpu swiftshader_indirect`.

### What was changed to modernize the build (vs 2017 config)

- `proj.android-studio/build.gradle`: AGP `1.3.0` → `8.3.0`,
  `jcenter()` → `google()` + `mavenCentral()`.
- `proj.android-studio/app/build.gradle`: `namespace`/`applicationId`
  `nya-qd.literacyapp.org` → `org.literacyapp.nya_qd` (the old ID contains `-`,
  illegal in an Android package name), `compileSdk 34`,
  `ndkVersion 28.2.13676358`, `externalNativeBuild.ndkBuild` wired to
  `jni/Android.mk`, `compile` → `implementation`; ABI filters
  `armeabi-v7a` + `arm64-v8a`.
- `proj.android-studio/app/AndroidManifest.xml`: removed legacy `package=`
  attribute (namespace comes from Gradle), added `android:exported="true"`
  (required when `targetSdk` ≥ 31).
- `cocos2d/.../libcocos2dx/build.gradle`: same SDK bump, `namespace`,
  `buildFeatures { aidl true }` (restores `IGameTuningService` generation).
- `proj.android-studio/app/jni/Application.mk`: `gnustl_static` → `c++_static`
  (gnustl was removed from modern NDKs), `APP_ABI := armeabi-v7a arm64-v8a`,
  `APP_PLATFORM := android-21` (`minSdk 21` preserved).
- `proj.android-studio/app/jni/Android.mk`: `LOCAL_SRC_FILES` lists the real
  QD `Classes/*.cpp` (the template's `HelloWorldScene.cpp` no longer exists);
  module renamed to `MyGame` (the target Gradle's ndk-build integration
  requests).
- `cocos2d/external/unzip/ioapi.h`: skip the unconditional
  `#define __USE_FILE_OFFSET64` on Android — it hides `fgetpos`/`fsetpos`
  declarations in NDK 25+ headers when targeting API < 24, which breaks
  `<cstdio>` (libc++). Game assets are far below the 4 GB this flag serves.
- ABIs: `armeabi`/`x86`/`x86_64` dropped (Cocos 3.14.1 ships no `x86_64`
  prebuilts, so it can never configure; `armeabi` was removed from modern
  NDKs). Re-add in `abiFilters` + `APP_ABI` if needed.
- NDK `25.2.9519653` → `28.2.13676358`: NDK 28 links the `arm64-v8a`
  `libMyGame.so` with 16 KB-aligned `LOAD` segments (`0x4000` instead of
  `0x1000`), which silences the "This app isn't 16 KB compatible" system
  dialog on Android 15+ devices with 16 KB page size.

---

<p align="center">
  <img src="https://github.com/elimu-ai/webapp/blob/main/src/main/webapp/static/img/logo-text-256x78.png" />
</p>
<p align="center">
  elimu.ai - Free open-source learning software for out-of-school children ✨🚀
</p>
<p align="center">
  <a href="https://elimu.ai">Website 🌐</a>
  &nbsp;•&nbsp;
  <a href="https://github.com/elimu-ai/wiki#readme">Wiki 📃</a>
  &nbsp;•&nbsp;
  <a href="https://github.com/orgs/elimu-ai/projects?query=is%3Aopen">Projects 👩🏽‍💻</a>
  &nbsp;•&nbsp;
  <a href="https://github.com/elimu-ai/wiki/milestones">Milestones 🎯</a>
  &nbsp;•&nbsp;
  <a href="https://github.com/elimu-ai/wiki#open-source-community">Community 👋🏽</a>
  &nbsp;•&nbsp;
  <a href="https://www.drips.network/app/drip-lists/41305178594442616889778610143373288091511468151140966646158126636698">Support 💜</a>
</p>
