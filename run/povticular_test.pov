#include "Povticular.inc"   
#include "Affector.inc"
#include "Keyframe.inc"

#declare sd = seed(frame_number*5667);
                           
#if(clock = 0) 
    Povticular_CreateEmitter("T")
    Affector_Create("aff", "ConstantForce") 
    setAttr("aff", "force", <0, -1, 0>)
    
    Affector_Create("Aff2", "SizeOverLife")
    setAttr("Aff2", "interpolator", "\"SmoothStep\"")
    setAttr("Aff2", "keyframeHolder", "\"mKey\"")
    Keyframeholder_Create("mKey")
    Keyframeholder_Add("mKey", 0, 0)
    Keyframeholder_Add("mKey", 0.5, 12)
    Keyframeholder_Add("mKey", 0.1, 24)
    
    Povticular_AddAffector("T", "Aff2")
    Povticular_AddAffector("T", "aff")
#end                   
  
#include "myCache.inc"

  


#if(1)
    #for(I, 0, 5) 
        #debug concat("\nframe=", str(I, 0, 0), "\n")
        //Povticular_CalculateFrame("T")
        //Povticular_Render("T")
        //Povticular_CacheFrame("T", I)
        object{myCache[I]}
    #end
#else
    Povticular_CalculateFrame("T")
    Povticular_Render("T")
#end


                             



light_source{ <0,10,-10> White }
camera{ 
    right x*image_width / image_height
    up y
    location <-5, 3, -10> 
    look_at < 0,0,0 > 
    angle 120
}  

//trajector d'anim etc