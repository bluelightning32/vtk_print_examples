#include "render.h"

#include <vtkAppendPolyData.h>
#include <vtkCubeSource.h>
#include <vtkCylinderSource.h>
#include <vtkDataSetSurfaceFilter.h>
#include <vtkDelaunay3D.h>
#include <vtkPolyDataNormals.h>
#include <vtkTransform.h>
#include <vtkTransformFilter.h>

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
  transform->Translate(0, 10, 0);
  transformFilter->SetTransform(transform);

  // This cube is completely enclosed by cu, so all of its points should be
  // excluded from the convex hull.
  auto cu3 = vtkSmartPointer<vtkCubeSource>::New();
  cu3->SetXLength(1);
  cu3->SetYLength(1);
  cu3->SetZLength(1);
  cu3->SetCenter(0, 0, 0);

  auto cyl = vtkSmartPointer<vtkCylinderSource>::New();
  cyl->SetResolution(32);
  cyl->SetHeight(10);
  cyl->SetRadius(5);
  cyl->SetCenter(0, 14, 0);

  auto append = vtkSmartPointer<vtkAppendPolyData>::New();
  append->UserManagedInputsOn();
  append->SetInputConnectionByNumber(0, cu->GetOutputPort());
  append->SetInputConnectionByNumber(1, transformFilter->GetOutputPort());
  append->SetInputConnectionByNumber(2, cu3->GetOutputPort());
  append->SetInputConnectionByNumber(3, cyl->GetOutputPort());

  auto delaunay = vtkSmartPointer<vtkDelaunay3D>::New();
  // Set this high enough to get a convex hull for the data, but low enough to
  // reduce the number of warnings that are printed.
  delaunay->SetOffset(4);
  delaunay->SetInputConnection(append->GetOutputPort());

  auto surface = vtkSmartPointer<vtkDataSetSurfaceFilter>::New();
  surface->SetInputConnection(delaunay->GetOutputPort());

  auto fix_normals = vtkSmartPointer<vtkPolyDataNormals>::New();
  fix_normals->SetInputConnection(surface->GetOutputPort());
  fix_normals->ComputePointNormalsOn();
  fix_normals->ComputeCellNormalsOn();
  return fix_normals;
}

int main(int argc, char *argv[]) {
  Render(GenerateShape(), "3dhull");
  return 0;
}
