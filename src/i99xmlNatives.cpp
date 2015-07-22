#include "i99xmlNatives.hpp"
#include "i99xmlDoc.hpp"
#include "i99xmlMemory.hpp"
#include "logprintf.hpp"
#include <string>
#include <cstring>
#include <algorithm>

namespace i99 {

	//FUNCTIONS
	std::string getString(AMX* amx, cell idx)
	{
		cell *p_strAddr = nullptr;
		int strLen = 0;
		amx_GetAddr(amx, idx, &p_strAddr);
		amx_StrLen(p_strAddr, &strLen);

		//Copy into the string.
		char data[strLen+1];
		memset(data, 0, strLen+1);
		amx_GetString(data, p_strAddr, 0, strLen+1);

		return std::string(data);
	}

	void setString(AMX* amx, std::string const& str, cell idx, std::size_t size)
	{
		//Get the address.
		cell *p_strAddr = nullptr;
		amx_GetAddr(amx, idx, &p_strAddr);

		//Copy the string.
		size = std::min(str.size()+1, size);
		amx_SetString(p_strAddr, str.c_str(), 0, 0, size);
	}

	/**************************************
					NATIVES
	**************************************/

	/**
		native doc:loadXmlDocFromFile(filename);

		Loads the document object and returns the id.
		Returns 0 if loading the doucment failed.
	*/
	cell AMX_NATIVE_CALL n_loadXmlDocFromFile(AMX* amx, cell* params)
	{
		#if defined(DEBUG)
		logprintf("Native n_loadXmlDocFromFile() called");
		#endif
		

		//Param check.
		if(params[0] != 4)
			return 0;

		cell filepathIdx = params[1];

		//Get the filename.
		std::string filepath = getString(amx, filepathIdx);
		
		//Load the file.
		return loadDocFromFile(filepath);
	}

	/**
		native unloadXmDoc(doc:docId);

		Unloads the document. All nodes and attributes belonging to this document
		will become invalid.

		Returns no particular value.
	*/
	cell AMX_NATIVE_CALL n_unloadXmlDoc(AMX* amx, cell* params)
	{
		#if defined(DEBUG)
		logprintf("Native n_unloadXmlDoc() called");
		#endif
		if(params[0] != 4)
			return 0;

		unloadDoc(params[1]);
		return 1;
	}

	/**
		native node:docFirstNode(doc:docId, nodeName[]);
	
		Returns a pointer to the first node in the document.
	*/
	cell AMX_NATIVE_CALL n_docFirstNode(AMX* amx, cell* params)
	{
		#if defined(DEBUG)
		logprintf("Native n_docFirstNode() called");
		#endif
		if(params[0] != 8)
			return 0;

		cell docId = params[1];
		cell nameIdx = params[2];

		//Get the name.
		std::string name = getString(amx, nameIdx);

		//Get the first node.
		rx::xml_node<>* p_node = nullptr;
		if(name.size() > 0)
			p_node = docFirstNode(docId);
		else
			p_node = docFirstNode(docId, name);

		//Convert the pointer
		cell c = ptrToCell(p_node);
		return c;
	}

	/**
		native nodeFirstNode(node:nodeId, name[])

		Obtain the first child node.
		Returns the address of the child node.
	*/
	cell AMX_NATIVE_CALL n_nodeFirstNode(AMX* amx, cell* params)
	{
		#if defined(DEBUG)
		logprintf("Native nodeFirstNode() called");
		#endif

		if(params[0] != 8)
			return -1;


		cell nodeId = params[1];
		cell nameIdx = params[2];

		//Get the node
		rx::xml_node<>* p_node = (rx::xml_node<>*)cellToPtr(nodeId);
		rx::xml_node<>* p_first = nullptr;

		if(!p_node)
			return -1;
	
		//Get the node name.
		std::string name = getString(amx, nameIdx);

		//Find the first node
		if(name.size() > 0)
			p_first = p_node->first_node(name.c_str());
		else
			p_first = p_node->first_node();

		
		return ptrToCell(p_first);
	}

	/**
		native nodeNextSibling(node:nodeId, name)
		
		Gets the next child node.
		Returns the address of the sibling node.
	*/

	cell AMX_NATIVE_CALL n_nodeNextSibling(AMX* amx, cell* params)
	{
		#if defined(DEBUG)
		logprintf("Native n_nodeNextSibling() called");
		#endif

		if(params[0] != 8)
			return -1;


		cell nodeId = params[1];
		cell nameIdx = params[2];

		//Get the node
		rx::xml_node<>* p_node = (rx::xml_node<>*)cellToPtr(nodeId);
		rx::xml_node<>* p_sibling = nullptr;
		
		if(!p_node)
			return -1;
		//Get the node name.
		std::string name = getString(amx, nameIdx);

		//Find the first node
		if(name.size() > 0)
			p_sibling = p_node->next_sibling(name.c_str(), name.size());
		else
			p_sibling = p_node->next_sibling();

		
		return ptrToCell(p_sibling);
	}

