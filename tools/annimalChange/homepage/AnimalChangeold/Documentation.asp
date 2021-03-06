<%
'**************************************************************************************************
' Author: Margit Jorgensen, Danish Institute of Agricultural Sciences
' Description: 
'**************************************************************************************************
sub CreateTabDocumentation()

if session("Language") = "uk" then
CreatePageUK
else
CreatePageDK
end if
CreateUpdateFormDocumentation
end sub
'**************************************************************************************************
sub CreatePageUK
  response.write("<table>" & vbCr)
  response.write(" <tr>" & vbCr)
  response.write("  <td style=""width: 20%;"">")
  response.write("         <a href=""documentation/FarmAC development/FarmAC users guide v2.htm"" target=""iframe"">User guide</a><br/><br/>" & vbCr)
  response.write("         <a href=""documentation/FarmAC development/Configuring and Parameterising FarmAC.htm"" target=""iframe"">Configuration of parameter-files</a>" & vbCr)
'  response.write("         <a href=""dokumentation/manual_lille.pdf"" target=""iframe"">Short introduction</a>" & vbCr)
'  response.write("    <br/><a href=""dokumentation/Documentation of the used coefficients in Farm.pdf"" target=""iframe"">General documentation</a>" & vbCr)
'  response.write("    <br/><a href=""dokumentation/FarmNTool.pdf"" target=""iframe"">FarmN paper</a>" & vbCr)
'  response.write("    <br/><a href=""dokumentation/Nles3 english.doc"" target=""iframe"">Nles3</a>" & vbCr)
'  response.write("    <br/><a href=""http://www.agro.au.dk/c-tool/"" target=""iframe"">C-tool</a>" & vbCr)
  response.write("  </td>" & vbCr)
  response.write("  <td style=""width: 80%; height: 800px;""><iframe name=""iframe"" src=""documentation/FarmAC development/FarmAC users guide v2.htm"" style=""width: 100%; height: 100%; background-color: white;""></iframe></td>" & vbCr)
  response.write(" <tr>" & vbCr)
  response.write("</table>" & vbCr)
end sub
'**************************************************************************************************
sub CreatePageDK
  response.write("<table>" & vbCr)
  response.write(" <tr>" & vbCr)
  response.write(" <td style=""width: 20%;"">")
'  response.write("         <a href=""dokumentation/manual_lille_Dansk.pdf"" target=""iframe"">Generel introduktion</a>" & vbCr)
'	response.write("  <br/><br/><a href=""dokumentation/Beregning af N-udvaskning med FarmN.pdf"" target=""iframe"">Beregning af N-udvaskning</a>" & vbCr)
'	response.write("  <br/><br/><a href=""dokumentation/beregning foderbehov.pdf"" target=""iframe"">Foderbehov til kv�g</a>" & vbCr)
'	response.write("  <br/><br/><a href=""dokumentation/staldtype og lagre.pdf"" target=""iframe"">Stald- og lagertyper til kv�g</a>" & vbCr)
'	response.write("  <br/><br/><a href=""http://www.dmu.dk/1_viden/2_publikationer/3_ovrige/rapporter/VMPII/Genberegning_af_modellen_NLES.pdf"" target=""iframe"">Nles3</a>" & vbCr)
'    response.write("  <br/><br/><a href=""http://www.agro.au.dk/c-tool/"" target=""iframe"">C-tool</a>" & vbCr)
'	response.write("  <br/><br/><a href=""http://www.agrsci.dk/var/agrsci/storage/original/application/6b5cf311a01dd1d8eb7c33a34ed63b54.pdf"" target=""iframe"">Samspillet mellem jordpulje�ndringer, N-balancer og udvaskning</a>" & vbCr)
'	response.write("  <br/><br/><a href=""http://www.agrsci.dk/var/agrsci/storage/original/application/47408f8f5a2bfd4be07038c49b7508c9"" target=""iframe"">Husdyrg�dning</a>" & vbCr)
  response.write("  </td>" & vbCr)
  response.write("  <td style=""width: 80%; height: 800px;""><iframe name=""iframe"" src=""dokumentation/manual_lille_Dansk.pdf"" style=""width: 100%; height: 100%;""></iframe></td>" & vbCr)
  response.write(" <tr>" & vbCr)
  response.write("</table>" & vbCr)
