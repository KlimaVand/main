function retVal =Temperature(avgTemperature, day,depth, amplitude, offset)

    rho=3.1415926*2.0/(365.0*24.0*3600.0);
	 Th_diff=0.35E-6;
	dampingDepth=CalcDampingDepth(Th_diff,rho);
   retVal=avgTemperature+amplitude*exp(-depth/dampingDepth)*sin(rho*(day+offset)*24.0*3600.0-depth/dampingDepth);


end