# Quantum Script Extension Random — Documentation

`quantum-script--random` is the **Random extension of Quantum Script**. Loading
it with `Script.requireExtension("Random")` adds the global constructor
`Random`: a pseudo-random number generator object (Mersenne Twister MT19937,
32-bit) that a script can **seed**, so the same seed always gives the same
sequence.

```javascript
Script.requireExtension("Random");

var rnd = new Random();
rnd.seed(12345);            // reproducible sequence
rnd.next();                 // 0.9296160866506398, in [0, 1)
rnd.toInteger();            // 3992670690, the same value as a 32-bit integer
rnd.next();                 // 0.8901547130662948
```

Every `Random` object is an independent generator with its own state of 624
words. `next()` advances it; `toNumber()`, `toInteger()` and `toString()` read
the last value without advancing.

- **Reproducible.** `seed(x)` with the same `x` gives the same sequence on
  every run, platform and thread; it is the sequence of C++ `std::mt19937`
  for the same seed (`seed(5489)` starts with `3499211612`).
- **Seeded from the clock by default.** A new `Random` is seeded with the
  time in seconds, and so is `seed(0)`: generators created in the same
  second produce the same numbers.
- **Not secure.** MT19937 is predictable once 624 outputs are seen. Never use
  it for keys, passwords, tokens, nonces or anything secret.

```
scripts: quantum-script .js, magnet, embedding hosts, Pixel32, SSHRemote, ...
quantum-script--random     <-- this extension: Random, next / toInteger / toNumber / toString / seed
quantum-script             (Executive, Variable, Context)
xyo-cryptography           (RandomMT: the MT19937 generator)
xyo-system, xyo-multithreading, xyo-encoding, xyo-data-structures, xyo-managed-memory, xyo-platform
```

## Why it exists

The Quantum Script core has no random numbers at all. The `Math` extension
adds `Math.random()`, but that is the C `rand()`: one hidden generator, only
32768 different values with MSVC, and no way to set the seed. `Random` fills
the gaps:

| Need | `Math.random()` | `Random` |
|------|-----------------|----------|
| Same sequence on every run (tests, replays, procedural content) | no, seeded with `time()` | `seed(x)` |
| Several independent streams | no, one global generator | one per `Random` object |
| Resolution | 1/32768 with MSVC | 1/2³² (32-bit values) |
| Same result on Windows and Linux | no (`rand()` differs) | yes |
| Pass the generator to native code | no | `Pixel32.Image.prototype.noise(rnd)`, `noise2Bit(rnd)` |
| Secrets (keys, tokens) | no | **no** |

Typical uses: dice, shuffles and random picks in scripts, test data that can
be regenerated from a seed, noise textures with the `Pixel32` extension,
unique-enough temporary file names (the `SSHRemote` extension does this).

## Concepts at a glance

| Need | Use | Notes |
|------|-----|-------|
| Load the extension | `Script.requireExtension("Random");` | loading twice does nothing |
| Create a generator | `var rnd = new Random();` | seeded with the time in seconds; `Random()` without `new` works too |
| Fixed sequence | `rnd.seed(12345);` | integer in `[1, 2³²)`; returns `true` |
| Different sequence per run | `rnd.seed(DateTime.timestampInMilliseconds());` | needs the `DateTime` extension |
| Next number in `[0, 1)` | `rnd.next()` | advances the generator |
| Next 32-bit integer | `rnd.next(); rnd.toInteger();` | in `[0, 4294967295]` |
| Read the last value again | `rnd.toNumber()`, `rnd.toInteger()`, `rnd.toString()` | do not advance |
| Integer in `[min, max]` | `min + Math.floor(rnd.next() * (max - min + 1))` | needs `Math` for `floor` |
| Random element | `list[Math.floor(rnd.next() * list.length)]` | |
| Shuffle | Fisher-Yates, see [Script API](script-api.md#shuffle) | |
| Secret values | not available in scripts | do it in C++ with `XYO::Cryptography::SystemRandom` |

## Contents

| Document | What it covers |
|----------|----------------|
| [Getting started](getting-started.md) | Build and install, load the extension from a script, magnet, register it in a C++ host, static builds, threads |
| [Script API](script-api.md) | The `Random` constructor and its methods, seeding rules, the value model, recipes (ranges, shuffle, picks, weights, strings, noise) |
| [C++ API](cpp-api.md) | `registerInternalExtension`, `initExecutive`, the DLL entry point, `VariableRandom`, using a script generator from another extension, notes for maintainers |
| [API reference](reference.md) | Every script and C++ symbol on one page |

Quantum Script itself (the language, `Script.requireExtension`, embedding,
writing extensions) is documented in the `quantum-script` repository,
`docs/`. The generator is `XYO::Cryptography::RandomMT`, documented in the
`xyo-cryptography` repository, `docs/random.md`.

## Source map

```
source/XYO/QuantumScript.Extension/Random.hpp            umbrella header, include this from C++
source/XYO/QuantumScript.Extension/Random.Amalgam.cpp    the whole extension in one translation unit
source/XYO/QuantumScript.Extension/Random/
    Dependency.hpp                                       <XYO/QuantumScript.hpp>, <XYO/Cryptography.hpp>, export macro
    Library[.hpp/.cpp]                                   initExecutive, registerInternalExtension,
                                                         the Random constructor and its five methods
    Context.hpp                                          RandomContext: the "Random" symbol and prototype
    VariableRandom[.hpp/.cpp]                            the script value: a Variable holding a RandomMT
    Copyright / License / Version                        library metadata
    Library.rc, *.rh                                     Windows version resource
test/test.01.cpp                                         C++ host registering Console and Random as internal
test/test.01.js                                          loads the extension
```

## AI assistant skill

A Claude Code skill describing how to use this extension lives in
[`.claude/skills/quantum-script--random/`](../.claude/skills/quantum-script--random/SKILL.md).
It is picked up automatically inside this repository; copy the folder to
`~/.claude/skills/` to have it available in the projects that use `Random`
(Quantum Script tools, magnet scripts, other extensions).
