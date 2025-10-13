# Rubidium Organ

Rubidium is a simple additive synthesizer plugin using VST3. Rubidium is still very early in development
and thus is missing many key features, namely envelopes, customizable intervals between oscillators,
and support for DAW defined presets. If you have any features that you would like to see in rubidium,
feel free to create an issue with the "enhancement" tag.

## Building from source

Currently, binaries are only provided for linux. If you wish to build rubidium from source, you must
first acquire the VST3 SDK from Steinberg (https://www.steinberg.net/developers/). Next, edit
CMakeLists.txt and specify the path to the VST3 SDK at `set(vst3sdk_SOURCE_DIR "/PATH/TO/VST3/SDK")`.
Then, run the following:

```shell
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ../
cmake --build .
```

(You may need to install cmake on your system)
