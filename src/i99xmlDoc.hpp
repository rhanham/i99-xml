#pragma once

#include "i99xmlMemory.hpp"
#include <string>
#include <sdk/plugin.h>

#define RAPIDXML_STATIC_POOL_SIZE I99XML_BLOCK_SIZE
#define RAPIDXML_DYNMAIC_POOL_SIZE I99XML_BLOCK_SIZE
#include <rapidxml/rapidxml.hpp>

namespace i99
{
	namespace rx = rapidxml;

	cell loadDocFromFile(std::string const& filepath);
	void unloadDoc(cell docId);

	rx::xml_node<>* docFirstNode(cell docId);
	rx::xml_node<>* docFirstNode(cell docId, std::string const& name);

}
