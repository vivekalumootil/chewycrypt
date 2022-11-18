# Chewycrypt
An asymmetric encryption scheme that uses nested polynomials $\pmod{p}$.

## The main idea

Consider polynomials of the form $f = f_1(f_2(f_3...(f_k(x))...)$, where each $f_i(x)$ is $(x+j_i)^3$, for some $j_i \geq 0$.
This polynomial can be expanded into typical form, or kept in this nested form. 
In the expanded form, we can calculate $f(a)$ in $O(n)$ with Horner's method, where n is $\text{deg}(f)$.
In the nested form, we can solve the equation $f(x) = a$ in $O(k)$ by repeatedly solving for the input of the outermost function.

Now consider instead a nested function $f$ like the one above, but we only care about $f \pmod{p}$ for some prime $p \equiv 2 \pmod{3}$.
Notice how this polynomial $\pmod{p}$ can also be reduced to have $\leq \text{deg}(p-2)$ with Fermat's little theorem. This simplified polynomial is the public key.
The private key is the array of $j$ values (see above). The central idea of the scheme is that in a manner likewise to that listed above, we can solve for 
$a$ given $f(a)$ in $O(k)$ by precomputing the cubic roots of all values $\pmod{p}$ and applying the nested form that the private key represents.
Someone without the private key would have to simply try all residues $\pmod{p}$ to find one which matches $f(a)$, which would be $O(p^2)$.
$p$ must be $\equiv 2 \pmod{3}$ so that each $f_i$ has an inverse $\pmod{p}$. 

Here is an analysis of a conversation between two people:
Person 1 calculates $f(a)$, where $a$ is the message being sent, using the expanded and reduced form of the polynomial. This value is sent to Person 2.
Person 2 uses the private key to find $a$. Message received. 

## Efficiency Analysis

Here is an analysis of the orders of all components of the encryption scheme:

**Set-up**: The expanded form of the polynomial can be created in $O(p\log{p}k)$ using FFT. This is by far the most time-consuming part. $k$ should be reasonably low, much lower than $p$.

**Encryption**: Horner's method calculates $f(a) \pmod{p}$ in $O(\text{deg}(f))$ = $O(p)$. 

**Decryption**: By precalculating the cubic roots of all residues $\pmod{p}$, with order $O(p)$, we can find $a$ from $f(a)$ in $O(k)$ using the private key.

**Hacking**: A hacker can compute a table of $f(a)$ for every residue $a$ in $O(p^2)$ by simply trying every residue.
Once this table is created, messages may be intercepted and decoded in $O(1)$. 

