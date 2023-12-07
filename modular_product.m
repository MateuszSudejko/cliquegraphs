function [adjacencyMatrix, indexMaping] = modular_product(matrix1, matrix2)
    next_vertex = 1;
    association = containers.Map('KeyType', 'char', 'ValueType', 'int32');
    edges = [];
    % Iterate over all vertices in both matrices
    indexMaping = cell(1, size(matrix1, 1) * size(matrix2, 1));
    for v1 = 1:size(matrix1, 1)
        for v2 = 1:size(matrix2, 1)
            for w1 = 1:size(matrix1, 1)
                for w2 = 1:size(matrix2, 1)
                    % Check conditions for modular product
                    if v1 ~= w1 && v2 ~= w2 && ...
                            matrix1(v1, w1) == matrix2(v2, w2) && ...
                            matrix1(w1, v1) == matrix2(w2, v2)

                        % Add vertices to the association if not already present
                        key1 = num2str([v1, v2]);
                        key2 = num2str([w1, w2]);
                        
                        if ~isKey(association, key1)
                            association(key1) = next_vertex;
                            indexMaping{next_vertex} = [v1, v2];
                            next_vertex = next_vertex + 1;
                        end
                        
                        if ~isKey(association, key2)
                            association(key2) = next_vertex;
                            indexMaping{next_vertex} = [w1, w2];
                            next_vertex = next_vertex + 1;
                        end

                        % Add edge to the result
                        edges = [edges; association(key1), association(key2)];
                    end
                end
            end
        end
    end
    adjacencyMatrix = zeros(numel(keys(association)));
    for i = 1:size(edges, 1)
        adjacencyMatrix(edges(i, 1), edges(i, 2)) = 1;
    end
%     plot(graph(adjacencyMatrix))
end
