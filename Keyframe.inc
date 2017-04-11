#ifndef(KEYFRAME_INC_VAR)
#declare KEYFRAME_INC_VAR = 1;

#include "IO.inc" 
#include "DiskArray.inc"
//these files are in the "POVYX" repo   

/***********************   

Keyframe.inc usage : 
Create a keyframeHolder by calling "create" and giving it a unique name
Add values at times by calling Add
You can also add values at time with custom bezier handles
by calling AddWithCustomCurve, leftHandle should default at <0,0> and rightHandle at <1,1>
to have a straight line

if you use CustomCurve every keyframe must have a custom curve associated, otherwise it will crash

************************/

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

//the right and left handle are vectors relative to the point     
#macro KeyFrameHolder_AddWithCustomCurve(Name, Value, Time, LeftHandle, RightHandle)
    #local TimeArrayName = concat(Name, ".times");
    #local ValuesArrayName = concat(Name, ".values");
    DiskArray_Add(TimeArrayName, Time)
    DiskArray_Add(ValuesArrayName, Value)                    
    #local Count = DiskArray_GetCount(TimeArrayName);
    #local VarName = DiskArray_GetVarName(TimeArrayName, Count-1)
    setAttr(VarName, "leftHandle", LeftHandle)
    setAttr(VarName, "rightHandle", RightHandle)
#end                                   

#macro KeyFrameHolder_GetValueAtTime(Name, Time, Interpolator)
    #local TimeArrayName = concat(Name, ".times");
    #local ValuesArrayName = concat(Name, ".values");
    #local Count = DiskArray_GetCount(TimeArrayName);
     
    #if(Count != 0)
        #local Interpolation = DiskArray_GetFloat(ValuesArrayName, Count-1);
        //careful for indexoutofbound, not secure yet
        #for(I, 0, Count-2)
            #local ThisTime = DiskArray_GetFloat(TimeArrayName, I);
            #local NextTime = DiskArray_GetFloat(TimeArrayName, I+1); 
            #if(Time >= ThisTime & Time < NextTime)
                #local From = DiskArray_GetFloat(ValuesArrayName, I);
                #local To = DiskArray_GetFloat(ValuesArrayName, I+1); 
                #local TimeVal = (Time - ThisTime)/(NextTime-ThisTime);
                #local Interpolation = KeyFrameHolder_Interpolator(From, To, TimeVal, Interpolator, I, I+1);
                #break
            #end
        #end
    #else
        #local Interpolation = 0;
    #end
     
    Interpolation;
#end  

#macro KeyFrameHolder_LinearInterpolation(From, To, Val)
     #declare Int = From + (To - From)*Val;
     Int;
#end 

#macro KeyFrameHolder_Interpolator(From, To, Val, Interpolator, CountFrom, CountTo) 
    #local Inter = Val;
    #if(Interpolator = "SmoothStep")
        #local Inter = Val * Val * (3 - (2 * Val));
    #end                          
    #if(Interpolator = "AccelerateDeccelerate")
        #local Inter = (cos((Val + 1) * pi) / 2.0) + 0.5;
    #end 
    #if(Interpolator = "CustomCurve") 
        #local TimeArrayName = concat(Name, ".times");
        #local VarNameFrom = DiskArray_GetVarName(TimeArrayName, CountFrom)
        #local VarNameTo = DiskArray_GetVarName(TimeArrayName, CountTo)
        #local Start = <0, 0>;
        #local End = <1, 1>;
        #local FHandle = getVectAttr(VarNameFrom, "rightHandle");
        #local SHandle = getVectAttr(VarNameTo, "leftHandle"); 
       // #debug concat("\n", str(Val, 0, -1), "\n")
        //trouver sur la courbe la valeur de y pour laquelle x = Val
        #local Inter = KeyFrameHolder_Evaluate(Val, Start, FHandle, SHandle, End, 50);
        //#debug concat("\n", str(Vect.x, 0, -1), ":", str(Vect.y, 0, -1) ,"\n") 
    #end
    #local Inter = KeyFrameHolder_LinearInterpolation(From, To, Inter); 
    Inter;
#end  

//trouver sur la courbe la valeur de y pour laquelle x = Val
#macro KeyFrameHolder_Evaluate(Val, Start, FHandle, SHandle, End, Res)
    #local Return = Val;
    #for(I, 0, Res)
        #local X = I/Res;
        #local Vect = KeyFrameHolder_BezierPoint2D(X, Start, FHandle, SHandle, End);
        #if(Vect.x >= Val)
            #local Return = Vect.y;
            #break
        #end    
    #end                           
    Return;
#end

#macro KeyFrameHolder_Evaluate2(Val, Start, FHandle, SHandle, End, Iterations)
    #local Return = Val;
    #local Low = 0;
    #local High = 1;
    #local Mid = 0.5;
     
    #for(I, 0, Iterations)
        #local Vect = KeyFrameHolder_BezierPoint2D(Mid, Start, FHandle, SHandle, End);
        #if(Val < Vect.x)
            #local High = Mid;      
        #else 
            #if(Val > Vect.x)
                #local Low = Mid;       
            #else
                //Val == Mid
                #break
            #end
        #end
        #local Mid = Low + (High-Low)/2;
    #end     
    #local Vect = KeyFrameHolder_BezierPoint2D(Mid, Start, FHandle, SHandle, End);     
    #local Return = Vect.x;
    
    Return;
#end 

#macro KeyFrameHolder_PartialBezier(T, A, B, C, D)
    #local C1 =  D - (3*C) + (3*B) - A;
    #local C2 = (3*C) - (6*B) + (3*A);
    #local C3 = (3*B) - (3*A);
    #local C4 = A;
    #local Result = C1*T*T*T + C2*T*T + C3*T + C4;
    Result;
#end 
                                   
//T=% on the curve                                   
#macro KeyFrameHolder_BezierPoint2D(T, Start, FHandle, SHandle, End)
    #local PX = KeyFrameHolder_PartialBezier(T, Start.x, FHandle.x, SHandle.x, End.x);
    #local PY = KeyFrameHolder_PartialBezier(T, Start.y, FHandle.y, SHandle.y, End.y);
    #local Result = <PX, PY>;
    Result;
#end 

#end // end Keyframe.inc