clear all
close all
clc

t = 0: 0.0000001: 2;

 Ac=1; u=0.25; fm=1; fc=25;
 s=Ac.*(1+u.*cos(2*pi.*fm.*t)).*cos(2.*pi.*fc.*t);
 m=1+u.*cos(2*pi.*fm.*t);

isCharging=1;
maximumDischarge=0;
timeOfDischarge=1;

tau = 0.7460;
y=s;

sizeOfT=size(t,2);

for j=1:(sizeOfT-1)
     if (y(j) > s(j+1))
         
         if(isCharging==1)
         maximumDischarge=s(j);
         timeOfDischarge=1;
         isCharging=0;
         end
         
     y(j)=maximumDischarge.*exp(-t(timeOfDischarge)./tau);
     y(j+1)=maximumDischarge.*exp(-t(timeOfDischarge+1)./tau);
     
     else
         
     isCharging=1;
     y(j)=s(j);
     
     end
     timeOfDischarge=1+timeOfDischarge;
end

plot(t,s,t,y,t,m);
xlabel('time');
legend('s(t)','y(t)','ideal demodulated signal');

