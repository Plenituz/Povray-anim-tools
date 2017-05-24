#ifndef(SHAKER_INC_VAR)
#declare SHAKER_INC_VAR = 1;

#macro ShakeVect(Name, Radius, Seed)
    
    ShakeVectWeighted(Name, Radius, Seed, <1,1,1>)
#end 

#macro ShakeVectWeighted(Name, Radius, Seed, Weight)
    
    #if(clock != 0)
        #declare ShakeStart = ReadVect(Name);
    #else
        #declare ShakeStart = <0, 0, 0>;
    #end
    #declare ShakeRand = seed(frame_number*Seed);                                                
    #declare Rot = <rand(ShakeRand)*Weight.x, rand(ShakeRand)*Weight.y, rand(ShakeRand)*Weight.z> *360;
    
    #declare Shake = vtransform(<ShakeRadius, 0, 0>, Rotate_Around_Trans(Rot, <0, 0, 0>) ); 
    #declare Shake = Keyframeholder_LinearInterpolation(ShakeStart, Shake, 1/24);
    StoreVar(Name, Shake)
    Shake;
#end   
    
#end