end sub
'**************************************************************************************************
sub CreatePageDK_old
%>
<br/><h2>Beregning af foderbehov til kv�g</h2>
<em>Troels Kristensen</em><br>
30 sept. 2003&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="stald.htm">Stald- og lagertyper til kv�g</a>
<p>
Foderbehovet udtrykkes pr �rsdyr (365 foderdage) ud fra standard effektivitet for energi- og proteinudnyttelse. Der er anvendt f�lgende kilder</p>
<ul>
<li>SH beretning 660,</li>
<li>DJF beretning 737,</li>
<li>DJF rapport husdyrbrug fodereffektivitet in press</li>
<li>DJF rapport husdyrbrug 36</li>
<li>Danske fodernormer</li>
</ul>
Der er arbejdet med tre kategorier indenfor henholdsvis bes�tninger med tunge racer og jersey. De tre kategorier er malkek�er, opdr�t og ungtyre. For alle kategorier er der defineret et standardniveau for produktion og tilh�rende foderoptagelse og udskillelse af N i husdyrg�dning. Herudover er der beskrevet et variationsomr�de for produktionen indenfor, hvilken der er udledt konsekvenserne p� foderoptagelse og N udskillelse.</p>

<h3>Tunge racer - k�er</h3>
<table border=1 cellspacing=0 cellpadding=3>
 <tr>
  <td><B>Omr�de</B></td>
  <td><B>Beregning</B></td>
  <td><B>Standard</B></td>
  <td><B>Min</B></td>
  <td><B>Maks</B></td>
 </tr>
 <tr>
  <td><EM>Produktionsoplysninger</EM></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>- �rsydelse, kg EKM</td>
  <td>Indberettes</td>
  <td>8500</td>
  <td>6500</td>
  <td>12500</td>
 </tr>
 <tr>
  <td>- FE afgr�sning</td>
  <td>Indberettes</td>
  <td>0</td>
  <td>0</td>
  <td>1800</td>
 </tr>
 <tr>
  <td><EM>Foderbehov</EM></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td colspan=2><font size=1>For ovenst�ende prod.</font></td>
 </tr>
 <tr>
  <td>- energi, FE</td>
  <td>1635+0,4*ydelse+1,67*10<sup>-5 </sup>*ydelse<sup>2</sup></td>
  <td>6242</td>
  <td>4941</td>
  <td>9244</td>
 </tr>
 <tr>
  <td>- t�rstof, kg</td>
  <td>FE*(1,10+(8500-ydelse)*0,00002)</td>
  <td>6866</td>
  <td>5633</td>
  <td>9429</td>
 </tr>
 <tr>
  <td>- N, kg</td>
  <td>FE*0,028</td>
  <td>175</td>
  <td>138</td>
  <td>259</td>
 </tr>
 <tr>
  <td><EM>Produktion</EM></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>- m�lkeprotein, kg N</td>
  <td>Ydelse*0,00518</td>
  <td>44</td>
  <td>34</td>
  <td>65</td>
 </tr>
 <tr>
  <td>- tilv�kst, foster, kg N</td>
  <td>Fast</td>
  <td>2</td>
  <td>2</td>
  <td>2</td>
 </tr>
 <tr>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>G�dning, kg N ab dyr</td>
  <td>N foder - N m�lk � N tilv�kst</td>
  <td>129</td>
  <td>102</td>
  <td>192</td>
 </tr>
 <tr>
  <td>- afsat p� marken, pct</td>
  <td>(FE afgr�sning*100) / FE</td>
  <td>0</td>
  <td>0</td>
  <td>19</td>
 </tr>
 <tr>
  <td>- andel i f�ces, pct</td>
  <td>(((0,04*(N foder <i>g/dag</i>)+((t�rstof<sup>2</sup> <i>kg/dag</i>*1,8)/6,25)+((t�rstof <i>kg/dag</i>*20)/6,25))*100)/(N foder � N m�lk � N tilv�kst) 1)</td>
  <td>51</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>- andel i urin, pct</td>
  <td>100 � andel i f�ces</td>
  <td>49</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td><EM>Effektivitet</EM></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>- FE, pct</td>
  <td>((1904+ydelse*0,4)*100)/FE foder</td>
  <td>85</td>
  <td>91</td>
  <td>75</td>
 </tr>
 <tr>
  <td>- N, pct</td>
  <td>((N m�lk+N tilv�kst)*100)/N foder</td>
  <td>26,3</td>
  <td>26,1</td>
  <td>25,9</td>
 </tr>
