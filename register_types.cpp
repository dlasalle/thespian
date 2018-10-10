/**
 * @file register_types.cpp
 * @brief Implementation of register types functions.
 * @author Dominique LaSalle <dominique@solidlake.com>
 * Copyright 2018
 * @version 1
 * @date 2018-02-23
 */

#include "Actor.hpp"
#include "register_types.h"

#include "core/class_db.h"

void register_actors_types()
{
  ClassDB::register_class<Actor>();
}

void unregister_actors_types()
{
  // nothing to do
}
