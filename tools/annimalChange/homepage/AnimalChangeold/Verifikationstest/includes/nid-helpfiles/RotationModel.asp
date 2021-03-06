<%
'**************************************************************************************************
' Author: Nina Detlefsen, Danish Institute of Agricultural Sciences
' Description: Calculates Crop rotation
'**************************************************************************************************
function CalcRotation(A, B, ObjectCoeff, byref arrRotationList)
   dim returnValue
   
   returnValue = False
   
'  Dimensionalize variables and sets   
   Dim BasicSolution ' matrix of the basic solution #A * #B
   Redim BasicSolution(Ubound(A),Ubound(B))
   Dim NoSolution
   Dim Optimal
   Dim DualPrice ' Dual prices of crops NUMBERCROPS
   Redim DualPrice(Ubound(A)+Ubound(B)+1)
   Dim ReducedCosts ' Reduced costs NUMBERCROPS * NUMBERCROPS
   Redim ReducedCosts(Ubound(A),Ubound(B))
   
   Dim TestTree
   Dim BasisTree ' Node-arc incidence matrix for basis tree 2NC-1 * 2NC-2
   Redim BasisTree(Ubound(A)+Ubound(B)+1,Ubound(A)+Ubound(B))

   Dim p,q,k,l,i,Z,Z_old,ConstObj,DimA, DimB
   Dim path()
   Dim Delta
   Dim PathFound
   Dim CA1, CA2
   ReDim path(Ubound(A)+Ubound(B)+1)  ' s kan h�jst indeholde alle knuder

   Optimal = false ' Boolean 
   NoSolution=true
   Z = 0
   Z_old=0
   ConstObj=0
   DimA=Ubound(A)+1
   DimB=Ubound(B)+1

   InitBasicSolution BasicSolution,BasisTree,A,B
'   PrintArrayInTable(BasisTree)
'   PrintArrayInTable(BasicSolution)


   do while not(Optimal)

   Z=CalcObjFctValue(ObjectCoeff,BasicSolution)
   TestTree=BasisTree
   CalcDualPrice TestTree,DualPrice,ObjectCoeff,DimA,DimB
   CalcReducedCosts ReducedCosts,DualPrice,ObjectCoeff,DimA,DimB
 '  PrintArrayInTable1(DualPrice)
 '  PrintArrayInTable(ReducedCosts)
   FindMinArr p,q,ReducedCosts,Optimal

   if not(optimal) then
     TestTree=BasisTree
     InitPath path,0,-1
     PathFound = 0
     FindPath TestTree, p, q, path, 0, PathFound
     FindOutVar Delta,k,l,path,BasicSolution,DimA,DimB
     UpdateBasicSolution p,q,path,Delta,BasicSolution,DimA,DimB
     UpdateBasisTree p,q,k,l,BasisTree
  '   PrintArrayInTable(BasicSolution)
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
    ' Udregner objektfunktionsv�rdi
    Dim value
    value=0
    For i=0 To Ubound(BasicSolution,1)
      for j=0 to Ubound(BasicSolution,2)
        value=value + BasicSolution(i,j)*ObjectCoeff(i,j)
      next
    next
    CalcObjFctValue=value
  End Function
'**************************************************************************************************

  Function CalcNoSolution(BasicSolution, ObjectCoeff)
    ' Udregner objektfunktionsv�rdi
    Dim value
    value=false
    For i=0 To Ubound(BasicSolution,1)
      for j=0 to Ubound(BasicSolution,2)
        if BasicSolution(i,j)>0 and ObjectCoeff(i,j)<-1000 then value=true
      next
    next
    CalcNoSolution=value
  End Function
'**************************************************************************************************

Sub InitBasicSolution(BS,BT,A,B)

  Dim rows, cols, r, c, NBcol, CA1, CA2

  CA1 = B
  CA2 = A
  
  rows = UBound(CA2)
  cols = UBound(CA1)

  NBcol=0

  InitArray BS,0
  InitArray BT,0
      
  ' Start i (r,c) = �verste h�jre hj�rne
  r=0
  c=cols
  ' L�s rekursivt
  SolveAll r,c,BS,BT,CA1,CA2,NBcol

End Sub
'**************************************************************************************************

