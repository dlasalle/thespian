/**
* @file GroundActor.hpp
* @brief The GroundActor class.
* @author Dominique LaSalle <dominique@solidlake.com>
* Copyright 2018
* @version 1
* @date 2018-10-22
*/


#ifndef THESPIAN_GROUND_ACTOR_HPP
#define THESPIAN_GROUND_ACTOR_HPP

#include "Actor.hpp"

class GroundActorSignals
{
  public:
  static GroundActorSignals * get_singleton();

  StringName jumped_signal;
  StringName start_running_signal;
  StringName stop_running_signal;
  GroundActorSignals();

  private:
  static GroundActorSignals * m_singleton;
};


class GroundActor : public Actor 
{
  GDCLASS(GroundActor, Actor);

  public:
  GroundActor();

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

  float get_air_resistance() const;

  void set_air_resistance(
      float resistance);

  void jump();


  protected:
    static void _bind_methods();

    void _notification(
        int notification);

  private:
    bool m_air_control;
    float m_run_speed;
    float m_jump_speed;
    float m_air_resistance;

    // state variables
    Vector2 m_motion;
    bool m_jumping;
    bool m_is_running;
 
};





#endif
