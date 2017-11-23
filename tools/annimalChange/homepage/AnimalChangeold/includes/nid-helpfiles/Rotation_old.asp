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
   
   PrintArrayInTable(ObjectCoeff)
   PrintVecInTable(CropArea)

'  Dimensionalize variables and sets   
   Dim BasicSolution ' matrix of the basic solution NUMBERCROPS * NUMBERCROPS
   Redim BasicSolution(NumberCrops-1,NumberCrops-1)
   Dim DualPriceCrop ' Dual prices of crops NUMBERCROPS
   Redim DualPriceCrop(NumberCrops-1)
   Dim DualPricePreCrop ' Dual prices of precrops NUMBERCROPS
   Redim DualPricePreCrop(NumberCrops-1)
   Dim ReducedCosts ' Reduced costs NUMBERCROPS * NUMBERCROPS
   Redim ReducedCosts(NumberCrops-1,NumberCrops-1)
   
   Dim BasisID ' set of indices of basic variables 2*NUMBERCROPS - 1
   Redim BasisID(2*NumberCrops-2,1)
   Dim NonBasisID ' set of indeces of nonbasic variables NUMBERCROPS * NUMBERCROPS - 2*NUMBERCROPS +1
   Redim NonBasisID(NumberCrops*NumberCrops-2*NumberCrops,1)

   Dim ForwardArcs()
   Dim BackwardArcs()




   Dim Optimal
   Optimal = false ' Boolean 

   InitBasicSolution BasicSolution,CropArea,NumberCrops
   PrintArrayInTable(BasicSolution)

   InitBasisID BasisID,NumberCrops
   PrintArrayInTable(BasisID)

   InitNonBasisID NonBasisID,NumberCrops
'   PrintArrayInTable(NonBasisID)

   Test=IsInArray(1, 8, BasisID)
'   Response.write "TEst " & Test

 
   CalculateDualPrices ObjectCoeff,DualPriceCrop,DualPricePreCrop,NumberCrops,ForwardArcs,BackwardArcs,BasisID
'  Repeat until Optimal = true
'  Calculate DualPrices
'   DualPriceCrop(NumberCrops) = 0
'   DPC = 1
'   DPPC = 0
'   j=NumberCrops
'   do 
'   for i=1 to NumberCrops
'     if (i,j) in BasisID
'	   DualPricePreCrop(i) =  ObjectCoeff(i,j) + DualPriceCrop(j)
'	   DPPC = DPPC + 1
'	   exit for
'	 end if
'   next
'   for j=1 to NumberCrops
'     if (i,j) in BasisID then 
'	   DualPriceCrop(j) =  DualPricePreCrop(i) - ObjectCoeff(i,j) 
'	   DPC = DPC + 1
'	   exit for 
'	 end if
'   next
'   loop until (DPC = NumberCrops and DPPC = NumberCrops)  

' Calculate reduced costs
'   for i=1 to NumberCrops  ' Alternativt for (i,j) in NonBasisID
'     for j=1 to NumberCrops
'       ReducedCosts(i,j) = ObjectCoeff(i,j) - DualPricePreCrop(i) + DualPriceCrop(j)
'	 next
'   next

' Optimality check
'   Optimal = false
'   Incoming={}
'   for i=1 to NumberCrops ' Alternativt for (i,j) in NonBasisID
'     for j=1 to NumberCrops
'       if ReducedCosts(i,j) < 0 then 
'         Incoming = (i,j)
'		 exit for
'		 exit for 
'	   end if
'	 next
'	 Optimal = true
'   next

 ' Calculate unique cycle 
'   (p,q) = Incoming
'   D={}  'Decrease
'   I={}  'Increase
'   for j=1 to NumberCrops
'     if (p,j) in B then 
'       for i=1 to NumberCrops
'	     if (i,j) in B then 
'		   for k=1 to NumberCrops
'		     if (i,k) in B then
'			   if k=q then exit for
'			   'mangler et par lykker
'			 end if
'		   next
'		 end if
'	   next
'     end if
'   next
 ' D skal indeholde de (i,j) som falder 
 ' I skal indeholde de (i,j) som stiger

 ' Find leaving variable
'   Delta=1000000
'   For (i,j) in D 
'     if BasicSolution(i,j) < Delta then 
'	 Delta = BasicSolution(i,j)
'	 Out=(i,j)
'   next 
	
 ' Update BasisID
'   BasisID = BasisID + Incoming - Out

 ' Update NonBasisID
'   NonBasisID = NonBasisID - Incoming + Out

 ' Update Solution
'   for i=1 to NumberCrops
'     for j=1 to NumberCrops
'	   if (i,j) in D then BasicSolution(i,j) = BasicSolution(i,j) - Delta
'	   if (i,j) in I then BasicSolution(i,j) = BasicSolution(i,j) + Delta
'     next
'   next


'  Repeat until Optimal=true


