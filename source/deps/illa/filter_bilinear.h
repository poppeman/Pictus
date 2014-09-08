#ifndef FILTER_BILINEAR
#define FILTER_BILINEAR

namespace Filter {
	namespace Scale {
		namespace Internal {
			struct Contrib {
				uint32_t				floor;
				uint32_t				frac;
				uint32_t				frac_inv;
			};

			void set_contrib(std::vector<Contrib>& contrib, uint32_t i, uint32_t max_i, uint32_t max_coord, uint32_t ofs);
		}
	}
}

#endif
