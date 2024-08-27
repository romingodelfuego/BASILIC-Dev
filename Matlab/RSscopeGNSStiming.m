visaresource = 'USB0::2733::470::203458::0::INSTR';   %define visa resource name
if(~exist('vsdev'))
    if(~exist('visaresource')||~any(visaresource))
        visadevlist
        return
    else
        vsdev = visadev(visaresource);
    end

end


dT = [];

ttMAX = 60*60;
tic;
tt = 0;
while(tt(end)<ttMAX)
    [ch1, time] = RSscopeReadData(vsdev,1);
    [ch2, time] = RSscopeReadData(vsdev,2);

    [x,k1]=max(diff(ch1));
    [x,k2]=max(diff(ch2));
    dT(end+1) = (time(k2)-time(k1) ) *1e9;
    pause(0.12);
    plot(tt,dT)
    hold on
    [dT_max, idx_max] = max(dT);
    [dT_min, idx_min] = min(dT);

    % Ajouter les points sur le graphique
    plot(tt(idx_max), dT_max, 'or', 'MarkerSize', 5, 'MarkerFaceColor', 'r'); % Point max
    plot(tt(idx_min), dT_min, 'or', 'MarkerSize', 5, 'MarkerFaceColor', 'r'); % Point min

    % Ajouter les annotations
    text(tt(idx_max), dT_max, sprintf('Max = %.2f ns', dT_max), 'VerticalAlignment', 'bottom', 'HorizontalAlignment', 'right');
    text(tt(idx_min), dT_min, sprintf('Min = %.2f ns', dT_min), 'VerticalAlignment', 'top', 'HorizontalAlignment', 'right');
    tt(end+1) = toc;
    hold off
end
tt(end) = [];

%plot(time,ch1,time,ch2)
hold on
plot([0,ttMAX],[median(dT),median(dT)],'r--',[0,ttMAX],[mean(dT),mean(dT)],'g')
legend('$\Delta$ S2:S1',"","","Median","Mean",'Interpreter','latex')
xlabel("Temps (s)",'Interpreter','latex')
ylabel('$\Delta$ S2:S1 (ns)','Interpreter','latex')
text(0,mean(dT),string(mean(dT)+" ns"),"Color","Red")
text(ttMAX,median(dT),string(median(dT)+" ns"),"Color","Green")
hold off
 