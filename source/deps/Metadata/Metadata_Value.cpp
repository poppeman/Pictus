#include "Metadata_Value.h"

namespace Metadata {
	std::basic_ostream<wchar_t>& operator << (std::basic_ostream<wchar_t>& out, const Rational& r) {
		if(r.Denominator == 1)
			out << r.Numerator;
		else if(r.Numerator == 0)
			out << L"0";
		else
			out << r.Numerator << L"/" << r.Denominator;

		return out;
	}

	std::basic_ostream<wchar_t>& operator << (std::basic_ostream<wchar_t>& out, const ResolutionUnit r) {
		if(r == ResolutionUnit::Centimeter)
			out << L"dpcm";
		else if(r == ResolutionUnit::Inch)
			out << L"dpi";
		return out;
	}

	std::basic_ostream<wchar_t>& operator << (std::basic_ostream<wchar_t>& out, const LightSource ls) {
		if(ls == LightSource::Auto)
			out << L"Auto";
		else if(ls == LightSource::Daylight)
			out << L"Daylight";
		else if(ls == LightSource::Fluorescent)
			out << L"Flourescent";
		else if(ls == LightSource::Tungsten)
			out << L"Tungsten";
		else if(ls == LightSource::Flash)
			out << L"Flash";
		else if(ls == LightSource::FineWeather)
			out << L"Fine weather";
		else if(ls == LightSource::CloudyWeather)
			out << L"Cloudy weather";
		else if(ls == LightSource::Shade)
			out << L"Shade";
		else if(ls == LightSource::DaylightFluorescent)
			out << L"Daylight fluorescent (D 5700 - 7100K)";
		else if(ls == LightSource::DayWhiteFluorescent)
			out << L"Day white fluorescent (N 4600 - 5400K)";
		else if(ls == LightSource::CoolWhiteFluorescent)
			out << L"Cool white fluorescent (W 3900 - 4500K)";
		else if(ls == LightSource::WhiteFluorescent)
			out << L"White fluorescent (WW 3200 - 3700K)";
		else if(ls == LightSource::StandardLightA)
			out << L"Standard light A";
		else if(ls == LightSource::StandardLightB)
			out << L"Standard light B";
		else if(ls == LightSource::StandardLightC)
			out << L"Standard light C";
		else if(ls == LightSource::D50)
			out << L"D50";
		else if(ls == LightSource::D55)
			out << L"D55";
		else if(ls == LightSource::D65)
			out << L"D65";
		else if(ls == LightSource::D75)
			out << L"D75";
		else if(ls == LightSource::IsoStudioTungsten)
			out << L"ISO studio tungsten";
		else if(ls == LightSource::Other)
			out << L"Other light source";

		return out;
	}

	std::basic_ostream<wchar_t>& operator << (std::basic_ostream<wchar_t>& out, const ExposureProgram ep) {
		if(ep == ExposureProgram::Action)
			out << L"Action program";
		else if(ep == ExposureProgram::AperturePriority)
			out << L"Aperture priority";
		else if(ep == ExposureProgram::Creative)
			out << L"Creative program";
		else if(ep == ExposureProgram::Landscape)
			out << L"Landscape mode";
		else if(ep == ExposureProgram::Manual)
			out << L"Manual";
		else if(ep == ExposureProgram::Normal)
			out << L"Normal program";
		else if(ep == ExposureProgram::Portrait)
			out << L"Portrait mode";
		else if(ep == ExposureProgram::ShutterPriority)
			out << L"Shutter priority";
		else if (ep == ExposureProgram::Undefined)
			out << L"Undefined";
		return out;
	}

	int Rational::ToInteger() const {
		return Numerator / Denominator;
	}

	float Rational::ToFloat() const {
		return Numerator / static_cast<float>(Denominator);
	}

	Rational::Rational(int num, int denom):Numerator(num), Denominator(denom) {
		if(Denominator == 0 || Numerator == 0)
			return;
		AttemptOptimize(Numerator);
		AttemptOptimize(Denominator);
		for(int i=0;i<10;i++) {
			AttemptOptimize(10);
			AttemptOptimize(2);
		}
	}

	Rational::Rational():Numerator(0), Denominator(0) {}

	void Rational::AttemptOptimize(int divisor) {
		if(Denominator == 0 || divisor == 0)
			return;

		int num_rest = Numerator % divisor;
		int denom_rest = Denominator % divisor;
		if(num_rest == 0 && denom_rest == 0) {
			Numerator = Numerator / divisor;
			Denominator = Denominator / divisor;
		}
	}
}
