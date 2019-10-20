#include "render.h"

#include <vtkBooleanOperationPolyDataFilter.h>
#include <vtkSphereSource.h>

vtkSmartPointer<vtkPolyDataAlgorithm> GenerateShape() {
  auto sphere1 = vtkSmartPointer<vtkSphereSource>::New();
  sphere1->SetRadius(10);
  sphere1->SetCenter(0, 0, 0);

  auto sphere2 = vtkSmartPointer<vtkSphereSource>::New();
  sphere2->SetRadius(10);
  sphere2->SetCenter(0, 15, 0);

  auto bf = vtkSmartPointer<vtkBooleanOperationPolyDataFilter>::New();
  bf->SetOperationToUnion();
  bf->SetInputConnection(0, sphere1->GetOutputPort());
  bf->SetInputConnection(1, sphere2->GetOutputPort());

  return bf;
}

int main(int argc, char *argv[]) {
  Render(GenerateShape(), "native_union");
  return 0;
}
