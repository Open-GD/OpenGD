#pragma once
#include <string>
#include <external/json.hpp>
#include <2d/CCNode.h>
#include <base/ccTypes.h>

namespace NodeSerializer
{
	void from_json(const nlohmann::ordered_json&, ax::Node*);
	void to_json(nlohmann::ordered_json&, const ax::Node*);

	void saveNodeToJsonFile(const std::string_view filename, ax::Node*);
	void copyNodeJsonToClipboard(ax::Node*);
};