function [theta, J_history] = gradientDescent(X, y, theta, alpha, num_iters)
%GRADIENTDESCENT Performs gradient descent to learn theta
%   theta = GRADIENTDESCENT(X, y, theta, alpha, num_iters) updates theta by 
%   taking num_iters gradient steps with learning rate alpha

% Initialize some useful values
%m = length(y); % number of training examples
J_history = zeros(num_iters, 1);
m = length(y);
Hnot = theta'.*X;
Hnot = sum(Hnot,2);

for iter = 1:num_iters

    Hnot = theta'.*X;
    Hnot = sum(Hnot,2);
    sun = sum(((Hnot-y)).*X, 1);
    theta = theta - (alpha/m)*sun';
    disp("cost");
    disp(computeCost(X, y, theta));
    % Save the cost J in every iteration    
    J_history(iter) = computeCost(X, y, theta);

end
end
