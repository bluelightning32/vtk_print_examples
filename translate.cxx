#include "render.h"

#include <vtkCubeSource.h>
#include <vtkTransform.h>
#include <vtkTransformFilter.h>

#include "vtkPolyDataBooleanFilter.h"

// Necessary to avoid linker errors from vtkbool
int Point::_tag = 0;

vtkSmartPointer<vtkPolyDataAlgorithm> GenerateShape() {
  auto cu = vtkSmartPointer<vtkCubeSource>::New();
  cu->SetXLength(10);
  cu->SetYLength(5);
  cu->SetZLength(10);
  cu->SetCenter(0, 0, 0);

  auto cu2 = vtkSmartPointer<vtkCubeSource>::New();
  cu2->SetXLength(10);
  cu2->SetYLength(10);
  cu2->SetZLength(10);
  cu2->SetCenter(0, 0, 0);

  auto transformFilter = vtkSmartPointer<vtkTransformFilter>::New();
  transformFilter->SetInputConnection(cu2->GetOutputPort());
  vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
  transform->PostMultiply();
  transform->RotateWXYZ(45, 1, 0, 1);
  transform->Translate(0, 8, 0);
  transformFilter->SetTransform(transform);

  auto bf = vtkSmartPointer<vtkPolyDataBooleanFilter>::New();
  bf->SetInputConnection(0, cu->GetOutputPort());
  bf->SetInputConnection(1, transformFilter->GetOutputPort());
  bf->SetOperModeToUnion();
  return bf;
}

int main(int argc, char *argv[]) {
  Render(GenerateShape(), "translate");
  return 0;
}
