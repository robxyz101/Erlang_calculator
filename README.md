# Erlang Calculator

A small command-line calculator in C for dimensioning trunked systems (e.g. cellular networks) with the **Erlang B** and **Erlang C** models. Given any two of the three parameters *traffic intensity*, *number of channels* and *Grade of Service*, it computes the third.

## Background

In a trunked system a large population of users shares a small pool of $C$ radio channels. A channel is assigned on a per-call basis and returned to the pool when the call ends. The **Grade of Service (GOS)** is the probability that a call is blocked (Erlang B) or delayed (Erlang C) during the busy hour.

| Symbol | Meaning |
|--------|---------|
| $A$ | Offered traffic intensity, in Erlang |
| $C$ | Number of trunked channels |
| $GOS$ | Pr[blocking] (Erlang B) or Pr[delay > 0] (Erlang C) |

Recall that $A = U A_u = U \lambda H$, where $U$ is the number of users, $\lambda$ the call request rate per user and $H$ the mean holding time. The program takes $A$ directly as input.

### Erlang B (Blocked Calls Cleared)

A call that finds all channels busy is rejected.

$$\text{Pr}[\text{blocking}] = \frac{\frac{A^C}{C!}}{\sum_{k=0}^{C} \frac{A^k}{k!}} = GOS$$

Assumptions (M/M/C): Poisson arrivals, infinite user population, exponential holding times, finite $C$. Since real populations are finite, the result is a conservative estimate.

### Erlang C (Blocked Calls Delayed)

A call that finds all channels busy waits in a queue. It requires $A < C$.

$$\text{Pr}[\text{delay} > 0] = \frac{A^C}{A^C + C! \left(1 - \frac{A}{C}\right) \sum_{k=0}^{C-1} \frac{A^k}{k!}} = GOS$$

## Features

For each model the menu offers three calculations:

| Option | Inputs | Output |
|:------:|--------|--------|
| 1 | $A$, $C$ | $GOS$ |
| 2 | $GOS$, $C$ | $A$ |
| 3 | $A$, $GOS$ | $C$ (minimum number of channels) |

Valid inputs: $A \ge 0$, $C > 0$ (integer), $0 < GOS < 1$. For Erlang C, option 1 also requires $A < C$.

## Build and run

```bash
gcc erlang_calc.c -o erlang -lm
./erlang
```

The `-lm` flag is needed for `pow`, `tgamma` and `floor`. Use a decimal **point** (`4.46`, not `4,46`).

## Example

Trunking efficiency check: 10 channels at $GOS = 0.01$ in a single pool vs. two pools of 5 channels.

```
Erlang B -> [2] Traffic intensity (A)
Desired GoS = 0.010000   C = 10   ->  A = 4.461176 Erlang
Desired GoS = 0.010000   C = 5    ->  A = 1.360787 Erlang
```

One pool carries 4.46 Erlang, while two pools carry $2 \times 1.36 = 2.72$ Erlang, so the single pool handles about 60% more traffic at the same GOS.

Erlang C, $A = 10$ Erlang with $C = 12$ channels gives $\text{Pr}[\text{delay} > 0] = 0.449388$. For $A = 10$ and $GOS = 0.2$, the required number of channels is $C = 14$.

## How it works

| Function | Purpose | Method |
|----------|---------|--------|
| `erlangB(A, C)` | $GOS$ for Erlang B | Direct formula (`pow`, `tgamma`) |
| `erlangC(A, C)` | $GOS$ for Erlang C | Direct formula (`pow`, `tgamma`) |
| `findA(target, C)` | $A$ for Erlang B | Bisection (upper bound doubled until it brackets the solution) |
| `findA_C(target, C)` | $A$ for Erlang C | Bisection on $[0, C)$ |
| `findC(A, target)` | $C$ for Erlang B | Linear search from $C = 1$ |
| `findC_C(A, target)` | $C$ for Erlang C | Linear search from $C = \lfloor A \rfloor + 1$ (smallest integer with $C > A$) |

Bisection works because $GOS$ is monotonically increasing in $A$, and the linear search works because it is decreasing in $C$. Bisection stops at an interval width of $10^{-6}$ Erlang.

## Limitations

This version is deliberately simple. The points below do not affect typical textbook cases (tens of channels, moderate traffic), but you should know them:

- **Numerical overflow for large $C$.** `tgamma(C + 1)` exceeds the `double` range for $C \ge 171$ (and `pow(A, k)` overflows earlier for very large $A$). The result becomes `inf`/`NaN` and the program does not warn about it. A stable alternative is the recursion $B(A,n) = \frac{A\,B(A,n-1)}{n + A\,B(A,n-1)}$ with $B(A,0)=1$, and $\text{Pr}[\text{delay} > 0] = \frac{B}{1 - \frac{A}{C}(1 - B)}$, but I preferred to stick to the textbook formulas for simplicity.
- **No upper bounds on inputs.** Very large $A$ or $C$ can make the searches slow, since each call to `erlangB`/`erlangC` is itself $O(C)$.
- **Fixed absolute tolerance** ($10^{-6}$) in the bisection.
- **Model assumptions.** Results are only as valid as the M/M/C hypotheses (Poisson arrivals, infinite population, exponential holding times).
- **Partial Erlang C.** Only $\text{Pr}[\text{delay} > 0]$ is computed, not $\text{Pr}[\text{delay} > t]$ or the mean delay $D$.

For a small educational project these limitations are acceptable: the code stays close to the textbook formulas, which keeps it easy to read and verify by hand.
