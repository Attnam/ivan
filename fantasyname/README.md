# Fantasy Name Generator

Four implementations -- JavaScript, C++, Elisp, and Perl -- of the
[name generator described at RinkWorks](http://rinkworks.com/namegen/).
The JavaScript and C++ implementations are by far the most mature.


## JavaScript

The JavaScript version uses an optimizing template-compiler to create
an efficient generator object.

```javascript
var generator = NameGen.compile("sV'i");
generator.toString();  // => "entheu'loaf"
generator.toString();  // => "honi'munch"
```

## C++

The C++ version also uses a template-compiler (based on the one for JavaScript)
to create an efficient generator object. It requires C++11.

```c++
NameGen::Generator generator("sV'i");
generator.toString();  // => "drai'sneeze"
generator.toString();  // => "ardou'bumble"
```

## Emacs Lisp

The Emacs Lisp version doesn't include a parser. It operates on
s-expressions.

```el
(fset 'generator (apply-partially #'namegen '(s V "'" i)))
(generator)  ; => "essei'knocker"
(generator)  ; => "tiaoe'nit"
```

## Perl

The Perl version is exceptionally slow, due to a slow parser.

```perl
generate("sV'i");  # => "echoi'bum"
```
