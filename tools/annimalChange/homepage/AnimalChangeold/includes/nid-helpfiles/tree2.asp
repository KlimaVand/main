<%
  ' Repræsenter et træ med n knuder som et nxn array (grafens incidensmatrix), 
  ' hvor m_i,j = 1, hvis (i,j) er en kant i grafen og 0 ellers.
  
  ''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
  '  Funktioner til at lave og gennemløbe træet
  
  Dim arrTree, NumberNodes, Weight
  
  Sub InitTree(tree,nodes)
    ' Sætter 0 i alle celler i arrayet
    Dim i,j
    Redim tree(nodes-1,nodes-1)
    For i = 0 To nodes-1
      For j = 0 To nodes-1
        tree(i,j) = 0
      Next
    Next
  End Sub
  
  Sub AddEdge(tree,n1,n2,w)
    ' Sætter vægten til w for begge celler for både (n1,n2) og (n2,n1)
    ' w skal være positiv
    tree(n1-1,n2-1) = w
    tree(n2-1,n1-1) = w
  End Sub
  
  Sub DepthFirst(tree, n1)
    ' Træet gennemløbes rekursivt med udgangspunkt i startknuden n1.
    ' Forhvert af n1's børn gennemløbes deltræet af barnet (depth first)
    Dim n2
    n2 = FindNextChild(tree,n1)
    Do While n2>-1
      DoSomething tree,n1,n2
      SetVisited tree,n1,n2
      DepthFirst tree,n2 
      n2 = FindNextChild(tree,n1)
    Loop
  End Sub
  
  Function FindNextChild(tree, n)
    ' Find næste ubesøgte barn af n. Hvis værdien af (n,c) = (c,n) og positiv, så er c ubesøgt
    Dim i
    FindNextChild = -1
    For i=0 To UBound(tree,2)
      If tree(n-1,i) > 0 And tree(n-1,i) = tree(i,n-1) Then 
        FindNextChild = i+1
        Exit For
      End If
    Next
  End Function

  Sub SetVisited(tree, n1, n2)
    ' Værdien under diagonalen tælles op for at markere, at kanten er besøgt.
    Dim r,c
    r = Max(n1-1,n2-1)
    c = Min(n1-1,n2-1)
    tree(r,c) = tree(r,c) + 1
    
  End Sub

  Sub DoSomething(tree,n1,n2)
    Response.Write "<br>Behandler (" & Cstr(n1) & "," & Cstr(n2) & ")"  
  End Sub
  
  
  
  Sub InitPath(s,d,v)
    ' Sæt elementer i s fra nr d til værdien v
    Dim i
    For i = d To UBound(s)
      s(i) = v
    Next
  End Sub
  
  Sub FindPath(tree, n1, n2, s, d)
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
      FindPath tree,n,n2,s,d
      n = FindNextChild(tree,n1)
    Loop
  End Sub
  
  Sub WritePath(n,s,d)
    s(d) = n
    InitPath s,d+1,-1
    PrintPath s
  End Sub


  Function DepthInTree(n,d)
    If CInt(n)>CInt(-1) Then 
      d = d + 1
    Else
      d = d - 1
    End If
    DepthInTree = d
  End Function
  
  Sub PrintPath(s)
    Dim i, str
    str = CStr(s(0))
    i = 1
    Do While s(i)>-1
      str = str & " - " & CStr(s(i))
      i = i + 1
    Loop
    Response.Write "<br>Path = " & str
  End Sub



  ''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
  ' Lav dette testtræ:
  ' 
  '  1 - 5
  '  |    
  '  2 - 4
  '  |    
  '  3   
  
  NumberNodes = 6
  Weight = 1
  InitTree arrTree,NumberNodes 
  AddEdge arrTree,1,4, Weight
  AddEdge arrTree,1,6, Weight
  AddEdge arrTree,2,4, Weight
  AddEdge arrTree,3,4, Weight
  AddEdge arrTree,2,5, Weight
  
  PrintArrayInTable arrTree
  
  ' Gennemløb testtræet
'  DepthFirst arrTree, 1
  
  Dim s()
  ReDim s(NumberNodes)  ' s kan højst indeholde alle knuder

  InitPath s,0,-1
  Dim PathFound
  PathFound = 0
  FindPath arrTree, 1, 5, s, 0
  PrintPath s
  
 
  
  ''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
  Function Max (a,b)
    Max = b
    If CDbl(a) > CDbl(b) Then Max = a
  End Function
  
  Function Min (a,b)
    Min = b
    If CDbl(a) < CDbl(b) Then Min = a
  End Function
  
  Sub PrintArrayInTable(anArray)
    Dim r,c
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




%>