#pragma once

#include <string>
#include <glad/glad.h>

namespace wen {

std::string readFile(const std::string& filename);

// SrcType -> DstType
template <typename DstType, typename SrcType>
DstType convert(SrcType src);

} // namespace wen