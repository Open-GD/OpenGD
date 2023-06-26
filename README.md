


<div align="center">

![issues](https://img.shields.io/github/issues/Open-GD/OpenGD?style=for-the-badge&color=blue)
![forks](https://img.shields.io/github/forks/Open-GD/OpenGD?style=for-the-badge)
![stars](https://img.shields.io/github/stars/Open-GD/OpenGD?style=for-the-badge&color=blue)
![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/Open-GD/OpenGD?style=for-the-badge&color=blue)
![LICENSE](https://img.shields.io/github/license/Open-GD/OpenGD?style=for-the-badge&color=blue)
</div>

<!-- PROJECT LOGO -->
<br />
<div align="center">
  <a href="https://github.com/Open-GD/OpenGD/releases/latest">
    <img src="https://user-images.githubusercontent.com/54410739/226145157-61edd6d9-eec4-479c-83b6-3f0c32e278c3.png" alt="Logo" width="80" height="80">
  </a>

  <h3 align="center">OpenGD</h3>

  <p align="center">
    Open source implementation of Geometry Dash
    <br />   
  </p>
  
![](https://img.shields.io/badge/platforms-windows%20%7C%20linux%20%7C%20mac%20%7C%20android%20%7C%20ios-blue)
    <p align="center">
    <a href="https://github.com/Open-GD/OpenGD/issues">Report Bug</a>
    ·
    <a href="https://github.com/Open-GD/OpenGD/releases/latest">Latest Release</a>
 · 
 <a href="https://github.com/Open-GD/OpenGD/issues">Request Feature</a>
  </p>
</div>


<!-- ABOUT THE PROJECT -->
## About The Project

![Stereo Madness running in OpenGD](https://cdn.discordapp.com/attachments/847950548921614366/1086798200146497647/6046uyhlekoa1.png "OpenGD")


OpenGD is an open-source implementation of the popular game Geometry Dash. Our main goal is to remake the gameplay 1:1, while also improving performance through new engine features and C++ enhancements. We also plan to implement multithreading in the future.

### Built With

OpenGD is powered by [axmol](https://github.com/axmolengine/axmol), which is maintained a fork of cocos2dx 4.0 that adds many new features and improvements over the original cocos2dx. The original Geometry Dash is also made with cocos2dx, but with a much older version from 2014.

## Build


Before building OpenGD, you need to install Python 3.7+ and axmol. To install axmol, clone the **release branch** and run `setup.py`. After it has finished, restart the console for environment variables to take effect.
```
git clone --branch release https://github.com/axmolengine/axmol
cd axmol
python setup.py
```

After installing axmol, you can build OpenGD as a CMake project.
```
git clone https://github.com/Open-GD/OpenGD
cd OpenGD
cmake -B build
cmake --build build --config RelWithDebInfo
```

> **Warning**
> VS 2019 might not work on Windows, VS 2022 is recommended

To actually run the executable you will need the resources from the 2.1 version of Geometry Dash, but game would still try to access them if it possible.

For more details check out the [axmol](https://github.com/axmolengine/axmol) README.

## Linux Specific Notify

**Also one thing that should be mentioned here.** After pull request for header optimization (removed all unused stuff from the code) being merged to `main` branch, **Linux support was entirely broken.** For anyone, who want to build OpenGD under this platform, please use `no-header-optimization` branch, where all related to HDO changes were reverted.


<!-- LICENSE -->
## License

Distributed under the GPL v3 License . See `LICENSE` for more information.

<!-- CONTACT -->
## Contact

[Discord Server](https://discord.gg/gcbuuR4JWg)


<!-- ACKNOWLEDGMENTS -->
## Credits

* [axmol](https://github.com/axmolengine/axmol) a fork of cocos2d-x-4.0
* [GD 1.0 decomps](https://github.com/Wyliemaster/Geometry-Dash-1.0) by Wylie
* [GD Physics decomps](https://github.com/camila314/gdp) by Camila
* [GD 2.1 decomps](https://github.com/matcool/gd-decomps) by mat
* [hps](https://github.com/jl2922/hps) high performance C++11 serialization library
* [gdclone](https://github.com/opstic/gdclone) another gd reconstruction project

### Contributors
This project exists thanks to all the people who have contributed:

<a href="https://github.com/Open-GD/OpenGD/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=Open-GD/OpenGD" />
</a>
