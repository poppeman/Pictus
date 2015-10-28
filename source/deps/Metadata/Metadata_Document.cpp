#include "Metadata_Document.h"

namespace Metadata {
	void Merge(std::shared_ptr<Document> lhs, const std::shared_ptr<Document> rhs) {
		if(lhs == 0 || rhs == 0)
			return;

		lhs->Field.insert(rhs->Field.begin(), rhs->Field.end());
	}
}
