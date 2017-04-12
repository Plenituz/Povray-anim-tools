#ifndef(AFFECTOR_INc_VAR)
#declare AFFECTOR_INc_VAR = 1;  

#include "strings.inc"

#macro Affector_Create(Name, Type)
    setAttr(Name, "type", concat("\"", Type, "\""))
#end                             

#macro Affector_Affect(Name, Pos, Vel)
    #local Type = getStrAttr(Name, "type")
    #local Func = concat("Affector_", Type, "(Name, Pos, Vel)");
    Parse_String(Func)
#end

#macro Affector_ConstantForce(Name, Pos, Vel)
    #local Force = getVectAttr(Name, "force");
    #declare Vel = Vel + Force;
#end


#end // end Affector.inc