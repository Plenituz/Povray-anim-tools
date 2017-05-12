#ifndef(DEBUG_INc_VAR)
#declare DEBUG_INc_VAR = 1;


#macro DebugFloat(Float)
    #debug concat("\n", str(Float, 0, -1), "\n")
#end

#macro DebugFloatStr(Txt, Float)
    #debug concat("\n", Txt, str(Float, 0, -1), "\n")
#end

#macro DebugVectStr(Txt, Float)
    #debug concat("\n", Txt, vstr(3, Float, ",",  0, -1), "\n")
#end

#macro DebugVect(Float)
    #debug concat("\n", vstr(3, Float, ",",  0, -1), "\n")
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

#macro DrawVector(Point, Vect)
   DrawVectorColor(Point, Vect, Red)
#end

#macro DrawVectorColor(Point, Vect, Color)
    sphere_sweep{
        linear_spline
        2
        Point, 0.1
        Point + Vect, 0.1
        pigment{Color}
    }               
    sphere{
        Point + Vect, 0.15
        pigment{Color}
    }
#end 

#macro DrawLineColor(P1, P2, Color)
    sphere_sweep{
        linear_spline
        2
        P1, 0.1
        P2, 0.1
        pigment{Color}
    }
#end

#macro DrawPoint(Point)
    sphere{
        Point, 0.2
        pigment{Yellow}
    }
#end
    
#end