#include "FunctionBinding.h"
#include "Generator.h"


// Helper functions.
static inline void outputLuaTypeCheck(ostream& o, int index, const FunctionBinding::Param& p = 
    FunctionBinding::Param(FunctionBinding::Param::TYPE_OBJECT, FunctionBinding::Param::KIND_POINTER));
static inline void indent(ostream& o, int indentLevel);
static inline void outputBindingInvocation(ostream& o, const FunctionBinding& b, unsigned int paramCount, unsigned int indentLevel);
static inline void outputGetParam(ostream& o, const FunctionBinding::Param& p, int i, int indentLevel, bool offsetIndex = false);
static inline void outputMatchedBinding(ostream& o, const FunctionBinding& b, unsigned int paramCount, unsigned int indentLevel);
static inline void outputReturnValue(ostream& o, const FunctionBinding& b, int indentLevel);


FunctionBinding::Param::Param(FunctionBinding::Param::Type type, Kind kind, const string& info) : 
    type(type), kind(kind), info(info), hasDefaultValue(false)
{
}

bool FunctionBinding::Param::operator==(const Param& p) const
{
    return this->type == p.type && this->kind == p.kind && this->info == p.info;
}

bool FunctionBinding::Param::operator!=(const Param& p) const
{
    return !(*this == p);
}

FunctionBinding::FunctionBinding(string classname, string uniquename) : classname(classname), uniquename(uniquename), name(""), own(false)
{
}

string FunctionBinding::getFunctionName() const
{
    if (functionName.length() == 0)
    {
        functionName = "lua_";
        functionName += uniquename;
    
        if (type == STATIC_FUNCTION ||
            type == STATIC_CONSTANT ||
            type == STATIC_VARIABLE)
            functionName += "_static";
        functionName += "_";

        if (returnParam.type == Param::TYPE_CONSTRUCTOR)
            functionName += "_init";
        else if (returnParam.type == Param::TYPE_DESTRUCTOR)
            functionName += "_gc";
        else
            functionName += name;
    }

    return functionName;
}

unsigned int FunctionBinding::getMinParamCount() const
{
    for (unsigned int i = 0; i < paramTypes.size(); i++)
    {
        if (paramTypes[i].hasDefaultValue)
            return i;
    }
    return paramTypes.size();
}

