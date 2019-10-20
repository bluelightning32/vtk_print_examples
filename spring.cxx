#include "render.h"

#include <vtkRotationalExtrusionFilter.h>
#include <vtkPolyDataNormals.h>
#include <vtkRegularPolygonSource.h>
#include <vtkTransform.h>
#include <vtkTransformFilter.h>

vtkSmartPointer<vtkAlgorithm> GenerateShape() {
  auto pentagon = vtkSmartPointer<vtkRegularPolygonSource>::New();
  pentagon->SetNumberOfSides(5);
  pentagon->SetCenter(3, 0, 0);
  pentagon->SetNormal(0, 0, 1);
  pentagon->SetRadius(2);

  auto extrusion = vtkSmartPointer<vtkRotationalExtrusionFilter>::New();
  extrusion->SetTranslation(20);
  extrusion->SetAngle(360 * 3);
  extrusion->SetResolution(20 * 3);
  extrusion->SetInputConnection(pentagon->GetOutputPort());

  auto fix_normals = vtkSmartPointer<vtkPolyDataNormals>::New();
  fix_normals->SetInputConnection(extrusion->GetOutputPort());
  fix_normals->ComputePointNormalsOn();
  fix_normals->ComputeCellNormalsOn();

  auto transformFilter = vtkSmartPointer<vtkTransformFilter>::New();
  transformFilter->SetInputConnection(fix_normals->GetOutputPort());
  vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
  transform->RotateWXYZ(90, 1, 0, 0);
  transformFilter->SetTransform(transform);

  return transformFilter;
}

int main(int argc, char *argv[]) {
  Render(GenerateShape(), "spring");
  return 0;
}
