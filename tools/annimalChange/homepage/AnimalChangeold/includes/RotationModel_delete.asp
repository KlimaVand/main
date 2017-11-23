<%
'**************************************************************************************************
' Author: Nina Detlefsen, Danish Institute of Agricultural Sciences
' Description: Calculates Crop rotation
'**************************************************************************************************
function CalcRotation(CropArea, ObjectCoeff, byref arrRotationList)
   dim returnValue
   
   returnValue = False
   
'  Dimensionalize variables and sets   
   Dim BasicSolution ' matrix of the basic solution NUMBERCROPS * NUMBERCROPS
   Redim BasicSolution(NumberCrops-1,NumberCrops-1)
   Dim NoSolution
   Dim Optimal
   Dim DualPrice ' Dual prices of crops NUMBERCROPS
   Redim DualPrice(2*NumberCrops-1)
   Dim ReducedCosts ' Reduced costs NUMBERCROPS * NUMBERCROPS
   Redim ReducedCosts(NumberCrops-1,NumberCrops-1)
   
   Dim TestTree
   Dim BasisTree ' Node-arc incidence matrix for basis tree 2NC-1 * 2NC-2
   Redim BasisTree(2*NumberCrops-1,2*NumberCrops-2)

   Dim p,q,k,l,i,Z,Z_old,ConstObj
   Dim path()
   Dim Delta
   Dim PathFound
   Dim CA1, CA2
   ReDim path(2*NumberCrops-1)  ' s kan højst indeholde alle knuder

   Optimal = false ' Boolean 
   NoSolution=true
   Z = 0
   Z_old=0
   ConstObj=0

   InitBasicSolution BasicSolution,BasisTree,CropArea

   do while not(Optimal)

   Z=CalcObjFctValue(ObjectCoeff,BasicSolution)
   TestTree=BasisTree
   CalcDualPrice TestTree,NumberCrops,DualPrice,ObjectCoeff
   CalcReducedCosts ReducedCosts,NumberCrops,DualPrice,ObjectCoeff
   FindMinArr p,q,ReducedCosts,Optimal

   if not(optimal) then
     TestTree=BasisTree
     InitPath path,0,-1
     PathFound = 0
     FindPath TestTree, p, q, path, 0, PathFound
     FindOutVar Delta,k,l,path,BasicSolution,NumberCrops
     UpdateBasicSolution p,q,path,Delta,BasicSolution,NumberCrops
     UpdateBasisTree p,q,k,l,BasisTree
   end if

   if Z=Z_old then ConstObj=ConstObj+1
   Z_old=Z
   if ConstObj=10 then exit do

   loop

   NoSolution=CalcNoSolution(BasicSolution, ObjectCoeff)
   if not(NoSolution) then arrRotationList=BasicSolution
   if Optimal and not(NoSolution) then returnValue=true
   CalcRotation = returnValue
end function
'**************************************************************************************************

  Function CalcObjFctValue(ObjectCoeff,BasicSolution)
    ' Udregner objektfunktionsværdi
    Dim value
    value=0
    For i=0 To Ubound(BasicSolution)
      for j=0 to Ubound(BasicSolution)
        value=value + BasicSolution(i,j)*ObjectCoeff(i,j)
      next
    next
    CalcObjFctValue=value
  End Function
'**************************************************************************************************

  Function CalcNoSolution(BasicSolution, ObjectCoeff)
    ' Udregner objektfunktionsværdi
    Dim value
    value=false
    For i=0 To Ubound(BasicSolution)
      for j=0 to Ubound(BasicSolution)
        if BasicSolution(i,j)>0 and ObjectCoeff(i,j)<-1000 then value=true
      next
    next
    CalcNoSolution=value
  End Function
'**************************************************************************************************

Sub InitBasicSolution(A,BT,CA)

  Dim rows, cols, r, c, NBcol, CA1, CA2

  CA1 = CA
  CA2 = CA
  
  rows = UBound(CA2)
  cols = UBound(CA1)

  NBcol=0

  InitArray A,0
  InitArray BT,0
      
  ' Start i (r,c) = øverste højre hjørne
  r=0
  c=cols
  ' Løs rekursivt
  SolveAll r,c,A,BT,CA1,CA2,NBcol

End Sub
'**************************************************************************************************

Sub UpdateBasicSolution(p,q,path,Delta,BasicSolution,NumberCrops)
    Dim i
    i=0
    Do while i<=Ubound(path)/2
      if path(2*i)=-1 then exit do
      BasicSolution(path(2*i),path(2*i+1)-NumberCrops)=BasicSolution(path(2*i),path(2*i+1)-NumberCrops)-Delta
      i=i+1
    loop
    i=1
    Do while i<=Ubound(path)/2
      if path(2*i)=-1 then exit do
      BasicSolution(path(2*i),path(2*i-1)-NumberCrops)=BasicSolution(path(2*i),path(2*i-1)-NumberCrops)+Delta
      i=i+1
    loop

    BasicSolution(path(0),path(2*i-1)-NumberCrops)=Delta

