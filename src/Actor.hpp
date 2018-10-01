/**
* @file Actor.hpp
* @brief The Actor class.
* @author Dominique LaSalle <dominique@solidlake.com>
* Copyright 2018
* @version 1
* @date 2018-09-25
*/


#ifndef THESPIAN_ACTOR_HPP
#define THESPIAN_ACTOR_HPP

#include "scene/3d/physics_body.h"

class Actor : public KinematicBody
{
  GDCLASS(Actor, KinematicBody);

  public:
  Actor();

  void set_motion(
      Vector2 p_motion);

  Vector2 get_motion() const;

  float get_run_speed() const;

  void set_run_speed(
      float p_speed);

  float get_jump_speed() const;

  void set_jump_speed(
      float p_speed);

  bool has_air_control() const;

  void set_air_control(
      bool p_control);

  float get_max_look_angle() const;

  void set_max_look_angle(
      float p_angle);

  void jump();

  void look_up(
      float p_delta);

  float get_look_angle() const;

  void set_look_angle(
      float p_angle);

  Transform get_look_transform() const;

  protected:
    static void _bind_methods();

    void _notification(
        int notification);

  private:
    bool m_air_control;
    float m_run_speed;
    float m_jump_speed;
    float m_max_look_angle;

    // state variables
    Vector3 m_velocity;
    Vector2 m_motion;
    bool m_jumping;
    float m_look_angle;
 
};



#endif
