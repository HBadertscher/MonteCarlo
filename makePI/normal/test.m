% Test
time = zeros(1,9);
for i=1:9
    tic
    [status,result] = system(['./makePI ',num2str(10^i)]);
    time(i) = toc;
    res(i) = str2num(result);
end

figure(1);
loglog(10.^(1:9),time);

figure(2);
loglog(10.^(1:9),pi-result);