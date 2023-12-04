% Define a directed graph
edges = [1 2; 1 5; 1 4; 1 6; 2 1; 2 3; 2 5; 2 6; 3 2; 4 1; 4 7; 4 5; 5 2; 5 1; 5 6; 5 4; 5 7; 6 1; 6 2; 6 5; 6 7; 7 6];
G = digraph(edges(:, 1), edges(:, 2));

% Create a directed graph object
G = digraph(edges(:, 1), edges(:, 2));

% Plot the directed graph
figure;
h = plot(G, 'Layout', 'force'); % 'force' layout for a more organized graph

% Customize the plot (optional)
title('Directed Graph');
h.NodeColor = 'r'; % Node color
h.LineWidth = 1.5; % Edge line width
