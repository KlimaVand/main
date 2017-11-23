<%
  Option Explicit

  Dim rows, cols, s, r, c
  Dim A(), CA, CA1, CA2

  s = "10,11,7"
  CA = Split(s,",")
  CA1 = CA
  CA2 = CA
  
  rows = UBound(CA2)
  cols = UBound(CA1)
  ReDim A(rows,cols)
  InitArray A,-1
    
  PrintArrayInTable1 CA
  PrintArrayInTable2 A
  
  ' Start i (r,c) = øverste højre hjørne
  r=0
  c=cols
  ' Løs rekursivt
  SolveAll r,c

  ' Skriv løsning
  PrintArrayInTable2 A
  
  
  
  ''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
  
  Sub SolveAll (i, j)
    Do While i<=UBound(CA2) And j>=0
      Solve i,j
      NextCell i,j
      SolveAll i,j
    Loop
  End Sub
  

  Sub Solve (i, j)
    Dim y 
    y = Min(CA1(j),CA2(i))
    A(i,j) = y
    CA1(j) = CA1(j) - y
    CA2(i) = CA2(i) - y
  End Sub
  
  Sub NextCell (i, j)
    If CA1(j) = 0 and not(CA2(i) = 0) Then j = j - 1
    If CA2(i) = 0 and not(CA1(j) = 0) Then i = i + 1
    If CA1(j) = 0 and CA2(i) = 0 Then i = i + 1
  End Sub

  ''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
  Function Max (a,b)
    Max = b
    If CDbl(a) > CDbl(b) Then Max = a
  End Function
  
  Function Min (a,b)
    Min = b
    If CDbl(a) < CDbl(b) Then Min = a
  End Function
  
  Sub InitArray(anArray,val)
    ' Sætter val i alle celler i arrayet
    Dim r,c
    For r = 0 To UBound(anArray,1)
      For c = 0 To UBound(anArray,2)
        anArray(r,c) = val
      Next
    Next
  End Sub
  
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
 
%>

