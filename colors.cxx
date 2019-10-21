#include "render.h"

#include <vtkAppendPolyData.h>
#include <vtkApplyColors.h>
#include <vtkCylinderSource.h>
#include <vtkRotationalExtrusionFilter.h>
#include <vtkPolyDataNormals.h>
#include <vtkRegularPolygonSource.h>
#include <vtkTransform.h>
#include <vtkTransformFilter.h>

#include "vtkPolyDataBooleanFilter.h"

// Necessary to avoid linker errors from vtkbool
int Point::_tag = 0;

vtkSmartPointer<vtkAlgorithm> Rotate(
        vtkSmartPointer<vtkAlgorithm> input, double degrees,
        double axis_x, double axis_y, double axis_z) {
  auto transformFilter = vtkSmartPointer<vtkTransformFilter>::New();
  transformFilter->SetInputConnection(input->GetOutputPort());
  auto transform = vtkSmartPointer<vtkTransform>::New();
  transform->RotateWXYZ(degrees, axis_x, axis_y, axis_z);
  transformFilter->SetTransform(transform);
  return transformFilter;
}

vtkSmartPointer<vtkAlgorithm> MakeSpring(double cross_section_radius,
                                         double radius, double turns,
                                         double height) {
  auto cross_section = vtkSmartPointer<vtkRegularPolygonSource>::New();
  cross_section->SetNumberOfSides(15);
  cross_section->SetCenter(radius - cross_section_radius, 0, 0);
  cross_section->SetNormal(0, 0, 1);
  cross_section->SetRadius(cross_section_radius);

  auto extrusion = vtkSmartPointer<vtkRotationalExtrusionFilter>::New();
  extrusion->SetTranslation(height);
  extrusion->SetAngle(360 * turns);
  extrusion->SetResolution(20 * turns);
  extrusion->SetInputConnection(cross_section->GetOutputPort());

  auto fix_normals = vtkSmartPointer<vtkPolyDataNormals>::New();
  fix_normals->SetInputConnection(extrusion->GetOutputPort());
  fix_normals->ComputePointNormalsOn();
  fix_normals->ComputeCellNormalsOn();

  return Rotate(fix_normals, /*degrees=*/-90,
                /*axis_x=*/1, /*axis_y=*/0, /*axis_z=*/0);
}

vtkSmartPointer<vtkAlgorithm> Color(vtkSmartPointer<vtkAlgorithm> input,
                                    double r, double g, double b) {
  auto filter = vtkSmartPointer<vtkApplyColors>::New();
  filter->SetInputConnection(0, input->GetOutputPort());
  filter->SetDefaultCellColor(r, g, b);
  return filter;
}

vtkSmartPointer<vtkAlgorithm> Union(
        vtkSmartPointer<vtkAlgorithm> first) {
  return first;
}

template<typename... Remaining>
vtkSmartPointer<vtkPolyDataBooleanFilter> Union(
        vtkSmartPointer<vtkAlgorithm> first, Remaining... args) {
  auto bf = vtkSmartPointer<vtkPolyDataBooleanFilter>::New();
  bf->SetInputConnection(0, first->GetOutputPort());
  bf->SetInputConnection(1, Union(args...)->GetOutputPort());
  bf->SetOperModeToUnion();
  return bf;
}

vtkSmartPointer<vtkAlgorithm> GenerateShape() {
  auto spring = Color(
          MakeSpring(/*cross_section_radius=*/5, /*radius=*/7,
                     /*turns=*/3, /*height=*/20),
          1, 0, 0);

  auto spring2 = Color(
          Rotate(
              MakeSpring(/*cross_section_radius=*/5, /*radius=*/7, /*turns=*/3,
                         /*height=*/20),
              /*degrees=*/180, /*axis_x=*/0, /*axis_y=*/1, /*axis_z=*/0),
          0, 0, 1);

  auto cyl = vtkSmartPointer<vtkCylinderSource>::New();
  cyl->SetResolution(32);
  cyl->SetHeight(20);
  cyl->SetRadius(5);
  cyl->SetCenter(0, 10, 0);

  auto cyl_color = Color(cyl, 1, 1, 0);

  return Union(spring, spring2, cyl_color);
}

int main(int argc, char *argv[]) {
  Render(GenerateShape(), "colors");
  return 0;
}
