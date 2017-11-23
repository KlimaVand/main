<%
'**************************************************************************************************
' Author: Nina Detlefsen, Danish Institute of Agricultural Sciences
' Description: 
'**************************************************************************************************
'function CalcRotation(arrCropList, byref arrRotationList)
function CalcRotation
   dim returnValue
   
   returnValue = False
   
'  Write function logics here   


'  Initialize 
   NumberCrops = 4
   Dim ObjectCoeff   ' matrix of objective function coefficients NUMBERCROPS * NUMBERCROPS
                     ' -10000 indicates that it is an impossible combination of precrop and crop
   Redim ObjectCoeff(NumberCrops,NumberCrops)
   Dim CropArea  ' array of crop areas NUMBERCROPS
   Redim CropArea(NumberCrops)

   for i=0 to NumberCrops-1
     for j=0 to NumberCrops-1
       ObjectCoeff(i,j)=i*j+i+j
     next
     CropArea(i)=20*i+10
   next
   
'  Dimensionalize variables and sets   
   Dim BasicSolution ' matrix of the basic solution NUMBERCROPS * NUMBERCROPS
   Redim BasicSolution(NumberCrops-1,NumberCrops-1)
   
   Dim BasisID ' set of indices of basic variables 2*NUMBERCROPS - 1
   Redim BasisID(2*NumberCrops-2,1)


   InitBasicSolution BasicSolution,CropArea,NumberCrops
   PrintArrayInTable(BasicSolution)

   InitBasisID BasisID,NumberCrops
   PrintArrayInTable(BasisID)

end function
'**************************************************************************************************

  Sub InitBasicSolution(BasicSolution,CropArea,NumberCrops)

'  Initialize solution
   for i=0 to NumberCrops-1
     for j=0 to NumberCrops-1
       if i=j then 
       BasicSolution(i,i) = CropArea(i)
       else 
       BasicSolution(i,j) = 0 
       end if
     next 
   next
  End Sub


  Sub InitBasisID(BasisID,NumberCrops)

'  Initialize BasisID
   for i=0 to 2*NumberCrops-2
     BasisID(i,0)= Int(i/2) + 1
     BasisID(i,1)= Int((i+1)/2) + NumberCrops + 1 
   next
  End Sub


  Sub PrintArrayInTable(anArray)
    Response.Write "<table border>" & Chr(13)
    For r = 0 to UBound(anArray,1)
      Response.Write "<tr>" & Chr(13)
      For c = 0 to UBound(anArray,2)
        Response.Write "<td>" & CStr(anArray(r,c)) & "</td>" & Chr(13)
      Next
      Response.Write "</tr>" & Chr(13)
    Next
    Response.Write "</table>" & Chr(13)
  End Sub

  Sub PrintVecInTable(anArray)
    Response.Write "<table border>" & Chr(13)
    For r = 0 to UBound(anArray,1)
      Response.Write "<tr>" & Chr(13)
        Response.Write "<td>" & CStr(anArray(r)) & "</td>" & Chr(13)
      Response.Write "</tr>" & Chr(13)
    Next
    Response.Write "</table>" & Chr(13)
  End Sub

CalcRotation



%>