End Sub
'**************************************************************************************************

Sub UpdateBasisTree(p,q,k,l,BasisTree)

'  Initialize BasisTree
   for j=0 to Ubound(BasisTree,2)
     if BasisTree(k,j)=1 and BasisTree(l,j)=-1 then
       BasisTree(k,j)=0   
       BasisTree(l,j)=0
       BasisTree(p,j)=1
       BasisTree(q,j)=-1     
     end if
   next

End Sub
'**************************************************************************************************


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
'**************************************************************************************************
   
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
'**************************************************************************************************
  
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
'**************************************************************************************************

  Sub SetVisited(tree, n1, n2)
    ' Værdien under diagonalen tælles op for at markere, at kanten er besøgt.
    for j=0 to Ubound(tree,2)
      if not(tree(n1,j)=0) and not(tree(n2,j)=0) then
        tree(n1,j)=2*tree(n1,j)
        tree(n2,j)=2*tree(n2,j)
      end if
    next
  End Sub
'**************************************************************************************************

  Sub DoSomething(tree,n1,n2,NumberCrops,DualPrice,ObjectCoeff)
    if n1 < n2 then
      DualPrice(n2)=DualPrice(n1)-ObjectCoeff(n1,n2-NumberCrops)
    else
      DualPrice(n2)=ObjectCoeff(n2,n1-NumberCrops)+DualPrice(n1)
    end if
  End Sub
'**************************************************************************************************

  Sub InitPath(s,d,v)
    ' Sæt elementer i s fra nr d til værdien v
    Dim i
    For i = d To UBound(s)
      s(i) = v
    Next
  End Sub
'**************************************************************************************************
  
  Sub FindPath(tree, n1, n2, s, d, PathFound)
    ' Skriv stien mellem n1 og n2 i et array s
    Dim n
    WritePath n1,s,d                  ' Skriv n
    n = FindNextChild(tree,n1)        ' Lad n være n1's første barn i træet tree
    Do While PathFound = 0
      d = DepthInTree(n,d)
      If CInt(n)=CInt(-1) Then Exit Do
      SetVisited tree,n1,n
      If CInt(n)=CInt(n2) Then
        WritePath n,s,d
        PathFound = 1
        Exit Do
      End If
      FindPath tree,n,n2,s,d,PathFound
      n = FindNextChild(tree,n1)
    Loop
  End Sub
'**************************************************************************************************
  
  Sub WritePath(n,s,d)
    s(d) = n
    InitPath s,d+1,-1
'    printpath s
  End Sub
'**************************************************************************************************


  Function DepthInTree(n,d)
    If CInt(n)>CInt(-1) Then 
      d = d + 1
    Else
      d = d - 1
    End If
    DepthInTree = d
  End Function
'**************************************************************************************************
  
  Sub PrintPath(s)
    Dim i, str
    str = CStr(s(0))
    i = 1
    Do while i<=Ubound(s)
      if s(i)=-1 then exit do
      str = str & " - " & CStr(s(i))
      i = i + 1
    Loop 
'    Response.Write "<br>Path = " & str
  End Sub
'**************************************************************************************************

  Sub CalcDualPrice(TestTree,NumberCrops,DualPrice,ObjectCoeff)
  Dim startknude
 '      Response.write " Her starter CalcDualPrices <br>" 
    startknude=Ubound(TestTree,1)
    DualPrice(startknude)=0
    DepthFirst TestTree, startknude, NumberCrops,DualPrice,ObjectCoeff

  end sub
'**************************************************************************************************

Sub CalcReducedCosts(ReducedCosts,NumberCrops,DualPrice,ObjectCoeff)
'  Calculate ReducedCosts

    for i=0 to Ubound(ReducedCosts,1)
      for j=0 to Ubound(ReducedCosts,2)
        ReducedCosts(i,j)=DualPrice(i) -DualPrice(j+NumberCrops) - ObjectCoeff(i,j)
      next
    next
end sub
'**************************************************************************************************
   
Sub FindMaxArr(p,q,ReducedCosts,Optimal)
    p=-1
    q=-1
    max=0
    for i=0 to Ubound(ReducedCosts,1)
      for j=0 to Ubound(ReducedCosts,2)
        if ReducedCosts(i,j)>max then
          max=ReducedCosts(i,j)
          p=i
          q=j
        end if
      next
    next
    if max=0 then 
    Optimal=true
    else 
    q=q+Ubound(ReducedCosts,2)+1
    end if
