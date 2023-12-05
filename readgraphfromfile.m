filename = 'graph.txt'; % Replace with your file name
fileID = fopen(filename, 'r');

if fileID == -1
    error('File not found or unable to open the file.');
end

% Read lines from the file
lines = textscan(fileID, '%s', 'Delimiter', '\n');
fclose(fileID);

numLines = length(lines{1});
graphs = {}; % Store adjacency matrices for each graph
currentGraph = [];
for i = 1:numLines
    line = lines{1}{i};
    if isempty(line) % Empty line indicates the end of the current graph
        if ~isempty(currentGraph)
            graphs{end+1} = currentGraph;
            currentGraph = [];
        end
    else
        currentRow = str2double(strsplit(line));
        currentGraph = [currentGraph; currentRow];
    end
end

% Process the last graph if it exists
if ~isempty(currentGraph)
    graphs{end+1} = currentGraph;
end

% Create directed graphs for each matrix
for j = 1:numel(graphs)
    graphMatrix = graphs{j};
    G = digraph(graphMatrix);
    
    % Display the graph
    figure;
    plot(G, 'Layout', 'force');
    title(sprintf('Directed Graph %d from Matrix', j));

    cliques = BronKerboschDirected2(graphMatrix);

    maxSize = 0;
    for j = 1:numel(cliques)
        currentSize = numel(cliques{j});
        if currentSize > maxSize
            maxSize = currentSize;
        end
    end

    disp(maxSize);
end

