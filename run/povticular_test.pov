#include "Povticular.inc"   
#include "Affector.inc"

#declare sd = seed(frame_number*5667);
                           
#if(clock = 0) 
    Povticular_CreateEmitter("T")
    Affector_Create("aff", "ConstantForce") 
    setAttr("aff", "force", <0, -1, 0>)
  
    Povticular_AddAffector("T", "aff")
#end                   




#if(0)
    #for(I, 0, 48) 
        #debug concat("\nframe=", str(I, 0, 0), "\n")
        Povticular_CalculateFrame("T")
        Povticular_Render("T")
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