void FunctionBinding::write(ostream& o, const vector<FunctionBinding>& bindings)
{
    GP_ASSERT(bindings.size() > 0);

    // Print the function signature.
    o << "int " << bindings[0].getFunctionName() << "(lua_State* state)\n";
    o << "{\n";

    if (bindings.size() == 1 && bindings[0].type == FunctionBinding::MEMBER_VARIABLE)
    {
        // Validate the parameter count.
        o << "    // Validate the number of parameters.\n";
        o << "    if (lua_gettop(state) > 2)\n";
        o << "    {\n";
        o << "        lua_pushstring(state, \"Invalid number of parameters (expected 1 or 2).\");\n";
        o << "        lua_error(state);\n";
        o << "    }\n\n";
        
        // Get or set the member variable depending on the number of parameters.
        o << "    " << bindings[0].classname << "* instance = getInstance(state);\n";
        o << "    if (lua_gettop(state) == 2)\n";
        o << "    {\n";
        outputGetParam(o, bindings[0].returnParam, 1, 2);

        if (bindings[0].returnParam.kind == FunctionBinding::Param::KIND_POINTER &&
            bindings[0].returnParam.type != FunctionBinding::Param::TYPE_OBJECT &&
            bindings[0].returnParam.info.size() > 0)
        {
            o << "        memcpy(instance->" << bindings[0].name << ", param2, sizeof(";
            o << FunctionBinding::Param(bindings[0].returnParam.type) << ") * " << bindings[0].returnParam.info << ");\n";
        }
        else
        {
            o << "        instance->" << bindings[0].name << " = ";
            if (bindings[0].returnParam.type == FunctionBinding::Param::TYPE_OBJECT &&
                bindings[0].returnParam.kind != FunctionBinding::Param::KIND_POINTER)
            {
                o << "*";
            }
            o << "param2;\n";
        }

        o << "        return 0;\n";
        o << "    }\n";
        o << "    else\n";
        o << "    {\n";
        
        // Pass the return value back to Lua.
        if (bindings[0].returnParam.type == FunctionBinding::Param::TYPE_OBJECT)
        {
            switch (bindings[0].returnParam.kind)
            {
            case FunctionBinding::Param::KIND_POINTER:
                o << "        void* returnPtr = (void*)instance->" << bindings[0].name << ";\n";
                break;
            case FunctionBinding::Param::KIND_VALUE:
                o << "        void* returnPtr = (void*)new " << bindings[0].returnParam << "(instance->" << bindings[0].name << ");\n";
                break;
            case FunctionBinding::Param::KIND_REFERENCE:
                o << "        void* returnPtr = (void*)&(instance->" << bindings[0].name << ");\n";
                break;
            default:
                GP_ERROR("Invalid return value kind '%d'.", bindings[0].returnParam.kind);
                break;
            }
        }
        else
        {
            o << "        " << bindings[0].returnParam << " result = instance->" << bindings[0].name << ";\n";
        }
        outputReturnValue(o, bindings[0], 2);
        o << "    }\n";
        o << "}\n\n";
    }
    else if (bindings.size() == 1 && 
        (bindings[0].type == FunctionBinding::STATIC_VARIABLE ||
        bindings[0].type == FunctionBinding::GLOBAL_VARIABLE))
    {
        // Validate the parameter count.
        o << "    // Validate the number of parameters.\n";
        o << "    if (lua_gettop(state) > 1)\n";
        o << "    {\n";
        o << "        lua_pushstring(state, \"Invalid number of parameters (expected 0 or 1).\");\n";
        o << "        lua_error(state);\n";
        o << "    }\n\n";
        
        // Get or set the static variable depending on the number of parameters.
        o << "    if (lua_gettop(state) == 1)\n";
        o << "    {\n";
        outputGetParam(o, bindings[0].returnParam, 0, 2);

        if (bindings[0].returnParam.kind == FunctionBinding::Param::KIND_POINTER &&
            bindings[0].returnParam.type != FunctionBinding::Param::TYPE_OBJECT &&
            bindings[0].returnParam.info.size() > 0)
        {
            o << "        memcpy(";
            if (bindings[0].classname.size() > 0)
                o << bindings[0].classname << "::";
            o << bindings[0].name << ", param1, sizeof(";
            o << FunctionBinding::Param(bindings[0].returnParam.type) << ") * " << bindings[0].returnParam.info << ");\n";
        }
        else
        {
            o << "        ";
            if (bindings[0].classname.size() > 0)
                o << bindings[0].classname << "::";
            o << bindings[0].name << " = ";
            if (bindings[0].returnParam.type == FunctionBinding::Param::TYPE_OBJECT &&
                bindings[0].returnParam.kind != FunctionBinding::Param::KIND_POINTER)
            {
                o << "*";
            }
            o << "param1;\n";
        }
        o << "        return 0;\n";
        o << "    }\n";
        o << "    else\n";
        o << "    {\n";
        
        // Pass the return value back to Lua.
        if (bindings[0].returnParam.type == FunctionBinding::Param::TYPE_OBJECT)
        {
            switch (bindings[0].returnParam.kind)
            {
            case FunctionBinding::Param::KIND_POINTER:
                o << "        void* returnPtr = (void*)";
                if (bindings[0].classname.size() > 0)
                    o << bindings[0].classname << "::";
                o << bindings[0].name << ";\n";
                break;
            case FunctionBinding::Param::KIND_VALUE:
                o << "        void* returnPtr = (void*)new " << bindings[0].returnParam << "(";
                if (bindings[0].classname.size() > 0)
                    o << bindings[0].classname << "::";
                o << bindings[0].name << ");\n";
                break;
            case FunctionBinding::Param::KIND_REFERENCE:
                o << "        void* returnPtr = (void*)&(";
                if (bindings[0].classname.size() > 0)
                    o << bindings[0].classname << "::";
                o << bindings[0].name << ");\n";
                break;
            default:
                GP_ERROR("Invalid return value kind '%d'.", bindings[0].returnParam.kind);
                break;
            }
        }
        else
        {
            o << "        " << bindings[0].returnParam << " result = ";
            if (bindings[0].classname.size() > 0)
                o << bindings[0].classname << "::";
            o << bindings[0].name << ";\n";
        }
        outputReturnValue(o, bindings[0], 2);
        o << "    }\n";
        o << "}\n\n";
    }
    else if (bindings.size() == 1 && bindings[0].type == FunctionBinding::MEMBER_CONSTANT)
    {
        // Validate the parameter count.
        o << "    // Validate the number of parameters.\n";
        o << "    if (lua_gettop(state) > 1)\n";
        o << "    {\n";
        o << "        lua_pushstring(state, \"Invalid number of parameters (expected 1).\");\n";
        o << "        lua_error(state);\n";
        o << "    }\n\n";
        
        // Pass the return value back to Lua.
        o << "    " << bindings[0].classname << "* instance = getInstance(state);\n";
        if (bindings[0].returnParam.type == FunctionBinding::Param::TYPE_OBJECT)
        {
            switch (bindings[0].returnParam.kind)
            {
            case FunctionBinding::Param::KIND_POINTER:
                o << "    void* returnPtr = (void*)instance->" << bindings[0].name << ";\n";
                break;
            case FunctionBinding::Param::KIND_VALUE:
                o << "    void* returnPtr = (void*)new " << bindings[0].returnParam << "(instance->" << bindings[0].name << ");\n";
                break;
            case FunctionBinding::Param::KIND_REFERENCE:
                o << "    void* returnPtr = (void*)&(instance->" << bindings[0].name << ");\n";
                break;
            default:
                GP_ERROR("Invalid return value kind '%d'.", bindings[0].returnParam.kind);
                break;
            }
        }
        else
        {
            o << "    " << bindings[0].returnParam << " result = instance->" << bindings[0].name << ";\n";
        }
        outputReturnValue(o, bindings[0], 1);
        o << "}\n\n";
    }
    else if (bindings.size() == 1 && 
        (bindings[0].type == FunctionBinding::STATIC_CONSTANT ||
        bindings[0].type == FunctionBinding::GLOBAL_CONSTANT))
    {
        // Validate the parameter count.
        o << "    // Validate the number of parameters.\n";
        o << "    if (lua_gettop(state) > 0)\n";
        o << "    {\n";
        o << "        lua_pushstring(state, \"Invalid number of parameters (expected 0).\");\n";
        o << "        lua_error(state);\n";
        o << "    }\n\n";
        
        // Pass the return value back to Lua.
        if (bindings[0].returnParam.type == FunctionBinding::Param::TYPE_OBJECT)
        {
            switch (bindings[0].returnParam.kind)
            {
            case FunctionBinding::Param::KIND_POINTER:
                o << "    void* returnPtr = (void*)";
                if (bindings[0].classname.size() > 0)
                    o << bindings[0].classname << "::";
                o << bindings[0].name << ";\n";
                break;
            case FunctionBinding::Param::KIND_VALUE:
                o << "    void* returnPtr = (void*)new " << bindings[0].returnParam << "(";
                if (bindings[0].classname.size() > 0)
                    o << bindings[0].classname << "::";
                o << bindings[0].name << ");\n";
                break;
            case FunctionBinding::Param::KIND_REFERENCE:
                o << "    void* returnPtr = (void*)&(";
                if (bindings[0].classname.size() > 0)
                    o << bindings[0].classname << "::";
                o << bindings[0].name << ");\n";
                break;
            default:
                GP_ERROR("Invalid return value kind '%d'.", bindings[0].returnParam.kind);
                break;
            }
        }
        else
        {
            o << "    " << bindings[0].returnParam << " result = ";
            if (bindings[0].classname.size() > 0)
                o << bindings[0].classname << "::";
            o << bindings[0].name << ";\n";
        }
        outputReturnValue(o, bindings[0], 1);
        o << "}\n\n";
    }
    else
    {
        // Get all valid parameter counts.
        unsigned int paramCountOffset;
        map<unsigned int, vector<const FunctionBinding*> > paramCounts;
        for (unsigned int i = 0, count = bindings.size(); i < count; i++)
        {
            unsigned int minParamCount = bindings[i].getMinParamCount();
            paramCountOffset = (bindings[i].type == FunctionBinding::MEMBER_FUNCTION && bindings[i].returnParam.type != FunctionBinding::Param::TYPE_CONSTRUCTOR) ? 1 : 0;
            paramCounts[minParamCount + paramCountOffset].push_back(&bindings[i]);

            if (minParamCount < bindings[i].paramTypes.size())
            {
                for (unsigned int c = minParamCount + 1; c <= bindings[i].paramTypes.size(); c++)
                {
                    paramCounts[c + paramCountOffset].push_back(&bindings[i]);
                }
            }
        }
        
        // Get the parameter count.
        o << "    // Get the number of parameters.\n";
        o << "    int paramCount = lua_gettop(state);\n\n";
        
        // Retrieve all the parameters and attempt to match them to a valid binding,
        // notifying the user if the number of parameters is invalid.
        o << "    // Attempt to match the parameters to a valid binding.\n";
        o << "    switch (paramCount)\n";
        o << "    {\n";
        map<unsigned int, vector<const FunctionBinding*> >::iterator iter;
        unsigned int checkCount = 0;
        for (iter = paramCounts.begin(); iter != paramCounts.end(); iter++)
        {
            o << "        case " << iter->first << ":\n";
            o << "        {\n";

            for (unsigned int i = 0, count = iter->second.size(); i < count; i++)
            {
                // Only indent if there are parameters.
                if (iter->first > 0)
                    indent(o, 3);

                if (i > 0)
                {                    
                    o << "else ";
                }
                outputMatchedBinding(o, *(iter->second[i]), iter->first, 3);
            }

            // Only print an else clause with error report if there are parameters.
            if (iter->first > 0)
            {
                indent(o, 3);
                o << "else\n";
                indent(o, 3);
                o << "{\n";
                indent(o, 4);
                o << "lua_pushstring(state, \"Failed to match the given parameters to a valid function signature.\");\n";
                indent(o, 4);
                o << "lua_error(state);\n";
                indent(o, 3);
                o << "}\n";
            }
            
            o << "            break;\n";
            o << "        }\n";
        }
        
        o << "        default:\n";
        o << "        {\n";
        o << "            lua_pushstring(state, \"Invalid number of parameters (expected "; 
        for (iter = paramCounts.begin(), checkCount = 1; iter != paramCounts.end(); iter++)
        {
            if (checkCount == paramCounts.size() && paramCounts.size() > 1)
                o << " or ";
            o << iter->first;
            checkCount++;

            if (checkCount < paramCounts.size())
                o << ", ";
        }
        o << ").\");\n";
        o << "            lua_error(state);\n";
        o << "            break;\n";
        o << "        }\n";
        o << "    }\n";
        o << "    return 0;\n";
        o << "}\n\n";
    }
}

