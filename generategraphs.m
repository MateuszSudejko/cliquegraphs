N = 10; % Number of graphs to generate

T1=zeros(1,N);
T2=zeros(1,N);
err=zeros(1,N);

for i = 1:N
    numVertices = randi([100, 200]);
    nodes = cellstr(num2str((1:numVertices).')); % Create node labels
    
    % Generate random edges based on 50% probability
    edges = [];
    for j = 1:numVertices
        for k = 1:numVertices
            if j ~= k && rand < 0.75
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
    
    tic
    cliques = BronKerboschDirected2(graphMatrix);

    maxSize = 0;
    for j = 1:numel(cliques)
        currentSize = numel(cliques{j});
        if currentSize > maxSize
            maxSize = currentSize;
        end
    end

    disp(maxSize);

    T1(i)=toc;
    
    tic
    max=0;

    for j=1:7
        new = greedyRandom(graphMatrix);
        if new>max
            max=new;
        end
    end

    disp(max);
    T2(i)=toc;

    err(i)=(maxSize-max)/maxSize;
end

disp(T1);
disp(T2);
disp(err);