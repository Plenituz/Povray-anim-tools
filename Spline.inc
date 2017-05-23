#include "colors.inc" 

#macro CreateSpline(PointArr)
    #local Max = dimension_size(PointArr, 1);
    spline{            
        -1/Max, PointArr[0]
        #for(I, 0, Max-2)          
            I/Max PointArr[I]        
        #end
        Max + 1/Max PointArr[Max-1]
    }
#end
  
#macro PosOnCurveAtDist(Spline, Dist)
    #local CurrentDist = 0;
    #local CurrentStep = 0;
    #local LastPoint = Spline(0, cubic_spline);
    #local Step = 0.001;
    
    #while(CurrentDist < Dist)
        #local CurrentStep = CurrentStep + Step;
        #local NextPoint = Spline(CurrentStep, cubic_spline);
        #local NewDist = vlength(NextPoint - LastPoint);
        #local CurrentDist = CurrentDist + NewDist;
        #local LastPoint = NextPoint;
         
    #end
    Spline(CurrentDist, cubic_spline)
#end 

#macro DispSpline(Spline)
    #declare Nr = 0;     // start
    #declare EndNr = 1;  // end
    #while (Nr< EndNr)   // -----------------------------------
    sphere{ <0,0,0>,0.01
          texture{ 
            pigment{color rgb <1-Nr/2,0.75+Nr/4,0>}
            finish {diffuse 0.9 phong 1}
          }
         translate Spline(Nr, cubic_spline)
       } // end of sphere
    
    #declare Nr = Nr + 0.005;  // next Nr
    #end // -------------------------------------- end of loop   
#end

  
// #declare MySpline = CreateSpline(array[5]{<0,0,-1>, <1,0,0>, <0,0,1>, <-1, 0, 0>,  <0,0,-1>})
  

 
/*DispSpline(MySpline) 

sphere{
    MySpline(clock, cubic_spline), 0.1
    pigment{Red}
}   */