bool FunctionBinding::signaturesMatch(const FunctionBinding& b1, const FunctionBinding& b2)
{
    if (b1.name == b2.name &&
        b1.type == b2.type)
    {
        if (b1.paramTypes.size() != b2.paramTypes.size())
            return false;

        for (unsigned int i = 0, count = b1.paramTypes.size(); i < count; i++)
        {
            if (b1.paramTypes[i] != b2.paramTypes[i])
                return false;
        }

        return true;
    }
    return false;
}

ostream& operator<<(ostream& o, const FunctionBinding::Param& param)
{
    switch (param.type)
    {
    case FunctionBinding::Param::TYPE_VOID:
        o << "void";
        break;
    case FunctionBinding::Param::TYPE_BOOL:
        o << "bool";
        break;
    case FunctionBinding::Param::TYPE_CHAR:
        o << "char";
        break;
    case FunctionBinding::Param::TYPE_SHORT:
        o << "short";
        break;
    case FunctionBinding::Param::TYPE_INT:
        o << "int";
        break;
    case FunctionBinding::Param::TYPE_LONG:
        o << "long";
        break;
    case FunctionBinding::Param::TYPE_UCHAR:
        o << "unsigned char";
        break;
    case FunctionBinding::Param::TYPE_USHORT:
        o << "unsigned short";
        break;
    case FunctionBinding::Param::TYPE_UINT:
        o << "unsigned int";
        break;
    case FunctionBinding::Param::TYPE_ULONG:
        o << "unsigned long";
        break;
    case FunctionBinding::Param::TYPE_FLOAT:
        o << "float";
        break;
    case FunctionBinding::Param::TYPE_DOUBLE:
        o << "double";
        break;
    case FunctionBinding::Param::TYPE_ENUM:
        o << Generator::getInstance()->getIdentifier(param.info);
        break;
    case FunctionBinding::Param::TYPE_STRING:
        if (param.info == "string")
            o << "std::string";
        else
            o << "const char";
        break;
    case FunctionBinding::Param::TYPE_OBJECT:
    case FunctionBinding::Param::TYPE_CONSTRUCTOR:
        o << Generator::getInstance()->getIdentifier(param.info);
        break;
    case FunctionBinding::Param::TYPE_UNRECOGNIZED:
        o << param.info;
        break;
    case FunctionBinding::Param::TYPE_DESTRUCTOR:
    default:
        break;
    }

    if (param.kind == FunctionBinding::Param::KIND_POINTER)
        o << "*";

    return o;
}

