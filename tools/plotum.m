umatrix = dlmread('umatrix.csv');
[y, x, label] = textread('bestmatches.csv', '%d%d%s');
after = dlmread('grid_after.csv');
z = zeros(length(x), 1);
for i=1:length(x)
    z(i) = after(y(i)+1, x(i)+1); 
end
surf(umatrix);
view(2);
set(gca, 'Color', [0 0 0]);
hold on;
plot3(x, y, z, 'w.');
text(x, y, z, label, 'Color', [1 1 1], 'Interpreter', 'none');
hold off