/**
* @file Actor.cpp
* @brief Implementation of the Actor class.
* @author Dominique LaSalle <dominique@solidlake.com>
* Copyright 2018
* @version 1
* @date 2018-09-26
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/


#include "Actor.hpp"

#include "core/math/math_funcs.h"
#include "core/math/math_defs.h"

#include <algorithm>


/******************************************************************************
* HELPER FUNCTIONS ************************************************************
******************************************************************************/

namespace
{

template <typename T>
T in_range(
    T const lower_bound,
    T const upper_bound,
    T const value)
{
  return std::max(lower_bound, std::min(upper_bound, value));
}


template <typename T>
void adjust_in_range(
    T const lower_bound,
    T const upper_bound,
    T const delta,
    T * const value)
{
  *value = std::max(lower_bound, std::min(upper_bound, *value + delta));
}

}


/******************************************************************************
* CONSTRUCTORS / DESTRUCTOR ***************************************************
******************************************************************************/

Actor::Actor() :
  m_display_name(),
  m_max_look_angle(Math_PI/2.0),
  m_head_node_path(),
  m_velocity(0, 0, 0),
  m_look_angle(0)
{
  // do nothing
}


/******************************************************************************
* PUBLIC METHODS **************************************************************
******************************************************************************/

String Actor::get_display_name() const
{
  return m_display_name;
}

void Actor::set_display_name(
    String const p_name)
{
  m_display_name = p_name;
}

float Actor::get_max_look_angle() const
{
  return m_max_look_angle;
}

void Actor::set_max_look_angle(
    float const p_angle)
{
  m_max_look_angle = p_angle;
}

void Actor::set_head_node(
    NodePath const & p_head)
{
  m_head_node_path = p_head;
}

NodePath Actor::get_head_node() const
{
  return m_head_node_path;
}

void Actor::look_up(
    float const p_delta)
{
  adjust_in_range(-m_max_look_angle, m_max_look_angle, p_delta, &m_look_angle);
  _update_head();
}

float Actor::get_look_angle() const
{
  return m_look_angle;
}


void Actor::set_look_angle(
    float const p_angle)
{
  m_look_angle = in_range(-m_max_look_angle, m_max_look_angle, p_angle);
  _update_head();
}


Transform Actor::get_look_transform() const
{
  return Transform(Basis(Vector3(0, 0, m_look_angle)));
}


void Actor::apply_impulse(
    Vector3 const delta_v)
{
  m_velocity += delta_v;
}

void Actor::set_velocity(
    Vector3 const p_vel)
{
  m_velocity = p_vel;
}

Vector3 Actor::get_velocity() const
{
  return m_velocity;
}

/******************************************************************************
* STATIC PROTECTED METHODS ****************************************************
******************************************************************************/

void Actor::_bind_methods()
{
  ClassDB::bind_method(D_METHOD("get_display_name"), &Actor::get_display_name);
  ClassDB::bind_method(D_METHOD("set_display_name", "display_name"), &Actor::set_display_name);

  ClassDB::bind_method(D_METHOD("get_max_look_angle"), &Actor::get_max_look_angle);
  ClassDB::bind_method(D_METHOD("set_max_look_angle", "angle"), &Actor::set_max_look_angle);

  ClassDB::bind_method(D_METHOD("get_look_angle"), &Actor::get_look_angle);
  ClassDB::bind_method(D_METHOD("set_look_angle", "angle"), &Actor::set_look_angle);
  ClassDB::bind_method(D_METHOD("get_look_transform"), &Actor::get_look_transform);

  ClassDB::bind_method(D_METHOD("get_head_node"), &Actor::get_head_node);
  ClassDB::bind_method(D_METHOD("set_head_node", "node_path"), &Actor::set_head_node);

  ClassDB::bind_method(D_METHOD("look_up", "delta"), &Actor::look_up);

  ClassDB::bind_method(D_METHOD("get_velocity"), &Actor::get_velocity);
  ClassDB::bind_method(D_METHOD("set_velocity", "velocity"), &Actor::set_velocity);

  ClassDB::bind_method(D_METHOD("apply_impulse", "delta_v"), \
      &Actor::apply_impulse);

	ADD_PROPERTY(PropertyInfo(Variant::STRING, "display_name"), \
      "set_display_name", "get_display_name");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "max_look_angle", PROPERTY_HINT_RANGE, \
      "0.1,100.0"), "set_max_look_angle", "get_max_look_angle");
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "head_node"), "set_head_node", "get_head_node");
}


/******************************************************************************
* PROTECTED METHODS ***********************************************************
******************************************************************************/

void Actor::_notification(
    int const p_notification)
{
  switch (p_notification)
  {
    case NOTIFICATION_PHYSICS_PROCESS: {
      float const delta = get_physics_process_delta_time();

      // handle gravity
      PhysicsDirectBodyState * const state = \
          PhysicsServer::get_singleton()->body_get_direct_state(get_rid());
      Vector3 const gravity = state->get_total_gravity();
      m_velocity += gravity*delta;

      // update velocity 
      m_velocity = move_and_slide(m_velocity, Vector3(0, 1, 0), true, 4, \
          Math::deg2rad(55.0), false);
      if (is_on_floor()) {
        m_velocity = Vector3(0, 0, 0);
      }

      break;
    }
  }
}

void Actor::_update_head()
{
  Spatial * const head_node = \
      Object::cast_to<Spatial>(get_node(get_head_node()));

  if (head_node != nullptr) {
    head_node->set_rotation(Vector3(m_look_angle, 0, 0));
  }
}

