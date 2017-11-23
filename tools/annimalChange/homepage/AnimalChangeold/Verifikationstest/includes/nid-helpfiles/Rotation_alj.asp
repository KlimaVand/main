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
   NumberCrops = 3
   Dim ObjectCoeff   ' matrix of objective function coefficients NUMBERCROPS * NUMBERCROPS
                     ' -10000 indicates that it is an impossible combination of precrop and crop
   Redim ObjectCoeff(NumberCrops-1,NumberCrops-1)
   Dim CropArea  ' array of crop areas NUMBERCROPS
   Redim CropArea(NumberCrops-1)

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
   Dim DualPrice ' Dual prices of crops NUMBERCROPS
   Redim DualPrice(2*NumberCrops-1)
   Dim ReducedCosts ' Reduced costs NUMBERCROPS * NUMBERCROPS
   Redim ReducedCosts(NumberCrops-1,NumberCrops-1)
   
   Dim BasisTree ' Node-arc incidence matrix for basis tree 2NC-1 * 2NC-2
   Redim BasisTree(2*NumberCrops-1,2*NumberCrops-2)

   Dim ForwardArcs()
   Dim BackwardArcs()




   Dim Optimal
   Optimal = false ' Boolean 

   InitBasicSolution BasicSolution,CropArea,NumberCrops
   PrintArrayInTable(BasicSolution)

   InitBasisTree BasisTree,NumberCrops
   PrintArrayInTable(BasisTree)

   TestTree=BasisTree

   CalcDualPrice TestTree,NumberCrops,DualPrice,ObjectCoeff

   PrintVecInTable(DualPrice)

   CalcReducedCosts ReducedCosts,NumberCrops,DualPrice,ObjectCoeff

   PrintArrayInTable(ReducedCosts)


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


Sub InitBasisTree(BasisTree,NumberCrops)

'  Initialize BasisTree
   for i=0 to 2*NumberCrops-1
     for j=0 to 2*NumberCrops-2
     BasisTree(i,j)=0
     next
   next

   for j=0 to 2*NumberCrops-2
     BasisTree(Int(j/2),j)= 1
     BasisTree(Int((j+1)/2) + NumberCrops ,j)= -1 
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
   


  Sub DepthFirst(tree, n1, NumberCrops,DualPrice,ObjectCoeff)
    ' Træet gennemløbes rekursivt med udgangspunkt i startknuden n1.
    ' Forhvert af n1's børn gennemløbes deltræet af barnet (depth first)
    Dim n2
    n2 = FindNextChild(tree,n1)
    Do While n2>-1
      DoSomething tree,n1,n2,NumberCrops,DualPrice,ObjectCoeff
      SetVisited tree,n1,n2
      DepthFirst tree,n2,NumberCrops,DualPrice,ObjectCoeff 
      n2 = FindNextChild(tree,n1)
    Loop
  End Sub

  Function FindNextChild(tree, n)
    ' Find næste ubesøgte barn af n. Hvis værdien af (n,c) = (c,n) og positiv, så er c ubesøgt
    Dim i,j
    FindNextChild = -1
    For j=0 To Ubound(tree,2)
      If tree(n,j) = 1 or tree(n,j) = -1  Then 
        for i=0 to Ubound(tree,1)
          if not(i=n) then 
            if (tree(i,j) = 1 or tree(i,j) = -1) then 
              FindNextChild = i
            end if
          end if
        next
        Exit For
      End If
    Next
  End Function

  Sub SetVisited(tree, n1, n2)
    ' Værdien under diagonalen tælles op for at markere, at kanten er besøgt.
    for j=0 to Ubound(tree,2)
      if not(tree(n1,j)=0) and not(tree(n2,j)=0) then
        tree(n1,j)=2*tree(n1,j)
        tree(n2,j)=2*tree(n2,j)
      end if
    next
  End Sub

  Sub DoSomething(tree,n1,n2,NumberCrops,DualPrice,ObjectCoeff)
    Response.Write "<br>Behandler (" & Cstr(n1) & "," & Cstr(n2) & ")"  
    if n1 < n2 then
      DualPrice(n2)=DualPrice(n1)-ObjectCoeff(n1,n2-NumberCrops)
    else
      DualPrice(n2)=ObjectCoeff(n2,n1-NumberCrops)+DualPrice(n1)
    end if
  End Sub

  Sub FindPath(tree, n1, n2, s)
    ' Skriv stien mellem n1 og n2 i en streng s
    Dim n
    n = FindNextChild(tree,n1)
    Do While (n>-1 And n<>n2)
      WritePath tree,n1,n,s
      SetVisited tree,n1,n
      FindPath tree,n,n2,s
      n = FindNextChild(tree,n1)
    Loop
  End Sub
  
  Sub WritePath(tree,n1,n2,s)
    s = s & " - " & CStr(n2)
    Response.Write "<br>s = " & Cstr(s)  
  End Sub

  Sub CalcDualPrice(TestTree,NumberCrops,DualPrice,ObjectCoeff)
       Response.write " Her starter CalcDualPrices <br>" 
    startknude=Ubound(TestTree,1)
    DualPrice(startknude)=0
    DepthFirst TestTree, startknude, NumberCrops,DualPrice,ObjectCoeff

  end sub


Sub CalcReducedCosts(ReducedCosts,NumberCrops,DualPrice,ObjectCoeff)
'  Calculate ReducedCosts

    for i=0 to Ubound(ReducedCosts,1)
      for j=0 to Ubound(ReducedCosts,2)
        ReducedCosts(i,j)=DualPrice(i) -DualPrice(j+NumberCrops) - ObjectCoeff(i,j)
      next
    next
end sub
   

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