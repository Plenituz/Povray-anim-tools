#include "colors.inc" 
#include "Keyframe.inc" 

/*
  Add the path to the Keyframes.inc, IO.inc and DiskArray.inc to the command line
  by doing +L"path/to/incs"


*/

light_source{ <0,10,-10> White }
camera{ 
    right x*image_width / image_height
    up y
    location <-5, 3, -10> 
    look_at < 0,0,0 > 
    angle 80
}      

#declare EXTENSION_DIST = 5*clock; 
#declare ECHELLE_ROT = 80*clock;   
#declare TOUR_ROT = 360*clock;


#declare Echelle = box{
    <-1, 0, 0.1>, <1, 5, -0.1> 
    pigment{Red}
}
 
#declare Extension = box{
    <-1, 0, 0.1>, <1, 5, -0.1> 
    pigment{Blue}
    translate <0, 0, -0.15>
}

#declare Tour = cylinder{
    0, <0, -1, 0>, 2
    pigment{bozo}    
} 

#declare Hull = box{
    -1, 1 translate <0, -2, 0> scale <5, 1, 3> translate x*2
    pigment{Yellow}    
}
                  

#declare Echelle_Prol = 
union{
    object{Echelle}
    object{Extension translate y*EXTENSION_DIST}    
}

#declare Tour_Ech_Prol = 
union{
    object{Tour}
    object{Echelle_Prol rotate x*ECHELLE_ROT}    
}      

#declare Camion = 
union {
    object{Hull}
    object{Tour_Ech_Prol rotate y*TOUR_ROT}
}                                      

//object{Camion} 

#if(clock = 0)
    KeyframeHolder_Create("mKey")    
    //left handle default at <1,1> before it's always the top right
    //right handle default at <0,0> because it's always the bot left
    KeyFrameHolder_AddWithCustomCurve("mKey", 0, 0, <1, 1>, <0, 0>)
    KeyFrameHolder_AddWithCustomCurve("mKey", 0, 10, <1, 1>, <0.5, 0>)
    KeyFrameHolder_AddWithCustomCurve("mKey", 2, 20, <0.5, 1>, <0, 0>)
    KeyFrameHolder_AddWithCustomCurve("mKey", 1, 40, <1, 1>, <0, 0>)
    KeyFrameHolder_AddWithCustomCurve("mKey", 0, 50, <1, 1>, <0, 0>)
   
#end  
#local Interpolation = "CustomCurve"

#for(I, 0, 70)                            
    #local Value = KeyFrameHolder_GetValueAtTime("mKey", I, Interpolation);
    #debug concat("\nvalue=", str(Value, 0,-1), " i=", str(I, 0, -1), "\n")
    sphere{
        <I/5, Value, 0>, .1
        pigment{Red}
    }
#end 


/*
#local Value = KeyFrameHolder_GetValueAtTime("mKey", frame_number, Interpolation);
#debug concat("\nvalue=", str(Value, 0,-1), " i=", str(frame_number, 0, -1), "\n")
sphere{
    <frame_number/5, Value, 0>, .1
    pigment{Red}
} 
*/


/*
#local Start = <0, 0, 0>;
#local End = <1, 1, 0>;
#local FHandle = <0, 0>;
#local SHandle = <-1, 1>;
#local Res = 50;

sphere{Start, 0.1 pigment{Red}}
sphere{End, 0.1 pigment{Red}}
sphere{FHandle, 0.1 pigment{Blue}}
sphere{SHandle, 0.1 pigment{Blue}}

#for(I, 0, Res)
    #local T = I/Res;
    #local Pos = KeyFrameHolder_BezierPoint2D(T, Start, FHandle, SHandle, End);
    sphere{Pos, 0.01 pigment{Yellow}}
#end 
*/