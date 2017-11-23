<%
'**************************************************************************************************
' Author: Nina Detlefsen, Danish Institute of Agricultural Sciences
' Description: Find shortest path from p to q in network
'**************************************************************************************************
function CalcShortestPath(p,q)
   dim returnValue
   
   returnValue = False
   
'  Write function logics here   
      

'  Initialize 

   L = ' Labels for all nodes consisting of predecessor (an index) and a value (i,w(i)) 2*NUMBERCROPS
   w = ' array of length of paths
   SE = ' Set of indices EMPTY
   D = ' Set of arcs that decreases
   I = ' Set of arcs that increases
   
   for i=1 to 2*NumberCrops
   w(i) = 100
   L(i) = (null, w(i))
   next
   w(p) = 0
   L(p) = (null, w(p))
   SE=(p)

   if SE=empty done

   select i in SE
   SE = SE \ {i}

   if i<=NumberCrops then
     for j=NumberCrops+1 to 2*NumberCrops
       if (i,j) in B and w(j)>w(i)+1 then
         w(j) = w(i) +1
         L(j) = (i,w(j))
         SE = SE union {j}
         if j=q then end select
       end if 
     next
   else 
     for j=1 to NumberCrops
       if (j,i) in B and w(j)>w(i)+1 then
         w(j) = w(i) +1
         L(j) = (i,w(j))
         SE = SE union {j}
       end if 
     next

   loop select 
  
   j=q
   do 
     for i=1 to 2*NumberCrops
       if L(i) = j then
         if i < j then 
           D= D + (i,j)
         else 
           I= I + (i,j)
         end if
       j=i
       exit for
       end if
     next
   until j=p





   CalcShortestPath = returnValue
end function
'**************************************************************************************************
%>