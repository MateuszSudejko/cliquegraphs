function output = MCS(graph1, graph2)
    %   graph1, graph2: Adjacency matrices of the input graphs
    [modular, index_mapping] = modular_product(graph1, graph2);
    all_cliques = BronKerboschDirected2(modular);
    max_size = 0;
    for i = 1:length(all_cliques)
        if size(all_cliques{i}) > max_size
            max_size = length(all_cliques{i});
            idx = i;
        end
    end
    max_clique = all_cliques{idx};
    index_mapping_graph1 = [];
    for i = 1:length(max_clique)
        index_mapping_graph1 = [index_mapping_graph1, index_mapping{max_clique(i)}(1)];
    end
    output = zeros(size(graph1,1));
    for i = 1:size(graph1,1)
        for j = 1:size(graph1)
            if ismember(i, index_mapping_graph1) && ismember(j, index_mapping_graph1)
                output(i,j) = graph1(i,j);
            end
        end
    end
    isolatedVertices = sum(output, 1) == 0 & sum(output, 2)' == 0;
    output = output(~isolatedVertices, ~isolatedVertices);
    plot(digraph(output));
end