'   If Optimal=true then
'     NonFeasible=false
'	   For (i,j) in BasisID
'	     if BasicSolution(i,j) > 0 and ObjectCoeff(i,j)<0 then NonFeasible=true
'	   next

'   If NonFeasible=true then 
'   returnValue=false
'   else 
'   returValue=true
'   arrRotationList = BasicSolution(i,j)
'   end if

   CalcRotation = returnValue
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


Sub InitNonBasisID(NonBasisID,NumberCrops)

'  Initialize NonBasisID
   k=0
   for i=0 to NumberCrops-1
     for j=0 to NumberCrops-1
       if not(j=i) and not(j=i+1) then
       NonBasisID(k,0)=i+1
       NonBasisID(k,1)=NumberCrops+j+1
       k=k+1
       end if 
     next	 
   next
   
End Sub
   

Sub CalculateDualPrices(ObjectCoeff,DualPriceCrop,DualPricePreCrop,NumberCrops,ForwardArcs,BackwardArcs,BasisID)
'  Calculate DualPrices
   Response.write " Her starter CalculateDualPrices <br>" 
   Dim PreCrops()
   Redim PreCrops(0)
   Dim Crops()
   Redim Crops(0)
   DualPriceCrop(NumberCrops-1) = 0
   Crops(0)=NumberCrops+NumberCrops-1
   DPC = 1
   DPPC = 0
   j=NumberCrops-1
 '  do 
   BArc 8, 4, BackwardArcs, BasisID, tom
   Response.write "Tom " & tom
   if not(tom) then PrintVecInTable(BackwardArcs)
   BArc (j+NumberCrops+1), NumberCrops+1, BackwardArcs, BasisID, tom

   for i=0 to Ubound(BackwardArcs)

 '    Response.write i & " BackwardArcs " & BackwardArcs(i) & "<br>"
 '    Response.write i & " ObjectCoeff " & ObjectCoeff(i,j) & "<br>"
 '    Response.write j & " DualPriceCrop(j) " & DualPriceCrop(j) & "<br>"
     l=BackwardArcs(i)
     test=false
     for k=0 to Ubound(PreCrops)
       if PreCrops(k)=l then test=true
     next 
     if test=false then
     DualPricePreCrop(l-1) =  ObjectCoeff(BackwardArcs(i)-1,j) + DualPriceCrop(j)
 '    PrintVecInTable(DualPricePreCrop)

     Redim preserve PreCrops(DPPC)
     PreCrops(DPPC)=BackwardArcs(i)
 '    Response.write "PreCrops(DPPC) " & PreCrops(DPPC) & "<br>"
     DPPC = DPPC + 1
     end if
   next
 '  PrintVecInTable(PreCrops)
    	  
   for k=0 to Ubound(PreCrops)
   i=PreCrops(k)
   Response.write "i " & i 
   FArc i, ForwardArcs, BasisID
   PrintVecInTable(ForwardArcs)
   for h=0 to Ubound(ForwardArcs)
     test=false
     for l=0 to Ubound(Crops)
       if Crops(l)=ForwardArcs(h) then 
       test=true
       Response.write "Test " & test & "<br>"
       end if
     next 
     if test=false then
 	   j=ForwardArcs(h)-NumberCrops-1
     Response.write i & " Precrop " & "<br>"
     Response.write j & " Crop "  & "<br>"
     Response.write " ObjectCoeff(i,j) " & ObjectCoeff(i,j) & "<br>"
     Response.write " DualPricePreCrop(i) " & DualPricePreCrop(i) & "<br>"

     DualPriceCrop(j) =  DualPricePreCrop(i) - ObjectCoeff(i,j) 
     Redim preserve Crops(DPC)
     Crops(DPC)=ForwardArcs(h)
'     Response.write "Crops(DPC) " & Crops(DPC) & "<br>"
 	   DPC = DPC + 1
     end if
   next
   next

  
  Response.write "end of calculations "
   
   PrintVecInTable(DualPriceCrop)
   PrintVecInTable(DualPricePreCrop)

'   loop until (DPC = NumberCrops and DPPC = NumberCrops)  
end sub



   
function IsInArray(p, q, arr)
   dim returnValue
   returnValue = False

   for i=0 to UBound(arr,1)
     if arr(i,0) = p and arr(i,1) = q then
       returnValue = True
     end if
   next

   IsInArray = returnValue
end function

function FArc(p, notq, Arcs, arr, tom)

   k=0
   tom=true
   for i=0 to UBound(arr,1)
     if arr(i,0) = p and not(arr(i,0)=notq)  then
       tom=false
       Redim preserve Arcs(k)
       Arcs(k)=arr(i,1)
       k=k+1
     end if
   next
end function

function BArc(q, notp, Arcs, arr, tom)

   k=0
   tom=true
   for i=0 to UBound(arr,1)
     if arr(i,1) = q and not(arr(i,1)=notp) then
       tom=false
       Redim preserve Arcs(k)
       Arcs(k)=arr(i,0)
       k=k+1
     end if
   next
end function


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