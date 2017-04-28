#ifndef(LATICE_INc_VAR)
#declare LATICE_INc_VAR = 1;

#include "colors.inc"
#include "Debug.inc"

#declare S = seed(2348);


#macro Linear(From, To, Val)
     #declare Int = From + (To - From)*Val;
     Int;
#end  
 
#macro Voxelize(BotLeftFront, TopRightBack, Obj)
    #local Precision = 10;
    
    /*box{
        BotLeftFront, TopRightBack
        pigment{color rgbt <0.5, 0.5, 0.5, 0.5>}
    }*/
    
    #for(X, 0, Precision-1)
        #for(Y, 0, Precision-1)
            #for(Z, 0, Precision-1) 
                #local LocalX = Linear(BotLeftFront.x, TopRightBack.x, X/Precision);
                #local LocalY = Linear(BotLeftFront.y, TopRightBack.y, Y/Precision);
                #local LocalZ = Linear(BotLeftFront.z, TopRightBack.z, Z/Precision);
                
                #local LocalX2 = Linear(BotLeftFront.x, TopRightBack.x, (X+1)/Precision);
                #local LocalY2 = Linear(BotLeftFront.y, TopRightBack.y, (Y+1)/Precision);
                #local LocalZ2 = Linear(BotLeftFront.z, TopRightBack.z, (Z+1)/Precision);
                
                
                #local Corner1 = <LocalX, LocalY, LocalZ>;
                #local Corner2 = <LocalX2, LocalY2, LocalZ2>; 
                DebugVectStr("corner1=", Corner1)
                DebugVectStr("corner2=", Corner2)
                
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
                }*/ 
                                   
                object{
                    Obj
                    clipped_by{ CutBox }
                    pigment{color rgb <rand(S), rand(S), rand(S)>}
                    translate Corner1*0.5*clock
                    
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

#declare tmp = union{

    Voxelize(<1, 1, 1>*-4, <1, 1, 1>*4, O)
    
}

object{
 tmp
 rotate y*clock*360
}


light_source{ <0,10,-10> White }
camera{ 
    right x*image_width / image_height
    up y
    location <-5, 5, -10> 
    look_at < 0,0,0 > 
    angle 80
}
    
#end