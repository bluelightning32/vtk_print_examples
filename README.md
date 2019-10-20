# vtk_print_examples

This repository shows how to use VTK and vtkbool to make STL files in C++ for 3d printing.

VTK library was created on 1993. As such, its API does not use new C++ features (such as std::function or std::shared_ptr)
where they would be helpful. All of the objects are internally reference counted. The reference count can be managed by
wrapping the type up in a vtkSmartPointer, such as `vtkSmartPointer<vtkAlgorithm>`.

VTK stands for "The Visualization Toolkit". It is able to process and visualize 3d meshes, but the library also deals
with plots, images, cameras, widgets, etc... There is a decent amount of documentation for VTK, but the documentation
tends to focus on the all aspects of VTK except for 3d mesh processing. I have found the doxygen pages to be most useful
for the 3d mesh functions. For these examples, all of the rendering boilerplate has been moved to render.h.

VTK has a BSD 3 clause license, and vtkbool is licensed Apache 2.0.

VTK is designed to have scalar data associated with each vertex, then the mapper would convert that scalar data into
colors, which would be applied to the polyhedron faces. However, none of the simple mesh generators (cube, cylinder,
sphere, etc.) have options to set their color/scalar data. There is no existing filter to set the color on a mesh either.
Even if one wrote a custom filter to do so, vtkbool will overwrite the color/scalar data. I have not found a solution
to get colored meshes.

Other missing CAD features:
* Minkowski sum
* 3d to 2d projection
* Rounded corner option for shapes

## Building on Linux
First install the VTK development libraries and CMake. Make sure that you cloned vtk_print_examples recursively by checking that
the vtkbool directory is non-empty. Build the examples with these commands:
```
mkdir build
cd build
cmake ..
make -j4
```

## Examples

## vtkbool_union
Typically in VTK all of the meshes (cubes, spheres, etc) would have its own actor which renders it to the output window.
However, to generate an STL file, all of those meshes need to be merged together. With the CSG model, one would use a
union to do so. I have been unable to get the boolean filter (union/intersection) that is built into VTK to work with
anything other than their sphere example. I suspect that's what the authors means when they say it does not work with
"co-planar surfaces". The vtkbool library is basically a drop in boolean filter replacement.

This example shows 2 cubes and a cylinder connected together with a union. The colors are an artifact of the last
vtkbool union that is applied.

## native_union
This shows the one example I can get of the native VTK union working. It unions 2 spheres together.

## append_for_union
One problem with vtkbool
is that the input objects must intersect. If one tries to union two disjoint meshes together, vtkbool will print an
error to stderr and return an empty mesh. As a workaround, 3d meshes can be "appended". The output will be completely
correct if the inputs were disjoint. Even if the inputs intersect, appending them still produces something cura can
print, even if the intersection is technically covered twice.

This example uses append for 2 cubes that intersect and a cylinder that's not connected.

## translate
`vtkTransformFilter` can do rotations, scaling, and translations. This example shows it doing a rotation and translation.

### 3dhull
The `vtkDelaunay3D` computes a mostly accurate 3d convex hull. However, it outputs unstructured data (a collection of
tetrahedons), which has to be converted back into a 3d mesh with `tkDataSetSurfaceFilter`.

The `SetOffset` option is important. If it is set too low, the outputed 3d "convex" hull won't be complete convex.
Setting it higher creates more warning spam on the console.

## extrusion
This example shows a linear extrusion of a pentagon.

## spring
This example shows a pentagon that is extruded linearly and rotated at the same time. That forms a spring shape.

## torus

`vtkParametricFunctionSource` can generate a 3d mesh by evaluating a parameteric function with a specified resolution.
This example uses the built in torus function.
