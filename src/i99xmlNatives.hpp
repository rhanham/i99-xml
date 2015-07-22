#include <sdk/plugin.h>

namespace i99
{
	cell AMX_NATIVE_CALL n_loadXmlDocFromFile(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL n_unloadXmlDoc(AMX* amx, cell* params);

	cell AMX_NATIVE_CALL n_docFirstNode(AMX* amx, cell* params);

	cell AMX_NATIVE_CALL n_nodeFirstNode(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL n_nodeNextSibling(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL n_nodeFirstAttr(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL n_nodeName(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL n_nodeValue(AMX* amx, cell* params);

	cell AMX_NATIVE_CALL n_attrNextAttr(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL n_attrName(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL n_attrValue(AMX* amx, cell* params);

}
