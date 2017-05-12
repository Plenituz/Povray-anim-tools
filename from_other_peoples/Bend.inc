/*************************************************************************
        OBJECT BENDER INCLUDE FILE FOR PERSISTENCE OF VISION 3.x
**************************************************************************

Created by Chris Colefax, 13 June 1997
Updated 21 July 1997: fixed bend_start bug
Updated 13 July 1998: updated for POV-Ray 3.1,
                      added twist_angle option,  
                      fixed Domain Error caused by some compilers

See "Bend.htm" for more information

found : http://www.oocities.org/ccolefax/bend.html

*************************************************************************/

// CHECK BEND OBJECT AND REQUIRED PARAMETERS
// *****************************************
   #declare _BN_tempver = version; #version 3.0;
   #declare _BN_nobend = false;
   #ifndef (bend_object)  #declare _BN_nobend = true; #declare _BN_warning = "You must specify a bend_object!" #end
   #ifndef (object_axis1) #declare _BN_axis1  = -y;   #else #declare _BN_axis1 = object_axis1 * <1, 1, 1>; #end
   #ifndef (object_axis2) #declare _BN_axis2  =  y;   #else #declare _BN_axis2 = object_axis2 * <1, 1, 1>; #end
   #ifndef (bend_angle)   #declare _BN_angle  = 45;   #else #declare _BN_angle = bend_angle;               #end
   #ifndef (twist_angle)  #declare _BN_tangle = 0;    #else #declare _BN_tangle = twist_angle;             #end

   #declare _BN_axisvec = _BN_axis2 - _BN_axis1;
   #if (_BN_axisvec.x = 0 & _BN_axisvec.y = 0 & _BN_axisvec.z = 0) #declare _BN_nobend = true; #declare _BN_warning = "object_axis1 and object_axis2 must be different!" #end

   #if (_BN_nobend) #warning concat (_BN_warning, "\n") #else

// DETERMINE BEND DIRECTION
// ************************
   #ifndef (bend_direction) #declare _BN_dir = x; #else #declare _BN_dir = vnormalize(bend_direction * <1, 1, 1>); #end
   #declare _BN_roty = _BN_axisvec.x; #if (_BN_roty != 0 | _BN_axisvec.z != 0) #declare _BN_roty = degrees(atan2(_BN_roty, _BN_axisvec.z)); #end
   #declare _BN_rotx = vlength(_BN_axisvec * <1, 0, 1>); #if (_BN_rotx != 0 | _BN_axisvec.y != 0) #declare _BN_rotx = degrees(atan2(_BN_rotx, _BN_axisvec.y)); #end
   #declare _BN_dir = vrotate(vrotate(_BN_dir, -y * _BN_roty), -x * _BN_rotx);

// DON'T BEND OBJECT IF ANGLE IS 0
// *******************************
   #if (abs(_BN_dir.y) = 1) #declare _BN_angle = 0; #end
   #if (_BN_angle = 0 & _BN_tangle = 0) object {bend_object} #else

// DETERMINE OTHER PARAMETERS
// **************************
   #declare _BN_ospin = _BN_dir.x; #if (_BN_ospin !=0 | _BN_dir.z != 0) #declare _BN_ospin = degrees(atan2(_BN_dir.x, _BN_dir.z)); #end
   #if (_BN_angle < 0) #declare _BN_angle = abs(_BN_angle); #declare _BN_ospin = _BN_ospin + 180; #end
   #if (_BN_angle > 360) #warning "bend_angle cannot be larger than 360 degrees!\n" #declare _BN_angle = 360; #end
   #if (_BN_angle > (abs(_BN_tangle) / 2)) #declare _BN_fangle = _BN_angle; #else #declare _BN_fangle = (abs(_BN_tangle) / 2); #end
   #ifndef (bend_smoothness) #declare _BN_step = int(_BN_fangle / 3.6); #else #declare _BN_step = int(bend_smoothness * _BN_fangle / 360); #end
   #if (_BN_step < 6) #declare _BN_step = 1 / 6; #else #declare _BN_step = 1 / _BN_step; #end
   #ifndef (bend_start) #declare bend_start = 0; #end
   #if (bend_start < 0 | bend_start >= 1)
      #warning "bend_start must be bewteen 0 and 1!\n"
      #declare bend_start = 0; #end
   #ifndef (bend_finish) #declare bend_finish = 1; #end
   #if (bend_finish > 1 | bend_finish <= bend_start)
      #warning "bend_finish must be larger than bend_start, but less than 1!\n"
      #declare bend_finish = 1; #end
   #declare _BN_obase      = _BN_axis1 + (_BN_axisvec * bend_start);
   #declare _BN_oheight    = vlength(_BN_axisvec) * (bend_finish - bend_start);
   #declare _BN_clipheight = _BN_oheight * _BN_step / 2;
   #declare _BN_clipangle  = _BN_angle * _BN_step / 2;
   #if (_BN_angle > 0) #declare _BN_radius = (_BN_oheight * 180) / (pi * _BN_angle); #end

// CREATE BENT OBJECT
// ******************
   union {
   #declare _BN_basetransform = transform {
      translate -_BN_obase
      rotate -y * _BN_roty   rotate -x * _BN_rotx
      rotate -y * _BN_ospin}

   #declare _BN_clipplane = plane {y, 0 rotate  x * _BN_clipangle translate  y * _BN_clipheight}
   #declare _BN_clipplanes = intersection {
      object {_BN_clipplane} object {_BN_clipplane scale <1, -1, 1>}}

// FIRST SECTION (STRAIGHT)
// ************************
   object {bend_object
      transform _BN_basetransform
      clipped_by {_BN_clipplane}
      #if (_BN_angle > 0) translate -z * _BN_radius #end} 

// MIDDLE SECTION (BENT)
// *********************
   #declare _BN_count = _BN_step; #while (_BN_count < 1)
      object {bend_object
         transform _BN_basetransform
         translate -(y * _BN_count * _BN_oheight)
         rotate y * _BN_count * _BN_tangle
         clipped_by {_BN_clipplanes}
         #if (_BN_angle = 0)
            translate y * _BN_count * _BN_oheight
         #else
            translate -z * _BN_radius   rotate x * _BN_count * _BN_angle
         #end
         }
   #declare _BN_count = _BN_count + _BN_step; #end   

// LAST SECTION (STRAIGHT)
// ***********************
   object {bend_object
      transform _BN_basetransform
      translate -(y * _BN_oheight)
      rotate y * _BN_tangle
      clipped_by {object {_BN_clipplane scale <1, -1, 1>}}
      #if (_BN_angle = 0)
         translate y * _BN_oheight
      #else
         translate -z * _BN_radius   rotate x * _BN_angle
      #end
      } 

// POSITION OBJECT
// ***************
   #if (_BN_angle > 0) translate z * _BN_radius #end
   rotate y * _BN_ospin
   rotate x * _BN_rotx   rotate y * _BN_roty
   translate _BN_obase   
   }

   #end #end
   #version _BN_tempver;
