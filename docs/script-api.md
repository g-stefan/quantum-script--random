# Script API

Everything the extension defines after `Script.requireExtension("Random")`.
All results shown below were produced by the interpreter.

## The `Random` constructor

```javascript
var rnd = new Random();
var other = Random();       // same thing, `new` is optional
```

Creates an independent generator (Mersenne Twister MT19937, the algorithm of
C++ `std::mt19937`) **seeded with the current time in seconds**. Arguments
are ignored: `new Random(5)` is not seeded with `5`; call `seed(5)`.

- `typeof(rnd)` is `"Random"`; `rnd instanceof Random` is `true`.
- `Script.isObject(rnd)` is `false`: it is a native value, like a `Buffer`
  or a `DateTime`, not a plain object. Assigning a property
  (`rnd.name = 1`) throws `setPropertyBySymbol`.
- A `Random` is always truthy.
- Assignment copies the reference: after `var b = a;`, `b.next()` advances
  `a` too.

## Methods

| Method | Advances | Returns |
|--------|----------|---------|
| `rnd.next()` | yes | the new value as a Number in `[0, 1)` |
| `rnd.toNumber()` | no | the last value as a Number in `[0, 1)` |
| `rnd.toInteger()` | no | the last value as an integer in `[0, 4294967295]` |
| `rnd.toString()` | no | `toNumber()` as text, e.g. `"0.9296160866506398"` |
| `rnd.seed(x)` | restarts | `true` if `x` was accepted, `false` otherwise |

All five throw `invalid parameter` when `this` is not a `Random`
(`Random.prototype.next.call({})`).

## Value model

The generator keeps **one current value**, a 32-bit unsigned integer `v`:

- `next()` computes the next MT19937 output, stores it in `v` and returns
  `v / 4294967296`;
- `toInteger()` returns `v`;
- `toNumber()` returns `v / 4294967296` (exactly what the last `next()`
  returned);
- `toString()` returns that number as text.

```javascript
rnd.seed(12345);
rnd.next();          // 0.9296160866506398
rnd.toInteger();     // 3992670690
rnd.toNumber();      // 0.9296160866506398   (3992670690 / 4294967296)
rnd.toString();      // "0.9296160866506398"
rnd.next();          // 0.8901547130662948
rnd.toInteger();     // 3823185381
```

To get several integers call `next()` before each `toInteger()`.

**Call `next()` before reading.** Right after `seed(x)` (and right after
`new Random()`), `v` is the seed itself, not a random value:

```javascript
rnd.seed(12345);
rnd.toInteger();     // 12345, the seed
rnd.toNumber();      // 2.8742942959070206e-06 (12345 / 4294967296)
```

### Text conversion

The method and the implicit conversion differ:

| Expression | Result |
|------------|--------|
| `rnd.toString()` | the last value as text, `"0.9296160866506398"` |
| `"" + rnd`, `Console.writeLn(rnd)`, `Convert.toString(rnd)` | `"Random"` (the type name) |
| `Convert.toNumber(rnd)`, `rnd * 1` | `NaN` |

Write `Console.writeLn(rnd.toString())` or `Console.writeLn(rnd.toNumber())`
to print the value.

## Seeding

```javascript
rnd.seed(12345);                                  // true: fixed sequence
rnd.seed(DateTime.timestampInMilliseconds());     // true: differs per run (DateTime extension)
```

`seed(x)` converts `x` with `toNumber`, then:

| `x` | Result | Effect |
|-----|--------|--------|
| integer in `[1, 4294967295]` | `true` | restarts the sequence of that seed |
| fractional, e.g. `7.9` | `true` | truncated toward zero: same as `seed(7)` |
| `4294967296` or more | `true` | only the low 32 bits are used: `seed(4294967297)` is `seed(1)` |
| `0`, or a multiple of `4294967296` | `true` | **seeded from the clock** (time in seconds), not "seed 0" |
| a number string, e.g. `"7"` | `true` | converted: same as `seed(7)` |
| negative (including a computed `-0`) | `false` | generator unchanged |
| `NaN`, `Infinity`, missing argument, a non-number string | `false` | generator unchanged |

Always check the result when the seed comes from outside the script:

```javascript
if (!rnd.seed(Convert.toNumber(text))) {
	throw new Error("invalid seed: " + text);
};
```

Properties of the sequence:

- **Same seed, same sequence**, on every run, platform, compiler and thread.
- **Compatible with `std::mt19937`**: for the same non-zero seed the values
  of `toInteger()` are those of C++ `std::mt19937`. `seed(5489)` (the C++
  default seed) gives `3499211612` first.
- **Only 2³² sequences**: the seed is 32 bits, so different seeds that agree
  in their low 32 bits give the same sequence.
- **Clock seeds repeat within a second.** `new Random()` and `seed(0)` use
  `time()`: every generator created in the same second starts with the same
  numbers.

```javascript
var a = new Random();
var b = new Random();
a.next() == b.next();       // true (same second, same seed)
```

For different streams in one script, seed each one explicitly
(`a.seed(base + 1); b.seed(base + 2);`).

### Saving and replaying a run

