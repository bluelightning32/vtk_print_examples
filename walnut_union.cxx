#include "render.h"

#include <vtkCubeSource.h>
#include <vtkCylinderSource.h>

#include "walnut/vtkWalnutBooleanFilter.h"

vtkSmartPointer<vtkPolyDataAlgorithm> GenerateShape() {
  auto cu = vtkSmartPointer<vtkCubeSource>::New();
  cu->SetXLength(10);
  cu->SetYLength(5);
  cu->SetZLength(10);
  cu->SetCenter(0, -1, 0);

  auto cu2 = vtkSmartPointer<vtkCubeSource>::New();
  cu2->SetXLength(10);
  cu2->SetYLength(5);
  cu2->SetZLength(10);
  cu2->SetCenter(1, 1, 1);

  auto cyl = vtkSmartPointer<vtkCylinderSource>::New();
  cyl->SetResolution(32);
  cyl->SetHeight(5);
  cyl->SetRadius(5);
  cyl->SetCenter(0, 8, 0);

  auto bf = vtkSmartPointer<walnut::vtkWalnutBooleanFilter>::New();
  bf->AddInputConnection(cu->GetOutputPort());
  bf->AddInputConnection(cyl->GetOutputPort());
  bf->AddInputConnection(cu2->GetOutputPort());
  bf->SetOperationToUnion();

  return bf;
}

int main(int argc, char *argv[]) {
  Render(GenerateShape(), "walnut_union");
  return 0;
}
