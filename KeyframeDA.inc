#ifndef(KEYFRAMEDA_INC_VAR)
#declare KEYFRAMEDA_INC_VAR = 1;

#include "IO.inc" 
#include "DiskArray2.inc"
//these files are in the "POVYX" repo   

/***********************   

Keyframe.inc usage : 
Create a Keyframeholder by calling "create" and giving it a unique name
Add values at times by calling Add
You can also add values at time with custom bezier handles
by calling AddWithCustomCurve, leftHandle should default at <0,0> and rightHandle at <1,1>
to have a straight line

if you use CustomCurve every keyframe must have a custom curve associated, otherwise it will crash 

You can only call Add or AddWithCustomCurve in the same frame you call Create 
doing other will crash and is a bad practise 

keyframes should be added in the right order, the list doesn't sort itself

************************/

#macro Keyframeholder_Create(Name)
    #local TimeArrayName = concat(Name, "_times");
    #local ValuesArrayName = concat(Name, "_values");
    #local LeftHandleArrayName = concat(Name, "_lefts");
    #local RightHandleArrayName = concat(Name, "_rights");
    //I am separating the times values, lefts and right in different files because you might need to read only 
    //one of those values per frame sometimes
    
    DA_Create(TimeArrayName, 10)
    DA_Create(ValuesArrayName, 10)
    DA_Create(LeftHandleArrayName, 10)
    DA_Create(RightHandleArrayName, 10)
    
    DA_LoadInMemory(TimeArrayName)
    DA_LoadInMemory(ValuesArrayName)
    DA_LoadInMemory(LeftHandleArrayName)
    DA_LoadInMemory(RightHandleArrayName)
#end 

#macro Keyframeholder_Finish(Name)
    #local TimeArrayName = concat(Name, "_times");
    #local ValuesArrayName = concat(Name, "_values");
    #local LeftHandleArrayName = concat(Name, "_lefts");
    #local RightHandleArrayName = concat(Name, "_rights");
    
    DA_SaveToDisk(TimeArrayName)
    DA_SaveToDisk(ValuesArrayName)
    DA_SaveToDisk(LeftHandleArrayName)
    DA_SaveToDisk(RightHandleArrayName)
#end

#macro Keyframeholder_Add(Name, Value, Time)
    #local TimeArrayName = concat(Name, "_times");
    #local ValuesArrayName = concat(Name, "_values");
    #local LeftHandleArrayName = concat(Name, "_lefts");
    #local RightHandleArrayName = concat(Name, "_rights");
    
    DA_Add(TimeArrayName, Time)
    DA_Add(ValuesArrayName, Value)
    DA_Add(LeftHandleArrayName, <0, 0, 0>)
    DA_Add(RightHandleArrayName, <1, 1, 0>)
#end 

//the right and left handle are vectors relative to the point     
#macro Keyframeholder_AddWithCustomCurve(Name, Value, Time, LeftHandle, RightHandle)
    #local TimeArrayName = concat(Name, "_times");
    #local ValuesArrayName = concat(Name, "_values");
    #local LeftHandleArrayName = concat(Name, "_lefts");
    #local RightHandleArrayName = concat(Name, "_rights");
    
    DA_Add(TimeArrayName, Time)
    DA_Add(ValuesArrayName, Value) 
    DA_Add(LeftHandleArrayName, LeftHandle)
    DA_Add(RightHandleArrayName, RightHandle)
#end                                   

#macro Keyframeholder_GetValueAtTime(Name, Time, Interpolator)
    #local TimeArrayName = concat(Name, "_times");
    #local ValuesArrayName = concat(Name, "_values");
    #local LeftHandleArrayName = concat(Name, "_lefts");
    #local RightHandleArrayName = concat(Name, "_rights");
    DA_LoadInMemory(TimeArrayName)
    DA_LoadInMemory(ValuesArrayName)
    DA_LoadInMemory(LeftHandleArrayName)
    DA_LoadInMemory(RightHandleArrayName)

    #local TimeArrayName = concat(Name, "_times");
    #local ValuesArrayName = concat(Name, "_values");
    #local Count = DA_GetCount(TimeArrayName);
     
    #if(Count != 0)
        #local Interpolation = DA_Get(ValuesArrayName, Count-1);
        //careful for indexoutofbound, not secure yet        
        #for(I, 0, Count-2)
            #local ThisTime = DA_Get(TimeArrayName, I);
            #local NextTime = DA_Get(TimeArrayName, I+1); 
            #if(Time >= ThisTime & Time < NextTime)
                #local From = DA_Get(ValuesArrayName, I);
                #local To = DA_Get(ValuesArrayName, I+1); 
                #local TimeVal = (Time - ThisTime)/(NextTime-ThisTime);
                #local Interpolation = Keyframeholder_Interpolator(From, To, TimeVal, Interpolator, I, I+1, Name);
                #break
            #end
        #end
    #else
        #local Interpolation = 0;
    #end
     
    Interpolation;