	/**
		native attr:nodeFirstAttr(node:nodeId, name[])
		
		Get the nodes first attribute.
		Returns the address of the first attribute.
	*/
	cell AMX_NATIVE_CALL n_nodeFirstAttr(AMX* amx, cell* params)
	{
		#if defined(DEBUG)
		logprintf("Native n_nodeFirstAttr() called");
		#endif
		if(params[0] != 8)
			return ptrToCell(nullptr);

		//Get the params
		cell nodeId = params[1];
		cell nameIdx = params[2];

		//Get the node
		rx::xml_node<>* p_node = (rx::xml_node<>*)cellToPtr(nodeId);
		rx::xml_attribute<>* p_attr = nullptr;
		if(!p_node)
			return ptrToCell(nullptr);

		//Get the attribute name.
		std::string name = getString(amx, nameIdx);
		if(name.size() > 0)
			p_attr = p_node->first_attribute(name.c_str());
		else
			p_attr = p_node->first_attribute();
		
		return ptrToCell(p_attr);
	}
	

	/**
		native nodeName(node:nodeId, buffer[], bufsize)

		Gets the name of the node. If nodeId is invalid the buffer is unchanged.
		Returns no particular value.
	*/
	cell AMX_NATIVE_CALL n_nodeName(AMX* amx, cell* params)
	{
		#if defined(DEBUG)
		logprintf("Native n_nodeName() called");
		#endif
		if(params[0] != 12)
			return 0;

		//Get params.
		cell nodeId = params[1];
		cell bufferId = params[2];
		cell bufSize = params[3];

		//Convert to pointer.
		rx::xml_node<>* p_node = (rx::xml_node<>*)cellToPtr(nodeId);

		//Get the name.
		std::string name(p_node->name(), p_node->name_size());
		setString(amx, name, bufferId, bufSize);
		return 1;
	}


	/**
		native attrNextAttr(node:nodeId, name[])
	*/
	cell AMX_NATIVE_CALL n_attrNextAttr(AMX* amx, cell* params)
	{
		#if defined(DEBUG)
		logprintf("Native n_attrNextAttr() called");
		#endif
		if(params[0] != 8)
			return ptrToCell(nullptr);

		//Get params
		cell attrId = params[1];
		cell nameIdx = params[2];


		//Get the attribute
		rx::xml_attribute<>* p_attr = (rx::xml_attribute<>*)cellToPtr(attrId);
		rx::xml_attribute<>* p_next = nullptr;

		if(!p_attr)
			return ptrToCell(nullptr);

		//Get the name
		std::string name = getString(amx, nameIdx);
		if(name.size() > 0)
			p_next = p_attr->next_attribute();
		else
			p_next = p_attr->next_attribute();
		
		return ptrToCell(p_next);
	}

	/**
		native attrName(attr:attrId, buffer[], bufsize)
		
		Gets the attribute name and stores it inside buffer.
		Returns no particular value.
	*/
	cell AMX_NATIVE_CALL n_attrName(AMX* amx, cell* params)
	{
		#if defined(DEBUG)
		logprintf("Native n_attrName() called");
		#endif

		if(params[0] != 12)
			return 0;

		//Get params
		cell attrId = params[1];
		cell bufferIdx = params[2];
		cell bufSize = params[3];

		rx::xml_attribute<>* p_attr = (rx::xml_attribute<>*)cellToPtr(attrId);
		if(!p_attr)
			return 0;

		//Get the name
		std::string str(p_attr->name(), p_attr->name_size());
		setString( amx, str, bufferIdx, bufSize );

		return 1;	
	}

	/**
		native attrValue(attr:attrId, buffer[], bufsize)

		Gets the value of the attribute. Stores it inside buffer.
		Returns no particular value.
	*/

	cell AMX_NATIVE_CALL n_attrValue(AMX* amx, cell* params)
	{
		#if defined(DEBUG)
		logprintf("Native n_attrValue() called");
		#endif

		if(params[0] != 12)
			return 0;

		
		cell attrId = params[1];
		cell bufferIdx = params[2];
		cell bufSize = params[3];

		//Get attr
		rx::xml_attribute<>* p_attr = (rx::xml_attribute<>*)cellToPtr(attrId);
		if(!p_attr)
			return 0;

		//Get name
		std::string str(p_attr->value(), p_attr->value_size());
		setString(amx, str, bufferIdx, bufSize);
		return 1;
	}
}
