#ifndef(TOON_INc_VAR)
#declare TOON_INc_VAR = 1; 

#include "transforms.inc"
#include "Debug.inc"
global_settings { ambient_light rgb 2 }
                                                                        
#ifndef(LightPos)
    #error "you have to define \"LightPos\" (location of the light)"
#end
#ifndef(LightLook)
    #error "you have to define \"LightLook\" (point the light looks at)"
#end                                                                    

#declare Toon_Direction = LightLook-LightPos; 

//intensité des shadows dans l'ordre du plus clair au plus sombre
//P=<pas d'ombre, un peu d'ombre...>
//en weighted sum 
#declare Toon_P = <5, 4, 2, 1>;
#declare Toon_LL = <0.1, 0.2, 0.5, 1>;//transmit levels at different P
#declare Toon_StepOver = 0.005;   //"antialising" kinda, how much one shadow bleeds into the other
#declare Sum = Toon_P.x + Toon_P.y + Toon_P.z + Toon_P.t;  
#declare Toon_BlackLevel = 0.6;  
#declare Toon_TransmitVals = <0, 1>;
/*#declare ToonLighting =     
texture { 
    pigment {
        slope{ Toon_Direction, 0, 1}  //light direction
        pigment_map{
            [0.0 Black transmit Toon_LL.x] 
            [Toon_P.t/Sum - Toon_StepOver Black transmit Toon_LL.x] //0.08
            
            [Toon_P.t/Sum Black transmit Toon_LL.y]   //0.08
            [Toon_P.z/Sum + Toon_P.t/Sum Black transmit Toon_LL.y] //0.25
            
            [Toon_P.z/Sum + Toon_P.t/Sum Black + Toon_StepOver transmit Toon_LL.z] //0.25
            [Toon_P.z/Sum + Toon_P.t/Sum + Toon_P.y/Sum - Toon_StepOver Black transmit Toon_LL.z]
            
            [Toon_P.z/Sum + Toon_P.t/Sum + Toon_P.y/Sum Black transmit Toon_LL.t]
        }
    } 
}
//la texture suivante est stockée dans Lighting aussi, c'est une layered texture 
texture { 
    pigment {
        aoi 
        pigment_map {
            [0.0 White transmit 0] 
            [Toon_BlackLevel Black transmit 0]
            [Toon_BlackLevel Black transmit Toon_TransmitVal]
            [1.0 Black transmit Toon_TransmitVal]
        }
    }
}*/

#macro ToonLit()
    ToonLitCustom(LightPos, LightLook, Toon_P, Toon_LL, Toon_StepOver, Toon_BlackLevel, Toon_TransmitVals)
#end

#macro ToonLitCustom(LightPos, LightLook, Toon_P, Toon_LL, Toon_StepOver, Toon_BlackLevel, Toon_TransmitVals)
    #local Toon_Direction = LightLook-LightPos; 
    #local Sum = Toon_P.x + Toon_P.y + Toon_P.z + Toon_P.t;
         
    texture { 
        pigment {
            slope{ Toon_Direction, 0, 1}  //light direction
            pigment_map{
                [0.0 Black transmit Toon_LL.x] 
                [Toon_P.t/Sum - Toon_StepOver Black transmit Toon_LL.x] //0.08
                
                [Toon_P.t/Sum Black transmit Toon_LL.y]   //0.08
                [Toon_P.z/Sum + Toon_P.t/Sum Black transmit Toon_LL.y] //0.25
                
                [Toon_P.z/Sum + Toon_P.t/Sum Black + Toon_StepOver transmit Toon_LL.z] //0.25
                [Toon_P.z/Sum + Toon_P.t/Sum + Toon_P.y/Sum - Toon_StepOver Black transmit Toon_LL.z]
                
                [Toon_P.z/Sum + Toon_P.t/Sum + Toon_P.y/Sum Black transmit Toon_LL.t]
            }
        } 
    }
    //la texture suivante est stockée dans Lighting aussi, c'est une layered texture 
    texture { 
        pigment {
            aoi 
            pigment_map {
                [0.0 White transmit Toon_TransmitVals.x] 
                [Toon_BlackLevel Black transmit Toon_TransmitVals.x]
                [Toon_BlackLevel Black transmit Toon_TransmitVals.y]
                [1.0 Black transmit Toon_TransmitVals.y]
            }
        }
    }
    
#end

#default{
    finish{ambient 0.5}
}



#end //end toon.inc