#end  

#macro Keyframeholder_LinearInterpolation(From, To, Val)
     #declare Int = From + (To - From)*Val;
     Int;
#end

#macro bounce(T)
    #local V = T*T*8;
    V
#end

#macro A(T,S)
    #local V = T * T * ((S + 1) * T - S);
    V
#end
#macro O(T,S)
   #local V = T * T * ((S + 1) * T + S);
   V
#end

#macro Keyframeholder_Interpolator(From, To, Val, Interpolator, CountFrom, CountTo, Name) 
    
    
    #local Inter = Val;
    #if(Interpolator = "SmoothStep")
        #local Inter = Val * Val * (3 - (2 * Val));
    #end                          
    #if(Interpolator = "AccelerateDeccelerate")
        #local Inter = (cos((Val + 1) * pi) / 2.0) + 0.5;
    #end
    #if(Interpolator = "Accelerate")
        #local Inter = Val * Val;
    #end
    #if(Interpolator = "Bounce")

        #if (Val < 0.3535)
            #local Inter = bounce(Val);
        #else 
            #if (Val < 0.7408)
                #local Inter = bounce(Val - 0.54719) + 0.7;
            #else
                #if (Val < 0.9644)
                    #local Inter = bounce(Val - 0.8526) + 0.9;
                #else
                    #local Inter = bounce(Val - 1.0435) + 0.95;
                #end
            #end
        #end
    #end 
    #if(Interpolator = "Anticipate")
        #local tension = getFloatAttr(Name, "tension");
        #local Inter = Val * Val * ((tension + 1) * Val - tension);
    #end
    #if(Interpolator = "Spring")
        #local factor = getFloatAttr(Name, "factor");
        #local Inter = pow(2, -10 * Val) * sin((Val - factor / 4) * (2 * pi) / factor) + 1;
    #end
    #if(Interpolator = "AnticipateOvershoot")
        #local tension = 10;
        #if (Val < 0.5)
            #local Inter = 0.5 * A(Val * 2.0, tension);
        #else
            #local Inter = 0.5 * (O(Val * 2.0 - 2.0, tension) + 2.0);
        #end
    #end
         
    #if(Interpolator = "CustomCurve") 
        #local TimeArrayName = concat(Name, "_times");
        #local LeftHandleArrayName = concat(Name, "_lefts");
        #local RightHandleArrayName = concat(Name, "_rights");
        
        #local Start = <0, 0>;
        #local End = <1, 1>;
        #local FHandle = DA_Get(RightHandleArrayName, CountFrom);
        #local SHandle = DA_Get(LeftHandleArrayName, CountTo); 
       // #debug concat("\n", str(Val, 0, -1), "\n")
        //trouver sur la courbe la valeur de y pour laquelle x = Val
        #local Inter = Keyframeholder_Evaluate(Val, Start, FHandle, SHandle, End, 50);
        //#debug concat("\n", str(Vect.x, 0, -1), ":", str(Vect.y, 0, -1) ,"\n") 
    #end
    #local Inter = Keyframeholder_LinearInterpolation(From, To, Inter); 
    Inter;
#end  

//trouver sur la courbe la valeur de y pour laquelle x = Val
#macro Keyframeholder_Evaluate(Val, Start, FHandle, SHandle, End, Res)
    #local Return = Val;
    #for(I, 0, Res)
        #local X = I/Res;
        #local Vect = Keyframeholder_BezierPoint2D(X, Start, FHandle, SHandle, End);
        #if(Vect.x >= Val)
            #local Return = Vect.y;
            #break
        #end    
    #end                           
    Return;
#end

#macro Keyframeholder_PartialBezier(T, A, B, C, D)
    #local C1 =  D - (3*C) + (3*B) - A;
    #local C2 = (3*C) - (6*B) + (3*A);
    #local C3 = (3*B) - (3*A);
    #local C4 = A;
    #local Result = C1*T*T*T + C2*T*T + C3*T + C4;
    Result;
#end 
                                   
//T=% on the curve                                   
#macro Keyframeholder_BezierPoint2D(T, Start, FHandle, SHandle, End)
    #local PX = Keyframeholder_PartialBezier(T, Start.x, FHandle.x, SHandle.x, End.x);
    #local PY = Keyframeholder_PartialBezier(T, Start.y, FHandle.y, SHandle.y, End.y);
    #local Result = <PX, PY>;
    Result;
#end 

#end // end Keyframe.inc