end sub
'**************************************************************************************************

Sub FindMinArr(p,q,ReducedCosts,Optimal)
    Dim min
    p=-1
    q=-1
    min=0
    for i=0 to Ubound(ReducedCosts,1)
      for j=0 to Ubound(ReducedCosts,2)
        if ReducedCosts(i,j)<min then
          min=ReducedCosts(i,j)
          p=i
          q=j
        end if
      next
    next
    if min=0 then 
    Optimal=true
    else 
    q=q+Ubound(ReducedCosts,2)+1
    end if
end sub
'**************************************************************************************************

Sub FindOutVar(Delta,k,l,path,BasicSolution,NumberCrops)
    k=path(0)
    l=path(1)
    Delta = BasicSolution(path(0),path(1)-NumberCrops)
 '   Response.write "<br> Test af Delta initialisering i Fkt FindOutVar " & Delta & "<br>"
    Dim i
    i=1
    Do while i<=Ubound(path)/2
      if path(2*i)=-1 then exit do
      if Delta > BasicSolution(path(2*i),path(2*i+1)-NumberCrops) then 
      Delta = BasicSolution(path(2*i),path(2*i+1)-NumberCrops)
      k=path(2*i)
      l=path(2*i+1)
      end if
      i=i+1
    loop

end sub
'**************************************************************************************************

  Sub SolveAll (i, j, A, BT, CA1, CA2,NBcol)
    Do While i<=UBound(CA2) And j>=0
      Solve i,j,A,BT,CA1,CA2,NBcol
      NextCell i,j,A,CA1,CA2,NBcol
      SolveAll i,j,A,BT,CA1,CA2,NBcol
    Loop
  End Sub
'**************************************************************************************************
  
  Sub Solve (i, j, A, BT, CA1, CA2, NBcol)
    Dim y 
    y = Min(CA1(j),CA2(i))
    A(i,j) = y
    BT(i,NBcol) = 1
    BT(j+Ubound(CA2)+1,NBcol)=-1
    CA1(j) = CA1(j) - y
    CA2(i) = CA2(i) - y
  End Sub
'**************************************************************************************************
  
  Sub NextCell (i, j, A, CA1, CA2, NBcol)
    If CA1(j) < 0.0001 and CA2(i) >= 0.0001 Then j = j - 1
    If CA2(i) < 0.0001 and CA1(j) >= 0.0001 Then i = i + 1
    If CA1(j) < 0.0001 and CA2(i) < 0.0001 Then i = i + 1
    NBcol=NBcol+1
  End Sub
'**************************************************************************************************

  Function Max (a,b)
    Max = b
    If CDbl(a) > CDbl(b) Then Max = a
  End Function
'**************************************************************************************************
  
  Function Min (a,b)
    Min = b
    If CDbl(a) < CDbl(b) Then Min = a
  End Function
'**************************************************************************************************
  
  Sub InitArray(anArray,val)
    ' Sætter val i alle celler i arrayet
    Dim r,c
    For r = 0 To UBound(anArray,1)
      For c = 0 To UBound(anArray,2)
        anArray(r,c) = val
      Next
    Next
  End Sub
'**************************************************************************************************

  Sub PrintArrayInTable(anArray)
  dim r,c
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
'**************************************************************************************************

  Sub PrintArrayInTable1(anArray)
    Dim r,c
    Response.Write "<br><table border>" & Chr(13)
    Response.Write "<tr>" & Chr(13)
    For c = 0 to UBound(anArray)
      Response.Write "<td>" & CStr(anArray(c)) & "</td>" & Chr(13)
    Next
    Response.Write "</tr>" & Chr(13)
    Response.Write "</table>" & Chr(13)
  End Sub
'**************************************************************************************************

  Sub PrintArrayInTable2(anArray)
    Dim r,c
    Response.Write "<br><table border>" & Chr(13)
    For r = 0 to UBound(anArray,1)
      Response.Write "<tr>" & Chr(13)
      For c = 0 to UBound(anArray,2)
        Response.Write "<td>" & CStr(anArray(r,c)) & "</td>" & Chr(13)
      Next
      Response.Write "</tr>" & Chr(13)
    Next
    Response.Write "</table>" & Chr(13)
  End Sub
'**************************************************************************************************
 
  Sub PrintVecInTable(anArray)
    Response.Write "<table border>" & Chr(13)
    For r = 0 to UBound(anArray,1)
      Response.Write "<tr>" & Chr(13)
        Response.Write "<td>" & CStr(anArray(r)) & "</td>" & Chr(13)
      Response.Write "</tr>" & Chr(13)
    Next
    Response.Write "</table>" & Chr(13)
  End Sub
'**************************************************************************************************

%>