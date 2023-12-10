function output = MCS(number_of_graphs,graphs)
    %   graphs: call array with adjacency matrices of the input graphs
    if number_of_graphs == 0 || number_of_graphs == 1
        return
    end
    graph1 = graphs{1};
    graph2 = graphs{2};
    for k = 2:(number_of_graphs)
        [modular, index_mapping] = modular_product(graph1, graph2);
        all_cliques = BronKerboschDirected2(modular);
        max_size = 0;
        for i = 1:length(all_cliques)
            if length(all_cliques{i}) > max_size
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
            for j = 1:size(graph1, 1)
                if ismember(i, index_mapping_graph1) && ismember(j, index_mapping_graph1)
                    output(i,j) = graph1(i,j);
                end
            end
        end
        isolatedVertices = sum(output, 1) == 0 & sum(output, 2)' == 0;
        output = output(~isolatedVertices, ~isolatedVertices);
        graph1 = output;
        if k+1 <= number_of_graphs
            graph2 = graphs{k+1};
        end
    end
end
