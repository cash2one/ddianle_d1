/*
 * Macros.h
 * Description: 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2015-6-4 15:35:36
 */

#ifndef __MACROS_H__
#define __MACROS_H__

/** @def CLASS_CREATE_FUNC
 * Define the static Create() function for class.
 *
 * @param __CLASS_NAME__    The class name.
 */
#define CLASS_CREATE_FUNC(__CLASS_NAME__)   \
static __CLASS_NAME__* Create()             \
{                                           \
    return new __CLASS_NAME__;              \
}

/** @def CLASS_INSTANCE_FUNC
 * Define the static Instance() function for class.
 *
 * @param __CLASS_NAME__    The class name.
 */
#define CLASS_INSTANCE_FUNC(__CLASS_NAME__) \
static __CLASS_NAME__& Instance()           \
{                                           \
    static __CLASS_NAME__ s_Instance;       \
    return s_Instance;                      \
}


/** @def CLASS_BOOL_PROPERTY_READONLY
 * It's used to declare a protected bool variable.
 * We can use getter to read the variable.
 *
 * @param varName   Variable name.
 * @param funName   "Has + funName" will be the name of getter.
 */
#define CLASS_BOOL_PROPERTY_READONLY(varName, funName)      \
protected: bool varName;                                    \
public: bool Has##funName() const { return varName; }

/** @def CLASS_PROPERTY_READONLY
 * It's used to declare a protected variable.
 * We can use getter to read the variable.
 *
 * @param varType   The type of variable.
 * @param varName   Variable name.
 * @param funName   "Get + funName" will be the name of getter.
 */
#define CLASS_PROPERTY_READONLY(varType, varName, funName)  \
protected: varType varName;                                 \
public: varType Get##funName() const { return varName; }

#define CLASS_PROPERTY_READONLY_BY_REF(varType, varName, funName)   \
protected: varType varName;                                         \
public: const varType& Get##funName() const { return varName; }


/** @def CLASS_BOOL_PROPERTY
 * It's used to declare a protected bool variable.
 * We can use getter to read the variable, and use setter to change the variable.
 *
 * @param varName   Variable name.
 * @param funName   "Has + funName" will be the name of getter.
 *                  "Set + funName" will be the name of setter.
 */
#define CLASS_BOOL_PROPERTY(varName, funName)                   \
protected: bool varName;                                        \
public: bool Has##funName() const { return varName; }   \
public: void Set##funName(bool var) { varName = var; }

/** @def CLASS_PROPERTY
 * It's used to declare a protected variable.
 * We can use getter to read the variable, and use setter to change the variable.
 *
 * @param varType   The type of variable.
 * @param varName   Variable name.
 * @param funName   "Get + funName" will be the name of getter.
 *                  "Set + funName" will be the name of setter.
 */
#define CLASS_PROPERTY(varType, varName, funName)                   \
protected: varType varName;                                         \
public: varType Get##funName() const { return varName; }    \
public: void Set##funName(varType var) { varName = var; }

#define CLASS_PROPERTY_BY_REF(varType, varName, funName)                \
protected: varType varName;                                             \
public: const varType& Get##funName() const { return varName; } \
public: void Set##funName(const varType& var) { varName = var; }


#define SAFE_DELETE(p)              if (p) { delete (p); (p) = NULL; }
#define SAFE_DELETE_ARRAY(p)        if (p) { delete[] (p); (p) = NULL; }

#define BREAK_IF(cond)              if (cond) break

#define MINUTE_PRE_SECONDS  60
#define HOUR_PRE_SECONDS    (60 * 60)
#define DAY_PRE_SECONDS     (24 * 60 * 60)
#define WEEK_PRE_SECONDS    (7 * DAY_PRE_SECONDS)
#define MONTH_PRE_SECONDS   (31 * DAY_PRE_SECONDS)

#define ARRY_LENGTH(ary) (sizeof(ary)/sizeof(ary[0]))

#endif // __MACROS_H__

