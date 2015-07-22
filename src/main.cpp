#include <stdio.h>
#include <string.h>
#include <stdexcept>
#include "sdk/plugincommon.h"
#include "sdk/amx/amx.h"
#include "logprintf.hpp"
#include "i99xmlNatives.hpp"
#include "i99xmlMemory.hpp"

typedef void(*logprintf_t)(const char *format, ...);
logprintf_t logprintf  = 0;

extern void *pAMXFunctions;

AMX_NATIVE_INFO g_natives[] = 
{
	{"loadXmlDocFromFile", &i99::n_loadXmlDocFromFile},
	{"unloadXmlDoc", &i99::n_unloadXmlDoc},	
	
	{"docFirstNode", &i99::n_docFirstNode},

	{"nodeFirstNode", &i99::n_nodeFirstNode},
	{"nodeNextSibling", &i99::n_nodeNextSibling},
	{"nodeFirstAttr", &i99::n_nodeFirstAttr},
	{"nodeName", &i99::n_nodeName},

	{"attrNextAttr", &i99::n_attrNextAttr},
	{"attrName", &i99::n_attrName},
	{"attrValue", &i99::n_attrValue},

	
	{0, 0}
};

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports() {
	return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES | SUPPORTS_PROCESS_TICK;
}

PLUGIN_EXPORT unsigned int PLUGIN_CALL Load(void **ppPluginData)
{
	pAMXFunctions = ppPluginData[PLUGIN_DATA_AMX_EXPORTS];	
	logprintf = (logprintf_t)ppPluginData[PLUGIN_DATA_LOGPRINTF];	
	logprintf("\n\n*** i99 XML Plugin loaded ***\n");
	logprintf("Heap size: %d, Block size: %d", I99XML_HEAP_SIZE, I99XML_BLOCK_SIZE);
	
	try
	{		
		i99::memPoolInit();		
	}

	catch(std::runtime_error const& ex) { 
		logprintf("Plugin Load Runtime exception: %s", ex.what());
		return false;
	}

		
	return true;
}

PLUGIN_EXPORT void PLUGIN_CALL Unload() { 
	i99::memPoolCleanup();
	return; 
}

PLUGIN_EXPORT int PLUGIN_CALL AmxLoad(AMX* amx)
{
	int result = amx_Register(amx, g_natives, -1);	
	return result;
}

PLUGIN_EXPORT int PLUGIN_CALL AMXUnload(AMX* amx) { return AMX_ERR_NONE; }

PLUGIN_EXPORT void PLUGIN_CALL ProcessTick() { return; }


