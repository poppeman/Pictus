#ifndef IMAGE_METADATA_VALUE_H
#define IMAGE_METADATA_VALUE_H

#include "Metadata_Types.h"
#include <stdexcept>
#include <iostream>
#include <vector>
#include <sstream>

namespace Metadata {
	class Value {
	public:
		// UTF-8
		virtual std::string ToString()=0;
		virtual int ToInteger() { throw std::domain_error("ToInteger conversion not defined"); }
	};

	class Rational {
	public:
		int Numerator, Denominator;
		int ToInteger() const;
		float ToFloat() const;

		Rational(int num, int denom);
		Rational();

	private:
		void AttemptOptimize(int divisor);
	};

	std::basic_ostream<char>& operator << (std::basic_ostream<char>& out, const Rational& r);
	std::basic_ostream<char>& operator << (std::basic_ostream<char>& out, const ResolutionUnit r);
	std::basic_ostream<char>& operator << (std::basic_ostream<char>& out, const LightSource ls);
	std::basic_ostream<char>& operator << (std::basic_ostream<char>& out, const ExposureProgram ep);
	
	template <typename T>
	class SimpleValue:public Value {
	public:
		std::string ToString() {
			std::stringstream ss;
			ss << m_val;
			return ss.str();
		}

		int ToInteger() {
			std::stringstream ss;
			ss << m_val;
			int out;
			ss >> out;
			return out;
		}

		explicit SimpleValue(T val):m_val(val) {}

	protected:
		T m_val;
	};

	template <typename T>
	class SimpleUnitValue:public SimpleValue<T> {
	public:
		virtual std::string ToString() {
			std::stringstream ss;
			ss << m_prefix << m_val << " " << m_unit;
			return ss.str();
		}

		explicit SimpleUnitValue(T val, std::string unit):SimpleValue<T>(val), m_unit(unit) {}
		explicit SimpleUnitValue(T val, std::string prefix, std::string unit):SimpleValue<T>(val), m_prefix(prefix), m_unit(unit) {}

	protected:
		std::string m_prefix;
		std::string m_unit;
	};

	template <typename T>
	class ResolutionAxisValue:public SimpleValue<T> {
	public:
		std::string ToString() {
			std::stringstream ss;
			ss << m_val << " " << m_unit;
			return ss.str();
		}

		explicit ResolutionAxisValue(T val, ResolutionUnit unit):SimpleValue<T>(val), m_unit(unit) {}

	private:
		ResolutionUnit m_unit;
	};

	enum class GeographicalAxis {
		Latitude,
		Longitude
	};

	template <typename T>
	class ArrayValue:public Value {
	public:
		std::string ToString() {
			std::stringstream ss;
			for(auto item=m_items.begin(); item != m_items.end(); ++item) {
				if(item != m_items.begin()) {
					ss << ", ";
				}
				ss << *item;
			}
			return ss.str();
		}

		void Add(const T& item) {
			m_items.push_back(item);
		}

		const std::vector<T> Items() const {
			return m_items;
		}

		explicit ArrayValue() {}

	protected:
		std::vector<T> m_items;
	};

	class GeographicalCoordinateValue:public SimpleValue<double> {
	public:
		enum class Format {
			Default,
			Decimal
		};

		std::string ToString() {
			return ToString(Format::Default);
		}

		std::string ToString(Format fmt) {
			std::stringstream ss;
			if(fmt == Format::Default) {
				double deg, frac;
				frac = std::modf(fabs(m_val), &deg);
				ss << deg << static_cast<unsigned char>(0xc2) << static_cast<unsigned char>(0xb0);
				double sec, min;
				sec = std::modf(frac * 60.0, &min);
				ss << min << "'" << sec << " " << Unit();
			}
			else {
				ss << m_val;
			}
			return ss.str();
		}

		std::string Unit() const {
			if(m_axis == GeographicalAxis::Latitude) {
				if(m_val > 0)
					return "N";
				return "S";
			}
			if(m_val > 0)
				return "E";
			return "W";
		}

		explicit GeographicalCoordinateValue(double val, GeographicalAxis axis):SimpleValue<double>(val), m_axis(axis) {}

	private:
		GeographicalAxis m_axis;
	};
}

#endif
