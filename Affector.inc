#ifndef(AFFECTOR_INc_VAR)
#declare AFFECTOR_INc_VAR = 1;  

#include "strings.inc"
#include "Keyframe.inc"

#macro Affector_Create(Name, Type)
    setAttr(Name, "type", concat("\"", Type, "\""))
#end                             

#macro Affector_Affect(Name, Pos, Vel, Size, Life, Color)
    #local Type = getStrAttr(Name, "type")
    #local Func = concat("Affector_", Type, "(Name, Pos, Vel, Size, Life, Color)");
    Parse_String(Func)
#end

#macro Affector_ConstantForce(Name, Pos, Vel, Size, Life, Color)
    #local Force = getVectAttr(Name, "force");
    #declare Vel = Vel + Force;
#end 

#macro Affector_SizeOverLife(Name, Pos, Vel, Size, Life, Color)
    #local Interpolator = getStrAttr(Name, "interpolator")
    #local KeyframeHolder = getStrAttr(Name, "keyframeHolder")   
    #declare Size = Keyframeholder_GetValueAtTime(KeyframeHolder, Life, Interpolator);
   // DebugStr(concat("life=", str(Life, 0, 0), " Size=", str(Size, 0, -1)))
#end


#end // end Affector.inc