</table>
<p>
1) Ligning side 49 i DJF Rapport Husdyrbrug nr 36.</p>

<h3>Tunge racer � opdr�t ved en k�lvealder p� 26 mdr.</h3>
<table border=1 cellspacing=0 cellpadding=3>
 <tr>
  <td><B>Omr�de</B></td>
  <td><B>Beregning</B></td>
  <td><B>Standard</B></td>
  <td><B>Min</B></td>
  <td><B>Maks</B></td>
 </tr>
 <tr>
  <td><EM>Produktionsoplysninger</EM></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>- tilv�kst, g dagligt</td>
  <td>Indberettes</td>
  <td>700</td>
  <td>500</td>
  <td>900</td>
 </tr>
 <tr>
  <td>- FE afgr�sning</td>
  <td>Indberettes</td>
  <td>0</td>
  <td>0</td>
  <td>700</td>
 </tr>
 <tr>
  <td><EM>Foderbehov</EM></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td colspan=2><font size=1>For ovenst�ende prod.</font></td>
 </tr>
 <tr>
  <td>- energi, FE</td>
  <td>1683+2,19*(700-tilv�kst)</td>
  <td>1683</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>- t�rstof, kg</td>
  <td>FE*1,35+0,00025*(700-tilv�kst)</td>
  <td>2272</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>- N, kg</td>
  <td>FE*0,025+10*(FE afgr�sning/FE)</td>
  <td>42</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td><EM>Produktion</EM></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>- tilv�kst, foster, kg N</td>
  <td>365*tilv*0,000025</td>
  <td>6,4</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>G�dning, kg N ab dyr</td>
  <td>N foder � N tilv�kst</td>
  <td>35,6</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>- afsat p� marken, pct</td>
  <td>(FE afgr�sning*100)/FE</td>
  <td>0</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>- andel i f�ces, pct</td>
  <td>(((0,04*(N foder, <i>g/dag</i>)+((t�rstof<sup>2</sup> <i>kg/dag</i>*1,8)/6,25)+((t�rstof <i>kg/dag</i>*20)/6,25))*100)/(N foder � N tilv�kst) 1)</td>
  <td>37</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>- andel i urin, pct</td>
  <td>100 � andel i f�ces</td>
  <td>63</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td><EM>Effektivitet</EM></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>- N, pct</td>
  <td>((N tilv�kst)*100)/N foder</td>
  <td>15,2</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
</table>
<p>
1) Ligning side 49 i DJF Rapport Husdyrbrug nr 36.</p>
<p>
<h3>Tunge racer � ungtyre med en afgangsv�gt p� 450 kg.</h3>
<table border=1 cellspacing=0 cellpadding=3>
 <tr>
  <td><B>Omr�de</B></td>
  <td><B>Beregning</B></td>
  <td><B>Standard</B></td>
  <td><B>Min</B></td>
  <td><B>Maks</B></td>
 </tr>
 <tr>
  <td><EM>Produktionsoplysninger</EM></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>- tilv�kst, g dagligt</td>
  <td>Indberettes</td>
  <td>1100</td>
  <td>900</td>
  <td>1300</td>
 </tr>
 <tr>
  <td><EM>Foderbehov</EM></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td colspan=2><font size=1>For ovenst�ende prod.</font></td>
 </tr>
 <tr>
  <td>- energi, FE</td>
  <td>1,825*450+0,00605*450<sup>2</sup> - 75</td>
  <td>1971</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>- t�rstof, kg</td>
  <td>FE*0,96</td>
  <td>1892</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>- N, kg</td>
  <td>FE*0,022</td>
  <td>42</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td><EM>Produktion</EM></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>- tilv�kst, foster, kg N</td>
  <td>365*tilv*0,000025</td>
  <td>10</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>G�dning, kg N ab dyr</td>
  <td>N foder � N tilv�kst</td>
  <td>32</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>- afsat p� marken, pct</td>
  <td>(FE afgr�sning*100)/FE</td>
  <td>0</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>- andel i f�ces, pct</td>
  <td>(((0,04*(N foder, <i>g/dag</i>)+((t�rstof<sup>2</sup> <i>kg/dag</i>*1,8)/6,25)+((t�rstof <i>kg/dag</i>*20)/6,25))*100)/(N foder � N tilv�kst) 1)</td>
  <td>33</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>- andel i urin, pct</td>
  <td>100 � andel i f�ces</td>
  <td>67</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td><EM>Effektivitet</EM></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>- N, pct</td>
  <td>((N tilv�kst)*100)/N foder</td>
  <td>24</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
