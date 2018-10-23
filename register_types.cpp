/**
 * @file register_types.cpp
 * @brief Implementation of register types functions.
 * @author Dominique LaSalle <dominique@solidlake.com>
 * Copyright 2018
 * @version 1
 * @date 2018-02-23
 */

#include "GroundActor.hpp"
#include "register_types.h"

#include "core/class_db.h"

void register_thespian_types()
{
  ClassDB::register_class<GroundActor>();
}

void unregister_thespian_types()
{
  // nothing to do
}
