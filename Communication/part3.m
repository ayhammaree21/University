clear all
close all
clc

 Ac=1; u=0.25; fm=1; fc=25;
 t = 0: 0.00001: 1;
 s=Ac.*(1+u.*cos(2*pi.*fm.*t)).*cos(2.*pi.*fc.*t);
 y = s;
 m=1+u.*cos(2*pi.*fm.*t);

tau = 0.04 : 0.001 : 1;

sizeOfT = size(t,2);
sizeOfTau = size(tau,2);

D = zeros(1,sizeOfTau);

maximumDischarge = 0;
timeOfDischarge = 1;
isCharging = 0;
 
minError = inf; optimalTau = inf;

for i = 1 : sizeOfTau
         for j = 1 : (sizeOfT-1)
                             if (y(j) > s(j+1) )
                                         if(isCharging == 0)
                                             
                                         maximumDischarge=s(j);
                                         timeOfDischarge = 1;
                                         isCharging = 1;
                                         
                                         end
                             y(j) = maximumDischarge.*exp(-t(timeOfDischarge)./tau(i));
                             y(j+1) = maximumDischarge.*exp(-t(timeOfDischarge+1)./tau(i));
                             
                             else
                                 
                             y(j)=s(j);
                             isCharging=0;
                             
                             end
         timeOfDischarge = 1+timeOfDischarge;
         end
 
 squareERROR = ((y-m).^2);
 
 integration=0;
 
     for n=1:sizeOfT
         
     integration=integration+squareERROR(n);
     
     end
 
 D(i)=(integration/sizeOfT);
 
     if(minError > D(i))
         optimalTau=tau(i);
         minError=D(i);
     
     end
end

disp(minError)
disp(optimalTau)

plot(tau,D,'r');
ylabel("D");
xlabel("\tau");