// ---------------------------------------------
// Helper functions

static inline void outputLuaTypeCheck(ostream& o, int index, const FunctionBinding::Param& p)
{
    switch (p.type)
    {
    case FunctionBinding::Param::TYPE_BOOL:
        if (p.kind == FunctionBinding::Param::KIND_POINTER) 
        {
            o << "(lua_type(state, " << index << ") == LUA_TTABLE || ";
            o << "lua_type(state, " << index << ") == LUA_TLIGHTUSERDATA)";
        }
        else
        {
            o << "lua_type(state, " << index << ") == LUA_TBOOLEAN";
        }
        break;
    case FunctionBinding::Param::TYPE_CHAR:
    case FunctionBinding::Param::TYPE_SHORT:
    case FunctionBinding::Param::TYPE_INT:
    case FunctionBinding::Param::TYPE_LONG:
    case FunctionBinding::Param::TYPE_UCHAR:
    case FunctionBinding::Param::TYPE_USHORT:
    case FunctionBinding::Param::TYPE_UINT:
    case FunctionBinding::Param::TYPE_ULONG:
    case FunctionBinding::Param::TYPE_FLOAT:
    case FunctionBinding::Param::TYPE_DOUBLE:
        if (p.kind == FunctionBinding::Param::KIND_POINTER) 
        {
            o << "(lua_type(state, " << index << ") == LUA_TTABLE || ";
            o << "lua_type(state, " << index << ") == LUA_TLIGHTUSERDATA)";
        }
        else
        {
            o << "lua_type(state, " << index << ") == LUA_TNUMBER";
        }
        break;
    case FunctionBinding::Param::TYPE_STRING:
    case FunctionBinding::Param::TYPE_ENUM:
        o << "(lua_type(state, " << index << ") == LUA_TSTRING || ";
        o << "lua_type(state, " << index << ") == LUA_TNIL)";
        break;
    case FunctionBinding::Param::TYPE_OBJECT:
        o << "(lua_type(state, " << index << ") == LUA_TUSERDATA || ";
        o << "lua_type(state, " << index << ") == LUA_TNIL)";
        break;
    case FunctionBinding::Param::TYPE_CONSTRUCTOR:
    case FunctionBinding::Param::TYPE_DESTRUCTOR:
    case FunctionBinding::Param::TYPE_VOID:
    case FunctionBinding::Param::TYPE_VARARGS:
    default:
        o << "lua_type(state, " << index << ") == LUA_TNONE";
    }
}

