                               
 //ArrObj is an array of single element array
#include "DiskArray2.inc"



DA_Create("mArrSym", 10)
DA_LoadInMemory("mArrSym")            
#declare mArr = array[1]
DA_GetArrayFromMemory("mArrSym", mArr) 

DA_Add("mArrSym", "\"cava?\"")

DA_GetArrayFromMemory("mArrSym", mArr)
DA_SaveToDisk("mArrSym")

#error DA_Get("mArrSym", 0)