x=zeros(40,1);
for i = 1 :1: 40
    if i <= 20
        x(i, 1) = 1;
    else
        x(i, 1) = 20;
    end
end
y = zeros( 40, 1);
for i = 1 :1: 40
    if i <= 20
        y(i, 1) = i;
    else
        y(i, 1) =i - 20;
    end
end

subplot(1,2,1);
scatter(x(1:20, 1), y(1:20,1), 'r*');
hold on
scatter(x(21:40, 1), y(21:40,1), 'b', 'filled');
legend('Class 1','Class 2');
title('Complete Linkage Method');


subplot(1,2,2);
scatter(x(1:20, 1), y(1:20,1), 'r', 'filled');
hold on
scatter(x(21:40, 1), y(21:40,1), 'b*');
legend('Class 1','Class 2');
title('Llord Clustering Method');
