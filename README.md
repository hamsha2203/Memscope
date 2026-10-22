# MemScope — C Program Memory Visualizer

MemScope is an educational C memory-inspection project. A small C instrumentation library records explicitly registered objects and emits JSON. A local Flask dashboard displays each object's name, type, current value, byte size, address, and declared memory region.

## Features

- Inspect demo variables in **Stack**, **Global/Data**, and **Heap** regions.
- Display actual runtime addresses and `sizeof`-based object/allocation sizes.
- Search variable names, types, values, addresses, and regions.
- Filter the dashboard by region and copy an address.
- C source, instrumentation macros, build script, and automated tests included.

## Requirements

- GCC or a compatible C compiler
- Python 3.9+
- `make`
- Flask (installed from `requirements.txt`)

### Windows

Use WSL2 with Ubuntu, then install build tools:

```bash
sudo apt update
sudo apt install build-essential python3 python3-pip
```

Unzip this project inside your WSL filesystem for the simplest setup.

## Run

From this directory:

```bash
make
python3 -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
python app.py
```

Open `http://127.0.0.1:5000` in your browser. The dashboard runs the bundled demo binary when the page is loaded.

To run the C demonstration without the web UI:

```bash
./memscope_demo
```

It prints a JSON report to standard output.

## Run tests

```bash
make test
```

The tests compile the demo, validate its JSON output, check the region and field metadata, and verify the heap allocation record.

## Project structure

```text
memscope/
├── app.py                  # Local Flask application
├── memscope.h              # Instrumentation API and helper macros
├── memscope.c              # Registry and JSON serializer
├── sample.c                # Demonstration variables and heap allocation
├── templates/index.html    # Dashboard UI
├── static/style.css        # Dashboard styling
├── tests/test_memscope.py  # Automated tests
├── Makefile                # Build, run, and test targets
└── requirements.txt        # Python dependency
```

## How it works

1. The sample program creates global/static, local, and dynamically allocated objects.
2. Instrumentation calls register each object with its label, type, value, address, size, and region.
3. The C library serializes those records as JSON.
4. Flask launches the fixed demo executable, parses the JSON, and renders the dashboard.
5. Search and region filters operate in the browser.

## Instrumentation example

```c
int request_count = 7;
MS_INT(request_count, "Stack");
```

The macros shown are for the supported demo types. For new variable types, register them using `ms_register()` with a formatted value and the address and size of the object.

## Important accuracy notes

- The tool uses **explicit instrumentation** to label storage regions. It does not infer a variable's storage duration from an address alone.
- Stack/global/heap are common conceptual regions, but exact process memory layouts vary by operating system, compiler, optimization settings, and address-space randomization.
- `sizeof(pointer)` reports the pointer object's size, not the number of bytes allocated at its target. The demo registers the heap allocation as a separate record.
- The displayed addresses are specific to that run and can change on each execution.
- The dashboard executes only the bundled demo program. It deliberately does not accept uploaded C source or execute arbitrary user code through the web interface.
- This is an educational inspector, not a debugger or a complete process-memory scanner.

## Possible next improvements

- Add support for structs, arrays, pointers, and user-defined formatters.
- Add an address-sorted view and byte-level memory map.
- Add debug-symbol/DWARF integration for a more advanced debugger-oriented version.
- Add comparison across runs while clearly marking addresses as run-specific.

## Resume bullet (use only after you understand and validate the project)

**MemScope — C Program Memory Visualizer | C, Python, Flask**
- Built a C instrumentation library to capture variable metadata, runtime addresses, values, and `sizeof`-based memory usage across stack, global/data, and heap objects; visualized structured reports through a searchable Flask dashboard.
