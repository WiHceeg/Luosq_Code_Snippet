w=0.5;
c1=2;
c2=2;
minfs=zeros();

v=unifrnd(-2,2,3,2);
x=unifrnd(-10,10,3,2);
for i=1:3
    f(i)=x(i,1)^2+x(i,2)^2;
end
pBest=x;
[minf,index]=min(f);
gBest=pBest(index,:);
a=1;
minfs(a)=minf;

while(minf>0.01)

    for i=1:3
        for j=1:2
            v(i,j)=w*v(i,j)+c1*rand()*(pBest(i,j)-x(i,j))+c2*rand()*(gBest(j)-x(i,j));
            x(i,j)=x(i,j)+v(i,j);
            if x(i,j)<-10
                x(i,j)=-10;
            end
            if x(i,j)>10
                x(i,j)=10;
            end
        end
    end

    for i=1:3
            f1(i)=x(i,1)^2+x(i,2)^2;
            if f1(i)<f(i)
                f(i)=f1(i);
                pBest(i,:)=x(i,:);
            end
    end
    [minf,index]=min(f);
    gBest=pBest(index,:);
    a=a+1;
    minfs(a)=minf;
    
end

plot(minfs,'o-');