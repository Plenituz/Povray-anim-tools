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
    #local Precision = 100;  
    //A 100 : 3m17           
    
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
                #local VolumesArray = array[8];
                #local VolumesSum = 0;
                //calculate the volume of each corner
                #for(I, 0, 7)
                    #local VectToCorner = VectsFirst[I] - PointToMove;
                    #local ThisVolume = VectToCorner.x * VectToCorner.y * VectToCorner.z;
                    #local ThisVolume = abs(ThisVolume);//a volume can't be negative
                    #local VolumesArray[I] = ThisVolume;//this is not normalized yet
                    #local VolumesSum = VolumesSum + ThisVolume;
                #end                                            
                //normalize the VolmuesArray
                #for(I, 0, 7)
                    #local VolumesArray[I] = VolumesArray[I] / VolumesSum;
                    //DebugFloatStr(concat("VolumesArray[", str(I, 0, 0), "]="), VolumesArray[I])
                #end                                                      
                
                #local PointMoved = 
                    VolumesArray[7]*VectsSecond[0] +  
                    VolumesArray[6]*VectsSecond[1] +  
                    VolumesArray[5]*VectsSecond[2] +  
                    VolumesArray[4]*VectsSecond[3] +
                    VolumesArray[3]*VectsSecond[4] +
                    VolumesArray[2]*VectsSecond[5] +
                    VolumesArray[1]*VectsSecond[6] +
                    VolumesArray[0]*VectsSecond[7];
                #local MoveVector = PointMoved - PointToMove;                
                //DebugVectStr("pointmoved=", MoveVector)
                
                #local CutBox = 
                box{
                    Corner1, Corner2            
                }
                /*sphere{
                    Corner1, 0.1
                    pigment{Red}
                }
                sphere{
                    Corner2, 0.1
                    pigment{Blue}
                }  */
                object{
                        Obj
                        clipped_by{ CutBox }
                        pigment{Red}//color rgb <rand(S), rand(S), rand(S)>}
                        translate MoveVector
                        no_shadow                                
                    }                 
                   
                
            #end
        #end
    #end    
                   
#end    


#declare O = 
union{
  //corps
sphere_sweep{
    linear_spline
    2,
    <0, 3, 0>, 2.27
    <0, 11, 0>, 3.17

    pigment{Green}  
} 

//bras droit (du point de vue du cactus)
sphere_sweep{
    linear_spline
    3,
    <-3, 8, 0>, 0.8
    <-5, 8, 0>, 0.9
    <-5, 10, 0>, 1.2
    
    pigment{Green}   
}

//bras gauche
sphere_sweep{
    linear_spline
    3,
    <3, 6.5, 0>, 0.8
    <5, 6.5, 0>, 0.9
    <5, 8.5, 0>, 1.2
    texture{
        pigment{Green}
        //finish{ambient }
    }   
}

//oeil droit
ovus {0.9,0.9 scale <0.75,1,1> translate <-1.2,9.5,-2.5> } 
//oeil gauche
ovus {0.9,0.9 scale <0.75,1,1> translate <1.2,9.5,-2.5>}
//pupille gauche
ovus {
    0.9,0.9 
    
    pigment{White}
    scale <0.25,0.35,0.35> 
    translate <1.35, 10.25, -3.25>
} 
//pupille droite
ovus {
    0.9,0.9 
    
    pigment{White}
    scale <0.25,0.35,0.35> 
    translate <-1.1, 10.25, -3.25>
}
scale 0.5
translate <0, -4, 0>
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
   <4, 4, -20>,
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
    location <-5, 5, -20> 
    look_at < 0,0,0 > 
    angle 100
}
    
#end