static inline void indent(ostream& o, int indentLevel)
{
    for (int k = 0; k < indentLevel; k++)
        o << "    ";
}

static inline void outputBindingInvocation(ostream& o, const FunctionBinding& b, unsigned int paramCount, unsigned int indentLevel)
{
    bool isNormalMember = (b.type == FunctionBinding::MEMBER_FUNCTION && b.returnParam.type != FunctionBinding::Param::TYPE_CONSTRUCTOR);

    // Get the passed in parameters.
    for (unsigned int i = 0, count = paramCount - (isNormalMember ? 1 : 0); i < count; i++)
    {
        outputGetParam(o, b.paramTypes[i], i, indentLevel, isNormalMember);
    }

    // Get the instance for member functions.
    if ((b.type == FunctionBinding::MEMBER_FUNCTION) &&
        b.returnParam.type != FunctionBinding::Param::TYPE_CONSTRUCTOR &&
        b.returnParam.type != FunctionBinding::Param::TYPE_DESTRUCTOR)
    {
        indent(o, indentLevel);
        o << b.classname << "* instance = getInstance(state);\n";
    }

    if (b.returnParam.type == FunctionBinding::Param::TYPE_DESTRUCTOR)
    {
        indent(o, indentLevel);
        o << "void* userdata = luaL_checkudata(state, 1, \"" << Generator::getUniqueName(b.classname) << "\");\n";
        indent(o, indentLevel);
        o << "luaL_argcheck(state, userdata != NULL, 1, \"\'" << Generator::getUniqueName(b.classname) << "\' expected.\");\n";
        indent(o, indentLevel);
        o << LUA_OBJECT << "* object = (" << LUA_OBJECT << "*)userdata;\n";
        indent(o, indentLevel);
        o << "if (object->owns)\n";
        indent(o, indentLevel);
        o << "{\n";
        indent(o, indentLevel + 1);
        o << b.classname << "* instance = (" << b.classname << "*)object->instance;\n";
        if (Generator::getInstance()->isRef(b.classname))
        {
            indent(o, indentLevel + 1);
            o << "SAFE_RELEASE(instance);\n";
        }
        else
        {
            indent(o, indentLevel + 1);
            o << "SAFE_DELETE(instance);\n";
        }
        indent(o, indentLevel);
        o << "}\n";
    }
    else
    {
        // Create a variable to hold the return type (if appropriate).
        if (!(b.returnParam.type == FunctionBinding::Param::TYPE_CONSTRUCTOR || 
            b.returnParam.type == FunctionBinding::Param::TYPE_VOID || 
            b.returnParam.type == FunctionBinding::Param::TYPE_OBJECT))
        {
            indent(o, indentLevel);
            o << b.returnParam << " result = ";
        }

        // For functions that return objects, create the appropriate user data in Lua.
        if (b.returnParam.type == FunctionBinding::Param::TYPE_CONSTRUCTOR || b.returnParam.type == FunctionBinding::Param::TYPE_OBJECT)
        {
            indent(o, indentLevel);
            switch (b.returnParam.kind)
            {
            case FunctionBinding::Param::KIND_POINTER:
                o << "void* returnPtr = (void*)";
                break;
            case FunctionBinding::Param::KIND_VALUE:
                o << "void* returnPtr = (void*)new " << b.returnParam << "(";
                break;
            case FunctionBinding::Param::KIND_REFERENCE:
                o << "void* returnPtr = (void*)&(";
                break;
            default:
                GP_ERROR("Invalid return value kind '%d'.", b.returnParam.kind);
                break;
            }
        }

        if (b.type == FunctionBinding::STATIC_FUNCTION)
        {
            if (b.returnParam.type == FunctionBinding::Param::TYPE_VOID)
                indent(o, indentLevel);
            o << b.classname << "::" << b.name << "(";
        }
        else if (b.type == FunctionBinding::GLOBAL_FUNCTION)
        {
            if (b.returnParam.type == FunctionBinding::Param::TYPE_VOID)
                indent(o, indentLevel);
            o << b.name << "(";
        }
        else if (b.type == FunctionBinding::MEMBER_FUNCTION)
        {
            if (b.returnParam.type == FunctionBinding::Param::TYPE_CONSTRUCTOR)
            {
                o << "new " << Generator::getInstance()->getIdentifier(b.returnParam.info) << "(";
            }
            else
            {
                if (b.returnParam.type == FunctionBinding::Param::TYPE_VOID)
                    indent(o, indentLevel);
                o << "instance->" << b.name << "(";
            }
        }

        // Pass the arguments.
        for (unsigned int i = 0, count = paramCount - ((isNormalMember) ? 1 : 0); i < count; i++)
        {
            if (b.paramTypes[i].type == FunctionBinding::Param::TYPE_OBJECT && b.paramTypes[i].kind != FunctionBinding::Param::KIND_POINTER)
                o << "*";
            o << "param" << i + 1;

            if (i != count - 1)
                o << ", ";
        }

        // Output the matching parenthesis for the case where a non-pointer object is being returned.
        if (b.returnParam.type == FunctionBinding::Param::TYPE_OBJECT && b.returnParam.kind != FunctionBinding::Param::KIND_POINTER)
            o << ")";

        o << ");\n";
    }

    outputReturnValue(o, b, indentLevel);
}

