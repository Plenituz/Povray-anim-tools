#ifndef(AFFECTOR_INc_VAR)
#declare AFFECTOR_INc_VAR = 1;  

#include "strings.inc"
#include "KeyframeDA.inc"

#macro Affector_Create(Name, Type)
    setAttr(Name, "type", concat("\"", Type, "\""))
#end                             

#macro Affector_Affect(Index, Name, Pos, Vel, Size, Life, Color, MaxLife, CustomData)
    #local Type = getStrAttr(Name, "type")
    #local Func = concat("Affector_", Type, "(Index, Name, Pos, Vel, Size, Life, Color, MaxLife, CustomData)");
    Parse_String(Func)
#end

#macro Affector_AffectRender(Index, Name, Pos, Vel, Size, Life, Color, MaxLife, CustomData, RenderString)
    #local Type = getStrAttr(Name, "type")
    #local Func = concat("RenderAffector_", Type, "(Index, Name, Pos, Vel, Size, Life, Color, MaxLife, CustomData, RenderString)");
    Parse_String(Func)
#end 

#macro RenderAffector_RandomColor(Index, Name, Pos, Vel, Size, Life, Color, MaxLife, CustomData, RenderString)
    #declare RenderString = "pigment{color rgb <Affector_Utils_RandomReal(sd,0,1),Affector_Utils_RandomReal(sd,0,1),Affector_Utils_RandomReal(sd,0,1)>}"
#end

#macro Affector_ConstantForce(Index, Name, Pos, Vel, Size, Life, Color, MaxLife, CustomData)
    #local Force = getVectAttr(Name, "force");
    #declare Vel = Vel + Force;
#end

#macro Affector_RandomBaseVel(Index, Name, Pos, Vel, Size, Life, Color, MaxLife, CustomData)
    #if(Life = 1)
        #declare Vel = <Affector_Utils_RandomReal(sd, -5, 5), Affector_Utils_RandomReal(sd, -5, 5), Affector_Utils_RandomReal(sd, -5, 5)>; 
    #end
#end

#macro Affector_RandomBaseVelUpward(Index, Name, Pos, Vel, Size, Life, Color, MaxLife, CustomData)
    #local Strength = getFloatAttr(Name, "strength");
    #if(Life = 1)
        #declare Vel = <Affector_Utils_RandomReal(sd, -5, 5), Affector_Utils_RandomReal(sd, -5, 5), Affector_Utils_RandomReal(sd, -5, 5)>;
        #declare Vel = Vel * Strength;
        #if(Vel.y < 0)
            #declare Vel = <Vel.x, Vel.y*-1, Vel.z>;
        #end 
    #end
#end 

#macro Affector_VelocityDamp(Index, Name, Pos, Vel, Size, Life, Color, MaxLife, CustomData)
    #local Damp = getFloatAttr(Name, "damp");
    #declare Vel = Vel * Damp;
#end

//setAttr with the name of a keyframe setting the value between 0 and 1
#macro Affector_SizeOverLife(Index, Name, Pos, Vel, Size, Life, Color, MaxLife, CustomData)
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

#macro Affector_Utils_RandomReal(sd,mn,mx)
  (rand(sd)*(mx-mn) + mn)
#end

#macro Affector_Reactor(Index, Name, Pos, Vel, Size, Life, Color, MaxLife, CustomData)
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