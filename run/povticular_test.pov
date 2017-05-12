#include "Povticular.inc"   
#include "Affector.inc"
#include "KeyframeDA.inc"

#declare sd = seed(frame_number*5667);
                           
#if(clock = 0) 
    Povticular_CreateEmitter("T")
    Affector_Create("aff", "ConstantForce") 
    setAttr("aff", "force", <0, -1, 0>)
    
    Affector_Create("Aff2", "SizeOverLife")
    setAttr("Aff2", "interpolator", "\"SmoothStep\"")
    setAttr("Aff2", "keyframeHolder", "\"mKey\"")
    Keyframeholder_Create("mKey")
    Keyframeholder_Add("mKey", 0.1, 0)
    Keyframeholder_Add("mKey", 0.5, .25)
    Keyframeholder_Add("mKey", 0.1, 0.5)
    Keyframeholder_Add("mKey", 0.5, 0.75)
    Keyframeholder_Add("mKey", 0.1, 1)
    Keyframeholder_Finish("mKey")
    
    Keyframeholder_Create("emit")
    Keyframeholder_Add("emit", 0, 0)
    Keyframeholder_Add("emit", 10, 12)
    Keyframeholder_Add("emit", 0, 24)
    Keyframeholder_Finish("emit")
    
    
    Povticular_AddAffector("T", "Aff2")
    Povticular_AddAffector("T", "aff")
#end

#declare TF = Keyframeholder_GetValueAtTime("emit", frame_number, "AccelerateDeccelerate");                                 
setAttr(Povticular_Name("T"), "emitterPos", <TF, 0, 0>) 

#if(frame_number = 25)
    setAttr(Povticular_Name("T"), "particlePerFrame", 0)
#end
  


//#include "myCache.inc"  


#if(0)
    #for(I, 0, 5) 
        #debug concat("\nframe=", str(I, 0, 0), "\n")
        Povticular_CalculateFrame("T")
        Povticular_Render("T")
        //Povticular_CacheFrame("T", I)
       // object{myCache[I]}
    #end
#else       
    Povticular_CalculateFrame("T")
    Povticular_CacheFrame("T", frame_number)
    Povticular_Render("T")
    //object{myCache[frame_number]}
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