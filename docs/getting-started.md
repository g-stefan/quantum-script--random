# Getting started

## 1. Build and install

The extension is built with [fabricare](https://github.com/g-stefan/fabricare),
the build tool used by all XYO C++ projects. `quantum-script` (and everything
below it: `xyo-system`, `xyo-encoding`, ...), `quantum-script--console` and
`xyo-cryptography` must be installed to the SDK first. From the repository
root:

```bash
fabricare make       # build into output/
fabricare test       # build and run test/test.01 (run make first)
fabricare install    # copy output/{bin,include,lib} to ~/.fabricare/<platform>
fabricare clean      # remove output/ and temp/
```

`fabricare.json` declares two projects:

| Project | Kind | Purpose |
|---------|------|---------|
| `quantum-script--random` | `dll-or-lib`: shared library in a dynamic build, static library in a static build | the extension |
| `test.01` | executable, category `test` | runs `test/test.01.js` with the extension registered as internal |

After `fabricare install`, `quantum-script--random.dll` (Windows) /
`libquantum-script--random.so` (Linux) sits in the SDK `bin` folder next to
`quantum-script.exe`, which is where `Script.requireExtension("Random")`
finds it.

## 2. Use it from a script

```javascript
Script.requireExtension("Console");
Script.requireExtension("Math");
Script.requireExtension("Random");

var rnd = new Random();
rnd.seed(12345);

Console.writeLn(rnd.next());         // 0.9296160866506398
Console.writeLn(rnd.toInteger());    // 3992670690 (same value, as an integer)
Console.writeLn(rnd.next());         // 0.8901547130662948

var dice = 1 + Math.floor(rnd.next() * 6);   // 1 .. 6
Console.writeLn(dice);
```

Run it with:

```bash
quantum-script hello-random.js
```

Remove the `seed` line to get a different sequence on every run (the
generator is then seeded with the current time in seconds).

`Script.requireExtension("Random")` looks for an external
`quantum-script--random` library first (the file as named, then every
include path folder: next to the interpreter, next to the script), then for
an internal extension registered by the host. Loading twice does nothing. A
missing extension throws `Unable to open "Random"`.

Until the extension is loaded `Random` does not exist: `typeof(Random)` is
`"undefined"` and `new Random()` throws.

`Random` does not need `Math`, but most recipes use `Math.floor` to turn
`next()` into an integer range; load `Math` too, or use the integer form
`rnd.toInteger() % n` (see [Script API](script-api.md#integers-in-a-range)).

## 3. Hosts that already have it

- `magnet` (through `quantum-script--magnet`) registers `Random` as an
  internal extension: its scripts call `Script.requireExtension("Random")`
  and need no DLL.
- The `Pixel32` and `SSHRemote` extensions require `Random` themselves, so
  loading either of them loads `Random`.
- `fabricare` does **not** preload `Random`; a build script that needs it
  calls `Script.requireExtension("Random")`, which loads the installed DLL.

## 4. Register it in a C++ host

A host that embeds Quantum Script makes `Random` available as an internal
extension by registering it in the init callback (this is what
`test/test.01.cpp` does):

```cpp
#include <XYO/QuantumScript.hpp>
#include <XYO/QuantumScript.Extension/Console.hpp>
#include <XYO/QuantumScript.Extension/Random.hpp>

using namespace XYO::QuantumScript;

void initExecutive(Executive *executive) {
	Extension::Console::registerInternalExtension(executive);
	Extension::Random::registerInternalExtension(executive);
};

int main(int cmdN, char *cmdS[]) {
	if (ExecutiveX::initExecutive(cmdN, cmdS, initExecutive)) {
		if (!ExecutiveX::executeString(
		        "Script.requireExtension(\"Console\");"
		        "Script.requireExtension(\"Random\");"
		        "var rnd = new Random();"
		        "rnd.seed(12345);"
		        "Console.writeLn(rnd.next());")) {
			printf("%s\n", (ExecutiveX::getError()).value());
			printf("%s", (ExecutiveX::getStackTrace()).value());
		};
		ExecutiveX::endProcessing();
	};
	return 0;
};
```

Registering only makes the extension *available*: scripts still call
`Script.requireExtension("Random")`. With the DLL build of the engine an
external `quantum-script--random.dll` found on the include path wins over
the internal one for `requireExtension`; use
`Script.requireInternalExtension("Random")` to force the internal one.

In the host's `fabricare.json`:

```json
{
	"name": "my-host",
	"make": "exe",
	"sourcePath": "XYO/MyHost",
	"dependency": [
		"quantum-script--random"
	]
}
```

`quantum-script--random` depends on `quantum-script`,
`quantum-script--console` and `xyo-cryptography`; fabricare resolves them
transitively.

## 5. Static builds

There is no separate `.static` project. On a static platform (for example
`win64-msvc-2026.static`) the `dll-or-lib` project `quantum-script--random`
is built as a static library, `XYO_PLATFORM_COMPILE_STATIC` empties the
export macro and the `quantumScriptExtension` DLL entry point is left out.
A static host must register the extension with `registerInternalExtension`
(section 4): external DLLs cannot be loaded into a host that does not use
the engine DLL.

## 6. Threads

Each thread that runs scripts has its own engine, so every thread loads the
extension itself with `Script.requireExtension("Random")` and creates its
own generators. A `Random` object has no lock: use it from the thread that
created it.

Threads that create a `Random` in the same second without seeding it get
the **same** sequence. Give every thread its own seed, for example
`rnd.seed(DateTime.timestampInMilliseconds() + threadIndex)`.
