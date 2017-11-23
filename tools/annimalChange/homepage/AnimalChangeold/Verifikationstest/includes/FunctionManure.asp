<%
'**************************************************************************************************
' Author: Margit Jørgensen, Danish Institute of Agricultural Sciences
' Description: 
'**************************************************************************************************
'sub CalculateDistributeManure(totalManureNContent,totalFertManureNContent,arrManureNContent,MeanUtilization) 
' totalManureNContent skal indeholde summen af N i HG
' totalFertManureNContent skal indeholde handelsgødningsækvivalenten
' arrManureNContent skal indeholde et array med de enkelte typer af manure som skal fordeles
'InitFarmNDB


'end sub
'**************************************************************************************************
function CalcFertEq(anArr, i, j)
dim k,total

  for k=0 to Ubound(anArr,2) -1  
     total = total + Round(anArr(i,k) * anArr(j,k)/100 , 2)
  next
  
  CalcFertEq =  total

end function

'**************************************************************************************************

  Sub PrintArrayInTable2(anArray)
    Dim r,c
  Response.Write "<br><table border>" & Chr(13)
    For r = 0 to UBound(anArray,2)
      Response.Write "<tr>" & Chr(13)
      For c = 0 to UBound(anArray,1)
        Response.Write "<td>" & CStr(anArray(c,r)) & "</td>" & Chr(13)
      Next
      Response.Write "</tr>" & Chr(13)
    Next
    Response.Write "</table>" & Chr(13)
  End Sub

'**************************************************************************************************

  function Sum(anArray,k)
    Dim r, total

    For r = 0 to UBound(anArray,2)
      total = total + anArray(k,r)
    Next
    Sum = total
  End function

%>