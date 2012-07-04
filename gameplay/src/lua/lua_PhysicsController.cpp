#include "Base.h"
#include "ScriptController.h"
#include "PhysicsController.h"
#include "lua_PhysicsController.h"
#include "lua_Global.h"

namespace gameplay
{

void luaRegister_PhysicsController()
{
    ScriptController* sc = ScriptController::getInstance();

    const luaL_Reg lua_members[] = 
    {
        {"addStatusListener", lua_PhysicsController_addStatusListener},
        {"createFixedConstraint", lua_PhysicsController_createFixedConstraint},
        {"createGenericConstraint", lua_PhysicsController_createGenericConstraint},
        {"createHingeConstraint", lua_PhysicsController_createHingeConstraint},
        {"createSocketConstraint", lua_PhysicsController_createSocketConstraint},
        {"createSpringConstraint", lua_PhysicsController_createSpringConstraint},
        {"drawDebug", lua_PhysicsController_drawDebug},
        {"getGravity", lua_PhysicsController_getGravity},
        {"rayTest", lua_PhysicsController_rayTest},
        {"removeStatusListener", lua_PhysicsController_removeStatusListener},
        {"setGravity", lua_PhysicsController_setGravity},
        {"sweepTest", lua_PhysicsController_sweepTest},
        {NULL, NULL}
    };
    const luaL_Reg* lua_statics = NULL;
    std::vector<std::string> scopePath;

    sc->registerClass("PhysicsController", lua_members, NULL, NULL, lua_statics, scopePath);
}

static PhysicsController* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "PhysicsController");
    luaL_argcheck(state, userdata != NULL, 1, "'PhysicsController' expected.");
    return (PhysicsController*)((ScriptController::LuaObject*)userdata)->instance;
}

