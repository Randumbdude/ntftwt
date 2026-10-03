# ntftwt

`ntftwt` is a Windows-focused C++ project for experimenting with low-level systems programming, inter-process communication, CPU SIMD instructions, embedded Julia, and game/AI interfaces.

The project is primarily developed as a collection of interconnected experiments rather than a single-purpose application. It currently contains the main `ntftwt` executable, a debugging/launcher utility, and a Klondike interface intended to provide a foundation for communicating game state and actions with an AI.

> **Status:** Experimental / active development  
> **Platform:** Windows  
> **Language:** C++20  
> **Build system:** Visual Studio / MSBuild

---

## Features

### Windows IPC

`ntftwt` contains a Windows named-pipe IPC system that allows separate instances of the program to communicate.

The current IPC endpoint is:

```text
\\.\pipe\ntftwt_IPC
```

The IPC protocol uses a small command structure containing a command identifier, command arguments, and the sender's process ID.

```cpp
struct ipc_command {
    uint32_t cmd;
    char cmd_args[64];
    DWORD pid;
};
```

The current command types include:

```cpp
enum class command_e : uint32_t {
    ping = 1,
    exe_cmd = 2,
    terminate = 3,
    klondike_game_data = 4
};
```

A process can be **parentized**, causing it to become the primary instance and start the named-pipe server. Other instances can then connect to that process and send commands.

The IPC implementation is built directly on the Windows API using functions such as:

- `CreateNamedPipeA`
- `ConnectNamedPipe`
- `CreateFileA`
- `ReadFile`
- `WriteFile`
- `DisconnectNamedPipe`

This is intended to eventually provide a general-purpose communication mechanism between the main program, other `ntftwt` instances, games, and external AI processes.

---

## AVX2 Benchmarking

The project contains a small AVX2 benchmark for comparing ordinary scalar operations with SIMD operations.

The benchmark currently operates on eight floating-point values and compares:

```cpp
result[i] = avx_t_a[i] + avx_t_b[i];
```

against the equivalent AVX2 operation:

```cpp
__m256 va = _mm256_load_ps(avx_t_a);
__m256 vb = _mm256_load_ps(avx_t_b);

__m256 vc = _mm256_add_ps(va, vb);

_mm256_store_ps(result, vc);
```

The benchmark reports:

- Scalar execution time
- AVX2 execution time
- Calculated speedup

Example output:

```text
AVX2 Benchmark results for 100000000 iterations:
Scalar: ...
AVX2:   ...
Speedup: ...x
```

Input data can also be loaded from a file using the `-i` command-line option. The current input format consists of two lines containing eight values each.

Example:

```text
[1,2,3,4,5,6,7,8]
[10,20,30,40,50,60,70,80]
```

---

## Embedded Julia

`ntftwt` can embed the Julia runtime directly into the C++ application.

The project currently includes:

```text
ntftwt/
├── benchmark.jl
└── julia_t.cpp
```

The C++ side initializes Julia and loads the benchmark script:

```cpp
jl_init();

jl_eval_string("include(\"benchmark.jl\")");
```

It then locates and calls the Julia function:

```julia
ntftwt_main()
```

The included Julia benchmark performs a numerical workload over 10,000,000 iterations and reports its execution time and iteration rate.

This provides a simple testbed for calling Julia code from C++ and passing execution control between the two runtimes.

---

## Command-Line Interface

The main executable has a small command-line interface.

Current options include:

| Option | Description |
|---|---|
| `--help` | Display command usage |
| `--version` | Display the current version |
| `-i <file>` | Load benchmark input data from a file |
| `-p` | Parentize the current process |
| `--castlemania` | Execute the current `castlemania` command |

For usage information:

```powershell
ntftwt.exe --help
```

For version information:

```powershell
ntftwt.exe --version
```

The current version is defined in `globals.h`:

```cpp
constexpr const char* VERSION = "0.0.0.1a";
```

---

# Klondike Interface

The repository also contains a separate `klondike_interface` project.

Its purpose is to provide a C++ implementation of the game state and an IPC interface that can eventually be used by an external AI.

The project currently contains:

```text
klondike_interface/
├── ipc_client.cpp
├── ipc_client.h
├── klondike.cpp
├── klondike.h
├── klondike_interface.cpp
├── klondike_ipc.h
└── solitare.py
```

The C++ implementation models the major components of a Klondike game.

### Card

Cards contain:

- Value
- Suit
- Card name
- Card title
- Attachment rules

Supported suits are:

```cpp
enum class Suit : uint8_t {
    CLUB,
    DIAMOND,
    HEART,
    SPADE
};
```

### Deck

The deck implementation supports:

- Resetting
- Shuffling
- Drawing cards
- Dealing multiple cards
- Checking deck size

A standard 52-card deck is constructed from four suits and values 1 through 13.

### Foundation

The foundation implementation tracks four suit-specific piles and supports:

- Adding cards
- Checking the top card
- Checking pile sizes
- Determining whether the game has been won

### Stock / Waste

The stock/waste system supports:

- Moving cards from the stock to the waste
- Accessing the current waste card
- Removing a waste card
- Checking stock/waste sizes
- Accessing the underlying piles

### Tableau

The tableau contains seven columns with separate hidden and visible card collections.

Supported operations include:

- Flipping cards
- Adding cards to columns
- Moving cards between tableau columns
- Moving tableau cards to the foundation
- Moving waste cards to the tableau

### Complete Game

The `klondike_game` class combines all of these components:

