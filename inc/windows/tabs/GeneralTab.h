#pragma once
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/choice.h>
#include <wx/slider.h>
#include <wx/tglbtn.h>
#include <wx/checkbox.h>
#include <wx/statbox.h>
#include <wx/glcanvas.h>

class GeneralTab : public wxPanel
{
public:
	GeneralTab(wxWindow* parent);
	~GeneralTab();

private:
	wxTextCtrl* eyeWidthResolutionTxt = nullptr;
	wxTextCtrl* eyeHeightResolutionTxt = nullptr;
	wxToggleButton* useSquareResolutionBtn = nullptr;
	wxTextCtrl* eyeFPSTxt = nullptr;

	wxSlider* gapSizeSlider = nullptr;
	wxTextCtrl* gapSizeTxt = nullptr;

	wxSlider* scaleFactorSlider = nullptr;
	wxTextCtrl* scaleFactorTxt = nullptr;

	wxSlider* cropFactorSlider = nullptr;
	wxTextCtrl* cropFactorTxt = nullptr;

	wxChoice* processingModeChc = nullptr;

	wxCheckBox* useFake3DChk = nullptr;
	
	// Left Eye 3D Settings
	wxSlider* leftEyeShiftSlider = nullptr;
	wxTextCtrl* leftEyeShiftTxt = nullptr;
	int leftEyeShift = 0;

	wxSlider* leftEyeK1Slider = nullptr;
	wxTextCtrl* leftEyeK1Txt = nullptr;
	double leftEyeK1 = 0.0;

	wxSlider* leftEyeK2Slider = nullptr;
	wxTextCtrl* leftEyeK2Txt = nullptr;
	double leftEyeK2 = 0.0;

	// Right Eye 3D Settings
	wxSlider* rightEyeShiftSlider = nullptr;
	wxTextCtrl* rightEyeShiftTxt = nullptr;
	int rightEyeShift = 0;

	wxSlider* rightEyeK1Slider = nullptr;
	wxTextCtrl* rightEyeK1Txt = nullptr;
	double rightEyeK1 = 0.0;

	wxSlider* rightEyeK2Slider = nullptr;
	wxTextCtrl* rightEyeK2Txt = nullptr;
	double rightEyeK2 = 0.0;
};