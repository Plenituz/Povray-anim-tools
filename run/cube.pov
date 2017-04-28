#include "colors.inc"
#include "Array.inc" 
#include "Debug.inc"

light_source{ <0,10,-10> White }
camera{ 
    right x*image_width / image_height
    up y
    location <-5, 3, -10> 
    look_at < 0,0,0 > 
    angle 80
}

#macro LinearInterpolation(From, To, Val)
     #declare Int = From + (To - From)*Val;
     Int;
#end 

#macro Cube(A, D, Iter)
    DebugStr(concat("iter=", str(Iter, 0, 0)))
    #local Delta3 = (D - A)/3;
    #local Exclude = array[7]{
        <1, 1, 1>,
        <1, 1, 0>,
        <1, 1, 2>,
        <1, 0, 1>,
        <1, 2, 1>,
        <0, 1, 1>,
        <2, 1, 1>
    } 
    
    #for(X, 0, 2)
        #for(Y, 0, 2)
            #for(Z, 0, 2)
                #local Vect = <X, Y, Z>;
                #local DoEx = ArrayContainsVect(Exclude, Vect);
                #if(!DoEx)
                    #local Vect = Vect * Delta3;
                    sphere{
                        Vect, 0.1 pigment{Red}
                    }
                    
                    #if(Iter < 1)
                        DebugStr(concat("doing iter:", vstr(3, Vect/Delta3, ",", 0, -1) ))
                        #local Size = Delta3.x/2;
                        #local A2 = Vect - <Size, Size, Size>;
                        #local D2 = Vect + <Size, Size, Size>; 
                        
                        //CubeAround(Vect, Delta3.x/2)
                        Cube(A2, D2, Iter+1)
                        
                        /*box{
                            Vect - <Size, Size, Size>, Vect + <Size, Size, Size>
                            pigment{color rgbt <rand(R), rand(R), rand(R), 0.9>}
                        } */
                         
                    #else 
                        DebugStr(concat("stopping:", vstr(3, Vect/Delta3, ",", 0, -1)))
                        //CubeAround(Vect, Delta3.x/2)
                    #end
                    
                    
                #end
            #end
        #end
    #end 
    
#end 

#macro CubeAround(Vect, Size)
    box{
        Vect - <Size, Size, Size>, Vect + <Size, Size, Size>
        pigment{color rgbt <rand(R), rand(R), rand(R), 0.5>}
    }  
#end

#declare R = seed(10109);
                         


#declare Obj = union{
    Cube(<0, 0, 0>, <5, 5, 5>, 0)
    }
object{Obj rotate <clock, clock, clock>*360}
