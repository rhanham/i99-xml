#pragma library "i99_xml"


const Doc:invalid_doc = Doc:0;
const Node:invalid_node = Node:-1;
const Attr:invalid_attr = Attr:-1;

native Doc:loadXmlDocFromFile(const filepath[]);
native unloadXmlDoc(Doc:docId);

native Node:docFirstNode(Doc:docId, const filepath[]);

native Node:nodeFirstNode(Node:nodeId, const name[]="");
native Node:nodeNextSibling(Node:nodeId, const name[]="");
native Attr:nodeFirstAttr(Node:nodeId, const name[]="");
native nodeName(Node:nodeId, buffer[], bufsize);

native Attr:attrNextAttr(Attr:attrId, const name[]="");
native attrName(Attr:attrId, buffer[], bufsize);
native attrValue(Attr:attrId, buffer[], bufsize);
