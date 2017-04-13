#ifndef(POVTICULAR_INC_VAR)
#declare POVTICULAR_INC_VAR = 1; 

#include "DiskArray2.inc" 
#include "DiskArray.inc" 
#include "IO.inc"
#include "colors.inc"
#include "Affector.inc"
#include "Array.inc" 

//http://news.povray.org/povray.general/thread/%3Cweb.4f05a8fc1cabb177be8719c80@news.povray.org%3E/


#macro RandomReal(sd,mn,mx)
  (rand(sd)*(mx-mn) + mn)
#end

#macro Povticular_CreateEmitter(Name) 
    #local Attr = Povticular_Name(Name)
    setAttr(Attr, "emitterPos", <0, 0, 0>)
    setAttr(Attr, "maxCount", 10)                 //max number of particle
    setAttr(Attr, "particlePerFrame", 10)           //number of particle emitted every frame
    setAttr(Attr, "framerate", 24)                  //render framerate, too calculate life and stuff
    setAttr(Attr, "particleLifeTimeInFrame", 24)    //after X frame the particle dies    
    
    DA_Create(Povticular_PosArray(Name), 100)
    DA_Create(Povticular_VelArray(Name), 100)
    DiskArray_CreateDiskArray(Povticular_AffectorArray(Name))
    DA_Create(Povticular_ColorArray(Name), 100)
    DA_Create(Povticular_SizeArray(Name), 100)
    DA_Create(Povticular_LifeArray(Name), 100)
    DA_Create(Povticular_MaxLifeArray(Name), 100)
    
    DA_LoadInMemory(Povticular_PosArray(Name))
    DA_LoadInMemory(Povticular_VelArray(Name))
   // DA_LoadInMemory(Povticular_AffectorArray(Name))
    DA_LoadInMemory(Povticular_ColorArray(Name))
    DA_LoadInMemory(Povticular_SizeArray(Name))
    DA_LoadInMemory(Povticular_LifeArray(Name))
    DA_LoadInMemory(Povticular_MaxLifeArray(Name)) 
#end

#macro Povticular_AddAffector(Name, AffectorName)  
    #local AffArr = Povticular_AffectorArray(Name)
    DiskArray_Add(AffArr, concat("\"", AffectorName, "\""))
#end

#macro Povticular_AddParticle(Name)
    #local Attr = Povticular_Name(Name) 
                                       
    #local MaxCount = getFloatAttr(Attr, "maxCount");
    #local Count = Povticular_ParticleCount(Name);
    #if(Count < MaxCount)
        #local Life = getFloatAttr(Attr, "particleLifeTimeInFrame");
        #local EmitterPos = getVectAttr(Attr, "emitterPos");
        #local BaseVel = <RandomReal(sd, -5, 5), RandomReal(sd, -5, 5), RandomReal(sd, -5, 5)>;
        
        DA_Add(Povticular_PosArray(Name), EmitterPos)
        DA_Add(Povticular_VelArray(Name), BaseVel)
        DA_Add(Povticular_ColorArray(Name), <1, 0.5, 0>)
        DA_Add(Povticular_SizeArray(Name), 0.1)
        DA_Add(Povticular_LifeArray(Name), 0)
        DA_Add(Povticular_MaxLifeArray(Name), Life)
    #end
#end

#macro Povticular_RemoveParticle(Name, Index)
    DA_Remove(Povticular_PosArray(Name), Index)
    DA_Remove(Povticular_VelArray(Name), Index)
    DA_Remove(Povticular_ColorArray(Name), Index)
    DA_Remove(Povticular_SizeArray(Name), Index)
    DA_Remove(Povticular_LifeArray(Name), Index)
    DA_Remove(Povticular_MaxLifeArray(Name), Index)
#end

#macro Povticular_UpdateParticle(Name, Index) 
    #local PosArr = Povticular_PosArray(Name) 
    #local VelArr = Povticular_VelArray(Name)
    #local LifeArr = Povticular_LifeArray(Name) 
    #local AffArr = Povticular_AffectorArray(Name)
    #local ColArr = Povticular_ColorArray(Name)
    #local SizeArr = Povticular_SizeArray(Name)
    #local MaxLifeArr = Povticular_MaxLifeArray(Name)      
    
    #local Attr = Povticular_Name(Name)
    #local AffCount = DiskArray_GetCount(AffArr);
    #local Framerate = getFloatAttr(Attr, "framerate");
    #local ParticleLife = DA_Get(LifeArr, Index);
    #local Vel = DA_Get(VelArr, Index);
    #local Pos = DA_Get(PosArr, Index);
    #local Life = DA_Get(LifeArr, Index);
    #local Color = DA_Get(ColArr, Index);
    #local Size = DA_Get(SizeArr, Index); 
    #local MaxLife = DA_Get(MaxLifeArr, Index);
    
    //remove life and kill particles
    #local ParticleLife = ParticleLife + 1; 
    //cant remove directly here because le list is being iterated over 
    #if(ParticleLife > MaxLife)
        AddToArray(DelList, Index)
        //DebugFloatStr("marking as remove=", Index)
        //add the index to the delList which will delete the particles at the end of the frame
    #else
    //only do further calculation if the particle wont be deleted
        //apply effectors
        #for(I, 0, AffCount-1)
            #local ThisAffector = DiskArray_GetStr(AffArr, I)
            Affector_Affect(ThisAffector, Pos, Vel, Size, Life, Color, MaxLife)
        #end
        //apply vel
        #local Pos = Pos + Vel*(1/Framerate);
        
        DA_Set(VelArr, Index, Vel)    
        DA_Set(PosArr, Index, Pos)
        DA_Set(LifeArr, Index, ParticleLife)
        DA_Set(SizeArr, Index, Size)
        DA_Set(ColArr, Index, Color)
        DA_Set(MaxLifeArr, Index, MaxLife)
    #end
