/**
* @file Actor.hpp
* @brief The Actor class.
* @author Dominique LaSalle <dominique@solidlake.com>
* Copyright 2018
* @version 1
* @date 2018-09-25
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


#ifndef THESPIAN_ACTOR_HPP
#define THESPIAN_ACTOR_HPP

#include "scene/3d/physics_body.h"

class Actor : public KinematicBody
{
  GDCLASS(Actor, KinematicBody);

  public:
  Actor();

  String get_display_name() const;

  void set_display_name(
      String p_name);

  float get_max_look_angle() const;

  void set_max_look_angle(
      float p_angle);

  void set_head_node(
      NodePath const & p_head);

  NodePath get_head_node() const;

  void look_up(
      float p_delta);

  float get_look_angle() const;

  void set_look_angle(
      float p_angle);

  Transform get_look_transform() const;

  void apply_impulse(
      Vector3 const delta_v);

  void set_velocity(
      Vector3 const vel);

  Vector3 get_velocity() const;


  protected:
    static void _bind_methods();

    void _notification(
        int notification);

    void _update_head();

  private:
    String m_display_name;
    float m_max_look_angle;
    NodePath m_head_node_path;

    // state variables
    Vector3 m_velocity;
    float m_look_angle;
 
};



#endif
