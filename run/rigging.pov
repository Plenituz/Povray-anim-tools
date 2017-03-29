#include "colors.inc" 
#include "Keyframe.inc"

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
    KeyFrameHolder_Add("mKey", 0, 0)
    KeyFrameHolder_Add("mKey", 0, 10)
    KeyFrameHolder_Add("mKey", 2, 20)
    KeyFrameHolder_Add("mKey", 1, 30)
    KeyFrameHolder_Add("mKey", 0, 50)
#end

#for(I, 0, 70)                            
    #local Value = KeyFrameHolder_GetValueAtTime("mKey", I);
    #debug concat("\nvalue=", str(Value, 0,-1), " i=", str(I, 0, -1), "\n")
    sphere{
        <I/5, Value, 0>, .1
        pigment{Red}
    }
#end