```javascript
Script.requireExtension("DateTime");

var seed = DateTime.timestampInMilliseconds() % 4294967296;
if (seed == 0) {
	seed = 1;
};
Console.writeLn("seed: " + seed);    // log it ...

var rnd = new Random();
rnd.seed(seed);                      // ... and run again with the same seed to reproduce
```

## Not for secrets

MT19937 is not a cryptographic generator: 624 consecutive 32-bit outputs
reveal its full state and every future value, and clock seeds can be
guessed. Never use `Random` for keys, passwords, salts, session ids, tokens
or nonces. Quantum Script has no secure generator for scripts; generate
secret values in C++ with `XYO::Cryptography::SystemRandom` (operating
system generator) and pass them to the script.

## Recipes

The recipes use `Math.floor` (load the `Math` extension) and take the
generator as a parameter, so one seeded `rnd` drives a whole run.

### Integers in a range

```javascript
function randomInt(rnd, min, max) {              // min and max included
	return min + Math.floor(rnd.next() * (max - min + 1));
};

function randomBelow(rnd, n) {                   // 0 .. n - 1
	return Math.floor(rnd.next() * n);
};

rnd.seed(12345);
randomInt(rnd, 1, 6);       // 6
randomBelow(rnd, 100);      // 89
```

Without `Math`, use the integer value with `%`:

```javascript
rnd.next();
var dice = 1 + rnd.toInteger() % 6;
```

Both have a bias below one part in 2³² / n, irrelevant unless `n` is huge.

### Random element, probability

```javascript
var colors = ["red", "green", "blue"];
var color = colors[Math.floor(rnd.next() * colors.length)];

if (rnd.next() < 0.25) {    // 25% chance
	// ...
};
```

### Weighted choice

```javascript
function weighted(rnd, weights) {                // returns an index
	var total = 0;
	for (var w of weights) {
		total += w;
	};
	var x = rnd.next() * total;
	for (var i = 0; i < weights.length; ++i) {
		if (x < weights[i]) {
			return i;
		};
		x -= weights[i];
	};
	return weights.length - 1;
};

weighted(rnd, [1, 2, 7]);   // 2 about 70% of the time
```

### Shuffle

```javascript
function shuffle(rnd, list) {                    // Fisher-Yates, returns a new array
	var out = [];
	for (var k = 0; k < list.length; ++k) {
		out[k] = list[k];
	};
	for (var i = out.length - 1; i > 0; --i) {
		var j = Math.floor(rnd.next() * (i + 1));
		var t = out[i];
		out[i] = out[j];
		out[j] = t;
	};
	return out;
};

rnd.seed(12345);
shuffle(rnd, [1, 2, 3, 4, 5, 6, 7, 8]);    // [6, 3, 4, 5, 1, 2, 7, 8]
```

### Random string

```javascript
function randomString(rnd, length, alphabet) {
	var out = "";
	for (var k = 0; k < length; ++k) {
		out += alphabet.substring(Math.floor(rnd.next() * alphabet.length), 1);
	};
	return out;
};

rnd.seed(12345);
randomString(rnd, 12, "abcdefghijklmnopqrstuvwxyz0123456789");    // "76legbh3utv8"
```

`substring(start, length)` takes a **length** in Quantum Script, and strings
cannot be indexed with `s[i]`. Fine for test data and temporary names,
**not** for passwords or tokens.

### Unique temporary file name

```javascript
Script.requireExtension("DateTime");
Script.requireExtension("SHA512");

var rnd = new Random();
rnd.seed(DateTime.timestampInMilliseconds());
rnd.next();
var tempFile = "_my-tool_" + SHA512.hash(rnd.toInteger() + ":" + name) + ".tmp";
```

This is the pattern of the `SSHRemote` extension.

### Noise images with Pixel32

The `Pixel32` extension accepts a `Random` and draws from it directly (one
`next()` per pixel, low 8 bits used):

```javascript
Script.requireExtension("Pixel32");     // also loads Random

var rnd = new Random();
rnd.seed(2024);                         // same texture on every run
var image = new Pixel32.Image(256, 256);
image.noise(rnd);                       // grey noise
image.noise2Bit(rnd);                   // black / white noise
var texture = Pixel32.perlinNoiseWrapBox(256, 256, [[16, 1, 1], [4, 1, 1], [1, 1, 1]], rnd);
```

An argument that is not a `Random` is ignored (the image is not changed).
The octave list of `perlinNoiseWrapBox` / `perlinNoise2BitWrapBox` is
described by the `Pixel32` extension.

## Comparison with `Math.random()`

| | `Math.random()` | `Random` |
|-|-----------------|----------|
| Extension | `Math` | `Random` |
| Algorithm | C `rand()` | MT19937 |
| Values | 32768 with MSVC | 2³² |
| Seed | `time()` at load, cannot be set | `seed(x)`, `time()` by default |
| Streams | one per thread / process | one per object |
| Same on Windows and Linux | no | yes |
| Secure | no | no |

Use `Math.random()` for a quick, unimportant random value; use `Random`
whenever the result must be reproducible, finer, or independent per stream.

## Not available

No `Random.prototype` method for ranges, floats with 53-bit precision,
normal distribution, or copying a generator's state from a script (the C++
`clone` exists, see [C++ API](cpp-api.md#variablerandom)). There is no
static `Random.next()`: always create an object.
