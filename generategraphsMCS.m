N = 5; % Number of graphs to generate
probability = 0.5; % probability of an edge between two vertices
lowerBoundVert = 50;
higherBoundVert = 60; % boundaries for number of generated vertices in graphs

T1=zeros(1,N);
for i = 1:N
    numVertices1 = randi([lowerBoundVert, higherBoundVert]);
    numVertices2 = randi([lowerBoundVert, higherBoundVert]);
    nodes1 = cellstr(num2str((1:numVertices1).')); % Create node labels
    edges1 = [];
    nodes2 = cellstr(num2str((1:numVertices2).')); % Create node labels
    edges2 = [];
    for j = 1:numVertices1
        for k = 1:numVertices1
            if j ~= k && rand < probability
                edges1 = [edges1; j, k]; % Add an edge
            end
        end
    end
    for j = 1:numVertices2
        for k = 1:numVertices2
            if j ~= k && rand < probability
                edges2 = [edges2; j, k]; % Add an edge
            end
        end
    end
    
    % Create and plot the directed graph
    G1 = digraph(edges1(:, 1), edges1(:, 2), [], nodes1);
    figure;
    plot(G1, 'Layout', 'force');
    title(sprintf('Random Graph %d 1', i));

    graphMatrix1=adjacency(G1);
    
    G2 = digraph(edges2(:, 1), edges2(:, 2), [], nodes2);
    figure;
    plot(G2, 'Layout', 'force');
    title(sprintf('Random Graph %d 2', i));

    graphMatrix2=adjacency(G1);

    tic
    
    disp(MCS(2,{graphMatrix1, graphMatrix2}));

    T1(i)=toc;
end

disp(T1);