```cpp
class klondike_game
{
public:
    klondike_game();

    void newGame();

    bool stockToWaste();

    bool wasteToFoundation();

    bool wasteToTableau(size_t column);

    bool tableauToFoundation(size_t column);

    bool tableauToTableau(
        size_t source,
        size_t destination
    );

    bool gameWon() const;

    const Tableau& tableau() const;
    const Foundation& foundation() const;
    const StockWaste& stockWaste() const;
};
```

The long-term purpose is to make the entire game state accessible to an AI process through IPC.

---

# Klondike IPC

The `klondike_interface` project contains an IPC client which communicates through the same Windows named-pipe infrastructure.

The current client connects to:

```text
\\.\pipe\ntftwt_IPC
```

and sends an `ipc_command` structure using `WriteFile`.

The current test program sends a simple PING message:

```text
Klondike IPC client starting...
Sending PING...
```

This is currently a starting point for a larger AI communication protocol.

The IPC command structure already contains a placeholder for game/AI data:

```cpp
struct ipc_command {
    uint32_t cmd;
    char cmd_args[64];
    DWORD pid;

    // ai data structs
};
```

The `klondike_game_data` command type is reserved for this future communication.

---

# Debugger / Launcher

The `dbg_ntftwt` project is a small utility for launching `ntftwt`.

It supports a simple configuration file containing values such as:

```text
program_path=.\ntftwt.exe
program_args=--help
```

The debugger can:

- Load a configuration file
- Create a missing configuration
- Configure the target executable
- Configure command-line arguments
- Launch the target program
- Display its exit code

Example:

```powershell
dbg_ntftwt.exe config.txt
```

A `--pause` argument is also supported for keeping the console open after execution.

---

# Project Structure

```text
ntftwt/
│
├── ntftwt.slnx
│
├── ntftwt/
│   ├── args.cpp
│   ├── args.h
│   ├── avx_t.cpp
│   ├── benchmark.jl
│   ├── benchmarks.h
│   ├── exit_codes.cpp
│   ├── globals.h
│   ├── ipc.h
│   ├── ipc_args.cpp
│   ├── ipc_cmd.cpp
│   ├── ipc_cmd.h
│   ├── julia_t.cpp
│   ├── ntftwt.cpp
│   ├── ntftwt.vcxproj
│   └── sha1.h
│
├── dbg_ntftwt/
│   ├── config.h
│   ├── debugger_main.cpp
│   └── dbg_ntftwt.vcxproj
│
├── klondike_interface/
│   ├── ipc_client.cpp
│   ├── ipc_client.h
│   ├── klondike.cpp
│   ├── klondike.h
│   ├── klondike_interface.cpp
│   ├── klondike_ipc.h
│   ├── solitare.py
│   └── klondike_interface.vcxproj
│
├── README.md
└── LICENSE.txt
```

The Visual Studio solution currently contains three projects:

1. `ntftwt`
2. `dbg_ntftwt`
3. `klondike_interface`

---

# Requirements

The current Visual Studio projects are configured for:

- Windows
- Visual Studio
- MSVC toolset `v145`
- C++20
- Windows SDK 10.0
- x64 or Win32 builds

The main `ntftwt` project also requires a Julia installation because it links against the Julia runtime and includes:

```cpp
#include <julia.h>
```

The current project configuration references Julia 1.13.0.

---

# Building

Open:

```text
ntftwt.slnx
```

in Visual Studio.

Select one of the available configurations, such as:

```text
Debug | x64
```

or:

```text
Release | x64
```

Then build the solution.

The solution contains separate projects for the main executable, debugger/launcher, and Klondike interface.

---

# Running

After building, the primary executable can be run normally:

```powershell
ntftwt.exe
```

With command-line arguments:

```powershell
ntftwt.exe --help
```

```powershell
ntftwt.exe --version
```

To load AVX2 benchmark input:

```powershell
ntftwt.exe -i input.txt
```

To start the process as the parent IPC instance:

```powershell
ntftwt.exe -p
```

Once parentized, the process creates the named-pipe server and waits for other instances to connect.

---

# Development Goals

The project is actively evolving. The general direction is to turn the individual experiments into a more cohesive systems-programming environment.

Current areas of development include:

- Improving the Windows IPC protocol
- Expanding IPC command support
- Passing structured data between processes
- Connecting Klondike game state to an AI
- Sending AI-generated moves back to the game
- Expanding SIMD/AVX2 experiments
- Improving the embedded Julia interface
- Expanding the debugger/launcher
- Improving command-line argument handling
- Building reusable low-level C++ components

The eventual IPC design is intended to allow a primary application to remain responsible for its normal operation while external processes communicate with it through structured commands and data.

---

# Design Philosophy

`ntftwt` is intentionally built close to the platform.

Rather than hiding Windows functionality behind large frameworks, the project experiments directly with APIs and language/runtime interfaces such as:

- Win32
- Named pipes
- Windows mutexes
- Process IDs
- MSVC
- C++20
- AVX2 intrinsics
- Julia's C API

The goal is to understand how these systems work and how they can be combined into larger applications.

---

# License

This project is licensed under the **GNU Affero General Public License v3.0**.

See [`LICENSE.txt`](LICENSE.txt) for the complete license text.

The repository currently contains the complete AGPLv3 license text.

---

# Disclaimer

This project is experimental software and is provided without warranty.

Interfaces, IPC structures, command-line options, benchmarks, and internal APIs may change as development continues.