#end

#macro Povticular_ParticlePos(Name, Index)
    #local PosArr = Povticular_PosArray(Name)
    #local Pos = DA_Get(PosArr, Index);
    Pos;
#end 

#macro Povticular_ParticleCount(Name)
    #local Count = DA_GetCount(Povticular_PosArray(Name));
    Count;
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

#macro Povticular_AffectorArray(Name)
    #local N = concat(Name, "_affectors")
    N
#end 

#macro Povticular_ColorArray(Name)
    #local N = concat(Name, "_colors")
    N
#end                                

#macro Povticular_SizeArray(Name)
    #local N = concat(Name, "_sizes")
    N
#end                               

#macro Povticular_LifeArray(Name)
    #local N = concat(Name, "_lifes")
    N
#end 

#macro Povticular_MaxLifeArray(Name)
    #local N = concat(Name, "_maxlifes")
    N
#end 

#macro Povticular_Render(Name)
    #local PosArr = Povticular_PosArray(Name) 
    #local ColArr = Povticular_ColorArray(Name)
    #local SizeArr = Povticular_SizeArray(Name) 

    #local ParticleCount = Povticular_ParticleCount(Name);
    #for(I, 0, ParticleCount-1)
        #local Pos = DA_Get(PosArr, I);
        #local Color = DA_Get(ColArr, I);
        #local Size = DA_Get(SizeArr, I);
    
        sphere{
            Pos, Size   
            pigment{ Red } 
        }
    #end
#end

#macro Povticular_GetRenderString(Name)
    #local PosArr = Povticular_PosArray(Name) 
    #local ColArr = Povticular_ColorArray(Name)
    #local SizeArr = Povticular_SizeArray(Name)
    
    #local FullStr = "" 

    #local ParticleCount = Povticular_ParticleCount(Name);
    #for(I, 0, ParticleCount-1)
        #local Pos = DA_Get(PosArr, I);
        #local Color = DA_Get(ColArr, I);
        #local Size = DA_Get(SizeArr, I);
        //TODO REMOVE THAT
        #local Color = "Red"; 
        
        #local mStr = concat("sphere{\n\t<", vstr(3, Pos, ",", 0,-1), ">, ", str(Size, 0, -1), "\n\tpigment{", Color, "}\n}")
        #local FullStr = concat(FullStr, "\n", mStr)
    #end    
    //DebugStr(FullStr)                                       
    FullStr
#end       

#macro Povticular_CacheFrame(Name, FrameNumber)
    #local mStr = Povticular_GetRenderString(Name)
    StoreVar(concat("tmp_cache_", str(FrameNumber, 0, 0)), mStr)
#end

#macro Povticular_CalculateFrame(Name)
    DA_LoadInMemory(Povticular_PosArray(Name))
    DA_LoadInMemory(Povticular_VelArray(Name))
    //DA_LoadInMemory(Povticular_AffectorArray(Name))
    DA_LoadInMemory(Povticular_ColorArray(Name))
    DA_LoadInMemory(Povticular_SizeArray(Name))
    DA_LoadInMemory(Povticular_LifeArray(Name))
    DA_LoadInMemory(Povticular_MaxLifeArray(Name))
    #declare DelList = array[1]{-1}  

    
    #local Attr = Povticular_Name(Name)
    #local NewParticleCount = getFloatAttr(Attr, "particlePerFrame");
    #for(I, 0, NewParticleCount-1)
        Povticular_AddParticle(Name)
    #end                            
    
    #local ParticleCount = DA_GetCount(Povticular_PosArray(Name))
    #for(I, 0, ParticleCount-1)         
        Povticular_UpdateParticle(Name, I)
    #end
    
    #for(I, 1, dimension_size(DelList, 1)-1)
        //deleted the particles marked as dead
        Povticular_RemoveParticle(Name, DelList[I])    
    #end
    
    DA_SaveToDisk(Povticular_PosArray(Name))
    DA_SaveToDisk(Povticular_VelArray(Name))
   // DA_SaveToDisk(Povticular_AffectorArray(Name))
    DA_SaveToDisk(Povticular_ColorArray(Name))
    DA_SaveToDisk(Povticular_SizeArray(Name))
    DA_SaveToDisk(Povticular_LifeArray(Name))
    DA_SaveToDisk(Povticular_MaxLifeArray(Name))
#end

#end // end povticular.inc