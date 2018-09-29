/**
* @file Actor.cpp
* @brief Implementation of the Actor class.
* @author Dominique LaSalle <dominique@solidlake.com>
* Copyright 2018
* @version 1
* @date 2018-09-26
*/


#include "Actor.hpp"

#include "core/math/math_funcs.h"
#include "core/math/math_defs.h"


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
  m_air_control(false),
  m_run_speed(1.0),
  m_jump_speed(1.0),
  m_max_look_angle(Math_PI/2.0),
  m_velocity(0, 0, 0),
  m_motion(0, 0),
  m_jumping(false),
  m_look_angle(0)
{
  // do nothing
}


/******************************************************************************
* PUBLIC METHODS **************************************************************
******************************************************************************/

void Actor::set_motion(
    Vector2 const p_motion)
{
  m_motion = p_motion.normalized();
}


Vector2 Actor::get_motion() const
{
  return m_motion;
}


float Actor::get_run_speed() const
{
  return m_run_speed;
}


void Actor::set_run_speed(
    float const p_speed)
{
  m_run_speed = p_speed;
}


float Actor::get_jump_speed() const
{
  return m_jump_speed;
}


void Actor::set_jump_speed(
    float const p_speed)
{
  m_jump_speed = p_speed;
}


bool Actor::has_air_control() const
{
  return m_air_control;
}


void Actor::set_air_control(
    bool const p_control)
{
  m_air_control = p_control;
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


void Actor::jump()
{
	if (this->is_on_floor()) {
		m_jumping = true;
  }
}


void Actor::look_up(
    float const p_delta)
{
  adjust_in_range(-m_max_look_angle, m_max_look_angle, p_delta, &m_look_angle);
}

float Actor::get_look_angle() const
{
  return m_look_angle;
}


void Actor::set_look_angle(
    float const p_angle)
{
  m_look_angle = in_range(-m_max_look_angle, m_max_look_angle, p_angle);
}


Transform Actor::get_look_transform() const
{
  return Transform(Basis(Vector3(0, 0, m_look_angle)));
}




/******************************************************************************
* STATIC PROTECTED METHODS ****************************************************
******************************************************************************/

void Actor::_bind_methods()
{
  ClassDB::bind_method(D_METHOD("get_motion"), &Actor::get_motion);
  ClassDB::bind_method(D_METHOD("set_motion", "motion"), &Actor::set_motion);

  ClassDB::bind_method(D_METHOD("get_run_speed"), &Actor::get_run_speed);
  ClassDB::bind_method(D_METHOD("set_run_speed", "speed"), &Actor::set_run_speed);

  ClassDB::bind_method(D_METHOD("get_jump_speed"), &Actor::get_jump_speed);
  ClassDB::bind_method(D_METHOD("set_jump_speed", "speed"), &Actor::set_jump_speed);

  ClassDB::bind_method(D_METHOD("get_max_look_angle"), &Actor::get_max_look_angle);
  ClassDB::bind_method(D_METHOD("set_max_look_angle", "angle"), &Actor::set_max_look_angle);

  ClassDB::bind_method(D_METHOD("get_look_angle"), &Actor::get_look_angle);
  ClassDB::bind_method(D_METHOD("set_look_angle", "angle"), &Actor::set_look_angle);
  ClassDB::bind_method(D_METHOD("get_look_transform"), &Actor::get_look_transform);

  ClassDB::bind_method(D_METHOD("has_air_control"), &Actor::has_air_control);
  ClassDB::bind_method(D_METHOD("set_air_control", "enabled"), &Actor::set_air_control);

  ClassDB::bind_method(D_METHOD("jump"), &Actor::jump);
  ClassDB::bind_method(D_METHOD("look_up", "delta"), &Actor::look_up);

	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "run_speed", PROPERTY_HINT_RANGE, \
      "0.1,100.0"), "set_run_speed", "get_run_speed");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "jump_speed", PROPERTY_HINT_RANGE, \
      "0.1,100.0"), "set_jump_speed", "get_jump_speed");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "max_look_angle", PROPERTY_HINT_RANGE, \
      "0.1,100.0"), "set_max_look_angle", "get_max_look_angle");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "air_control"), \
      "set_air_control", "has_air_control");
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
      // update position
      if (is_on_floor() || has_air_control()) {
        m_velocity.x = m_motion.x*m_run_speed;
        m_velocity.z = m_motion.y*m_jump_speed;
      }

      // handle gravity
      PhysicsDirectBodyState * const state = \
          PhysicsServer::get_singleton()->body_get_direct_state(get_rid());
      Vector3 const gravity = state->get_total_gravity();
      m_velocity += gravity*get_physics_process_delta_time();

      // handle jump
      if (is_on_floor()) {
        if (m_jumping) {
          m_jumping = false;
          m_velocity.y += m_jump_speed;
        } else {
          m_velocity.y = 0;
        }
      }

      // update velocity 
      m_velocity = move_and_slide(m_velocity, Vector3(0, 1, 0), 0.05, 2, \
          Math::deg2rad(55.0));
      if (this->is_on_floor()) {
        m_velocity = Vector3(0, 0, 0);
      }

      break;
    }
  }
}



