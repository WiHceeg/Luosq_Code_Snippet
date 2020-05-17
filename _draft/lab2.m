C=unifrnd(-5,5,5,4);
C1=zeros(5,4);
EvalC=zeros(1,5);
EvalC1=zeros(1,5);
Weight=zeros(1,5);
Roulette=zeros(1,5);
Rand=zeros(1,5);
Chromosome=zeros(1,5);
Crossover=zeros(1,5);
EvalBests=zeros();

for i=1:5
    EvalC(i)=1/(C(i,1)^2+C(i,2)^2+C(i,3)^2+C(i,4)^2+1);
end
EvalBest=max(EvalC);
a=1;
EvalBests(a)=EvalBest;


while(EvalBest<0.999)
    
    Sum=sum(EvalC);
    for i=1:5
        Weight(i)=EvalC(i)/Sum;
    end
    Roulette(1)=Weight(1);
    for i=2:5
        Roulette(i)=Roulette(i-1)+Weight(i);
    end
    Rand=rand(1,5);
    for i=1:5
        if Rand(i)<Roulette(1)
            Chromosome(i)=1;
        elseif Rand(i)>Roulette(1) && Rand(i)<Roulette(2)
            Chromosome(i)=2;
            elseif Rand(i)>Roulette(2) && Rand(i)<Roulette(3)
                Chromosome(i)=3;
                elseif Rand(i)>Roulette(3) && Rand(i)<Roulette(4)
                    Chromosome(i)=4;
        else
            Chromosome(i)=5;
        end
    end
    for i=1:5
        C1(i,:)=C(Chromosome(i),:);
    end

    Rand=rand(1,5);
    j=1;
    for i=1:5
        if Rand(i)<0.88
            Crossover(j)=i;
            j=j+1;
        end
    end
    if Crossover(1)~=0 && Crossover(2)~=0
        for i=unidrnd(4):4
            t=C1(Crossover(1),i);
            C1(Crossover(1),i)=C1(Crossover(2),i);
            C1(Crossover(2),i)=t;
        end
    end
    if Crossover(3)~=0 && Crossover(4)~=0
        for i=unidrnd(4):4
            t=C1(Crossover(3),i);
            C1(Crossover(3),i)=C1(Crossover(4),i);
            C1(Crossover(4),i)=t;
        end
    end

    for i=1:5
        for j=1:4
            if rand()<0.1
                C1(i,j)=10*rand()-5;
            end
        end
    end


    for i=1:5
        EvalC1(i)=1/(C1(i,1)^2+C1(i,2)^2+C1(i,3)^2+C1(i,4)^2+1);
    end
    EvalBest=max(EvalC1);
    if EvalBest>EvalBests(a)
        C=C1;
        EvalC=EvalC1;
        a=a+1;
        EvalBests(a)=EvalBest;
    end
    
end

plot(EvalBests,'-o');