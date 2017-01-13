#include "joy2servo.h"
#include "joystick.h"


Joystick_to_servo calculate_joystick_to_servo(Digital_position digital_joystick) {


  Joystick_to_servo return_struct;

  //idle position
  if (digital_joystick.posX == 0 && digital_joystick.posY == 0) {

    return_struct.w_1 = 0;
    return_struct.w_2 = 0;
  }

  //forward
  else if (digital_joystick.posX == 0 && digital_joystick.posY != 0) {

    return_struct.w_1 = digital_joystick.posY;
    return_struct.w_2 = digital_joystick.posY;
  }

  //rotation right
  else if (digital_joystick.posX > 0 && digital_joystick.posY == 0) {

    return_struct.w_1 = digital_joystick.posX;
    return_struct.w_2 = 0;
  }

  //rotation left
  else if (digital_joystick.posX < 0 &&  digital_joystick.posY == 0) {

    return_struct.w_1 = 0;
    return_struct.w_2 = (-1) * digital_joystick.posX;
  }

  //forward and rotation
  else if (digital_joystick.posY > 0) {
	  
    return_struct.w_1 = digital_joystick.posY;
    return_struct.w_2 = digital_joystick.posY;

    if (digital_joystick.posX > 0) {

      return_struct.w_2 = return_struct.w_2 - digital_joystick.posX;
    }
    else {

      return_struct.w_1 = return_struct.w_1 + digital_joystick.posX;
    }
  }

  //backward and rotation
  else if (digital_joystick.posY < 0) {

    return_struct.w_1 = digital_joystick.posY;
    return_struct.w_2 = digital_joystick.posY;

    if (digital_joystick.posX > 0) {

      return_struct.w_2 = return_struct.w_2 + digital_joystick.posX;
    }
    else {

      return_struct.w_1 = return_struct.w_1 - digital_joystick.posX;
    }
  }


  return return_struct;
}