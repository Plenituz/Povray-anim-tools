#ifndef(TEXTANIM_INC_VAR)
#declare TEXTANIM_INC_VAR = 1;

#include "colors.inc"
#include "Debug.inc" 
#include "KeyframeDA.inc"

#declare CaracPadding = 0.1;
#declare SpaceSize = 0.3;


#macro TextAnim(Text, Percent)
//#local Percent = Percent *0.9; 
//wrap everything in a union so you can call the macro dire
union{ 

    #local StrLen = strlen(Text);
    #local CurrentCarac = StrLen * Percent;
    #local CurrentCaracRounded = floor(CurrentCarac);
    
    #local CurrentPosOffset = 0;

    #local Char = substr(Text, CurrentCaracRounded, 1)
    
    #for(I, 1, CurrentCaracRounded-1)
        #local C = substr(Text, I, 1);
        #if(C = " ")
           #local CurrentPosOffset = CurrentPosOffset + SpaceSize;
        #else
           #local ThisCaraText = AnimCarac(C, 1)  //using AnimCarac at 100% so the 
           //user can override the default macro and the text will still look
           //as intended         
            
            #local SizeX = max_extent(ThisCaraText).x - min_extent(ThisCaraText).x;
            object{
                AnimCarac(C, 1)
                translate x*CurrentPosOffset
            }
            #local CurrentPosOffset = CurrentPosOffset + SizeX + CaracPadding;
        #end
    #end
    
    object{
       AnimCarac(Char, CurrentCarac - CurrentCaracRounded)
       translate x*CurrentPosOffset
    }
}//end union    
        
#end


//example usage : 
/*
#macro AnimCarac(Carac, PercentOnLocalAnim)
    #local Scale = Keyframeholder_GetValueAtTime("Scale",PercentOnLocalAnim,"AnticipateOvershoot");
    #local Rotation = Keyframeholder_GetValueAtTime("Rotation",PercentOnLocalAnim,"AnticipateOvershoot");
     text {
        ttf "GROBOLD.ttf" Carac  0, 0
        pigment { Red }
        scale Scale
        rotate z*Rotation
    }
#end

#if(clock = 0)
    Keyframeholder_Create ( "Scale")
    Keyframeholder_Add ("Scale", 0.4, 0)
    Keyframeholder_Add ("Scale", 1, 1)
    Keyframeholder_Finish ("Scale")
     
    Keyframeholder_Create ( "Rotation")
    Keyframeholder_Add ("Rotation", -30, 0)
   // Keyframeholder_Add ("Rotation", 30, 0.5)
    Keyframeholder_Add ("Rotation", 0, 1)
    Keyframeholder_Finish ("Rotation") 
    setAttr("Rotation", "factor", 0.4)
#end
 
object{
    TextAnim(" salut ca va  ", clock)
    translate x*-3
}

    
                        
 camera
{
    location <0, 0, -10>  
    look_at 0 
    angle 60
    right x * image_width / image_height
    up y
} 

light_source { <500,500,-1000> White }  
*/
 
                        
#end // end TextAnim.inc