Sub UpdateBasicSolution(p,q,path,Delta,BasicSolution,DimA,DimB)
    Dim i
    i=0
    Do while i<=Ubound(path)/2
      if path(2*i)=-1 then exit do
      BasicSolution(path(2*i),path(2*i+1)-DimA)=BasicSolution(path(2*i),path(2*i+1)-DimA)-Delta
      i=i+1
    loop
    i=1
    Do while i<=Ubound(path)/2
      if path(2*i)=-1 then exit do
      BasicSolution(path(2*i),path(2*i-1)-DimA)=BasicSolution(path(2*i),path(2*i-1)-DimA)+Delta
      i=i+1
    loop

    BasicSolution(path(0),path(2*i-1)-DimA)=Delta

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


'Sub InitNonBasisID(NonBasisID,NumberCrops)

'  Initialize NonBasisID
'   k=0
'   for i=0 to NumberCrops-1
'     for j=0 to NumberCrops-1
'       if not(j=i) and not(j=i+1) then
'       NonBasisID(k,0)=i+1
'       NonBasisID(k,1)=NumberCrops+j+1
'       k=k+1
'       end if 
'     next	 
'   next
'   
'End Sub
'**************************************************************************************************
   
  Sub DepthFirst(tree, n1, DualPrice,ObjectCoeff,DimA,DimB)
    ' Tr�et genneml�bes rekursivt med udgangspunkt i startknuden n1.
    ' Forhvert af n1's b�rn genneml�bes deltr�et af barnet (depth first)
    Dim n2
    n2 = FindNextChild(tree,n1)
    Do While n2>-1
      DoSomething tree,n1,n2,DualPrice,ObjectCoeff,DimA,DimB
      SetVisited tree,n1,n2
      DepthFirst tree,n2,DualPrice,ObjectCoeff,DimA,DimB 
      n2 = FindNextChild(tree,n1)
    Loop
  End Sub
'**************************************************************************************************
  
  Function FindNextChild(tree, n)
    ' Find n�ste ubes�gte barn af n. Hvis v�rdien af (n,c) = (c,n) og positiv, s� er c ubes�gt
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
    ' V�rdien under diagonalen t�lles op for at markere, at kanten er bes�gt.
    for j=0 to Ubound(tree,2)
      if not(tree(n1,j)=0) and not(tree(n2,j)=0) then
        tree(n1,j)=2*tree(n1,j)
        tree(n2,j)=2*tree(n2,j)
      end if
    next
  End Sub
'**************************************************************************************************

  Sub DoSomething(tree,n1,n2,DualPrice,ObjectCoeff,DimA,DimB)
'    Response.write " n1, n2 = " & n1 & " " & n2 & "<br>"
    if n1 < n2 then
      DualPrice(n2)=DualPrice(n1)-ObjectCoeff(n1,n2-DimA)
    else
      DualPrice(n2)=ObjectCoeff(n2,n1-DimA)+DualPrice(n1)
    end if
'   PrintArrayInTable1(DualPrice)
  End Sub
'**************************************************************************************************

  Sub InitPath(s,d,v)
    ' S�t elementer i s fra nr d til v�rdien v
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
    n = FindNextChild(tree,n1)        ' Lad n v�re n1's f�rste barn i tr�et tree
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
 '   printpath s
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
    Response.Write "<br>Path = " & str
  End Sub
'**************************************************************************************************

  Sub CalcDualPrice(TestTree,DualPrice,ObjectCoeff,DimA,DimB)
  Dim startknude
 '      Response.write " Her starter CalcDualPrices <br>" 
    startknude=Ubound(TestTree,1)
    DualPrice(startknude)=0
    DepthFirst TestTree, startknude, DualPrice,ObjectCoeff,DimA,DimB

  end sub
'**************************************************************************************************

Sub CalcReducedCosts(ReducedCosts,DualPrice,ObjectCoeff,DimA,DimB)
'  Calculate ReducedCosts

    for i=0 to Ubound(ReducedCosts,1)
      for j=0 to Ubound(ReducedCosts,2)
        ReducedCosts(i,j)=DualPrice(i) -DualPrice(j+DimA) - ObjectCoeff(i,j)
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
    q=q+Ubound(ReducedCosts,1)+1
    end if
end sub
'**************************************************************************************************

Sub FindOutVar(Delta,k,l,path,BasicSolution,DimA,DimB)
    k=path(0)
    l=path(1)
    Delta = BasicSolution(path(0),path(1)-DimA)
'   Response.write "<br> Test af Delta initialisering i Fkt FindOutVar " & Delta & "<br>"
    Dim i
    i=1
    Do while i<=Ubound(path)/2
      if path(2*i)=-1 then exit do
      if Delta > BasicSolution(path(2*i),path(2*i+1)-DimA) then 
      Delta = BasicSolution(path(2*i),path(2*i+1)-DimA)
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
    ' S�tter val i alle celler i arrayet
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