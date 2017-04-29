#ifndef(DISK_ARRAY2_INc_VAR)
#declare DISK_ARRAY2_INc_VAR = 1;

#include "IO.inc" 
#include "strings.inc"
#include "Array.inc"
#include "Debug.inc"
 
//this is an unordered list but optimized for deleting and adding a lot of objects  
//WARNING THIS LIST DOESN'T SUPPORT STRINGS, STRANGE BEHAVIOUR MAY HAPPEN
#macro DA_Create(Name, MaxSize)
    setAttr(Name, "count", 0)
    setAttr(Name, "realCount", MaxSize)
    setAttr(Name, "array", "")
    //separating count realcount and array in 3 files becaus eyou might want to only read the count some frames    
#end               
//count is how many element the array has
#macro DA_GetCount(Name)
    #local Count = getFloatAttr(Name, "count");
    Count;
#end
//real count is how many element the array can have
#macro DA_GetRealCount(Name)
    #local Count = getFloatAttr(Name, "realCount");
    Count;
#end

#macro DA_SetLoaded(Name, Val)
    Parse_String(concat("#declare ", Name, "_array_loaded = Val;")) 
#end

#macro DA_IsLoaded(Name) 

    #local IsLoaded = 0;               
    #local FullName = concat(Name, "_array_loaded");
    Parse_String(concat("#ifdef(", FullName,") #declare IsLoaded = ", FullName, "; #else #declare IsLoaded = 0; #end"))
    IsLoaded;
#end           

#macro DA_LoadInMemory(Name) 
    #local Loaded = DA_IsLoaded(Name);
    #if(Loaded = 0)
        #local Count = DA_GetCount(Name);
        #local RealCount = DA_GetRealCount(Name);
        
        #if(Count = 0)
            #local Array = array[1] 
        #else
            #local Array = ReadArray(concat(Name, ".array"), Count)
        #end
        
        ExpandArray(Array, RealCount-Count)
        
        DA_ReplaceInMemory(Name, Array)
        DA_SetLoaded(Name, 1)
    #end
#end 

#macro DA_SaveToDisk(Name)
    #local Arr = array[1]
    DA_GetArrayFromMemory(Name, Arr) 
    #local Count = DA_GetCount(Name)
    WriteArrayWCount(concat(Name, ".array"), Arr, Count)
#end

#macro DA_GetArrayFromMemory(Name, ArrOut)
    Parse_String(concat("#declare ArrOut = ", Name, "_array;"))
#end  

#macro DA_ReplaceInMemory(Name, nArr)
    Parse_String(concat("#declare ", Name, "_array = nArr;"))
#end 

#macro DA_Get(Name, Index)
    #local Arr = array[1]
    DA_GetArrayFromMemory(Name, Arr)
    //DebugStr(Arr[Index])
    Arr[Index]
#end

#macro DA_Add(Name, Val)
    #local Arr = array[1]
    DA_GetArrayFromMemory(Name, Arr)
    #local RealCount = DA_GetRealCount(Name);
    #local Count = DA_GetCount(Name);
    
    #if(Count >= RealCount-5)
        //We are getting close to the end of the array, expand it  (double it's size)
        ExpandArray(Arr, RealCount)
        #local RealCount = dimension_size(Arr, 1);
        setAttr(Name, "realCount", RealCount)        
    #end
    //DebugStrArray("arr", Arr)
    //DebugFloat("count", dimension_size(Arr, 1))                                    
    #local Arr[Count] = Val;
    
    #local Count = Count + 1;
    setAttr(Name, "count", Count)
    DA_ReplaceInMemory(Name, Arr)
#end 

#macro DA_Remove(Name, Index)
    #local Arr = array[1]
    DA_GetArrayFromMemory(Name, Arr)
    #local Count = DA_GetCount(Name); 
    
    //plae the last element in the place of the deleted element
    //and undefine the last element 
    //even tho changing the count should be enough for the last element to be redefined by an eventual 
    //new element 
    //DebugStr(concat("/////", Name, "/////"))
    //DebugFloatStr("REmoving:", Index)
    //DebugFloatStr("count=", Count)
    #local Arr[Index] = Arr[Count-1];
    #undef Arr[Count-1]
    DA_ReplaceInMemory(Name, Arr)
    
    #local Count = Count - 1;
    setAttr(Name, "count", Count)
#end

#macro DA_Set(Name, Index, Val)
    #local Arr = array[1]
    DA_GetArrayFromMemory(Name, Arr)
    #local Arr[Index] = Val;
    DA_ReplaceInMemory(Name, Arr)
#end                                   


    
#end /// end diskarray2.inc