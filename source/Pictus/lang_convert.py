# Python 3 only
# Quick ugly hack. Reads lang.json and spits out relevant C++ code.

import json
import codecs

def addLang(state, langName):
    for index, lang in enumerate(state["langs"]):
        if langName == state["langs"][index]:
            return index
    state["langs"].append(langName)
    for index, val in enumerate(state["vals"]):
        if len(state["vals"][index]) < len(state["langs"]):
            state["vals"][index].append("")
    return len(state["langs"]) - 1

def addEnum(state):
    for index, currEnum in enumerate(state["enums"]):
        if currEnum == state["path"]:
            return index
    state["enums"].append(state["path"])
    state["vals"].append([""] * len(state["langs"]))

    return len(state["enums"]) - 1


def parseKey(data, state):
    if state == None:
        state = {"index":0, "langs":[], "path": "", "enums": [], "vals": []}

    for key in data:
        typ = type(data[key])
        if(typ == dict):
            tmpState = state.copy()
            tmpState["path"] += key
            ret = parseKey(data[key], tmpState)
            state["langs"] = ret["langs"]
            state["enums"] = ret["enums"]
            state["vals"] = ret["vals"]
        elif(typ == str):
            valIdx = addEnum(state)
            langIdx = addLang(state, key)
            state["vals"][valIdx][langIdx] = data[key]
        else:
            raise ValueError("Unknown type " + typ)
    return state

def stringEncode(x):
    return 'u8"' + x.replace("\\", "\\\\").replace("\n", "\\n").replace("\"", "\\\"") + '"'

with codecs.open('Res/lang.json', mode = 'r', encoding='utf-8') as phil:
    data = json.load(phil)

flattened = parseKey(data, None)

with open("../deps/orz/intl_language.h", mode="w") as langf:
    langf.write("#ifndef ORZ_INTL_LANGUAGE_H\n")
    langf.write("#define ORZ_INTL_LANGUAGE_H\n\n")
    langf.write("namespace Intl {\n")
    langf.write("	enum class Language{\n")
    for idx, enum in enumerate(flattened["langs"]):
        langf.write("		" + enum + " = " + str(idx) + ",\n")
    langf.write("		Undefined = " + str(len(flattened["langs"])) + ",\n")
    langf.write("	};\n")
    langf.write("}\n\n")
    langf.write("#endif\n")

with open('lang_enum.h', 'w') as enf:
    enf.write("enum StringID {\n")
    for idx, enum in enumerate(flattened["enums"]):
        enf.write("	SID" + enum + " = " + str(idx) + ",\n")
    enf.write("};\n")

with open('lang_val.h', mode = 'w') as valf:

    valf.write("extern Intl::TableEntry c_lang_strings[];");


# Python doesn't like writing BOMs, so we'll do that manually.
with open('lang_val.cpp', mode = 'wb') as valf:
    valf.write(codecs.BOM_UTF8)

with codecs.open('lang_val.cpp', mode = 'a', encoding='utf-8') as valf:
    valf.write('#include "../deps/orz/intl_table_entry.h"\n')
    valf.write("Intl::TableEntry c_lang_strings[] = {\n")
    for idx, lang in enumerate(flattened["vals"]):
        cstr = u"	{"
        cstr += ", ".join(map(stringEncode, flattened["vals"][idx]))
        #for jdx, txt in enumerate(flattened["vals"][idx]):
        #	cstr += u'L"' + flattened["vals"][idx][jdx] + u'", '
        cstr += u"},\n"
        valf.write(cstr)
    valf.write("};\n")
