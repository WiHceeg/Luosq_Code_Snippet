l=0.9;
w=[ 0   0   0   0.2    -0.3    0;
        0   0   0   0.4     0.1     0;
        0   0   0   -0.5    0.2     0;
        0   0   0   0          0     -0.3;
        0   0   0   0          0     -0.2];
heta=[0 0 0 -0.4 0.2 0.1];
S=[0 0 0 0 0 0];
O=[1 0 1 0 0 0];
E=[0 0 0 0 0 0];
T=[0 0 0 0 0 1];

for a=1:5000

for j=4:5
    S(j)=heta(j);
    for i=1:3
        S(j)=w(i,j)*O(i)+S(j);
    end
    O(j)=1/(1+exp(-S(j)));
end

j=6;
S(j)=heta(j);
for i=4:5
    S(j)=w(i,j)*O(i)+S(j);
end
O(j)=1/(1+exp(-S(j)));

plot(a,O(j),'.');
hold on

j=6;
E(j)=O(j)*(1-O(j))*(T(j)-O(j));
k=6;
for j=5:-1:4
    E(j)=O(j)*(1-O(j))*w(j,k)*E(k);
end

for i=1:3
    for j=4:5
        w(i,j)=w(i,j)+l*E(j)*O(i);
    end
end

for i=4:5
    j=6;
    w(i,j)=w(i,j)+l*E(j)*O(i);
end

for j=6:-1:4
    heta(j)=heta(j)+l*E(j);
end
 
end
