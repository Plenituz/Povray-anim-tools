#include "Povticular.inc"   
#declare sd = seed(frame_number);
                           
#if(clock = 0)
    Povticular_CreateEmitter("T")
#end



#if(0)
    #for(I, 0, 24)
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
    angle 80
}