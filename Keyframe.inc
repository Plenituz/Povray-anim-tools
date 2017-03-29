#include "IO.inc" 
#include "DiskArray.inc"

#macro KeyframeHolder_Create(Name)
    #local TimeArrayName = concat(Name, ".times");
    #local ValuesArrayName = concat(Name, ".values");
    DiskArray_CreateDiskArray(TimeArrayName)
    DiskArray_CreateDiskArray(ValuesArrayName)
#end

#macro KeyFrameHolder_Add(Name, Value, Time)
    #local TimeArrayName = concat(Name, ".times");
    #local ValuesArrayName = concat(Name, ".values");
    DiskArray_Add(TimeArrayName, Time)
    DiskArray_Add(ValuesArrayName, Value)
#end                                     

#macro KeyFrameHolder_GetValueAtTime(Name, Time)
    #local TimeArrayName = concat(Name, ".times");
    #local ValuesArrayName = concat(Name, ".values");
    #local Count = DiskArray_GetCount(TimeArrayName);
    
    #local Interpolation = -1;
    //carrefull for indexoutofbound, not secure yet
    #for(I, 0, Count-1)
        #local ThisTime = DiskArray_GetFloat(TimeArrayName, I);
        #local NextTime = DiskArray_GetFloat(TimeArrayName, I+1); 
        #if(Time >= ThisTime & Time < NextTime)
            #local From = DiskArray_GetFloat(ValuesArrayName, I);
            #local To = DiskArray_GetFloat(ValuesArrayName, I+1); 
            #local TimeVal = (Time - ThisTime)/(NextTime-ThisTime);
            #local Interpolation = KeyFrameHolder_Interpolator(From, To, TimeVal, "AccelerateDeccelerate");
            #break
        #end
    #end 
    Interpolation;
#end  

#macro KeyFrameHolder_LinearInterpolation(From, To, Val)
     #declare Int = From + (To - From)*Val;
     Int;
#end 

#macro KeyFrameHolder_Interpolator(From, To, Val, Interpolator) 
    #local Inter = Val;
    #if(Interpolator = "SmoothStep")
        #local Inter = Val * Val * (3 - (2 * Val));
    #end                          
    #if(Interpolator = "AccelerateDeccelerate")
        #local Inter = (cos((Val + 1) * pi) / 2.0) + 0.5;
    #end
    #local Inter = KeyFrameHolder_LinearInterpolation(From, To, Inter); 
    Inter;
#end