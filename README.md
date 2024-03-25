# AudioFX - Compressor

[![cpp-linter](https://github.com/cpp-linter/cpp-linter-action/actions/workflows/cpp-linter.yml/badge.svg)](https://github.com/AudioFX-Compressor/.github/workflows/cpp-linter.yml)

This project implements an audio compressor plugin using JUCE and C++. The compressor applies dynamic range compression to audio signals.
A MATLAB/Python design script is provided in the Demonstration directory of the project, outlining the DSP used in the plugin.

## Features

### Completed

- Threshold, Ratio, Attack, Release, and Makeup Gain controls for fine-tuning compression parameters.
- Adjustable knee setting for smooth transition between compressed and uncompressed audio.

### To do

- Visual feedback of gain reduction.
- Side-chain support for more advanced compression techniques.
- Implementation of other compression types i.e. downwards / upwards, expander, limiters, gates, multiband.

## Building the project

When building the project, make sure to download the whole repo and open the project through the ProJucer application.
