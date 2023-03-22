
<a name="readme-top"></a>

![issues](https://img.shields.io/github/issues/Open-GD/OpenGD?style=for-the-badge&logo=appveyor)
![forks](https://img.shields.io/github/forks/Open-GD/OpenGD?style=for-the-badge&logo=appveyor)
![stars](https://img.shields.io/github/stars/Open-GD/OpenGD?style=for-the-badge&logo=appveyor)
![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/Open-GD/OpenGD?style=for-the-badge&logo=appveyor)
![LICENSE](https://img.shields.io/github/license/Open-GD/OpenGD?style=for-the-badge)

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
    <!-- <a href="."><strong>Explore the docs »</strong></a>  -->
    <br />
    <br />
    ·
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


Before building OpenGD, you need to install Python 3.7+ and axmol. To install axmol, clone the **release branch** and run `python.py`. After it has finished, restart the console for environment variables to take effect.
```
git clone --branch release https://github.com/axmolengine/axmol
cd axmol
python setup.py
```

After installing Axmol, you can build OpenGD as a CMake project.
```
git clone https://github.com/Open-GD/OpenGD
cd OpenGD
cmake -B build
cmake --build build --config RelWithDebInfo
```
To actually run it you will need the resources from the 2.1 version of Geometry Dash.

For more details check out the [axmol](https://github.com/axmolengine/axmol)

<!-- LICENSE -->
## License

Distributed under the GPL v3 License . See `LICENSE` for more information.

<!-- CONTACT -->
## Contact

[Discord Server](https://discord.gg/gcbuuR4JWg)


<!-- ACKNOWLEDGMENTS -->
## ACKNOWLEDGMENTS

* [axmol](https://github.com/axmolengine/axmol)
* [GD 1.0 Decomp by Wylie](https://github.com/Wyliemaster/Geometry-Dash-1.0)
* [GD Physics Decomp by Camila](https://github.com/camila314/gdp)

### Contributors
This project exists thanks to all the people who have contributed:

<a href="https://github.com/Open-GD/OpenGD/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=Open-GD/OpenGD" />
</a>
