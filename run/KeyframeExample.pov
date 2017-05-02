#include "colors.inc" 
#include "KeyframeDA.inc" 

/*
  Add the path to the Keyframes.inc, IO.inc and DiskArray.inc to the command line
  by doing +L"path/to/incs"  
  warning this also need "IO.inc" and "DiskArray.inc"
  warning2 it will create a bunch a file so you might want to run it in a separate folder


*/

light_source{ <0,10,-10> White }
camera{ 
    right x*image_width / image_height
    up y
    location <-5, 3, -10> 
    look_at < 0,0,0 > 
    angle 80
}      

#if(clock = 0)
    Keyframeholder_Create("mKey")
    //créé un KeyframeHolder, en gros 
    //tu donne un nom au groupe de keyframe  
    //c'est un systeme de keyframe a la after effect / premiere etc
        
       
    
    //tu peux ensuite ajouter une keyframe en faisant soit : 
    //Keyframeholder_Add(Name, Value, Time)
    //ou 
    //Keyframeholder_AddWithCustomCurve(Name, Value, Time, LeftHandle, RightHandle)
    //Name c'est le nom que t'as donné en faisant "Keyframeholder_Create(Name)"
    //Value c'est la valeur que la keyframe rendra au Time (numero de frame) indiqué
    /*
      je t'explique la difference entre les deux apres
      D'abord on va voir comment l'utiliser, c'est hyper simple : 
      pour avoir la valeur a une certaine frame tu fais : 
      Keyframeholder_GetValueAtTime(Name, Time, Interpolation)  
      
      Le deux premier arguments sont les meme qu'avant
      Interpolation est un string (text) qui represente l'interpolation entre chaque clef
      Pour l'instant tu as le choix entre "SmoothStep", "AccelerateDeccelerate" et "CustomCurve"
      tu peux ajouter tes propres interpolator en modifiant la fonction "Keyframeholder_Interpolator" dans Keyframe.inc
      
      Pour utiliser L'interpolator CustomCurve: 
      Cet interpolator est lié a "Keyframeholder_AddWithCustomCurve" et si tu n'as pas ajouter tes 
      keyframes en utilisant cette fonction et que tu essaie d'utiliser le CustomCurve interpolator 
      ca va crashé 
      
      Pour ajouter une CustomCurve :
      //https://upload.wikimedia.org/wikipedia/commons/thumb/d/d0/Bezier_curve.svg/300px-Bezier_curve.svg.png 
      Le "LeftHandle" represente le point P1 sur l'image au dessus
      ET le "RightHandle" le point P2
      sachant que la courbe d'interpolation est de <0, 0> a <1,1> et donc les handle ne doivent pas dépendre de tes keyframe
      si t'as des question/j'ai mal expliqué demande moi ca sera plus simple a expliquer irl    
      
 
        Si tu utilise KeyframeDA.inc et veut ajouter une custom curve, tu dois ajouter une composante Z a 
        tes vecteurs de handle sinon ca plante 
    */
    //example d'utilisation avec la custom curve
    
    Keyframeholder_AddWithCustomCurve("mKey", 0, 0, <1, 1, 0>, <0, 0, 0>)
    Keyframeholder_AddWithCustomCurve("mKey", 0, 10, <1, 1, 0>, <0.5, 0, 0>)
    Keyframeholder_AddWithCustomCurve("mKey", 2, 20, <0.5, 1, 0>, <0, 0, 0>)
    Keyframeholder_AddWithCustomCurve("mKey", 1, 40, <1, 1, 0>, <0, 0, 0>)
    Keyframeholder_AddWithCustomCurve("mKey", 0, 50, <1, 1, 0>, <0, 0, 0>) 
    Keyframeholder_Finish("mKey")
    //left handle default at <1,1> before it's always the top right
    //right handle default at <0,0> because it's always the bot left
   
#end  

            
 //affiche les 70 premiere frame en une seule image, tu peux enlever la for loop et remplacer
 //I par frame_number pour avoir une image a la fois
//#for(I, 0, 70)                            
    #local Value = Keyframeholder_GetValueAtTime("mKey", frame_number, "SmoothStep");
    //la ligne suivante affiche la valeur dans la console
    //#debug concat("\nvalue=", str(Value, 0,-1), " i=", str(I, 0, -1), "\n")
    sphere{
        <frame_number/5, Value, 0>, .1
        pigment{Red}
    }
//#end 