int lua_PhysicsController_addStatusListener(lua_State* state)
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
                PhysicsController::Listener* param1 = ScriptController::getInstance()->getObjectPointer<PhysicsController::Listener>(2, "PhysicsControllerListener", false);

                PhysicsController* instance = getInstance(state);
                instance->addStatusListener(param1);
                
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
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_PhysicsController_createFixedConstraint(lua_State* state)
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
                PhysicsRigidBody* param1 = ScriptController::getInstance()->getObjectPointer<PhysicsRigidBody>(2, "PhysicsRigidBody", false);

                PhysicsController* instance = getInstance(state);
                void* returnPtr = (void*)instance->createFixedConstraint(param1);
                if (returnPtr)
                {
                    ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "PhysicsFixedConstraint");
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
        case 3:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA || lua_type(state, 1) == LUA_TNIL) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                PhysicsRigidBody* param1 = ScriptController::getInstance()->getObjectPointer<PhysicsRigidBody>(2, "PhysicsRigidBody", false);

                // Get parameter 2 off the stack.
                PhysicsRigidBody* param2 = ScriptController::getInstance()->getObjectPointer<PhysicsRigidBody>(3, "PhysicsRigidBody", false);

                PhysicsController* instance = getInstance(state);
                void* returnPtr = (void*)instance->createFixedConstraint(param1, param2);
                if (returnPtr)
                {
                    ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "PhysicsFixedConstraint");
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
            lua_pushstring(state, "Invalid number of parameters (expected 2 or 3).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_PhysicsController_createGenericConstraint(lua_State* state)
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
                PhysicsRigidBody* param1 = ScriptController::getInstance()->getObjectPointer<PhysicsRigidBody>(2, "PhysicsRigidBody", false);

                PhysicsController* instance = getInstance(state);
                void* returnPtr = (void*)instance->createGenericConstraint(param1);
                if (returnPtr)
                {
                    ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "PhysicsGenericConstraint");
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
        case 3:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA || lua_type(state, 1) == LUA_TNIL) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                PhysicsRigidBody* param1 = ScriptController::getInstance()->getObjectPointer<PhysicsRigidBody>(2, "PhysicsRigidBody", false);

                // Get parameter 2 off the stack.
                PhysicsRigidBody* param2 = ScriptController::getInstance()->getObjectPointer<PhysicsRigidBody>(3, "PhysicsRigidBody", false);

                PhysicsController* instance = getInstance(state);
                void* returnPtr = (void*)instance->createGenericConstraint(param1, param2);
                if (returnPtr)
                {
                    ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "PhysicsGenericConstraint");
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
        case 4:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA || lua_type(state, 1) == LUA_TNIL) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL) &&
                (lua_type(state, 4) == LUA_TUSERDATA || lua_type(state, 4) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                PhysicsRigidBody* param1 = ScriptController::getInstance()->getObjectPointer<PhysicsRigidBody>(2, "PhysicsRigidBody", false);

                // Get parameter 2 off the stack.
                Quaternion* param2 = ScriptController::getInstance()->getObjectPointer<Quaternion>(3, "Quaternion", true);

                // Get parameter 3 off the stack.
                Vector3* param3 = ScriptController::getInstance()->getObjectPointer<Vector3>(4, "Vector3", true);

                PhysicsController* instance = getInstance(state);
                void* returnPtr = (void*)instance->createGenericConstraint(param1, *param2, *param3);
                if (returnPtr)
                {
                    ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "PhysicsGenericConstraint");
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
        case 5:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA || lua_type(state, 1) == LUA_TNIL) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL) &&
                (lua_type(state, 4) == LUA_TUSERDATA || lua_type(state, 4) == LUA_TNIL) &&
                (lua_type(state, 5) == LUA_TUSERDATA || lua_type(state, 5) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                PhysicsRigidBody* param1 = ScriptController::getInstance()->getObjectPointer<PhysicsRigidBody>(2, "PhysicsRigidBody", false);

                // Get parameter 2 off the stack.
                Quaternion* param2 = ScriptController::getInstance()->getObjectPointer<Quaternion>(3, "Quaternion", true);

                // Get parameter 3 off the stack.
                Vector3* param3 = ScriptController::getInstance()->getObjectPointer<Vector3>(4, "Vector3", true);

                // Get parameter 4 off the stack.
                PhysicsRigidBody* param4 = ScriptController::getInstance()->getObjectPointer<PhysicsRigidBody>(5, "PhysicsRigidBody", false);

                PhysicsController* instance = getInstance(state);
                void* returnPtr = (void*)instance->createGenericConstraint(param1, *param2, *param3, param4);
                if (returnPtr)
                {
                    ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "PhysicsGenericConstraint");
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
        case 6:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA || lua_type(state, 1) == LUA_TNIL) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL) &&
                (lua_type(state, 4) == LUA_TUSERDATA || lua_type(state, 4) == LUA_TNIL) &&
                (lua_type(state, 5) == LUA_TUSERDATA || lua_type(state, 5) == LUA_TNIL) &&
                (lua_type(state, 6) == LUA_TUSERDATA || lua_type(state, 6) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                PhysicsRigidBody* param1 = ScriptController::getInstance()->getObjectPointer<PhysicsRigidBody>(2, "PhysicsRigidBody", false);

                // Get parameter 2 off the stack.
                Quaternion* param2 = ScriptController::getInstance()->getObjectPointer<Quaternion>(3, "Quaternion", true);

                // Get parameter 3 off the stack.
                Vector3* param3 = ScriptController::getInstance()->getObjectPointer<Vector3>(4, "Vector3", true);

                // Get parameter 4 off the stack.
                PhysicsRigidBody* param4 = ScriptController::getInstance()->getObjectPointer<PhysicsRigidBody>(5, "PhysicsRigidBody", false);

                // Get parameter 5 off the stack.
                Quaternion* param5 = ScriptController::getInstance()->getObjectPointer<Quaternion>(6, "Quaternion", true);

                PhysicsController* instance = getInstance(state);
                void* returnPtr = (void*)instance->createGenericConstraint(param1, *param2, *param3, param4, *param5);
                if (returnPtr)
                {
                    ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "PhysicsGenericConstraint");
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
        case 7:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA || lua_type(state, 1) == LUA_TNIL) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL) &&
                (lua_type(state, 4) == LUA_TUSERDATA || lua_type(state, 4) == LUA_TNIL) &&
                (lua_type(state, 5) == LUA_TUSERDATA || lua_type(state, 5) == LUA_TNIL) &&
                (lua_type(state, 6) == LUA_TUSERDATA || lua_type(state, 6) == LUA_TNIL) &&
                (lua_type(state, 7) == LUA_TUSERDATA || lua_type(state, 7) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                PhysicsRigidBody* param1 = ScriptController::getInstance()->getObjectPointer<PhysicsRigidBody>(2, "PhysicsRigidBody", false);

                // Get parameter 2 off the stack.
                Quaternion* param2 = ScriptController::getInstance()->getObjectPointer<Quaternion>(3, "Quaternion", true);

                // Get parameter 3 off the stack.
                Vector3* param3 = ScriptController::getInstance()->getObjectPointer<Vector3>(4, "Vector3", true);

                // Get parameter 4 off the stack.
                PhysicsRigidBody* param4 = ScriptController::getInstance()->getObjectPointer<PhysicsRigidBody>(5, "PhysicsRigidBody", false);

                // Get parameter 5 off the stack.
                Quaternion* param5 = ScriptController::getInstance()->getObjectPointer<Quaternion>(6, "Quaternion", true);

                // Get parameter 6 off the stack.
                Vector3* param6 = ScriptController::getInstance()->getObjectPointer<Vector3>(7, "Vector3", true);

                PhysicsController* instance = getInstance(state);
                void* returnPtr = (void*)instance->createGenericConstraint(param1, *param2, *param3, param4, *param5, *param6);
                if (returnPtr)
                {
                    ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "PhysicsGenericConstraint");
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
            lua_pushstring(state, "Invalid number of parameters (expected 2, 3, 4, 5, 6 or 7).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_PhysicsController_createHingeConstraint(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 4:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA || lua_type(state, 1) == LUA_TNIL) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL) &&
                (lua_type(state, 4) == LUA_TUSERDATA || lua_type(state, 4) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                PhysicsRigidBody* param1 = ScriptController::getInstance()->getObjectPointer<PhysicsRigidBody>(2, "PhysicsRigidBody", false);

                // Get parameter 2 off the stack.
                Quaternion* param2 = ScriptController::getInstance()->getObjectPointer<Quaternion>(3, "Quaternion", true);

                // Get parameter 3 off the stack.
                Vector3* param3 = ScriptController::getInstance()->getObjectPointer<Vector3>(4, "Vector3", true);

                PhysicsController* instance = getInstance(state);
                void* returnPtr = (void*)instance->createHingeConstraint(param1, *param2, *param3);
                if (returnPtr)
                {
                    ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "PhysicsHingeConstraint");
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
        case 5:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA || lua_type(state, 1) == LUA_TNIL) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL) &&
                (lua_type(state, 4) == LUA_TUSERDATA || lua_type(state, 4) == LUA_TNIL) &&
                (lua_type(state, 5) == LUA_TUSERDATA || lua_type(state, 5) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                PhysicsRigidBody* param1 = ScriptController::getInstance()->getObjectPointer<PhysicsRigidBody>(2, "PhysicsRigidBody", false);

                // Get parameter 2 off the stack.
                Quaternion* param2 = ScriptController::getInstance()->getObjectPointer<Quaternion>(3, "Quaternion", true);

                // Get parameter 3 off the stack.
                Vector3* param3 = ScriptController::getInstance()->getObjectPointer<Vector3>(4, "Vector3", true);

                // Get parameter 4 off the stack.
                PhysicsRigidBody* param4 = ScriptController::getInstance()->getObjectPointer<PhysicsRigidBody>(5, "PhysicsRigidBody", false);

                PhysicsController* instance = getInstance(state);
                void* returnPtr = (void*)instance->createHingeConstraint(param1, *param2, *param3, param4);
                if (returnPtr)
                {
                    ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "PhysicsHingeConstraint");
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
        case 6:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA || lua_type(state, 1) == LUA_TNIL) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL) &&
                (lua_type(state, 4) == LUA_TUSERDATA || lua_type(state, 4) == LUA_TNIL) &&
                (lua_type(state, 5) == LUA_TUSERDATA || lua_type(state, 5) == LUA_TNIL) &&
                (lua_type(state, 6) == LUA_TUSERDATA || lua_type(state, 6) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                PhysicsRigidBody* param1 = ScriptController::getInstance()->getObjectPointer<PhysicsRigidBody>(2, "PhysicsRigidBody", false);

                // Get parameter 2 off the stack.
                Quaternion* param2 = ScriptController::getInstance()->getObjectPointer<Quaternion>(3, "Quaternion", true);

                // Get parameter 3 off the stack.
                Vector3* param3 = ScriptController::getInstance()->getObjectPointer<Vector3>(4, "Vector3", true);

                // Get parameter 4 off the stack.
                PhysicsRigidBody* param4 = ScriptController::getInstance()->getObjectPointer<PhysicsRigidBody>(5, "PhysicsRigidBody", false);

                // Get parameter 5 off the stack.
                Quaternion* param5 = ScriptController::getInstance()->getObjectPointer<Quaternion>(6, "Quaternion", true);

                PhysicsController* instance = getInstance(state);
                void* returnPtr = (void*)instance->createHingeConstraint(param1, *param2, *param3, param4, *param5);
                if (returnPtr)
                {
                    ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "PhysicsHingeConstraint");
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
        case 7:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA || lua_type(state, 1) == LUA_TNIL) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL) &&
                (lua_type(state, 4) == LUA_TUSERDATA || lua_type(state, 4) == LUA_TNIL) &&
                (lua_type(state, 5) == LUA_TUSERDATA || lua_type(state, 5) == LUA_TNIL) &&
                (lua_type(state, 6) == LUA_TUSERDATA || lua_type(state, 6) == LUA_TNIL) &&
                (lua_type(state, 7) == LUA_TUSERDATA || lua_type(state, 7) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                PhysicsRigidBody* param1 = ScriptController::getInstance()->getObjectPointer<PhysicsRigidBody>(2, "PhysicsRigidBody", false);

                // Get parameter 2 off the stack.
                Quaternion* param2 = ScriptController::getInstance()->getObjectPointer<Quaternion>(3, "Quaternion", true);

                // Get parameter 3 off the stack.
                Vector3* param3 = ScriptController::getInstance()->getObjectPointer<Vector3>(4, "Vector3", true);

                // Get parameter 4 off the stack.
                PhysicsRigidBody* param4 = ScriptController::getInstance()->getObjectPointer<PhysicsRigidBody>(5, "PhysicsRigidBody", false);

                // Get parameter 5 off the stack.
                Quaternion* param5 = ScriptController::getInstance()->getObjectPointer<Quaternion>(6, "Quaternion", true);

                // Get parameter 6 off the stack.
                Vector3* param6 = ScriptController::getInstance()->getObjectPointer<Vector3>(7, "Vector3", true);

                PhysicsController* instance = getInstance(state);
                void* returnPtr = (void*)instance->createHingeConstraint(param1, *param2, *param3, param4, *param5, *param6);
                if (returnPtr)
                {
                    ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "PhysicsHingeConstraint");
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
            lua_pushstring(state, "Invalid number of parameters (expected 4, 5, 6 or 7).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_PhysicsController_createSocketConstraint(lua_State* state)
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
                PhysicsRigidBody* param1 = ScriptController::getInstance()->getObjectPointer<PhysicsRigidBody>(2, "PhysicsRigidBody", false);

                PhysicsController* instance = getInstance(state);
                void* returnPtr = (void*)instance->createSocketConstraint(param1);
                if (returnPtr)
                {
                    ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "PhysicsSocketConstraint");
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
        case 3:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA || lua_type(state, 1) == LUA_TNIL) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                PhysicsRigidBody* param1 = ScriptController::getInstance()->getObjectPointer<PhysicsRigidBody>(2, "PhysicsRigidBody", false);

                // Get parameter 2 off the stack.
                PhysicsRigidBody* param2 = ScriptController::getInstance()->getObjectPointer<PhysicsRigidBody>(3, "PhysicsRigidBody", false);

                PhysicsController* instance = getInstance(state);
                void* returnPtr = (void*)instance->createSocketConstraint(param1, param2);
                if (returnPtr)
                {
                    ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "PhysicsSocketConstraint");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }
            else if ((lua_type(state, 1) == LUA_TUSERDATA || lua_type(state, 1) == LUA_TNIL) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                PhysicsRigidBody* param1 = ScriptController::getInstance()->getObjectPointer<PhysicsRigidBody>(2, "PhysicsRigidBody", false);

                // Get parameter 2 off the stack.
                Vector3* param2 = ScriptController::getInstance()->getObjectPointer<Vector3>(3, "Vector3", true);

                PhysicsController* instance = getInstance(state);
                void* returnPtr = (void*)instance->createSocketConstraint(param1, *param2);
                if (returnPtr)
                {
                    ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "PhysicsSocketConstraint");
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
        case 4:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA || lua_type(state, 1) == LUA_TNIL) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL) &&
                (lua_type(state, 4) == LUA_TUSERDATA || lua_type(state, 4) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                PhysicsRigidBody* param1 = ScriptController::getInstance()->getObjectPointer<PhysicsRigidBody>(2, "PhysicsRigidBody", false);

                // Get parameter 2 off the stack.
                Vector3* param2 = ScriptController::getInstance()->getObjectPointer<Vector3>(3, "Vector3", true);

                // Get parameter 3 off the stack.
                PhysicsRigidBody* param3 = ScriptController::getInstance()->getObjectPointer<PhysicsRigidBody>(4, "PhysicsRigidBody", false);

                PhysicsController* instance = getInstance(state);
                void* returnPtr = (void*)instance->createSocketConstraint(param1, *param2, param3);
                if (returnPtr)
                {
                    ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "PhysicsSocketConstraint");
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
        case 5:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA || lua_type(state, 1) == LUA_TNIL) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL) &&
                (lua_type(state, 4) == LUA_TUSERDATA || lua_type(state, 4) == LUA_TNIL) &&
                (lua_type(state, 5) == LUA_TUSERDATA || lua_type(state, 5) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                PhysicsRigidBody* param1 = ScriptController::getInstance()->getObjectPointer<PhysicsRigidBody>(2, "PhysicsRigidBody", false);

                // Get parameter 2 off the stack.
                Vector3* param2 = ScriptController::getInstance()->getObjectPointer<Vector3>(3, "Vector3", true);

                // Get parameter 3 off the stack.
                PhysicsRigidBody* param3 = ScriptController::getInstance()->getObjectPointer<PhysicsRigidBody>(4, "PhysicsRigidBody", false);

                // Get parameter 4 off the stack.
                Vector3* param4 = ScriptController::getInstance()->getObjectPointer<Vector3>(5, "Vector3", true);

                PhysicsController* instance = getInstance(state);
                void* returnPtr = (void*)instance->createSocketConstraint(param1, *param2, param3, *param4);
                if (returnPtr)
                {
                    ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "PhysicsSocketConstraint");
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
            lua_pushstring(state, "Invalid number of parameters (expected 2, 3, 4 or 5).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_PhysicsController_createSpringConstraint(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 3:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA || lua_type(state, 1) == LUA_TNIL) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                PhysicsRigidBody* param1 = ScriptController::getInstance()->getObjectPointer<PhysicsRigidBody>(2, "PhysicsRigidBody", false);

                // Get parameter 2 off the stack.
                PhysicsRigidBody* param2 = ScriptController::getInstance()->getObjectPointer<PhysicsRigidBody>(3, "PhysicsRigidBody", false);

                PhysicsController* instance = getInstance(state);
                void* returnPtr = (void*)instance->createSpringConstraint(param1, param2);
                if (returnPtr)
                {
                    ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "PhysicsSpringConstraint");
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
        case 7:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA || lua_type(state, 1) == LUA_TNIL) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL) &&
                (lua_type(state, 4) == LUA_TUSERDATA || lua_type(state, 4) == LUA_TNIL) &&
                (lua_type(state, 5) == LUA_TUSERDATA || lua_type(state, 5) == LUA_TNIL) &&
                (lua_type(state, 6) == LUA_TUSERDATA || lua_type(state, 6) == LUA_TNIL) &&
                (lua_type(state, 7) == LUA_TUSERDATA || lua_type(state, 7) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                PhysicsRigidBody* param1 = ScriptController::getInstance()->getObjectPointer<PhysicsRigidBody>(2, "PhysicsRigidBody", false);

                // Get parameter 2 off the stack.
                Quaternion* param2 = ScriptController::getInstance()->getObjectPointer<Quaternion>(3, "Quaternion", true);

                // Get parameter 3 off the stack.
                Vector3* param3 = ScriptController::getInstance()->getObjectPointer<Vector3>(4, "Vector3", true);

                // Get parameter 4 off the stack.
                PhysicsRigidBody* param4 = ScriptController::getInstance()->getObjectPointer<PhysicsRigidBody>(5, "PhysicsRigidBody", false);

                // Get parameter 5 off the stack.
                Quaternion* param5 = ScriptController::getInstance()->getObjectPointer<Quaternion>(6, "Quaternion", true);

                // Get parameter 6 off the stack.
                Vector3* param6 = ScriptController::getInstance()->getObjectPointer<Vector3>(7, "Vector3", true);

                PhysicsController* instance = getInstance(state);
                void* returnPtr = (void*)instance->createSpringConstraint(param1, *param2, *param3, param4, *param5, *param6);
                if (returnPtr)
                {
                    ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "PhysicsSpringConstraint");
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
            lua_pushstring(state, "Invalid number of parameters (expected 3 or 7).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_PhysicsController_drawDebug(lua_State* state)
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
                Matrix* param1 = ScriptController::getInstance()->getObjectPointer<Matrix>(2, "Matrix", true);

                PhysicsController* instance = getInstance(state);
                instance->drawDebug(*param1);
                
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
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_PhysicsController_getGravity(lua_State* state)
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
                PhysicsController* instance = getInstance(state);
                void* returnPtr = (void*)&(instance->getGravity());
                if (returnPtr)
                {
                    ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "Vector3");
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
            lua_pushstring(state, "Invalid number of parameters (expected 1).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_PhysicsController_rayTest(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 3:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA || lua_type(state, 1) == LUA_TNIL) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL) &&
                lua_type(state, 3) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                Ray* param1 = ScriptController::getInstance()->getObjectPointer<Ray>(2, "Ray", true);

                // Get parameter 2 off the stack.
                float param2 = (float)luaL_checknumber(state, 3);

                PhysicsController* instance = getInstance(state);
                bool result = instance->rayTest(*param1, param2);

                // Push the return value onto the stack.
                lua_pushboolean(state, result);

                return 1;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 4:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA || lua_type(state, 1) == LUA_TNIL) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL) &&
                lua_type(state, 3) == LUA_TNUMBER &&
                (lua_type(state, 4) == LUA_TUSERDATA || lua_type(state, 4) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                Ray* param1 = ScriptController::getInstance()->getObjectPointer<Ray>(2, "Ray", true);

                // Get parameter 2 off the stack.
                float param2 = (float)luaL_checknumber(state, 3);

                // Get parameter 3 off the stack.
                PhysicsController::HitResult* param3 = ScriptController::getInstance()->getObjectPointer<PhysicsController::HitResult>(4, "PhysicsControllerHitResult", false);

                PhysicsController* instance = getInstance(state);
                bool result = instance->rayTest(*param1, param2, param3);

                // Push the return value onto the stack.
                lua_pushboolean(state, result);

                return 1;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 5:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA || lua_type(state, 1) == LUA_TNIL) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL) &&
                lua_type(state, 3) == LUA_TNUMBER &&
                (lua_type(state, 4) == LUA_TUSERDATA || lua_type(state, 4) == LUA_TNIL) &&
                (lua_type(state, 5) == LUA_TUSERDATA || lua_type(state, 5) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                Ray* param1 = ScriptController::getInstance()->getObjectPointer<Ray>(2, "Ray", true);

                // Get parameter 2 off the stack.
                float param2 = (float)luaL_checknumber(state, 3);

                // Get parameter 3 off the stack.
                PhysicsController::HitResult* param3 = ScriptController::getInstance()->getObjectPointer<PhysicsController::HitResult>(4, "PhysicsControllerHitResult", false);

                // Get parameter 4 off the stack.
                PhysicsController::HitFilter* param4 = ScriptController::getInstance()->getObjectPointer<PhysicsController::HitFilter>(5, "PhysicsControllerHitFilter", false);

                PhysicsController* instance = getInstance(state);
                bool result = instance->rayTest(*param1, param2, param3, param4);

                // Push the return value onto the stack.
                lua_pushboolean(state, result);

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
            lua_pushstring(state, "Invalid number of parameters (expected 3, 4 or 5).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_PhysicsController_removeStatusListener(lua_State* state)
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
                PhysicsController::Listener* param1 = ScriptController::getInstance()->getObjectPointer<PhysicsController::Listener>(2, "PhysicsControllerListener", false);

                PhysicsController* instance = getInstance(state);
                instance->removeStatusListener(param1);
                
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
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_PhysicsController_setGravity(lua_State* state)
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
                Vector3* param1 = ScriptController::getInstance()->getObjectPointer<Vector3>(2, "Vector3", true);

                PhysicsController* instance = getInstance(state);
                instance->setGravity(*param1);
                
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
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_PhysicsController_sweepTest(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 3:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA || lua_type(state, 1) == LUA_TNIL) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                PhysicsCollisionObject* param1 = ScriptController::getInstance()->getObjectPointer<PhysicsCollisionObject>(2, "PhysicsCollisionObject", false);

                // Get parameter 2 off the stack.
                Vector3* param2 = ScriptController::getInstance()->getObjectPointer<Vector3>(3, "Vector3", true);

                PhysicsController* instance = getInstance(state);
                bool result = instance->sweepTest(param1, *param2);

                // Push the return value onto the stack.
                lua_pushboolean(state, result);

                return 1;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 4:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA || lua_type(state, 1) == LUA_TNIL) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL) &&
                (lua_type(state, 4) == LUA_TUSERDATA || lua_type(state, 4) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                PhysicsCollisionObject* param1 = ScriptController::getInstance()->getObjectPointer<PhysicsCollisionObject>(2, "PhysicsCollisionObject", false);

                // Get parameter 2 off the stack.
                Vector3* param2 = ScriptController::getInstance()->getObjectPointer<Vector3>(3, "Vector3", true);

                // Get parameter 3 off the stack.
                PhysicsController::HitResult* param3 = ScriptController::getInstance()->getObjectPointer<PhysicsController::HitResult>(4, "PhysicsControllerHitResult", false);

                PhysicsController* instance = getInstance(state);
                bool result = instance->sweepTest(param1, *param2, param3);

                // Push the return value onto the stack.
                lua_pushboolean(state, result);

                return 1;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 5:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA || lua_type(state, 1) == LUA_TNIL) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL) &&
                (lua_type(state, 4) == LUA_TUSERDATA || lua_type(state, 4) == LUA_TNIL) &&
                (lua_type(state, 5) == LUA_TUSERDATA || lua_type(state, 5) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                PhysicsCollisionObject* param1 = ScriptController::getInstance()->getObjectPointer<PhysicsCollisionObject>(2, "PhysicsCollisionObject", false);

                // Get parameter 2 off the stack.
                Vector3* param2 = ScriptController::getInstance()->getObjectPointer<Vector3>(3, "Vector3", true);

                // Get parameter 3 off the stack.
                PhysicsController::HitResult* param3 = ScriptController::getInstance()->getObjectPointer<PhysicsController::HitResult>(4, "PhysicsControllerHitResult", false);

                // Get parameter 4 off the stack.
                PhysicsController::HitFilter* param4 = ScriptController::getInstance()->getObjectPointer<PhysicsController::HitFilter>(5, "PhysicsControllerHitFilter", false);

                PhysicsController* instance = getInstance(state);
                bool result = instance->sweepTest(param1, *param2, param3, param4);

                // Push the return value onto the stack.
                lua_pushboolean(state, result);

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
            lua_pushstring(state, "Invalid number of parameters (expected 3, 4 or 5).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

}
