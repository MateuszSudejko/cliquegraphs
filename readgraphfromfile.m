filename = 'graph.txt'; % Replace with your file name
fileID = fopen(filename, 'r');

if fileID == -1
    error('File not found or unable to open the file.');
end

% Read the number of graphs from the first line
numGraphs = str2double(fgetl(fileID)); % Read the first line as the number of graphs

graphs = cell(1, numGraphs); % Store adjacency matrices for each graph
graphCounter = 0;

while ~feof(fileID)
    line = fgetl(fileID);
    if isempty(line) % Empty line indicates the end of the current graph
        graphCounter = graphCounter + 1;
    else
        currentRow = str2double(strsplit(line));
        if isempty(graphs{graphCounter + 1})
            graphs{graphCounter + 1} = currentRow;
        else
            graphs{graphCounter + 1} = [graphs{graphCounter + 1}; currentRow];
        end
    end
end

fclose(fileID);

% Create directed graphs for each matrix
for j = 1:numel(graphs)
    graphMatrix = graphs{j};
    if ~isempty(graphMatrix)
        G = digraph(graphMatrix);
        
        % Display the graph
        figure;
        plot(G, 'Layout', 'force');
        title(sprintf('Directed Graph %d from Matrix', j));

        cliques = BronKerboschDirected2(graphMatrix);

        maxSize = 0;
        for k = 1:numel(cliques)
            currentSize = numel(cliques{k});
            if currentSize > maxSize
                maxSize = currentSize;
            end
        end

        disp(maxSize);
    end
end
