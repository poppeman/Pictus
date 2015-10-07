#ifndef IMAGE_METADATA_DOCUMENT_H
#define IMAGE_METADATA_DOCUMENT_H

#include "Metadata_Types.h"
#include "Metadata_Value.h"
#include <map>
#include <memory>

namespace Metadata {
	class Document {
	public:
		std::map<FieldIdentifier, std::shared_ptr<Value>> Field;
	};

	void Merge(std::shared_ptr<Metadata::Document> lhs, const std::shared_ptr<Metadata::Document> rhs);
}

#endif