static inline void outputGetParam(ostream& o, const FunctionBinding::Param& p, int i, int indentLevel, bool offsetIndex)
{
    indent(o, indentLevel);
    o << "// Get parameter " << i + 1 << " off the stack.\n";

    switch (p.type)
    {
    case FunctionBinding::Param::TYPE_UNRECOGNIZED:
        indent(o, indentLevel);
        o << "GP_WARN(\"Attempting to get parameter " << i + 1 << " with unrecognized type " << p.info << " as an unsigned integer.\");\n";
    case FunctionBinding::Param::TYPE_BOOL:
    case FunctionBinding::Param::TYPE_CHAR:
    case FunctionBinding::Param::TYPE_SHORT:
    case FunctionBinding::Param::TYPE_INT:
    case FunctionBinding::Param::TYPE_LONG:
    case FunctionBinding::Param::TYPE_UCHAR:
    case FunctionBinding::Param::TYPE_USHORT:
    case FunctionBinding::Param::TYPE_UINT:
    case FunctionBinding::Param::TYPE_ULONG:
    case FunctionBinding::Param::TYPE_FLOAT:
    case FunctionBinding::Param::TYPE_DOUBLE:
    case FunctionBinding::Param::TYPE_STRING:
    case FunctionBinding::Param::TYPE_ENUM:
        indent(o, indentLevel);
        o << p << " param" << i + 1 << " = ";
        break;
    default:
        // Ignore these cases.
        break;
    }

    int paramIndex = (offsetIndex) ? i + 2 : i + 1;
    switch (p.type)
    {
    case FunctionBinding::Param::TYPE_BOOL:
        if (p.kind == FunctionBinding::Param::KIND_POINTER)
            o << "ScriptController::getInstance()->getBoolPointer(" << paramIndex << ");\n";
        else
            o << "(luaL_checkint(state, " << paramIndex << ") != 0);\n";
        break;
    case FunctionBinding::Param::TYPE_CHAR:
        o << "(char)luaL_checkint(state, " << paramIndex << ");\n";
        break;
    case FunctionBinding::Param::TYPE_SHORT:
        if (p.kind == FunctionBinding::Param::KIND_POINTER)
            o << "ScriptController::getInstance()->getShortPointer(" << paramIndex << ");\n";
        else
            o << "(short)luaL_checkint(state, " << paramIndex << ");\n";
        break;
    case FunctionBinding::Param::TYPE_INT:
        if (p.kind == FunctionBinding::Param::KIND_POINTER)
            o << "ScriptController::getInstance()->getIntPointer(" << paramIndex << ");\n";
        else
            o << "(int)luaL_checkint(state, " << paramIndex << ");\n";
        break;
    case FunctionBinding::Param::TYPE_LONG:
        if (p.kind == FunctionBinding::Param::KIND_POINTER)
            o << "ScriptController::getInstance()->getLongPointer(" << paramIndex << ");\n";
        else
            o << "(long)luaL_checklong(state, " << paramIndex << ");\n";
        break;
    case FunctionBinding::Param::TYPE_UCHAR:
        if (p.kind == FunctionBinding::Param::KIND_POINTER)
            o << "ScriptController::getInstance()->getUnsignedCharPointer(" << paramIndex << ");\n";
        else
            o << "(unsigned char)luaL_checkunsigned(state, " << paramIndex << ");\n";
        break;
    case FunctionBinding::Param::TYPE_USHORT:
        if (p.kind == FunctionBinding::Param::KIND_POINTER)
            o << "ScriptController::getInstance()->getUnsignedShortPointer(" << paramIndex << ");\n";
        else
            o << "(unsigned short)luaL_checkunsigned(state, " << paramIndex << ");\n";
        break;
    case FunctionBinding::Param::TYPE_UINT:
        if (p.kind == FunctionBinding::Param::KIND_POINTER)
            o << "ScriptController::getInstance()->getUnsignedIntPointer(" << paramIndex << ");\n";
        else
            o << "(unsigned int)luaL_checkunsigned(state, " << paramIndex << ");\n";
        break;
    case FunctionBinding::Param::TYPE_ULONG:
        if (p.kind == FunctionBinding::Param::KIND_POINTER)
            o << "ScriptController::getInstance()->getUnsignedLongPointer(" << paramIndex << ");\n";
        else
            o << "(unsigned long)luaL_checkunsigned(state, " << paramIndex << ");\n";
        break;
    case FunctionBinding::Param::TYPE_FLOAT:
        if (p.kind == FunctionBinding::Param::KIND_POINTER)
            o << "ScriptController::getInstance()->getFloatPointer(" << paramIndex << ");\n";
        else
            o << "(float)luaL_checknumber(state, " << paramIndex << ");\n";
        break;
    case FunctionBinding::Param::TYPE_DOUBLE:
        if (p.kind == FunctionBinding::Param::KIND_POINTER)
            o << "ScriptController::getInstance()->getDoublePointer(" << paramIndex << ");\n";
        else
            o << "(double)luaL_checknumber(state, " << paramIndex << ");\n";
        break;
    case FunctionBinding::Param::TYPE_STRING:
        o << "ScriptController::getInstance()->getString(" << paramIndex << ", " << ((p.info == "string") ? "true" : "false") << ");\n";
        break;
    case FunctionBinding::Param::TYPE_ENUM:
        o << "(" << p << ")lua_enumFromString_" << Generator::getInstance()->getUniqueNameFromRef(p.info) << "(luaL_checkstring(state, " << paramIndex << "));\n";
        break;
    case FunctionBinding::Param::TYPE_UNRECOGNIZED:
        // Attempt to retrieve the unrecognized type as an unsigned integer.
        o << "(" << p.info << ")luaL_checkunsigned(state, " << paramIndex << ");\n";
        break;
    case FunctionBinding::Param::TYPE_OBJECT:
        indent(o, indentLevel);
        o << p;
        if (p.kind != FunctionBinding::Param::KIND_POINTER)
            o << "*";
        o << " param" << i + 1 << " = ";
        o << "ScriptController::getInstance()->getObjectPointer<";
        o << Generator::getInstance()->getIdentifier(p.info) << ">(" << paramIndex;
        o << ", \"" << Generator::getInstance()->getUniqueNameFromRef(p.info) << "\", ";
        o << ((p.kind != FunctionBinding::Param::KIND_POINTER) ? "true" : "false") << ");\n";
        break;
    case FunctionBinding::Param::TYPE_CONSTRUCTOR:
    case FunctionBinding::Param::TYPE_DESTRUCTOR:
    case FunctionBinding::Param::TYPE_VOID:
    default:
        // Ignore these cases.
        break;
    }

    o << "\n";
}

