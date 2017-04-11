#ifndef(POVTICULAR_INC_VAR)
#declare POVTICULAR_INC_VAR = 1; 

#include "DiskArray.inc" 
#include "IO.inc"
#include "colors.inc"


#macro RandomReal(sd,mn,mx)
  (rand(sd)*(mx-mn) + mn)
#end

#macro Povticular_CreateEmitter(Name) 
    #local Attr = Povticular_Name(Name)
    setAttr(Attr, "emitterPos", <0, 0, 0>)
    setAttr(Attr, "maxCount", 1000)                 //max number of particle
    setAttr(Attr, "particlePerFrame", 1)           //number of particle emitted every frame
    setAttr(Attr, "framerate", 24)                  //render framerate, too calculate life and stuff
    setAttr(Attr, "particleLifeTimeInFrame", 10)    //after X frame the particle dies  
    setAttr(Attr, "baseVelocity", <0, -1, 0>)  
    
    DiskArray_CreateDiskArray(Povticular_PosArray(Name))
    DiskArray_CreateDiskArray(Povticular_VelArray(Name))
#end

#macro Povticular_AddParticle(Name)
    #local PosArr = Povticular_PosArray(Name) 
    #local VelArr = Povticular_VelArray(Name)
    #local Attr = Povticular_Name(Name) 
    #local Seed = seed(19191);
    
    #local EmitterPos = getVectAttr(Attr, "emitterPos");
    //#local BaseVel = getVectAttr(Attr, "baseVelocity");
    #local BaseVel = <RandomReal(sd, 0, 5), RandomReal(sd, 0, 5), RandomReal(sd, 0, 5)>;
    //#debug concat("\n", vstr(3, BaseVel, ",", 0, -1), "\n")
    DiskArray_Add(PosArr, EmitterPos)
    DiskArray_Add(VelArr, BaseVel) 
#end

#macro Povticular_UpdateParticle(Name, Index)
    #local PosArr = Povticular_PosArray(Name) 
    #local VelArr = Povticular_VelArray(Name)
    #local Attr = Povticular_Name(Name)
    #local Framerate = getFloatAttr(Attr, "framerate");
                                     
    //update vel (constant forces)
    #local Vel = DiskArray_GetVect(VelArr, Index);
    //#local Vel = Vel + <0, 1, 0>;      
    //apply vel
    #local Pos = DiskArray_GetVect(PosArr, Index);
    #local Pos = Pos + Vel*(1/Framerate);
    
    DiskArray_Set(VelArr, Index, Vel)    
    DiskArray_Set(PosArr, Index, Pos)
#end

#macro Povticular_ParticlePos(Name, Index)
    #local PosArr = Povticular_PosArray(Name)
    #local Pos = DiskArray_GetVect(PosArr, Index);
    Pos;
#end

#macro Povticular_Name(Name)
    #local N = concat(Name, "_settings")
    N
#end                          

#macro Povticular_PosArray(Name)
    #local N = concat(Name, "_pos")
    N
#end

#macro Povticular_VelArray(Name)
    #local N = concat(Name, "_vel")
    N
#end 

#macro Povticular_Render(Name)
    #local ParticleCount = DiskArray_GetCount(Povticular_PosArray(Name))
    #for(I, 0, ParticleCount-1)
        #local Pos = Povticular_ParticlePos(Name, I);
        sphere{
            Pos, 0.1   
            pigment{ Red } 
        }
    #end
#end

#macro Povticular_CalculateFrame(Name)    
    #local Attr = Povticular_Name(Name)
    #local NewParticleCount = getFloatAttr(Attr, "particlePerFrame");
    #for(I, 0, NewParticleCount-1)
        Povticular_AddParticle(Name)
    #end                            
    
    #local ParticleCount = DiskArray_GetCount(Povticular_PosArray(Name))
    #for(I, 0, ParticleCount-1)         
        Povticular_UpdateParticle(Name, I)
    #end
#end

#end // end povticular.inc