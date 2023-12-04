N = 5; % Number of graphs to generate

for i = 1:N
    numVertices = randi([150, 200]); % Random number of vertices between 50 and 100
    nodes = cellstr(num2str((1:numVertices).')); % Create node labels
    
    % Generate random edges based on 50% probability
    edges = [];
    for j = 1:numVertices
        for k = 1:numVertices
            if j ~= k && rand < 0.25
                edges = [edges; j, k]; % Add an edge
            end
        end
    end
    
    % Create and plot the directed graph
    G = digraph(edges(:, 1), edges(:, 2), [], nodes);
    figure;
    plot(G, 'Layout', 'force');
    title(sprintf('Random Graph %d', i));

    graphMatrix=adjacency(G);

    cliques = BronKerboschDirected2(graphMatrix);

    sizes = cellfun(@numel, cliques);

    disp(max(sizes));
end
