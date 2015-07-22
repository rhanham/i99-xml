#include "i99xmlDoc.hpp"
#include "i99xmlMemory.hpp"
#include "logprintf.hpp"
#include <memory>
#include <vector>
#include <fstream>
#include <cstring>

namespace i99 { 

	//FUNCTIONS
	void deleteDoc(rx::xml_document<>* doc)
	{
		if(doc)
		{
			doc->~xml_document();
			memPoolFree(doc);
		}
	}

	//FUNCTION DEFINITIONS
	cell loadDocFromFile(std::string const& filepath)
	{
		if(filepath.empty()) return -1; //Empty filepath.
		rx::xml_document<>* p_doc = nullptr;
		
		//Create the filestream
		std::ifstream xmlFile(filepath.c_str());
		if(!xmlFile) {
			logprintf("Failed to open file %s.", filepath.c_str());
			return -1;
		}

		try {
			//Create the document object.
			void* p = memPoolAlloc(sizeof(rx::xml_document<>));		
			p_doc = new (p) rx::xml_document<>();	
			p_doc->set_allocator(&memPoolAlloc, &memPoolFree);

			//Get the file size.
			xmlFile.seekg(0, std::ios_base::end);
			std::size_t fileSize = xmlFile.tellg();
			xmlFile.seekg(0, std::ios_base::beg);

			//Allocate memory.
		
			char *docStr = p_doc->allocate_string(0, fileSize+1);
			memset(docStr, 0, fileSize);

			//Read file contents.
			xmlFile.read(docStr, fileSize);
			if(xmlFile.bad())
			{
				deleteDoc(p_doc);
				logprintf("Error reading file");
				return -1; //Failed to read file.
			}

			//Parse the document.
			p_doc->parse<rx::parse_fastest>(docStr);

			//Add the document and return the id.
			cell docId = ptrToCell(p_doc);		
			return docId;

		} catch( rx::parse_error const& ex) {
			deleteDoc(p_doc);
			return -1; //Parse error.
		} catch( std::bad_alloc const& ex) {
			deleteDoc(p_doc);
			logprintf("Failed to allocate memory creating document.");
			throw std::bad_alloc();
			return -1;
		}

	}

	void unloadDoc(cell docId)
	{
		rx::xml_document<>* p_doc = (rx::xml_document<>*)cellToPtr(docId);
		if(!p_doc) return;
		
		deleteDoc(p_doc);
	}

	rx::xml_node<>* docFirstNode(cell docId) {
		rx::xml_document<>* p_doc = (rx::xml_document<>*)cellToPtr(docId);
		if(!p_doc)
			return nullptr;
			
		return p_doc->first_node();
	}

	rx::xml_node<>* docFirstNode(cell docId, std::string const& name) { 
		rx::xml_document<>* p_doc = (rx::xml_document<>*)cellToPtr(docId);
		if(!p_doc)
			return nullptr;
			
		return  p_doc->first_node(name.c_str(), name.size());
	}

}