static inline void outputMatchedBinding(ostream& o, const FunctionBinding& b, unsigned int paramCount, unsigned int indentLevel)
{
    bool isNormalMember = (b.type == FunctionBinding::MEMBER_FUNCTION && b.returnParam.type != FunctionBinding::Param::TYPE_CONSTRUCTOR);

    // If the current invocation of the function takes zero parameters, then invoke the binding.
    if (paramCount == 0)
    {
        outputBindingInvocation(o, b, paramCount, indentLevel);
    }
    else
    {
        // NOTE: The way this currently works may cause some issues since Lua
        // has a smaller set of types than C++. There may be cases where functions
        // that take types with less precision (i.e. int vs. float) are called
        // when the user in fact wanted to call the version with more precision.
        // (this will only happen for overloaded functions).

        o << "if (";
        for (unsigned int i = 0, count = paramCount; i < count; i++)
        {
            if (isNormalMember && i == 0)
                outputLuaTypeCheck(o, i + 1);
            else
                outputLuaTypeCheck(o, i + 1, b.paramTypes[(isNormalMember ? i - 1 : i)]);

            if (i == count - 1)
                o << ")\n";
            else
            {
                o << " &&\n";
                indent(o, indentLevel + 1);
            }
        }
        indent(o, indentLevel);
        o << "{\n";
            
        outputBindingInvocation(o, b, paramCount, indentLevel + 1);

        indent(o, indentLevel);
        o << "}\n";
    }
}

