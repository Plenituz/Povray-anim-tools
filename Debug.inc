#ifndef(DEBUG_INc_VAR)
#declare DEBUG_INc_VAR = 1;


#macro DebugFloat(Float)
    #debug concat("\n", str(Float, 0, -1), "\n")
#end

#macro DebugFloatStr(Txt, Float)
    #debug concat("\n", Txt, str(Float, 0, -1), "\n")
#end

#macro DebugStrArray(Txt, Arr)
    #local Len = dimension_size(Arr, 1);
    #for(I, 0, Len-1)
        #debug concat("\n", Txt, "[", str(I, 0, 0), "]=" Arr[I], "\n")
    #end
#end

#macro DebugStr(Txt)
    #debug concat("\n", Txt, "\n")
#end
    
#end