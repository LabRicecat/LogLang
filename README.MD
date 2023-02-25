# LogLang
(yes, name in progress, leave me alone)

## Instalation
```
$ git clone https://github.com/labricecat/loglang
$ cd LogLang
$ catmods sync
$ mkdir build && cd build
$ cmake .. && make install
$ ll
```

## Usage
```
$> fn <function definition>
$> eval <function>(parameter)
$> info <function>
$> file <file.ll>
```

## Syntax
```
FUNCTIONARGS STATEMENT, STATEMENT, ...
```
### Examples
```
; function t with no arguments that returns true
t TRUE

; function f1 with one parameter that returns its parameter
f1 1

; function f2 that returns the first argument AND the second argument
f2 1 & 2

; function "or2" with 2 parameters
or2 !(!1 & !1)

; function that takes 3 parameters and return arg1 OR arg2 OR arg3
or3 or2(or2(1 2) 3)

; function if3 that returns its second parameter if the first is true, otherwise the third
if3 or3((1 & 2) (2 & 3) (!1 & 3))
```