</table>
<p>
1) Ligning side 49 i DJF Rapport Husdyrbrug nr 36.</p>
<h3>Jersey racer - k�er</h3>
<table border=1 cellspacing=0 cellpadding=3>
 <tr>
  <td><B>Omr�de</B></td>
  <td><B>Beregning</B></td>
  <td><B>Standard</B></td>
  <td><B>Min</B></td>
  <td><B>Maks</B></td>
 </tr>
 <tr>
  <td><EM>Produktionsoplysninger</EM></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>- �rsydelse, kg EKM</td>
  <td>Indberettes</td>
  <td>7500</td>
  <td>5500</td>
  <td>11500</td>
 </tr>
 <tr>
  <td>- FE afgr�sning</td>
  <td>Indberettes</td>
  <td>0</td>
  <td>0</td>
  <td>1500</td>
 </tr>
 <tr>
  <td><EM>Foderbehov</EM></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td colspan=2><font size=1>For ovenst�ende prod.</font></td>
 </tr>
 <tr>
  <td>- energi, FE</td>
  <td>1192+0,4*ydelse+1,67*10<sup>-5</sup>*ydelse<sup>2</sup></td>
  <td>5131</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>- t�rstof, kg</td>
  <td>FE*(1,10+(7500-ydelse)*0,00002)</td>
  <td>5644</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>- N, kg</td>
  <td>FE*0,028</td>
  <td>144</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td><EM>Produktion</EM></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>- m�lkeprotein, kg N</td>
  <td>Ydelse*0,00488</td>
  <td>37</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>- tilv�kst, foster, kg N</td>
  <td>Fast</td>
  <td>1</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>G�dning, kg N ab dyr</td>
  <td>N foder - N m�lk � N tilv�kst</td>
  <td>106</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>- afsat p� marken, pct</td>
  <td>(FE afgr�sning*100)/FE</td>
  <td>0</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>- andel i f�ces, pct</td>
  <td>(((0,04*(N foder <i>g/dag</i>)+((t�rstof<sup>2</sup> <i>kg/dag</i>*1,8)/6,25)+((t�rstof <i>kg/dag</i>*20)/6,25))*100)/(N foder � N m�lk � N tilv�kst) 1)</td>
  <td>51</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>- andel i urin, pct</td>
  <td>100 � andel i f�ces</td>
  <td>49</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td><EM>Effektivitet</EM></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>- FE, pct</td>
  <td>((1461+ydelse*0,4)*100)/FE foder</td>
  <td>87</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>- N, pct</td>
  <td>((N m�lk+N tilv�kst)*100)/N foder</td>
  <td>26,3</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
</table>
<p>
1) Ligning side 49 i DJF Rapport Husdyrbrug nr 36.</p>

