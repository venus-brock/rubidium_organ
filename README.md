![](logo.png)

Rubidium is a simple additive synthesizer plugin using VST3. Rubidium is still very early in development
and thus is missing many key features, namely customizable intervals between oscillators, support for DAW
defined presets, etc. If you have any features that you would like to see in rubidium, feel free to create
an issue with the "enhancement" tag.

## Building from source

Currently, binaries are only provided for linux. If you wish to build rubidium from source, run the following:

```shell
git clone https://github.com/venus-brock/rubidium_organ --recurse-submodules
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ../
cmake --build .
```

(You may need to install cmake on your system)

Note that cloning this repo with the `--recurse-submodules` option will also download the entire VST3 SDK, which
is required to build the plugin. If you already have the SDK on your system, you may wish not to download it
again. In this case, run `git clone` above without the `--recurse-submodules` option. Then, edit CMakeLists.txt
and specify the path to the VST3 SDK at `set(vst3sdk_SOURCE_DIR "/PATH/TO/VST3/SDK")`.
