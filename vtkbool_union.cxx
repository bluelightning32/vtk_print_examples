#include "render.h"

#include <vtkCubeSource.h>
#include <vtkCylinderSource.h>

#include "vtkPolyDataBooleanFilter.h"

// Necessary to avoid linker errors from vtkbool
int Point::_tag = 0;

vtkSmartPointer<vtkPolyDataAlgorithm> GenerateShape() {
  auto cu = vtkSmartPointer<vtkCubeSource>::New();
  cu->SetXLength(10);
  cu->SetYLength(5);
  cu->SetZLength(10);
  cu->SetCenter(0, -5, 0);

  auto cu2 = vtkSmartPointer<vtkCubeSource>::New();
  cu2->SetXLength(10);
  cu2->SetYLength(5);
  cu2->SetZLength(10);
  cu2->SetCenter(0, 5, 0);

  auto cyl = vtkSmartPointer<vtkCylinderSource>::New();
  cyl->SetResolution(32);
  cyl->SetHeight(5);
  cyl->SetRadius(5);
  cyl->SetCenter(0, 0, 0);

  auto bf = vtkSmartPointer<vtkPolyDataBooleanFilter>::New();
  bf->SetInputConnection(0, cu->GetOutputPort());
  bf->SetInputConnection(1, cyl->GetOutputPort());
  bf->SetOperModeToUnion();

  auto bf2 = vtkSmartPointer<vtkPolyDataBooleanFilter>::New();
  bf2->SetOperModeToUnion();
  bf2->SetInputConnection(0, bf->GetOutputPort());
  bf2->SetInputConnection(1, cu2->GetOutputPort());

  return bf2;
}

int main(int argc, char *argv[]) {
  Render(GenerateShape(), "vtkbool_union");
  return 0;
}
