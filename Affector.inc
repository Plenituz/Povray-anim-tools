#ifndef(AFFECTOR_INc_VAR)
#declare AFFECTOR_INc_VAR = 1;  

#include "strings.inc"
#include "KeyframeDA.inc"

#macro Affector_Create(Name, Type)
    setAttr(Name, "type", concat("\"", Type, "\""))
#end                             

#macro Affector_Affect(Name, Pos, Vel, Size, Life, Color, MaxLife, CustomData)
    #local Type = getStrAttr(Name, "type")
    #local Func = concat("Affector_", Type, "(Name, Pos, Vel, Size, Life, Color, MaxLife, CustomData)");
    Parse_String(Func)
#end

#macro Affector_ConstantForce(Name, Pos, Vel, Size, Life, Color, MaxLife, CustomData)
    #local Force = getVectAttr(Name, "force");
    #declare Vel = Vel + Force;
#end 

//setAttr with the name of a keyframe setting the value between 0 and 1
#macro Affector_SizeOverLife(Name, Pos, Vel, Size, Life, Color, MaxLife, CustomData)
    #local Interpolator = getStrAttr(Name, "interpolator")
    #local KeyframeHolder = getStrAttr(Name, "keyframeHolder")   
    #declare Size = Keyframeholder_GetValueAtTime(KeyframeHolder, Life/MaxLife, Interpolator);
   // DebugStr(concat("life=", str(Life, 0, 0), " Size=", str(Size, 0, -1)))
#end 

#macro Affector_Utils_ProjectVectorOntoVector(U, V)
    #local Dot = vdot(U, V);
    #local VSquared = vlength(V)*vlength(V);
    #local Factor = Dot/VSquared;
    #local Return = Factor*V;
    Return;
#end

#macro Affector_Utils_GetClosestPoint(PointLine, VectorLine, OtherPoint)
    #local Vect = OtherPoint - PointLine;
    #local Proj = Affector_Utils_ProjectVectorOntoVector(Vect, VectorLine);
   /* DrawVectorColor(PointLine, VectorLine, Red)
    DrawVectorColor(PointLine, Proj, Yellow)
    DrawVectorColor(PointLine, Vect, Blue)
    DrawPoint(OtherPoint)
    DrawPoint(Proj + PointLine) */
    #local P = Proj + PointLine;
    P;
#end

#macro Affector_Reactor(Name, Pos, Vel, Size, Life, Color, MaxLife, CustomData)
    #local Point = getVectAttr(Name, "point");
    #local LineVector = getVectAttr(Name, "lineVector");
    #local Strength = getFloatAttr(Name, "strength"); 
    
    
    #local PointOnLine = Affector_Utils_GetClosestPoint(Point, LineVector, Pos);
    #local Vect = PointOnLine - Pos;
    #local Vect = Vect/vlength(Vect);
    #local Vect = Vect * Strength;
    
    #declare Vel = Vel + Vect;
#end


#end // end Affector.inc