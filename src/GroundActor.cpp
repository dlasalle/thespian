/**
* @file GroundActor.cpp
* @brief Implementation of the GroundActor class.
* @author Dominique LaSalle <dominique@solidlake.com>
* Copyright 2018
* @version 1
* @date 2018-10-22
*/


#include "GroundActor.hpp"

#include "core/math/math_funcs.h"
#include "core/math/math_defs.h"

/******************************************************************************
* PUBLIC STATIC METHODS *******************************************************
******************************************************************************/

GroundActorSignals * GroundActorSignals::m_singleton = nullptr;

GroundActorSignals * GroundActorSignals::get_singleton()
{
  if (m_singleton == nullptr) {
    m_singleton = new GroundActorSignals;
  }

  return m_singleton;
}

GroundActorSignals::GroundActorSignals() :
    jumped_signal(StaticCString::create("sig_jumped")),
    start_running_signal(StaticCString::create("sig_start_run")),
    stop_running_signal(StaticCString::create("sig_stop_run"))
{
  // do nothing
}




/******************************************************************************
* CONSTRUCTORS / DESTRUCTOR ***************************************************
******************************************************************************/

GroundActor::GroundActor() :
  Actor(),
  m_air_control(false),
  m_run_speed(1.0),
  m_jump_speed(1.0),
  m_air_resistance(0.0),
  m_motion(0, 0),
  m_jumping(false),
  m_is_running(false)
{
  // do nothing
}


/******************************************************************************
* PUBLIC METHODS **************************************************************
******************************************************************************/

void GroundActor::set_motion(
    Vector2 const p_motion)
{
  m_motion = p_motion.normalized();
}


Vector2 GroundActor::get_motion() const
{
  return m_motion;
}


float GroundActor::get_run_speed() const
{
  return m_run_speed;
}


void GroundActor::set_run_speed(
    float const p_speed)
{
  m_run_speed = p_speed;
}


float GroundActor::get_jump_speed() const
{
  return m_jump_speed;
}


void GroundActor::set_jump_speed(
    float const p_speed)
{
  m_jump_speed = p_speed;
}


bool GroundActor::has_air_control() const
{
  return m_air_control;
}


void GroundActor::set_air_control(
    bool const p_control)
{
  m_air_control = p_control;
}


void GroundActor::jump()
{
	if (this->is_on_floor()) {
		m_jumping = true;
  }
}


float GroundActor::get_air_resistance() const
{
  return m_air_resistance;
}


void GroundActor::set_air_resistance(
    float const p_resistance)
{
  m_air_resistance = p_resistance;
}



/******************************************************************************
* STATIC PROTECTED METHODS ****************************************************
******************************************************************************/

void GroundActor::_bind_methods()
{
  ClassDB::bind_method(D_METHOD("get_motion"), &GroundActor::get_motion);
  ClassDB::bind_method(D_METHOD("set_motion", "motion"), &GroundActor::set_motion);

  ClassDB::bind_method(D_METHOD("get_run_speed"), &GroundActor::get_run_speed);
  ClassDB::bind_method(D_METHOD("set_run_speed", "speed"), &GroundActor::set_run_speed);

  ClassDB::bind_method(D_METHOD("get_jump_speed"), &GroundActor::get_jump_speed);
  ClassDB::bind_method(D_METHOD("set_jump_speed", "speed"), &GroundActor::set_jump_speed);

  ClassDB::bind_method(D_METHOD("has_air_control"), &GroundActor::has_air_control);
  ClassDB::bind_method(D_METHOD("set_air_control", "enabled"), &GroundActor::set_air_control);

  ClassDB::bind_method(D_METHOD("get_air_resistance"), &GroundActor::get_air_resistance);
  ClassDB::bind_method(D_METHOD("set_air_resistance", "resistance"), &GroundActor::set_air_resistance);

  ClassDB::bind_method(D_METHOD("jump"), &GroundActor::jump);

	ADD_SIGNAL(MethodInfo(GroundActorSignals::get_singleton()->jumped_signal));
	ADD_SIGNAL(MethodInfo(GroundActorSignals::get_singleton()->start_running_signal));
	ADD_SIGNAL(MethodInfo(GroundActorSignals::get_singleton()->stop_running_signal));

	ADD_PROPERTY(PropertyInfo(Variant::REAL, "run_speed", PROPERTY_HINT_RANGE, \
      "0.1,100.0"), "set_run_speed", "get_run_speed");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "jump_speed", PROPERTY_HINT_RANGE, \
      "0.1,100.0"), "set_jump_speed", "get_jump_speed");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "air_control"), \
      "set_air_control", "has_air_control");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "air_resistance"), \
      "set_air_resistance", "get_air_resistance");
}


/******************************************************************************
* PROTECTED METHODS ***********************************************************
******************************************************************************/

void GroundActor::_notification(
    int const p_notification)
{
  switch (p_notification)
  {
    case NOTIFICATION_PHYSICS_PROCESS: {
      float const delta = get_physics_process_delta_time();

      Transform trans = get_global_transform();
      trans.set_origin(Vector3(0,0,0));

      Vector3 const relative_motion = trans.xform(Vector3(m_motion.x, 0, m_motion.y));
      // update position
      if (is_on_floor() || has_air_control()) {
        Vector3 const velocity( \
            relative_motion.x*m_run_speed, 0, relative_motion.z*m_run_speed);
        set_velocity(velocity);
      }

      // update running status
      if (!m_is_running) {
        if (is_on_floor() && m_motion.length() > 0) {
          m_is_running = true;
          emit_signal(GroundActorSignals::get_singleton()->start_running_signal);
        }
      } else {
        if (!is_on_floor() || m_motion.length() == 0) {
          m_is_running = false;
          emit_signal(GroundActorSignals::get_singleton()->stop_running_signal);
        }
      }

      if (is_on_floor()) {
        // handle jump
        if (m_jumping) {
          emit_signal(GroundActorSignals::get_singleton()->jumped_signal);
          m_jumping = false;
          apply_impulse(Vector3(0, m_jump_speed, 0));
        }
      } else {
        // handle air resistance 
        if (m_air_resistance > 0) {
          // handle air resistance
          float const v = get_velocity().length();
          // crude approximation of wind resistance which results in a terminal
          // of around 53 m/s (that of a human) for 1.0
          float const drag = 0.5 * v*v * 0.007 * m_air_resistance;
          apply_impulse(-get_velocity().normalized()*drag*delta);
        }
      }

      break;
    }
  }
}


