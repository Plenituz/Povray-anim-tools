 #include "colors.inc"

light_source{ <0,10,-10> White }
camera{ 
    right x*image_width / image_height
    up y
    location <-5, 3, -10> 
    look_at < 0,0,0 > 
    angle 80
}

#declare SPEED_MULT = 2; 
#declare EARTH_ARROUND_SUN_ROT = 300*clock*SPEED_MULT;
#declare EARTH_ARROUND_SELF_ROT = 1000*clock*SPEED_MULT;
#declare MOON_ARROUND_SELF_ROT = 28*clock*SPEED_MULT; 
#declare MOON_ARROUND_EARTH_ROT = 600*clock*SPEED_MULT; 
 

#declare Sun = 
sphere{
    0, 1
    pigment{marble}
}

#declare Earth = 
sphere{
    0, 0.3
    pigment{bozo}
}

#declare Moon = 
sphere{
    0, 0.1
    pigment{marble}
}

#declare MoonOrbit = 
union{
    object{Moon rotate <0,MOON_ARROUND_SELF_ROT, 0> translate x*0.5 }
    rotate <0, MOON_ARROUND_EARTH_ROT, 0> 
}

#declare MoonAndEarth = 
union{
    object{Earth rotate <0, EARTH_ARROUND_SELF_ROT, 0>}
    object{MoonOrbit}
    translate x*2 
    rotate <0, EARTH_ARROUND_SUN_ROT, 0>
}


object{Sun}

    object{MoonAndEarth}
    
