#include "render.h"

#include <vtkParametricFunctionSource.h>
#include <vtkParametricTorus.h>

vtkSmartPointer<vtkPolyDataAlgorithm> GenerateShape() {
  auto torusParameters = vtkSmartPointer<vtkParametricTorus>::New();
  torusParameters->SetRingRadius(10);
  torusParameters->SetCrossSectionRadius(3);
  auto torus = vtkSmartPointer<vtkParametricFunctionSource>::New();
  torus->SetParametricFunction(torusParameters);
  // Lower the subdivisions so that the STL file isn't huge. The UResolution
  // controls the big ring circle.
  torus->SetUResolution(30);
  // The VResolution controls the smaller cross section circle that is extruded
  // along the ring.
  torus->SetVResolution(15);
  // The W resolution is unused for the torus.
  torus->SetWResolution(10);
  return torus;
}

int main(int argc, char *argv[]) {
  Render(GenerateShape(), "torus");
  return 0;
}
