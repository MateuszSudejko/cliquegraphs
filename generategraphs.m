N = 10; % Number of graphs to generate
probability = 0.5; % probability of an edge between two vertices
lowerBoundVert = 100;
higherBoundVert = 200; % boundaries for number of generated vertices in graphs
reps = 10; % number of repetitions for greedyRandom algorithm

T1=zeros(1,N);
T2=zeros(1,N);
err=zeros(1,N);

for i = 1:N
    numVertices = randi([lowerBoundVert, higherBoundVert]);
    nodes = cellstr(num2str((1:numVertices).')); % Create node labels
    
    
    edges = [];
    for j = 1:numVertices
        for k = 1:numVertices
            if j ~= k && rand < probability
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

    for j=1:reps
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
