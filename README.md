# ray-tracer

![Results](https://raw.githubusercontent.com/quentinkaci/ray-tracer/main/docs/results.gif)

Ray-tracer written from scratch in C++.

## Prerequisites

* [libtbb-dev]()
* [nlohmann json](https://github.com/nlohmann/json)
* [libpng-dev](http://www.libpng.org/pub/png/libpng.html)
* [png++](http://www.nongnu.org/pngpp/)

## Usage

```
mkdir build && cd build
cmake .. && make
./raytracer ../scenes/default.json
```

## Features

- Objects: sphere, cubes, triangles, rectangles and blobs
- Handle diffuse, reflection, refraction and specular lighting
- Uniform, transparent, procedural and image textures
- Bump mapping on textures
- Point, directional and spot lights
- Configure scenes with JSON config files
- Anti-Aliasing
- Soft-shadows
- Depth of field
- Gamma correction
- Multithreading
- Load/save to .PNG and .PPM
- Skybox
- Billboards
- Anaglyph rendering