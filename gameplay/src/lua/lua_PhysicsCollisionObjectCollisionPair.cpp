#include "Base.h"
#include "ScriptController.h"
#include "PhysicsCollisionObject.h"
#include "lua_PhysicsCollisionObjectCollisionPair.h"
#include "lua_Global.h"

namespace gameplay
{

void luaRegister_PhysicsCollisionObjectCollisionPair()
{
    ScriptController* sc = ScriptController::getInstance();

    const luaL_Reg lua_members[] = 
    {
        {"objectA", lua_PhysicsCollisionObjectCollisionPair_objectA},
        {"objectB", lua_PhysicsCollisionObjectCollisionPair_objectB},
        {NULL, NULL}
    };
    const luaL_Reg* lua_statics = NULL;
    std::vector<std::string> scopePath;
    scopePath.push_back("PhysicsCollisionObject");

    sc->registerClass("PhysicsCollisionObjectCollisionPair", lua_members, lua_PhysicsCollisionObjectCollisionPair__init, lua_PhysicsCollisionObjectCollisionPair__gc, lua_statics, scopePath);
}

static PhysicsCollisionObject::CollisionPair* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "PhysicsCollisionObjectCollisionPair");
    luaL_argcheck(state, userdata != NULL, 1, "'PhysicsCollisionObjectCollisionPair' expected.");
    return (PhysicsCollisionObject::CollisionPair*)((ScriptController::LuaObject*)userdata)->instance;
}

int lua_PhysicsCollisionObjectCollisionPair__gc(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA || lua_type(state, 1) == LUA_TNIL))
            {
                void* userdata = luaL_checkudata(state, 1, "PhysicsCollisionObjectCollisionPair");
                luaL_argcheck(state, userdata != NULL, 1, "'PhysicsCollisionObjectCollisionPair' expected.");
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)userdata;
                if (object->owns)
                {
                    PhysicsCollisionObject::CollisionPair* instance = (PhysicsCollisionObject::CollisionPair*)object->instance;
                    SAFE_DELETE(instance);
                }
                
                return 0;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 1).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_PhysicsCollisionObjectCollisionPair__init(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA || lua_type(state, 1) == LUA_TNIL) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                PhysicsCollisionObject* param1 = ScriptController::getInstance()->getObjectPointer<PhysicsCollisionObject>(1, "PhysicsCollisionObject", false);

                // Get parameter 2 off the stack.
                PhysicsCollisionObject* param2 = ScriptController::getInstance()->getObjectPointer<PhysicsCollisionObject>(2, "PhysicsCollisionObject", false);

                void* returnPtr = (void*)new PhysicsCollisionObject::CollisionPair(param1, param2);
                if (returnPtr)
                {
                    ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                    object->instance = returnPtr;
                    object->owns = true;
                    luaL_getmetatable(state, "PhysicsCollisionObjectCollisionPair");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_PhysicsCollisionObjectCollisionPair_objectA(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 2)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
        lua_error(state);
    }

    PhysicsCollisionObject::CollisionPair* instance = getInstance(state);
    if (lua_gettop(state) == 2)
    {
        // Get parameter 2 off the stack.
        PhysicsCollisionObject* param2 = ScriptController::getInstance()->getObjectPointer<PhysicsCollisionObject>(2, "PhysicsCollisionObject", false);

        instance->objectA = param2;
        return 0;
    }
    else
    {
        void* returnPtr = (void*)instance->objectA;
        if (returnPtr)
        {
            ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
            object->instance = returnPtr;
            object->owns = false;
            luaL_getmetatable(state, "PhysicsCollisionObject");
            lua_setmetatable(state, -2);
        }
        else
        {
            lua_pushnil(state);
        }

        return 1;
    }
}

int lua_PhysicsCollisionObjectCollisionPair_objectB(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 2)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
        lua_error(state);
    }

    PhysicsCollisionObject::CollisionPair* instance = getInstance(state);
    if (lua_gettop(state) == 2)
    {
        // Get parameter 2 off the stack.
        PhysicsCollisionObject* param2 = ScriptController::getInstance()->getObjectPointer<PhysicsCollisionObject>(2, "PhysicsCollisionObject", false);

        instance->objectB = param2;
        return 0;
    }
    else
    {
        void* returnPtr = (void*)instance->objectB;
        if (returnPtr)
        {
            ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
            object->instance = returnPtr;
            object->owns = false;
            luaL_getmetatable(state, "PhysicsCollisionObject");
            lua_setmetatable(state, -2);
        }
        else
        {
            lua_pushnil(state);
        }

        return 1;
    }
}

}