<h3>Jersey racer � opdr�t ved en k�lvealder p� 24 mdr.</h3>
<table border=1 cellspacing=0 cellpadding=3>
 <tr>
  <td><B>Omr�de</B></td>
  <td><B>Beregning</B></td>
  <td><B>Standard</B></td>
  <td><B>Min</B></td>
  <td><B>Maks</B></td>
 </tr>
 <tr>
  <td><EM>Produktionsoplysninger</EM></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>- tilv�kst, g dagligt</td>
  <td>Indberettes</td>
  <td>500</td>
  <td>400</td>
  <td>700</td>
 </tr>
 <tr>
  <td>- FE afgr�sning</td>
  <td>Indberettes</td>
  <td>0</td>
  <td>0</td>
  <td>500</td>
 </tr>
 <tr>
  <td><EM>Foderbehov</EM></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td colspan=2><font size=1>For ovenst�ende prod.</font></td>
 </tr>
 <tr>
  <td>- energi, FE</td>
  <td>1236+1,83*(500-tilv�kst)</td>
  <td>1236</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>- t�rstof, kg</td>
  <td>FE*1,35+0,00025*(500-tilv�kst)</td>
  <td>2292</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>- N, kg</td>
  <td>FE*0,025+10*(FE afgr�sning/FE)</td>
  <td>31</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td><EM>Produktion</EM></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>- tilv�kst, foster, kg N</td>
  <td>365*tilv*0,000025</td>
  <td>4,6</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>G�dning, kg N ab dyr</td>
  <td>N foder � N tilv�kst</td>
  <td>26,4</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>- afsat p� marken, pct</td>
  <td>(FE afgr�sning*100) / FE</td>
  <td>0</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>- andel i f�ces, pct</td>
  <td>(((0,04*(N foder, <i>g/dag</i>)+((t�rstof<sup>2</sup> <i>kg/dag</i>*1,8)/6,25)+((t�rstof <i>kg/dag</i>*20)/6,25))*100)/(N foder � N tilv�kst) 1)</td>
  <td>37</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>- andel i urin, pct</td>
  <td>100 � andel i f�ces</td>
  <td>63</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td><EM>Effektivitet</EM></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>- N, pct</td>
  <td>((N tilv�kst)*100)/N foder</td>
  <td>14,8</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
</table>
<p>
1) Ligning side 49 i DJF Rapport Husdyrbrug nr 36.</p>

<h3>Jersey racer � ungtyre med en afgangsv�gt p� 300 kg.</h3>
<table border=1 cellspacing=0 cellpadding=3>
 <tr>
  <td><B>Omr�de</B></td>
  <td><B>Beregning</B></td>
  <td><B>Standard</B></td>
  <td><B>Min</B></td>
  <td><B>Maks</B></td>
 </tr>
 <tr>
  <td><EM>Produktionsoplysninger</EM></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>- tilv�kst, g dagligt</td>
  <td>Indberettes</td>
  <td>900</td>
  <td>700</td>
  <td>1100</td>
 </tr>
 <tr>
  <td><EM>Foderbehov</EM></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td colspan=2><font size=1>For ovenst�ende prod.</font></td>
 </tr>
 <tr>
  <td>- energi, FE</td>
  <td>(2,308*300+0,00676*300<sup>2</sup> - 35)</td>
  <td>1266</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>- t�rstof, kg</td>
  <td>0,96*FE</td>
  <td>1215</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>- N, kg</td>
  <td>FE*0,022</td>
  <td>28</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td><EM>Produktion</EM></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>- tilv�kst, foster, kg N</td>
  <td>365*tilv*0,000025</td>
  <td>8,2</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>G�dning, kg N ab dyr</td>
  <td>N foder � N tilv�kst</td>
  <td>19,8</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>- afsat p� marken, pct</td>
  <td>(FE afgr�sning*100)/FE</td>
  <td>0</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>- andel i f�ces, pct</td>
  <td>(((0,04*(N foder, <i>g/dag</i>)+((t�rstof<sup>2</sup> <i>kg/dag</i>*1,8)/6,25)+((t�rstof <i>kg/dag</i>*20)/6,25))*100)/(N foder � N tilv�kst) 1)</td>
  <td>33</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>- andel i urin, pct</td>
  <td>100 � andel i f�ces</td>
  <td>67</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td><EM>Effektivitet</EM></td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
 <tr>
  <td>- N, pct</td>
  <td>((N tilv�kst)*100)/N foder</td>
  <td>29</td>
  <td>&nbsp;</td>
  <td>&nbsp;</td>
 </tr>
</table>
<p>
1) Ligning side 49 i DJF Rapport Husdyrbrug nr 36.</p>
<%
end sub
'**************************************************************************************************
sub CreateUpdateFormDocumentation()
   Response.Write("<form name=""update"" action=""Dummy.asp"" method=""post"">" & vbCr)
   Response.Write("<input type=""hidden"" name=""saveUpdates"" value=""false"">" & vbCr)
   Response.Write("</form>" & vbCr)
end sub
'**************************************************************************************************
%>
