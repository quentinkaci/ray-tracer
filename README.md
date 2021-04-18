# ray-tracer

Ray-tracer written from scratch in C++.

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