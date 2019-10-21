#include <iostream>

#include <vtkActor.h>
#include <vtkInteractorStyleSwitch.h>
#include <vtkPNGWriter.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSphereSource.h>
#include <vtkSTLWriter.h>
#include <vtkTransform.h>
#include <vtkTransformFilter.h>
#include <vtkWindowToImageFilter.h>

inline void Render(vtkSmartPointer<vtkAlgorithm> shape, const char* output) {
  auto stl_writer = vtkSmartPointer<vtkSTLWriter>::New();
  std::string stl_filename(output);
  stl_filename.append(".stl");
  stl_writer->SetFileName(stl_filename.c_str());
  stl_writer->SetInputConnection(shape->GetOutputPort());
  stl_writer->Write();
  std::cout << "Wrote to " << stl_filename << std::endl;

  auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(shape->GetOutputPort());
  mapper->SetScalarModeToUseCellFieldData();
  mapper->SelectColorArray("vtkApplyColors color");
  mapper->SetScalarVisibility(true);

  auto actor = vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);

  auto renderer = vtkSmartPointer<vtkRenderer>::New();
  renderer->AddViewProp(actor);

  auto render_window = vtkSmartPointer<vtkRenderWindow>::New();
  render_window->AddRenderer(renderer);

  auto interator = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  interator->SetRenderWindow(render_window);

  auto interactor_style = vtkSmartPointer<vtkInteractorStyleSwitch>::New();
  interactor_style->SetCurrentStyleToTrackballCamera();
  interator->SetInteractorStyle(interactor_style);

  render_window->Render();

  auto screenshot_filter = vtkSmartPointer<vtkWindowToImageFilter>::New();
  screenshot_filter->SetInput(render_window);
  screenshot_filter->SetScale(1, 1);
  screenshot_filter->SetInputBufferTypeToRGBA();
  screenshot_filter->ReadFrontBufferOff();
  
  auto png_writer = vtkSmartPointer<vtkPNGWriter>::New();
  std::string png_filename(output);
  png_filename.append(".png");
  png_writer->SetFileName(png_filename.c_str());
  png_writer->SetInputConnection(screenshot_filter->GetOutputPort());
  png_writer->Write();
  std::cout << "Wrote to " << png_filename << std::endl;
  
  render_window->Render();
  interator->Start();
}