static inline void outputReturnValue(ostream& o, const FunctionBinding& b, int indentLevel)
{
    // Pass the return value back to Lua.
    if (!(b.returnParam.type == FunctionBinding::Param::TYPE_CONSTRUCTOR || 
        b.returnParam.type == FunctionBinding::Param::TYPE_DESTRUCTOR ||
        b.returnParam.type == FunctionBinding::Param::TYPE_VOID || 
        b.returnParam.type == FunctionBinding::Param::TYPE_OBJECT))
    {
        o << "\n";
        indent(o, indentLevel);
        o << "// Push the return value onto the stack.\n";

        // If the return type is a basic type pointer, return it as light user data.
        if (b.returnParam.type != FunctionBinding::Param::TYPE_ENUM &&
            b.returnParam.type != FunctionBinding::Param::TYPE_STRING &&
            b.returnParam.kind == FunctionBinding::Param::KIND_POINTER)
        {
            indent(o, indentLevel);
            o << "lua_pushlightuserdata(state, result);\n";
            indent(o, indentLevel);
            o << "return 1;\n";
            return;
        }
    }

    indent(o, indentLevel);
    switch (b.returnParam.type)
    {
    case FunctionBinding::Param::TYPE_BOOL:
        o << "lua_pushboolean(state, result);\n";
        break;
    case FunctionBinding::Param::TYPE_CHAR:
    case FunctionBinding::Param::TYPE_SHORT:
    case FunctionBinding::Param::TYPE_INT:
    case FunctionBinding::Param::TYPE_LONG:
        o << "lua_pushinteger(state, result);\n";
        break;
    case FunctionBinding::Param::TYPE_UCHAR:
    case FunctionBinding::Param::TYPE_USHORT:
    case FunctionBinding::Param::TYPE_UINT:
    case FunctionBinding::Param::TYPE_ULONG:
        o << "lua_pushunsigned(state, result);\n";
        break;
    case FunctionBinding::Param::TYPE_FLOAT:
    case FunctionBinding::Param::TYPE_DOUBLE:
        o << "lua_pushnumber(state, result);\n";
        break;
    case FunctionBinding::Param::TYPE_ENUM:
        o << "lua_pushstring(state, lua_stringFromEnum_" << Generator::getInstance()->getUniqueNameFromRef(b.returnParam.info) << "(result).c_str());\n";
        break;
    case FunctionBinding::Param::TYPE_STRING:
        if (b.returnParam.info == "string")
            o << "lua_pushstring(state, result.c_str());\n";
        else
            o << "lua_pushstring(state, result);\n";
        break;
    case FunctionBinding::Param::TYPE_OBJECT:
    case FunctionBinding::Param::TYPE_CONSTRUCTOR:
        o << "if (returnPtr)\n";
        indent(o, indentLevel);
        o << "{\n";
        indent(o, indentLevel + 1);
        o << LUA_OBJECT << "* object = (" << LUA_OBJECT << "*)lua_newuserdata(state, sizeof(" << LUA_OBJECT << "));\n";
        indent(o, indentLevel + 1);
        o << "object->instance = returnPtr;\n";
        indent(o, indentLevel + 1);
        o << "object->owns = ";
        if (b.own || (b.returnParam.type == FunctionBinding::Param::TYPE_OBJECT && b.returnParam.kind == FunctionBinding::Param::KIND_VALUE))
            o << "true";
        else
            o << "false";
        o << ";\n";
        indent(o, indentLevel + 1);
        o << "luaL_getmetatable(state, \"" << Generator::getInstance()->getUniqueNameFromRef(b.returnParam.info) << "\");\n";
        indent(o, indentLevel + 1);
        o << "lua_setmetatable(state, -2);\n";
        indent(o, indentLevel);
        o << "}\n";
        indent(o, indentLevel);
        o << "else\n";
        indent(o, indentLevel);
        o << "{\n";
        indent(o, indentLevel + 1);
        o << "lua_pushnil(state);\n";
        indent(o, indentLevel);
        o << "}\n";
        break;
    case FunctionBinding::Param::TYPE_DESTRUCTOR:
    case FunctionBinding::Param::TYPE_VOID:
        o << "\n";
        indent(o, indentLevel);
        o << "return 0;\n";
        return;
    }

    o << "\n";
    indent(o, indentLevel);
    o << "return 1;\n";
}
