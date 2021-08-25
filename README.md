# RPAutoBuild
RPAutoBuilder - The C++ Builder! (a make alternitive)

### RPAutoBuild
RPAutoBuild is a C++ alternitive to make (and I guess Cmake) and works by cloning a git repo (and submodules), reading the rpauto.build file and building targets according to the config. Operating systems can be freely selected and will automatically be defined in the source files though `-D{OS}` (This feature require a compiler for the target OS)

### Requirements
RPAutoBuild requires [RPCommonLib](https://github.com/AnzoDK/RPCommonLib) (a set of headers made to make my own life easier)

### Build
Ironically enough a makefile is provided as most people are assumed to have make installed - but a `rpauto.build` is also provided to serve as an example and does in fact build RPAutoBuild if a previous version of the software is already installed.<br>

The install commands are as following:

Make:

```
make OS={PUT_OS_HERE}
```

RPAutoBuild:

```
rpauto https://github.com/AnzoDK/RPAutoBuild
```

### Usage
RPAutoBuild (as of now) requires a git repo (this requirement will be removed) and will download it using the following command:


```
rpauto {Git_Repo}
```

It is possible however to build already downloaded repos, as long as the URL is provided (this will be changed to allow just a folder) with the following command:

```
rpauto {Git_Repo} --local
```
The downside of this command (as of now) is that the repo in question has to exist in the working directory of the terminal. (Also dumb, and will be fixed)

### Config
The rpauto.build file is simple to setup. The first lines will contain the settings for the builder itself. Here there are a few required lines:

```
OS={"{OS}","{OS2}","..."}
OS_COMPILER={"_null_","g++"};
OS_TARGET={"default","default"};
ARCH={"x86_64"}
```
These 4 lines decide what OS(ses) we need to build, what compilers to use, what build target to build, and what architecture will be targeted. It worth to note that `OS_COMPILER`, `OS_TARGET` and `OS` must have the same amount of arguments for the config to be accepted.

Besides these, there exists 2 other commands that are not required:
```
SET_BUILD_VAR={"",""}
SET_VAR={"",""}
```

`SET_BUILD_VAR` Sets a variable in the builder, that the builder will respond to, but only if it matches any of a few predetermined ones.
`SET_VAR` Allows to create user-defined variables and use them elsewhere in the config, as can be seen in the example rpauto.build file.


TODO Finish ReadMe
