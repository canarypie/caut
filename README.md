# caut - cellular automaton in the terminal

caut is a cellular simulation program written in C++.
It was created with the intention to be a performant, feature-rich and user-friendly cellular automata engine that runs in your terminal.

## Features

- **Lightweight, CLI-only engine**: easy to build and run on your machine without installing massive graphics libraries
- **Heavily customizable**: from grid dimensions and simulation rounds to rulesets and neighbor type
- **Loading your own patterns** from .txt and .rle files. Just explore patterns from [LifeWiki](https://conwaylife.com/wiki/) or write your own!
- Support for **exporting a simulation** as .ppm images
- **Benchmark** mode to analyze performance throughout the simulation

## Getting started

### Prerequisites
Before building, ensure you have the following installed:
- A C++17 (or newer) compatible compiler (GCC / Clang)
- CMake 3.15+

### Building

```bash
git clone https://github.com/canarypie/caut.git
cd caut
cmake -S . -B build
cmake --build build
```

### Launching

After building or downloading the binary, caut can be launched from the /bin path.
Loading the glider pattern in the default simulation (conway) for example:
```bash
./bin/caut -p "glider"
```

Running caut as a simulation of the "seeds" ruleset inside a 50x50 grid for example:
```bash
./bin/caut -r 50 -c 50 -R "seeds"
```

This assumes you are running a POSIX compliant OS such as Linux/macOS/BSD. If you are running Windows, you will need to execute the program under WSL for now. An update to support Windows entirely is planned for future releases.

## Gallery

<table>
  <tr>
    <td> <img src="media/diamond.gif" width="250" alt="diamond"/> </td>
    <td> <img src="media/rectangular.gif" width="250" alt="rectangular"/> </td>
    <td> <img src="media/diamoeba.gif" width="250" alt="diamoeba"/> </td>
  </tr>
</table>

![1](media/diamond.gif)
![2](media/rectangular.gif)
![3](media/diamoeba.gif)

## PPM export

When passing -E as a command line argument, caut will export every rendered frame as an individual .ppm image stored under bin/output. The frames can be converted to a video stream with ffmpeg:
```bash
ffmpeg -framerate 5 -i ./bin/output/frame%03d.ppm -pix_fmt yuv420p output.mp4
```
## Benchmarking

To measure the computational performance of the engine with rendering disabled, you can pass -B as an argument. It will record the time required to compute the simulation per generation and store the results in a .csv file under bin/output/benchmark.csv.
The provided benchmark.py script can be executed with this command:

```bash
python benchmark.py
```

If you do not have matplotlib installed, you can install it from pip:
```bash
pip install matplotlib
```

the final plot will be stored under bin/output/benchmark.png by default.
