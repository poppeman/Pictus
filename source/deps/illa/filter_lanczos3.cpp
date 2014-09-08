#include "StdAfx.h"
#include "filter_generic.h"

namespace Filter {
	namespace Scale {
		enum {
			NumSamples = 512
		};
		float c_lanczos3[NumSamples];
		bool c_isLanczos3Precalc = false;

		float Sinc(float x)  {
			if (x == 0.0)
				return(1.0);
			return(sin(Num::Pi * x)/(Num::Pi * x));
		}

		template <int _rad>
		float LanczosEval(float x)  {
			if ((x < Num::DEps) && (-Num::DEps < x))
				return 1.0;
			return Sinc(x) * Sinc(x / _rad);
		}

		void PrecalcLanczos3() {
			for(int i = 0; i < NumSamples; ++i) {
				float val = 3.0f * (i - NumSamples / 2) / ((float)NumSamples*0.5f);
				c_lanczos3[i] = LanczosEval<3>(val);
			}
			c_isLanczos3Precalc = true;
		}

		void Lanczos3(const FilterBuffer& source, FilterBuffer& dest, const Geom::RectInt& region, Img::Format format, float zoom) {
			if(c_isLanczos3Precalc == false)
				PrecalcLanczos3();

			Internal::GenericFilter<NumSamples, 3>(source, dest, region, format, zoom, c_lanczos3);
		}
	}
}
