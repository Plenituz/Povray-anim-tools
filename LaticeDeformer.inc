#ifndef(LATICE_INc_VAR)
#declare LATICE_INc_VAR = 1;

#include "colors.inc"
#include "Debug.inc"

#declare S = seed(2348);


#macro Linear(From, To, Val)
     #declare Int = From + (To - From)*Val;
     Int;
#end  

/*
  ArrayVect[8]
  {
      BotFrontLeft
      BotFrontRight
      BotBackLeft
      BotBackRight
      
      TopFrontLeft
      TopFrontRight
      TopBackLeft
      TopBackRight
  }
*/
#macro Voxelize(VectsFirst, VectsSecond, Obj)
    #local Precision = 5;
    
    /*box{
        BotLeftFront, TopRightBack
        pigment{color rgbt <0.5, 0.5, 0.5, 0.5>}
    }*/
    
    #for(X, 0, Precision-1)
        #for(Y, 0, Precision-1)
            #for(Z, 0, Precision-1) 
                #local LocalX = Linear(VectsFirst[0].x, VectsFirst[7].x, X/Precision);
                #local LocalY = Linear(VectsFirst[0].y, VectsFirst[7].y, Y/Precision);
                #local LocalZ = Linear(VectsFirst[0].z, VectsFirst[7].z, Z/Precision);
                
                #local LocalX2 = Linear(VectsFirst[0].x, VectsFirst[7].x, (X+1)/Precision);
                #local LocalY2 = Linear(VectsFirst[0].y, VectsFirst[7].y, (Y+1)/Precision);
                #local LocalZ2 = Linear(VectsFirst[0].z, VectsFirst[7].z, (Z+1)/Precision);
                
                //corners de la lattice originale               
                #local Corner1 = <LocalX, LocalY, LocalZ>;
                #local Corner2 = <LocalX2, LocalY2, LocalZ2>;
                //the mid point of this cutout section
                #local PointToMove = (Corner1 + Corner2) /2;
                
                
                
                #local CutBox = 
                box{
                    Corner1, Corner2            
                }
                sphere{
                    Corner1, 0.1
                    pigment{Red}
                }
                sphere{
                    Corner2, 0.1
                    pigment{Blue}
                } 
                                   
                object{
                    Obj
                    clipped_by{ CutBox }
                    pigment{color rgb <rand(S), rand(S), rand(S)>}
                    
                }   
                
            #end
        #end
    #end    
                   
#end    


#declare O = sphere{
  0, 4
}

/*#declare O = box{
  <1, 1, 1>*-4, <1, 1, 1>*4
}*/
   
   /*
  ArrayVect[8]
  {
      BotFrontLeft
      BotFrontRight
      BotBackLeft
      BotBackRight
      
      TopFrontLeft
      TopFrontRight
      TopBackLeft
      TopBackRight
  }
*/

#declare VectsFirst = array[8]{
   <-4, -4, -4>,
   <4, -4, -4>,
   <-4, -4, 4>,
   <4, -4, 4>,
   
   <-4, 4, -4>,
   <4, 4, -4>,
   <-4, 4, 4>,
   <4, 4, 4>
}

#declare VectsSecond = array[8]{
   <-4, -4, -4>,
   <4, -4, -4>,
   <-4, -4, 4>,
   <4, -4, 4>,
   
   <-4, 4, -4>,
   <6, 4, -4>,
   <-4, 4, 4>,
   <4, 4, 4>
}

#for(I, 0, 7)
    sphere{
        VectsFirst[I], 0.2
        pigment{Yellow}
    }
    sphere{
        VectsSecond[I], 0.25
        pigment{Blue}
    }
#end

#declare tmp = union{

    Voxelize(VectsFirst, VectsSecond, O)
    
}

object{
 tmp
}


light_source{ <0,10,-10> White }
camera{ 
    right x*image_width / image_height
    up y
    location <-5, 5, -10> 
    look_at < 0,0,0 > 
    angle 90
}
    
#end